#include "recentconnectionsmanager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace {
constexpr int MAX_RECENT = 15;
}

QStringList RecentConnection::toList() const
{
    return QStringList() << serverId << alias << lastConnected.toString(Qt::ISODate);
}

RecentConnection RecentConnection::fromList(const QStringList &fields)
{
    RecentConnection rc;
    if (fields.size() >= 2) {
        rc.serverId = fields[0];
        rc.alias = fields[1];
    }
    if (fields.size() >= 3) {
        rc.lastConnected = QDateTime::fromString(fields[2], Qt::ISODate);
    }
    return rc;
}

RecentConnectionsManager& RecentConnectionsManager::instance()
{
    static RecentConnectionsManager inst;
    return inst;
}

RecentConnectionsManager::RecentConnectionsManager(QObject *parent)
    : QObject(parent)
    , m_path(QDir::homePath() + "/.config/QTiSSH/recent.json")
{
    load();
}

QList<RecentConnection> RecentConnectionsManager::recent() const
{
    return m_recent;
}

void RecentConnectionsManager::add(const QString &serverId, const QString &alias)
{
    if (serverId.isEmpty()) {
        return;
    }
    for (int i = 0; i < m_recent.size(); ++i) {
        if (m_recent[i].serverId == serverId) {
            m_recent.removeAt(i);
            break;
        }
    }
    RecentConnection rc;
    rc.serverId = serverId;
    rc.alias = alias;
    rc.lastConnected = QDateTime::currentDateTime();
    m_recent.prepend(rc);
    while (m_recent.size() > MAX_RECENT) {
        m_recent.removeLast();
    }
    save();
}

void RecentConnectionsManager::remove(const QString &serverId)
{
    for (int i = 0; i < m_recent.size(); ++i) {
        if (m_recent[i].serverId == serverId) {
            m_recent.removeAt(i);
            save();
            break;
        }
    }
}

void RecentConnectionsManager::clear()
{
    m_recent.clear();
    save();
}

void RecentConnectionsManager::load()
{
    QFile file(m_path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) {
        return;
    }

    QJsonArray arr = doc.object()["recent"].toArray();
    for (const QJsonValue &value : arr) {
        QJsonObject obj = value.toObject();
        RecentConnection rc;
        rc.serverId = obj["serverId"].toString();
        rc.alias = obj["alias"].toString();
        rc.lastConnected = QDateTime::fromString(obj["lastConnected"].toString(), Qt::ISODate);
        if (!rc.serverId.isEmpty()) {
            m_recent.append(rc);
        }
    }
}

void RecentConnectionsManager::save()
{
    QDir().mkpath(QDir::homePath() + "/.config/QTiSSH");

    QJsonArray arr;
    for (const RecentConnection &rc : m_recent) {
        QJsonObject obj;
        obj["serverId"] = rc.serverId;
        obj["alias"] = rc.alias;
        obj["lastConnected"] = rc.lastConnected.toString(Qt::ISODate);
        arr.append(obj);
    }

    QJsonObject root;
    root["recent"] = arr;

    QFile file(m_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();
    }
}
