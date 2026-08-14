#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QStringList>

/**
 * @brief Manages named connection profiles.
 *
 * A profile is a named set of OpenSSH options (compression, ciphers,
 * keepalive, ...) that can be reused across servers. Profiles are stored in
 * ~/.config/QTiSSH/profiles.json.
 */
class ProfileManager : public QObject
{
    Q_OBJECT
public:
    static ProfileManager& instance();

    QStringList profileNames() const;
    QString optionsFor(const QString &name) const;
    bool exists(const QString &name) const;

    bool addProfile(const QString &name, const QString &options);
    bool updateProfile(const QString &name, const QString &options);
    bool removeProfile(const QString &name);

private:
    explicit ProfileManager(QObject *parent = nullptr);
    void load();
    void save();

    QMap<QString, QString> m_profiles;
    QString m_path;
};

#endif // PROFILEMANAGER_H
