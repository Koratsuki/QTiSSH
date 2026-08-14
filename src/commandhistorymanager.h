#ifndef COMMANDHISTORYMANAGER_H
#define COMMANDHISTORYMANAGER_H

#include <QObject>
#include <QStringList>
#include <QMap>

/**
 * @brief Stores per-server command history, persisted to JSON.
 */
class CommandHistoryManager : public QObject
{
    Q_OBJECT

public:
    static CommandHistoryManager& instance();

    void add(const QString &serverId, const QString &command);
    QStringList commands(const QString &serverId) const;
    void clear(const QString &serverId);
    int maxCommandsPerServer() const { return m_maxCommands; }

private:
    explicit CommandHistoryManager(QObject *parent = nullptr);
    void loadHistory();
    void saveHistory();
    QString getConfigFilePath() const;

    QMap<QString, QStringList> m_history;
    int m_maxCommands;
};

#endif // COMMANDHISTORYMANAGER_H
