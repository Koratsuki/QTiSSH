#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QList>
#include "serverconfig.h"

class ServerManager : public QObject
{
    Q_OBJECT

public:
    explicit ServerManager(QObject *parent = nullptr);
    
    void addServer(const ServerConfig &config);
    void updateServer(const QString &id, const ServerConfig &config);
    void removeServer(const QString &id);
    ServerConfig getServer(const QString &id) const;
    QList<ServerConfig> getAllServers() const;
    
    void saveServers();
    void loadServers();

signals:
    void serversChanged();

private:
    QList<ServerConfig> m_servers;
    void ensureConfigDirectory();
    QString getConfigFilePath() const;
};

#endif // SERVERMANAGER_H

