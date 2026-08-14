#include "passwordmanager.h"
#include <QDir>
#include <QSettings>
#include <QRandomGenerator>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

namespace {
constexpr int PBKDF2_ITERATIONS = 100000;
constexpr int KEY_BYTES = 32;   // AES-256
constexpr int IV_BYTES = 16;
constexpr int SALT_BYTES = 16;
}

PasswordManager& PasswordManager::instance()
{
    static PasswordManager inst;
    return inst;
}

PasswordManager::PasswordManager(QObject *parent)
    : QObject(parent)
    , m_settingsPath(QDir::homePath() + "/.config/QTiSSH/master.conf")
    , m_unlocked(false)
{
    m_salt = storedSalt();
}

bool PasswordManager::hasMasterPassword() const
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    QString verifier = settings.value("crypto/verifier").toString();
    return !verifier.isEmpty();
}

QByteArray PasswordManager::storedSalt() const
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    return QByteArray::fromBase64(settings.value("crypto/salt").toByteArray());
}

void PasswordManager::setStoredSalt(const QByteArray &salt)
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    settings.setValue("crypto/salt", salt.toBase64());
    settings.sync();
}

QByteArray PasswordManager::getOrCreateSalt()
{
    if (!m_salt.isEmpty()) {
        return m_salt;
    }
    QByteArray salt(SALT_BYTES, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(salt.data()), SALT_BYTES);
    m_salt = salt;
    setStoredSalt(salt);
    return m_salt;
}

QByteArray PasswordManager::deriveKey(const QString &password, const QByteArray &salt) const
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

bool PasswordManager::setMasterPassword(const QString &password)
{
    if (password.isEmpty()) {
        return false;
    }

    m_salt = getOrCreateSalt();
    QByteArray key = deriveKey(password, m_salt);

    QSettings settings(m_settingsPath, QSettings::IniFormat);
    settings.setValue("crypto/verifier", key.toHex());
    settings.sync();

    m_key = key;
    m_unlocked = true;
    return true;
}

bool PasswordManager::unlock(const QString &password)
{
    if (!hasMasterPassword()) {
        return false;
    }

    m_salt = getOrCreateSalt();
    QByteArray key = deriveKey(password, m_salt);

    QSettings settings(m_settingsPath, QSettings::IniFormat);
    QString storedVerifier = settings.value("crypto/verifier").toString();
    if (storedVerifier != QString::fromLatin1(key.toHex())) {
        return false;
    }

    m_key = key;
    m_unlocked = true;
    return true;
}

void PasswordManager::lock()
{
    m_unlocked = false;
    m_key.clear();
}

void PasswordManager::removeMasterPassword()
{
    QSettings settings(m_settingsPath, QSettings::IniFormat);
    settings.remove("crypto/verifier");
    settings.remove("crypto/salt");
    settings.sync();
    lock();
}

QString PasswordManager::encrypt(const QString &plain) const
{
    if (!m_unlocked || plain.isEmpty()) {
        return plain;
    }

    QByteArray plainBytes = plain.toUtf8();
    QByteArray iv(IV_BYTES, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(iv.data()), IV_BYTES);

    QByteArray cipherBytes(plainBytes.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);
    int outLen = 0, totalLen = 0;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return plain;
    }

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(m_key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));
    EVP_EncryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(cipherBytes.data()), &outLen,
                      reinterpret_cast<const unsigned char*>(plainBytes.constData()),
                      plainBytes.size());
    totalLen = outLen;
    EVP_EncryptFinal_ex(ctx,
                        reinterpret_cast<unsigned char*>(cipherBytes.data()) + outLen, &outLen);
    totalLen += outLen;
    EVP_CIPHER_CTX_free(ctx);

    cipherBytes.truncate(totalLen);
    return storagePrefix() + QString::fromLatin1((iv + cipherBytes).toBase64());
}

QString PasswordManager::decrypt(const QString &encrypted) const
{
    if (!m_unlocked || !encrypted.startsWith(storagePrefix())) {
        return encrypted;
    }

    QByteArray payload = QByteArray::fromBase64(encrypted.mid(storagePrefix().size()).toLatin1());
    if (payload.size() < IV_BYTES) {
        return encrypted;
    }

    QByteArray iv = payload.left(IV_BYTES);
    QByteArray cipherBytes = payload.mid(IV_BYTES);
    QByteArray plainBytes(cipherBytes.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);
    int outLen = 0, totalLen = 0;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return encrypted;
    }

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(m_key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));
    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(plainBytes.data()), &outLen,
                      reinterpret_cast<const unsigned char*>(cipherBytes.constData()),
                      cipherBytes.size());
    totalLen = outLen;
    if (EVP_DecryptFinal_ex(ctx,
                            reinterpret_cast<unsigned char*>(plainBytes.data()) + outLen, &outLen) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return QString();
    }
    totalLen += outLen;
    EVP_CIPHER_CTX_free(ctx);

    plainBytes.truncate(totalLen);
    return QString::fromUtf8(plainBytes);
}
