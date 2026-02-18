#include "serverconfig.h"
#include <QJsonDocument>

ServerConfig::ServerConfig()
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_port(22)
    , m_authType(AuthType::Password)
    , m_strictHostKeyChecking(false)
{
}

ServerConfig::ServerConfig(const QString &alias, const QString &host, int port,
                           const QString &username, AuthType authType)
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_alias(alias)
    , m_host(host)
    , m_port(port)
    , m_username(username)
    , m_authType(authType)
    , m_strictHostKeyChecking(false)
{
}

QJsonObject ServerConfig::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["alias"] = m_alias;
    json["host"] = m_host;
    json["port"] = m_port;
    json["username"] = m_username;
    json["password"] = m_password;
    json["keyPath"] = m_keyPath;
    json["authType"] = static_cast<int>(m_authType);
    json["group"] = m_group;
    json["tags"] = m_tags;
    json["strictHostKeyChecking"] = m_strictHostKeyChecking;
    json["tunnels"] = m_tunnels;
    return json;
}

ServerConfig ServerConfig::fromJson(const QJsonObject &json)
{
    ServerConfig config;
    config.m_id = json["id"].toString();
    config.m_alias = json["alias"].toString();
    config.m_host = json["host"].toString();
    config.m_port = json["port"].toInt(22);
    config.m_username = json["username"].toString();
    config.m_password = json["password"].toString();
    config.m_keyPath = json["keyPath"].toString();
    config.m_authType = static_cast<AuthType>(json["authType"].toInt());
    config.m_group = json["group"].toString();
    config.m_tags = json["tags"].toString();
    config.m_strictHostKeyChecking = json["strictHostKeyChecking"].toBool(false);
    config.m_tunnels = json["tunnels"].toString();
    return config;
}

bool ServerConfig::isValid() const
{
    return !m_host.isEmpty() && !m_username.isEmpty() && m_port > 0 && m_port < 65536;
}

