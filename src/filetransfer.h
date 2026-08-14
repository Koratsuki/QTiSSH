#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QFileInfo>
#include "serverconfig.h"

enum class TransferType {
    Upload,
    Download
};

enum class TransferStatus {
    Queued,
    InProgress,
    Completed,
    Failed,
    Cancelled
};

class FileTransfer : public QObject
{
    Q_OBJECT

public:
    explicit FileTransfer(QObject *parent = nullptr);
    FileTransfer(const QString &localPath, const QString &remotePath,
                 TransferType type, const ServerConfig &config, QObject *parent = nullptr);

    // Getters
    QString id() const { return m_id; }
    QString localPath() const { return m_localPath; }
    QString remotePath() const { return m_remotePath; }
    TransferType type() const { return m_type; }
    TransferStatus status() const { return m_status; }
    QString serverId() const { return m_config.id(); }
    ServerConfig serverConfig() const { return m_config; }
    qint64 totalBytes() const { return m_totalBytes; }
    qint64 transferredBytes() const { return m_transferredBytes; }
    int progressPercent() const;
    QString errorMessage() const { return m_errorMessage; }
    QString fileName() const;
    void setTotalBytes(qint64 total) { m_totalBytes = total; }

    // Control
    void start();
    void cancel();
    void pause();
    void resume();

signals:
    void progressChanged(int percent);
    void statusChanged(TransferStatus status);
    void finished(bool success);
    void error(const QString &message);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError processError);
    void onReadyRead();
    void updateProgress();

private:
    QString m_id;
    QString m_localPath;
    QString m_remotePath;
    TransferType m_type;
    TransferStatus m_status;
    ServerConfig m_config;
    qint64 m_totalBytes;
    qint64 m_transferredBytes;
    int m_percent;
    QString m_errorMessage;
    QString m_askPassPath;
    
    QProcess *m_process;
    QTimer *m_progressTimer;
    
    void setupProcess();
    QStringList buildScpCommand() const;
    QStringList buildWrappedCommand() const;
    void calculateFileSize();
    void setupAskPass(QProcessEnvironment &env);
    void cleanupAskPass();
    void parseScpOutput(const QByteArray &data);
    void setProgress(int percent);
};

#endif // FILETRANSFER_H
