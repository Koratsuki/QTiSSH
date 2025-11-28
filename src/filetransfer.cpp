#include "filetransfer.h"
#include <QUuid>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

FileTransfer::FileTransfer(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_type(TransferType::Upload)
    , m_status(TransferStatus::Queued)
    , m_totalBytes(0)
    , m_transferredBytes(0)
    , m_process(new QProcess(this))
    , m_progressTimer(new QTimer(this))
{
    setupProcess();
}

FileTransfer::FileTransfer(const QString &localPath, const QString &remotePath,
                           TransferType type, const QString &serverId, QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_localPath(localPath)
    , m_remotePath(remotePath)
    , m_type(type)
    , m_status(TransferStatus::Queued)
    , m_serverId(serverId)
    , m_totalBytes(0)
    , m_transferredBytes(0)
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
    
    m_progressTimer->setInterval(500); // Update progress every 500ms
    connect(m_progressTimer, &QTimer::timeout, this, &FileTransfer::updateProgress);
}

void FileTransfer::calculateFileSize()
{
    if (m_type == TransferType::Upload) {
        QFileInfo fileInfo(m_localPath);
        if (fileInfo.exists()) {
            m_totalBytes = fileInfo.size();
        }
    }
    // For downloads, we'll estimate or get size from remote listing
}

QString FileTransfer::fileName() const
{
    QFileInfo fileInfo(m_localPath);
    return fileInfo.fileName();
}

int FileTransfer::progressPercent() const
{
    if (m_totalBytes == 0) return 0;
    return static_cast<int>((m_transferredBytes * 100) / m_totalBytes);
}

void FileTransfer::start()
{
    if (m_status != TransferStatus::Queued) {
        return;
    }
    
    m_status = TransferStatus::InProgress;
    emit statusChanged(m_status);
    
    QStringList args = buildScpCommand();
    m_process->start("scp", args);
    m_progressTimer->start();
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
}

void FileTransfer::pause()
{
    // SCP doesn't support pausing, but we can implement this for future SFTP implementation
}

void FileTransfer::resume()
{
    // SCP doesn't support resuming, but we can implement this for future SFTP implementation
}

QStringList FileTransfer::buildScpCommand()
{
    QStringList args;
    
    // Add common SCP options
    args << "-r"; // Recursive for directories
    args << "-p"; // Preserve timestamps
    args << "-q"; // Quiet mode
    args << "-o" << "StrictHostKeyChecking=no";
    args << "-o" << "UserKnownHostsFile=/dev/null";
    
    // TODO: Get server config from serverId to build proper connection string
    // For now, we'll assume the remote path contains the full connection info
    
    if (m_type == TransferType::Upload) {
        args << m_localPath << m_remotePath;
    } else {
        args << m_remotePath << m_localPath;
    }
    
    return args;
}

void FileTransfer::updateProgress()
{
    // For SCP, we can estimate progress by checking destination file size
    // This is a simplified implementation
    if (m_type == TransferType::Download) {
        QFileInfo fileInfo(m_localPath);
        if (fileInfo.exists()) {
            m_transferredBytes = fileInfo.size();
            emit progressChanged(progressPercent());
        }
    } else {
        // For uploads, we can check if the process is still running
        // and estimate based on time elapsed (simplified)
        if (m_process->state() == QProcess::Running) {
            // Simple time-based estimation (not accurate, but better than nothing)
            static int fakeProgress = 0;
            fakeProgress += 10;
            if (fakeProgress > 90) fakeProgress = 90; // Don't go to 100% until finished
            emit progressChanged(fakeProgress);
        }
    }
}

void FileTransfer::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_progressTimer->stop();
    
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        m_status = TransferStatus::Completed;
        m_transferredBytes = m_totalBytes;
        emit progressChanged(100);
        emit finished(true);
    } else {
        m_status = TransferStatus::Failed;
        m_errorMessage = QString("Transfer failed with exit code %1").arg(exitCode);
        emit error(m_errorMessage);
        emit finished(false);
    }
    
    emit statusChanged(m_status);
}

void FileTransfer::onProcessError(QProcess::ProcessError error)
{
    m_progressTimer->stop();
    m_status = TransferStatus::Failed;
    
    switch (error) {
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

