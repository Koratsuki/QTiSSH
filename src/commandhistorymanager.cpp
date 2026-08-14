#include "commandhistorymanager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CommandHistoryManager& CommandHistoryManager::instance()
{
    static CommandHistoryManager inst;
    return inst;
}

CommandHistoryManager::CommandHistoryManager(QObject *parent)
    : QObject(parent)
    , m_maxCommands(200)
{
    loadHistory();
}

void CommandHistoryManager::add(const QString &serverId, const QString &command)
{
    if (serverId.isEmpty() || command.trimmed().isEmpty()) {
        return;
    }

    QStringList &commands = m_history[serverId];
    commands.removeAll(command);
    commands.prepend(command);

    while (commands.size() > m_maxCommands) {
        commands.removeLast();
    }

    saveHistory();
}

QStringList CommandHistoryManager::commands(const QString &serverId) const
{
    return m_history.value(serverId);
}

void CommandHistoryManager::clear(const QString &serverId)
{
    m_history.remove(serverId);
    saveHistory();
}

QString CommandHistoryManager::getConfigFilePath() const
{
    return QDir::homePath() + "/.config/QTiSSH/history.json";
}

void CommandHistoryManager::loadHistory()
{
    QFile file(getConfigFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    for (auto it = root.begin(); it != root.end(); ++it) {
        QStringList commands;
        if (it.value().isArray()) {
            for (const QJsonValue &value : it.value().toArray()) {
                commands.append(value.toString());
            }
        }
        m_history[it.key()] = commands;
    }
}

void CommandHistoryManager::saveHistory()
{
    QString dirPath = QDir::homePath() + "/.config/QTiSSH";
    QDir().mkpath(dirPath);

    QJsonObject root;
    for (auto it = m_history.begin(); it != m_history.end(); ++it) {
        root[it.key()] = QJsonArray::fromStringList(it.value());
    }

    QFile file(getConfigFilePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}
