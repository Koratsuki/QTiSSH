#include "servermanager.h"
#include "passwordmanager.h"
#include "keychainstore.h"
#include "settingsmanager.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QUuid>

namespace {
bool keychainEligible()
{
    return SettingsManager::instance().useKeychain() &&
           KeychainStore::isAvailable() &&
           !PasswordManager::instance().hasMasterPassword();
}

bool isKeychainMarker(const QString &password)
{
    return password.startsWith(KeychainStore::markerPrefix());
}
}

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
            KeychainStore::remove(id);
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
    QString configPath = QDir::homePath() + "/.config/QTiSSH";
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

QString ServerManager::getConfigFilePath() const
{
    QString configPath = QDir::homePath() + "/.config/QTiSSH";
    return configPath + "/servers.json";
}

void ServerManager::saveServers()
{
    ensureConfigDirectory();
    
    QJsonArray jsonArray;
    for (const auto &server : m_servers) {
        ServerConfig copy = server;
        QString pwd = copy.password();
        PasswordManager &pm = PasswordManager::instance();

        if (keychainEligible() && !pwd.isEmpty() && !pwd.startsWith(pm.storagePrefix())
            && !isKeychainMarker(pwd)) {
            // Store in the OS keychain and persist only a marker.
            if (KeychainStore::store(copy.id(), pwd)) {
                copy.setPassword(KeychainStore::markerPrefix() + copy.id());
            }
        } else if (pm.hasMasterPassword() && pm.isUnlocked()
                   && !pwd.isEmpty() && !pwd.startsWith(pm.storagePrefix())
                   && !isKeychainMarker(pwd)) {
            copy.setPassword(pm.encrypt(pwd));
        }
        jsonArray.append(copy.toJson());
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
    PasswordManager &pm = PasswordManager::instance();
    for (const auto &value : jsonArray) {
        if (value.isObject()) {
            ServerConfig server = ServerConfig::fromJson(value.toObject());
            if (isKeychainMarker(server.password())) {
                server.setPassword(KeychainStore::retrieve(server.id()));
            } else if (pm.hasMasterPassword() && pm.isUnlocked()) {
                QString pwd = server.password();
                if (pwd.startsWith(pm.storagePrefix())) {
                    server.setPassword(pm.decrypt(pwd));
                }
            }
            m_servers.append(server);
        }
    }
}

bool ServerManager::exportToFile(const QString &path) const
{
    QJsonArray jsonArray;
    for (const auto &server : m_servers) {
        jsonArray.append(server.toJson());
    }
    
    QJsonDocument doc(jsonArray);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(doc.toJson());
    file.close();
    return true;
}

int ServerManager::importFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return 0;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isArray()) {
        return 0;
    }
    
    int imported = 0;
    PasswordManager &pm = PasswordManager::instance();
    for (const auto &value : doc.array()) {
        if (!value.isObject()) {
            continue;
        }
        
        ServerConfig server = ServerConfig::fromJson(value.toObject());
        server.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
        
        // Keep encrypted passwords encrypted across imports; keychain markers
        // cannot be transferred (they reference the old server id), so clear them.
        if (isKeychainMarker(server.password())) {
            server.setPassword(QString());
        } else if (!pm.hasMasterPassword() && server.password().startsWith(pm.storagePrefix())) {
            server.setPassword(QString());
        }
        
        m_servers.append(server);
        imported++;
    }
    
    if (imported > 0) {
        saveServers();
        emit serversChanged();
    }
    return imported;
}

void ServerManager::encryptAllPasswords()
{
    PasswordManager &pm = PasswordManager::instance();
    if (!pm.hasMasterPassword() || !pm.isUnlocked()) {
        return;
    }
    
    for (auto &server : m_servers) {
        QString pwd = server.password();

        // Pull plaintext out of the keychain first.
        if (isKeychainMarker(pwd)) {
            pwd = KeychainStore::retrieve(server.id());
            KeychainStore::remove(server.id());
            server.setPassword(pwd);
            pwd = server.password();
        }

        if (!pwd.isEmpty() && !pwd.startsWith(pm.storagePrefix())) {
            server.setPassword(pm.encrypt(pwd));
        }
    }
}

void ServerManager::decryptAllPasswords()
{
    PasswordManager &pm = PasswordManager::instance();
    if (!pm.isUnlocked()) {
        return;
    }
    
    for (auto &server : m_servers) {
        QString pwd = server.password();
        if (pwd.startsWith(pm.storagePrefix())) {
            server.setPassword(pm.decrypt(pwd));
        } else if (isKeychainMarker(pwd)) {
            QString stored = KeychainStore::retrieve(server.id());
            if (!stored.isEmpty()) {
                server.setPassword(stored);
            }
        }
    }
}

