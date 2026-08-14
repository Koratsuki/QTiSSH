#include "profilemanager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ProfileManager& ProfileManager::instance()
{
    static ProfileManager inst;
    return inst;
}

ProfileManager::ProfileManager(QObject *parent)
    : QObject(parent)
    , m_path(QDir::homePath() + "/.config/QTiSSH/profiles.json")
{
    load();
}

QStringList ProfileManager::profileNames() const
{
    return m_profiles.keys();
}

QString ProfileManager::optionsFor(const QString &name) const
{
    return m_profiles.value(name);
}

bool ProfileManager::exists(const QString &name) const
{
    return m_profiles.contains(name);
}

bool ProfileManager::addProfile(const QString &name, const QString &options)
{
    if (name.trimmed().isEmpty() || m_profiles.contains(name)) {
        return false;
    }
    m_profiles.insert(name, options);
    save();
    return true;
}

bool ProfileManager::updateProfile(const QString &name, const QString &options)
{
    if (!m_profiles.contains(name)) {
        return false;
    }
    m_profiles.insert(name, options);
    save();
    return true;
}

bool ProfileManager::removeProfile(const QString &name)
{
    if (!m_profiles.remove(name)) {
        return false;
    }
    save();
    return true;
}

void ProfileManager::load()
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

    QJsonObject root = doc.object();
    QJsonArray profiles = root["profiles"].toArray();
    for (const QJsonValue &value : profiles) {
        QJsonObject obj = value.toObject();
        QString name = obj["name"].toString();
        QString options = obj["options"].toString();
        if (!name.isEmpty()) {
            m_profiles.insert(name, options);
        }
    }
}

void ProfileManager::save()
{
    QDir().mkpath(QDir::homePath() + "/.config/QTiSSH");

    QJsonArray profiles;
    for (auto it = m_profiles.constBegin(); it != m_profiles.constEnd(); ++it) {
        QJsonObject obj;
        obj["name"] = it.key();
        obj["options"] = it.value();
        profiles.append(obj);
    }

    QJsonObject root;
    root["profiles"] = profiles;

    QFile file(m_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();
    }
}
