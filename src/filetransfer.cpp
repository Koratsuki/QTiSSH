#include "filetransfer.h"
#include "passwordmanager.h"
#include <QUuid>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QProcessEnvironment>

FileTransfer::FileTransfer(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_type(TransferType::Upload)
    , m_status(TransferStatus::Queued)
    , m_totalBytes(0)
    , m_transferredBytes(0)
    , m_percent(0)
    , m_process(new QProcess(this))
    , m_progressTimer(new QTimer(this))
{
    setupProcess();
}

FileTransfer::FileTransfer(const QString &localPath, const QString &remotePath,
                           TransferType type, const ServerConfig &config, QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_localPath(localPath)
    , m_remotePath(remotePath)
    , m_type(type)
    , m_status(TransferStatus::Queued)
    , m_config(config)
    , m_totalBytes(0)
    , m_transferredBytes(0)
    , m_percent(0)
    , m_process(new QProcess(this))
    , m_progressTimer(new QTimer(this))
{
    setupProcess();
    calculateFileSize();
}

void FileTransfer::setupProcess()
{
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &FileTransfer::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &FileTransfer::onProcessError);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &FileTransfer::onReadyRead);
    connect(m_process, &QProcess::readyReadStandardError, this, &FileTransfer::onReadyRead);
    
    m_progressTimer->setInterval(500); // Update progress every 500ms
    connect(m_progressTimer, &QTimer::timeout, this, &FileTransfer::updateProgress);
}

void FileTransfer::calculateFileSize()
{
    if (m_type == TransferType::Upload) {
        QFileInfo fileInfo(m_localPath);
        if (fileInfo.exists() && fileInfo.isFile()) {
            m_totalBytes = fileInfo.size();
        }
    }
    // For downloads, the total size is provided by the caller (remote listing)
    // through setTotalBytes()/addTransfer; if unknown it stays 0.
}

QString FileTransfer::fileName() const
{
    if (m_type == TransferType::Upload) {
        QFileInfo fileInfo(m_localPath);
        return fileInfo.fileName();
    }
    // For downloads, the remote path may be user@host:/path/file
    QString path = m_remotePath;
    int colon = path.indexOf(':');
    if (colon != -1) {
        path = path.mid(colon + 1);
    }
    return QFileInfo(path).fileName();
}

int FileTransfer::progressPercent() const
{
    if (m_totalBytes > 0) {
        return static_cast<int>((m_transferredBytes * 100) / m_totalBytes);
    }
    return m_percent;
}

void FileTransfer::start()
{
    if (m_status != TransferStatus::Queued) {
        return;
    }
    
    m_status = TransferStatus::InProgress;
    emit statusChanged(m_status);
    
    QStringList args = buildScpCommand();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    setupAskPass(env);
    m_process->setProcessEnvironment(env);
    
#ifdef Q_OS_UNIX
    // Wrap scp in a PTY (`script`) so the progress meter is emitted and we can
    // report real progress. Fall back to plain scp if this fails.
    QStringList wrapped = buildWrappedCommand();
    if (!wrapped.isEmpty()) {
        m_process->start(wrapped.first(), wrapped.mid(1));
    } else {
        m_process->start("scp", args);
    }
#else
    m_process->start("scp", args);
#endif
    m_progressTimer->start();
}

QStringList FileTransfer::buildWrappedCommand() const
{
#ifdef Q_OS_UNIX
    QStringList args = buildScpCommand();
    QStringList quoted;
    for (const QString &arg : args) {
        quoted << "'" + QString(arg).replace("'", "'\\''") + "'";
    }
    QString command = "scp " + quoted.join(" ");
    return QStringList() << "script" << "-q" << "-e" << "-c" << command << "/dev/null";
#else
    return QStringList();
#endif
}

void FileTransfer::cancel()
{
    if (m_status == TransferStatus::InProgress) {
        m_process->kill();
        m_progressTimer->stop();
        m_status = TransferStatus::Cancelled;
        emit statusChanged(m_status);
        emit finished(false);
    }
    cleanupAskPass();
}

void FileTransfer::pause()
{
    // SCP doesn't support pausing; keep for API compatibility
}

void FileTransfer::resume()
{
    // SCP doesn't support resuming; keep for API compatibility
}

void FileTransfer::setupAskPass(QProcessEnvironment &env)
{
    if (m_config.authType() != AuthType::Password) {
        return;
    }
    QString pwd = m_config.password();
    if (pwd.isEmpty() || pwd.startsWith(PasswordManager::instance().storagePrefix())) {
        return;
    }
    
    QString escaped = pwd;
    escaped.replace("'", "'\\''");
    
    m_askPassPath = QDir::temp().filePath(
        "qtissh-askpass-" + QUuid::createUuid().toString(QUuid::WithoutBraces));
    QFile file(m_askPassPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write("#!/bin/sh\n");
        file.write("echo '" + escaped.toUtf8() + "'\n");
        file.close();
        QFile::setPermissions(m_askPassPath,
                              QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
        env.insert("SSH_ASKPASS", m_askPassPath);
        env.insert("SSH_ASKPASS_REQUIRE", "force");
        env.insert("DISPLAY", ":0");
    }
}

void FileTransfer::cleanupAskPass()
{
    if (!m_askPassPath.isEmpty()) {
        QFile::remove(m_askPassPath);
        m_askPassPath.clear();
    }
}

QStringList FileTransfer::buildScpCommand() const
{
    QStringList args;
    
    args << "-p"; // Preserve timestamps
    args << "-v"; // Verbose so we can parse "Transferred:" stats
    
    // Connection options
    if (m_config.port() != 22) {
        args << "-P" << QString::number(m_config.port());
    }
    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        args << "-i" << m_config.keyPath();
    }
    if (!m_config.jumpHost().isEmpty()) {
        args << "-J" << m_config.jumpHost();
    }
    if (m_config.forwardAgent() || m_config.authType() == AuthType::SSHAgent) {
        args << "-o" << "ForwardAgent=yes";
    }
    if (!m_config.strictHostKeyChecking()) {
        args << "-o" << "StrictHostKeyChecking=no";
        args << "-o" << "UserKnownHostsFile=/dev/null";
    } else {
        args << "-o" << "StrictHostKeyChecking=yes";
    }

    // Custom SSH options (from profile + server)
    args << m_config.sshOptionArgs();
    
    QString remoteTarget = QString("%1@%2:%3")
                               .arg(m_config.username(), m_config.host(), m_remotePath);
    
    if (m_type == TransferType::Upload) {
        args << m_localPath << remoteTarget;
    } else {
        args << remoteTarget << m_localPath;
    }
    
    return args;
}

void FileTransfer::setProgress(int percent)
{
    percent = qBound(0, percent, 100);
    if (percent == m_percent) {
        return;
    }
    m_percent = percent;
    if (m_totalBytes > 0) {
        m_transferredBytes = (m_totalBytes * percent) / 100;
    }
    emit progressChanged(percent);
}

void FileTransfer::parseScpOutput(const QByteArray &data)
{
    QString output = QString::fromUtf8(data);
    
    // Real transfer stats from scp -v: "Transferred: sent X, received Y bytes"
    static const QRegularExpression transferredRe(
        R"(Transferred: sent (\d+), received (\d+) bytes)");
    QRegularExpressionMatch transferredMatch = transferredRe.match(output);
    if (transferredMatch.hasMatch()) {
        bool sentOk = false, recvOk = false;
        qint64 sent = transferredMatch.captured(1).toLongLong(&sentOk);
        qint64 received = transferredMatch.captured(2).toLongLong(&recvOk);
        qint64 bytes = (m_type == TransferType::Upload) ? sent : received;
        if (sentOk && recvOk) {
            m_transferredBytes = bytes;
            if (m_totalBytes > 0) {
                emit progressChanged(progressPercent());
            }
        }
    }
    
    // Live progress meter (available when running under a PTY via `script`)
    static const QRegularExpression percentRe(R"(\b(\d{1,3})%\b)");
    QRegularExpressionMatchIterator it = percentRe.globalMatch(output);
    int lastPercent = -1;
    while (it.hasNext()) {
        bool ok = false;
        int percent = it.next().captured(1).toInt(&ok);
        if (ok && percent <= 100) {
            lastPercent = percent;
        }
    }
    if (lastPercent >= 0) {
        setProgress(lastPercent);
    }
}

void FileTransfer::updateProgress()
{
    // Downloads: watch the local file grow against the known total size.
    if (m_type == TransferType::Download && m_totalBytes > 0) {
        QFileInfo fileInfo(m_localPath);
        if (fileInfo.exists()) {
            m_transferredBytes = fileInfo.size();
            emit progressChanged(progressPercent());
        }
    }
}

void FileTransfer::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_progressTimer->stop();
    cleanupAskPass();
    
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        m_status = TransferStatus::Completed;
        m_transferredBytes = m_totalBytes;
        setProgress(100);
        emit finished(true);
    } else {
        m_status = TransferStatus::Failed;
        m_errorMessage = QString("Transfer failed with exit code %1").arg(exitCode);
        emit error(m_errorMessage);
        emit finished(false);
    }
    
    emit statusChanged(m_status);
}

void FileTransfer::onProcessError(QProcess::ProcessError processError)
{
    m_progressTimer->stop();
    cleanupAskPass();
    m_status = TransferStatus::Failed;
    
    switch (processError) {
        case QProcess::FailedToStart:
            m_errorMessage = "Failed to start SCP. Make sure 'scp' is installed and in your PATH.";
            break;
        case QProcess::Crashed:
            m_errorMessage = "SCP process crashed.";
            break;
        case QProcess::Timedout:
            m_errorMessage = "SCP process timed out.";
            break;
        default:
            m_errorMessage = "Unknown SCP error occurred.";
            break;
    }
    
    emit error(m_errorMessage);
    emit statusChanged(m_status);
    emit finished(false);
}

void FileTransfer::onReadyRead()
{
    QByteArray data = m_process->readAllStandardOutput();
    parseScpOutput(data);
    QByteArray errData = m_process->readAllStandardError();
    parseScpOutput(errData);
}
