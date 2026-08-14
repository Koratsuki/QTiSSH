#ifndef APPLOCKMANAGER_H
#define APPLOCKMANAGER_H

#include <QObject>
#include <QString>
#include <QByteArray>

/**
 * @brief Stores and validates the global password required to open the app.
 *
 * The password is never saved directly: only a salted PBKDF2-HMAC-SHA256
 * verifier is persisted in ~/.config/QTiSSH/applock.conf, so the stored data
 * cannot be used to recover the password.
 */
class AppLockManager : public QObject
{
    Q_OBJECT
public:
    static AppLockManager& instance();

    bool isEnabled() const;
    bool setPassword(const QString &password);
    bool verify(const QString &password) const;
    void remove();

private:
    explicit AppLockManager(QObject *parent = nullptr);

    QByteArray getOrCreateSalt();
    QByteArray deriveKey(const QString &password, const QByteArray &salt) const;

    QString m_settingsPath;
    QByteArray m_salt;
};

#endif // APPLOCKMANAGER_H
