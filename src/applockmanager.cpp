#include "applockmanager.h"
#include <QDir>
#include <QSettings>
#include <openssl/evp.h>
#include <openssl/rand.h>

namespace {
constexpr int PBKDF2_ITERATIONS = 100000;
constexpr int KEY_BYTES = 32;
constexpr int SALT_BYTES = 16;
}

AppLockManager& AppLockManager::instance()
{
    static AppLockManager inst;
    return inst;
}

AppLockManager::AppLockManager(QObject *parent)
    : QObject(parent)
    , m_settingsPath(QDir::homePath() + "/.config/QTiSSH/applock.conf")
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    m_salt = QByteArray::fromBase64(settings.value("lock/salt").toByteArray());
}

bool AppLockManager::isEnabled() const
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    return !settings.value("lock/verifier").toString().isEmpty();
}

QByteArray AppLockManager::getOrCreateSalt()
{
    if (!m_salt.isEmpty()) {
        return m_salt;
    }
    QByteArray salt(SALT_BYTES, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(salt.data()), SALT_BYTES);
    m_salt = salt;
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    settings.setValue("lock/salt", salt.toBase64());
    settings.sync();
    return m_salt;
}

QByteArray AppLockManager::deriveKey(const QString &password, const QByteArray &salt) const
{
    QByteArray key(KEY_BYTES, 0);
    PKCS5_PBKDF2_HMAC(password.toUtf8().constData(),
                      password.toUtf8().size(),
                      reinterpret_cast<const unsigned char*>(salt.constData()),
                      salt.size(),
                      PBKDF2_ITERATIONS,
                      EVP_sha256(),
                      KEY_BYTES,
                      reinterpret_cast<unsigned char*>(key.data()));
    return key;
}

bool AppLockManager::setPassword(const QString &password)
{
    if (password.isEmpty()) {
        return false;
    }

    QByteArray salt = getOrCreateSalt();
    QByteArray key = deriveKey(password, salt);

    QSettings settings(m_settingsPath, QSettings::IniFormat);
    settings.setValue("lock/verifier", key.toHex());
    settings.sync();
    return true;
}

bool AppLockManager::verify(const QString &password) const
{
    if (!isEnabled()) {
        return false;
    }

    QSettings settings(m_settingsPath, QSettings::IniFormat);
    QString storedVerifier = settings.value("lock/verifier").toString();
    if (storedVerifier.isEmpty()) {
        return false;
    }

    QByteArray salt = QByteArray::fromBase64(settings.value("lock/salt").toByteArray());
    if (salt.isEmpty()) {
        return false;
    }

    QByteArray key = deriveKey(password, salt);
    return storedVerifier == QString::fromLatin1(key.toHex());
}

void AppLockManager::remove()
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    settings.remove("lock/verifier");
    settings.remove("lock/salt");
    settings.sync();
    m_salt.clear();
}
