#ifndef RECENTCONNECTIONSMANAGER_H
#define RECENTCONNECTIONSMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>

struct RecentConnection
{
    QString serverId;
    QString alias;
    QDateTime lastConnected;

    QStringList toList() const;
    static RecentConnection fromList(const QStringList &fields);
};

/**
 * @brief Tracks the most recently used servers.
 *
 * Stored in ~/.config/QTiSSH/recent.json (most recent first, capped at 15).
 */
class RecentConnectionsManager : public QObject
{
    Q_OBJECT
public:
    static RecentConnectionsManager& instance();

    QList<RecentConnection> recent() const;
    void add(const QString &serverId, const QString &alias);
    void remove(const QString &serverId);
    void clear();

private:
    explicit RecentConnectionsManager(QObject *parent = nullptr);
    void load();
    void save();

    QList<RecentConnection> m_recent;
    QString m_path;
};

#endif // RECENTCONNECTIONSMANAGER_H
