#ifndef PASSWORDMANAGER_H
#define PASSWORDMANAGER_H

#include <QObject>
#include <QString>
#include <QByteArray>

/**
 * @brief Encrypts and decrypts stored passwords using a user-supplied master password.
 *
 * Uses OpenSSL AES-256-CBC. The encryption key is derived from the master
 * password with PBKDF2-HMAC-SHA256. A random salt is generated once and stored
 * together with a verifier so the master password can be validated at unlock
 * time without storing any key material on disk.
 *
 * Encrypted payloads are serialized as base64(iv + ciphertext) and, when
 * persisted, prefixed with "enc:" so they can be told apart from legacy
 * plaintext passwords.
 */
class PasswordManager : public QObject
{
    Q_OBJECT

public:
    static PasswordManager& instance();

    bool hasMasterPassword() const;
    bool isUnlocked() const { return m_unlocked; }

    bool setMasterPassword(const QString &password);
    bool unlock(const QString &password);
    void lock();
    void removeMasterPassword();

    QString encrypt(const QString &plain) const;
    QString decrypt(const QString &encrypted) const;

    QString storagePrefix() const { return QStringLiteral("enc:"); }

private:
    explicit PasswordManager(QObject *parent = nullptr);

    QByteArray deriveKey(const QString &password, const QByteArray &salt) const;
    QByteArray getOrCreateSalt();
    QByteArray storedSalt() const;
    void setStoredSalt(const QByteArray &salt);

    QString m_settingsPath;
    bool m_unlocked;
    QByteArray m_salt;
    QByteArray m_key;
};

#endif // PASSWORDMANAGER_H
