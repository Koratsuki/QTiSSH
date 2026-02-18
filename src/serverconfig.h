#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QString>
#include <QJsonObject>
#include <QUuid>

enum class AuthType {
    Password,
    PublicKey
};

class ServerConfig
{
public:
    ServerConfig();
    ServerConfig(const QString &alias, const QString &host, int port, 
                 const QString &username, AuthType authType);

    // Getters
    QString id() const { return m_id; }
    QString alias() const { return m_alias; }
    QString host() const { return m_host; }
    int port() const { return m_port; }
    QString username() const { return m_username; }
    QString password() const { return m_password; }
    QString keyPath() const { return m_keyPath; }
    AuthType authType() const { return m_authType; }
    QString group() const { return m_group; }
    QString tags() const { return m_tags; }
    bool strictHostKeyChecking() const { return m_strictHostKeyChecking; }
    QString tunnels() const { return m_tunnels; }

    // Setters
    void setAlias(const QString &alias) { m_alias = alias; }
    void setHost(const QString &host) { m_host = host; }
    void setPort(int port) { m_port = port; }
    void setUsername(const QString &username) { m_username = username; }
    void setPassword(const QString &password) { m_password = password; }
    void setKeyPath(const QString &keyPath) { m_keyPath = keyPath; }
    void setAuthType(AuthType authType) { m_authType = authType; }
    void setGroup(const QString &group) { m_group = group; }
    void setTags(const QString &tags) { m_tags = tags; }
    void setStrictHostKeyChecking(bool enable) { m_strictHostKeyChecking = enable; }
    void setTunnels(const QString &tunnels) { m_tunnels = tunnels; }

    // Serialization
    QJsonObject toJson() const;
    static ServerConfig fromJson(const QJsonObject &json);

    bool isValid() const;

private:
    QString m_id;
    QString m_alias;
    QString m_host;
    int m_port;
    QString m_username;
    QString m_password;
    QString m_keyPath;
    AuthType m_authType;
    QString m_group;
    QString m_tags;
    bool m_strictHostKeyChecking;
    QString m_tunnels;
};

#endif // SERVERCONFIG_H

