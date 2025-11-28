#ifndef SFTPCONNECTION_H
#define SFTPCONNECTION_H

#include <QObject>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include "serverconfig.h"

struct RemoteFileInfo {
    QString name;
    QString path;
    qint64 size;
    bool isDirectory;
    QString permissions;
    QDateTime lastModified;
    QString owner;
    QString group;
};

class SFTPConnection : public QObject
{
    Q_OBJECT

public:
    explicit SFTPConnection(const ServerConfig &config, QObject *parent = nullptr);
    ~SFTPConnection();

    bool isConnected() const { return m_connected; }
    ServerConfig serverConfig() const { return m_config; }
    QString currentRemotePath() const { return m_currentRemotePath; }

    // Connection management
    void connectToServer();
    void disconnect();

    // Directory operations
    void listDirectory(const QString &path = QString());
    void changeDirectory(const QString &path);
    void createDirectory(const QString &name);
    void removeDirectory(const QString &path);

    // File operations
    void deleteFile(const QString &path);
    void renameFile(const QString &oldPath, const QString &newPath);
    void downloadFile(const QString &remotePath, const QString &localPath);
    void uploadFile(const QString &localPath, const QString &remotePath);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void directoryListed(const QList<RemoteFileInfo> &files);
    void directoryChanged(const QString &path);
    void operationCompleted(const QString &operation);
    void operationFailed(const QString &operation, const QString &error);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();

private:
    ServerConfig m_config;
    QProcess *m_process;
    bool m_connected;
    QString m_currentRemotePath;
    QString m_pendingCommand;
    QString m_outputBuffer;
    
    void setupProcess();
    void sendCommand(const QString &command);
    void processOutput(const QString &output);
    QList<RemoteFileInfo> parseDirectoryListing(const QString &output);
    QString buildConnectionString();
    QStringList buildSftpCommand();
};

#endif // SFTPCONNECTION_H

