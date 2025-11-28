#include "sftpconnection.h"
#include <QRegularExpression>
#include <QDateTime>

SFTPConnection::SFTPConnection(const ServerConfig &config, QObject *parent)
    : QObject(parent)
    , m_config(config)
    , m_process(new QProcess(this))
    , m_connected(false)
    , m_currentRemotePath("/")
{
    setupProcess();
}

SFTPConnection::~SFTPConnection()
{
    if (m_connected) {
        disconnect();
    }
}

void SFTPConnection::setupProcess()
{
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SFTPConnection::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &SFTPConnection::onProcessError);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &SFTPConnection::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &SFTPConnection::onReadyReadStandardError);
}

void SFTPConnection::connectToServer()
{
    if (m_connected) {
        return;
    }

    QStringList args = buildSftpCommand();
    m_process->start("sftp", args);
    
    // Wait for connection to establish
    if (m_process->waitForStarted(5000)) {
        // Send initial commands to set up the session
        sendCommand("pwd"); // Get current directory
    }
}

void SFTPConnection::disconnect()
{
    if (m_process->state() == QProcess::Running) {
        sendCommand("quit");
        m_process->waitForFinished(3000);
        if (m_process->state() == QProcess::Running) {
            m_process->terminate();
            m_process->waitForFinished(1000);
            if (m_process->state() == QProcess::Running) {
                m_process->kill();
            }
        }
    }
    m_connected = false;
    emit disconnected();
}

QStringList SFTPConnection::buildSftpCommand()
{
    QStringList args;
    
    // Add port if not default
    if (m_config.port() != 22) {
        args << "-P" << QString::number(m_config.port());
    }
    
    // Add key file if using public key auth
    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        args << "-i" << m_config.keyPath();
    }
    
    // Disable strict host key checking for convenience
    args << "-o" << "StrictHostKeyChecking=no";
    args << "-o" << "UserKnownHostsFile=/dev/null";
    
    // Add connection string
    args << QString("%1@%2").arg(m_config.username()).arg(m_config.host());
    
    return args;
}

void SFTPConnection::sendCommand(const QString &command)
{
    if (m_process->state() == QProcess::Running) {
        m_pendingCommand = command;
        m_process->write(command.toUtf8() + "\n");
    }
}

void SFTPConnection::listDirectory(const QString &path)
{
    QString targetPath = path.isEmpty() ? m_currentRemotePath : path;
    sendCommand(QString("ls -la %1").arg(targetPath));
}

void SFTPConnection::changeDirectory(const QString &path)
{
    sendCommand(QString("cd %1").arg(path));
}

void SFTPConnection::createDirectory(const QString &name)
{
    sendCommand(QString("mkdir %1").arg(name));
}

void SFTPConnection::removeDirectory(const QString &path)
{
    sendCommand(QString("rmdir %1").arg(path));
}

void SFTPConnection::deleteFile(const QString &path)
{
    sendCommand(QString("rm %1").arg(path));
}

void SFTPConnection::renameFile(const QString &oldPath, const QString &newPath)
{
    sendCommand(QString("rename %1 %2").arg(oldPath).arg(newPath));
}

void SFTPConnection::downloadFile(const QString &remotePath, const QString &localPath)
{
    sendCommand(QString("get %1 %2").arg(remotePath).arg(localPath));
}

void SFTPConnection::uploadFile(const QString &localPath, const QString &remotePath)
{
    sendCommand(QString("put %1 %2").arg(localPath).arg(remotePath));
}

void SFTPConnection::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_connected = false;
    emit disconnected();
    
    if (exitStatus == QProcess::CrashExit || exitCode != 0) {
        emit connectionError(QString("SFTP process finished with exit code %1").arg(exitCode));
    }
}

void SFTPConnection::onProcessError(QProcess::ProcessError error)
{
    QString errorMsg;
    switch (error) {
        case QProcess::FailedToStart:
            errorMsg = "Failed to start SFTP. Make sure 'sftp' is installed and in your PATH.";
            break;
        case QProcess::Crashed:
            errorMsg = "SFTP process crashed.";
            break;
        case QProcess::Timedout:
            errorMsg = "SFTP process timed out.";
            break;
        default:
            errorMsg = "Unknown SFTP error occurred.";
            break;
    }
    
    emit connectionError(errorMsg);
}

void SFTPConnection::onReadyReadStandardOutput()
{
    QByteArray data = m_process->readAllStandardOutput();
    QString output = QString::fromUtf8(data);
    m_outputBuffer += output;
    
    // Process complete lines
    if (m_outputBuffer.contains('\n')) {
        processOutput(m_outputBuffer);
        m_outputBuffer.clear();
    }
}

void SFTPConnection::onReadyReadStandardError()
{
    QByteArray data = m_process->readAllStandardError();
    QString error = QString::fromUtf8(data);
    
    // Check for password prompt
    if (error.contains("password:", Qt::CaseInsensitive) && 
        m_config.authType() == AuthType::Password && 
        !m_config.password().isEmpty()) {
        m_process->write(m_config.password().toUtf8() + "\n");
        return;
    }
    
    // Check for connection success indicators
    if (error.contains("sftp>") || error.contains("Connected to")) {
        if (!m_connected) {
            m_connected = true;
            emit connected();
            listDirectory(); // Get initial directory listing
        }
        return;
    }
    
    emit connectionError(error);
}

void SFTPConnection::processOutput(const QString &output)
{
    // Handle different command responses
    if (m_pendingCommand.startsWith("pwd")) {
        // Extract current directory from pwd output
        QRegularExpression pwdRegex(R"(Remote working directory: (.+))");
        QRegularExpressionMatch match = pwdRegex.match(output);
        if (match.hasMatch()) {
            m_currentRemotePath = match.captured(1).trimmed();
            emit directoryChanged(m_currentRemotePath);
        }
    }
    else if (m_pendingCommand.startsWith("ls")) {
        // Parse directory listing
        QList<RemoteFileInfo> files = parseDirectoryListing(output);
        emit directoryListed(files);
    }
    else if (m_pendingCommand.startsWith("cd")) {
        // Directory changed, get new path
        sendCommand("pwd");
    }
    else if (m_pendingCommand.startsWith("mkdir") || 
             m_pendingCommand.startsWith("rmdir") ||
             m_pendingCommand.startsWith("rm") ||
             m_pendingCommand.startsWith("rename")) {
        // Operation completed, refresh directory listing
        emit operationCompleted(m_pendingCommand);
        listDirectory();
    }
    else if (m_pendingCommand.startsWith("get") || m_pendingCommand.startsWith("put")) {
        // File transfer completed
        emit operationCompleted(m_pendingCommand);
    }
    
    m_pendingCommand.clear();
}

QList<RemoteFileInfo> SFTPConnection::parseDirectoryListing(const QString &output)
{
    QList<RemoteFileInfo> files;
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        if (line.trimmed().isEmpty() || line.startsWith("sftp>")) {
            continue;
        }
        
        // Parse ls -la output format
        // Example: drwxr-xr-x    2 user     group        4096 Jan 01 12:00 dirname
        QRegularExpression regex(R"(^([d\-rwx]+)\s+\d+\s+(\w+)\s+(\w+)\s+(\d+)\s+(\w+\s+\d+\s+[\d:]+)\s+(.+)$)");
        QRegularExpressionMatch match = regex.match(line);
        
        if (match.hasMatch()) {
            RemoteFileInfo fileInfo;
            fileInfo.permissions = match.captured(1);
            fileInfo.owner = match.captured(2);
            fileInfo.group = match.captured(3);
            fileInfo.size = match.captured(4).toLongLong();
            // Parse date/time (simplified)
            fileInfo.lastModified = QDateTime::currentDateTime(); // TODO: Parse actual date
            fileInfo.name = match.captured(6);
            fileInfo.isDirectory = fileInfo.permissions.startsWith('d');
            fileInfo.path = m_currentRemotePath + "/" + fileInfo.name;
            
            // Skip . and .. entries
            if (fileInfo.name != "." && fileInfo.name != "..") {
                files.append(fileInfo);
            }
        }
    }
    
    return files;
}
