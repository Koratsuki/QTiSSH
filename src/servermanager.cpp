#include "servermanager.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>

ServerManager::ServerManager(QObject *parent)
    : QObject(parent)
{
    loadServers();
}

void ServerManager::addServer(const ServerConfig &config)
{
    m_servers.append(config);
    saveServers();
    emit serversChanged();
}

void ServerManager::updateServer(const QString &id, const ServerConfig &config)
{
    for (int i = 0; i < m_servers.size(); ++i) {
        if (m_servers[i].id() == id) {
            m_servers[i] = config;
            saveServers();
            emit serversChanged();
            return;
        }
    }
}

void ServerManager::removeServer(const QString &id)
{
    for (int i = 0; i < m_servers.size(); ++i) {
        if (m_servers[i].id() == id) {
            m_servers.removeAt(i);
            saveServers();
            emit serversChanged();
            return;
        }
    }
}

ServerConfig ServerManager::getServer(const QString &id) const
{
    for (const auto &server : m_servers) {
        if (server.id() == id) {
            return server;
        }
    }
    return ServerConfig();
}

QList<ServerConfig> ServerManager::getAllServers() const
{
    return m_servers;
}

void ServerManager::ensureConfigDirectory()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

QString ServerManager::getConfigFilePath() const
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return configPath + "/servers.json";
}

void ServerManager::saveServers()
{
    ensureConfigDirectory();
    
    QJsonArray jsonArray;
    for (const auto &server : m_servers) {
        jsonArray.append(server.toJson());
    }
    
    QJsonDocument doc(jsonArray);
    QFile file(getConfigFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void ServerManager::loadServers()
{
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        return;
    }
    
    m_servers.clear();
    QJsonArray jsonArray = doc.array();
    for (const auto &value : jsonArray) {
        if (value.isObject()) {
            m_servers.append(ServerConfig::fromJson(value.toObject()));
        }
    }
}

