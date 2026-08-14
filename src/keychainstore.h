#ifndef KEYCHAINSTORE_H
#define KEYCHAINSTORE_H

#include <QString>

/**
 * @brief Stores passwords in the OS keychain (Linux Secret Service).
 *
 * Uses org.freedesktop.secrets through QtDBus. On platforms without a Secret
 * Service (or when it is locked/unavailable), all operations simply report
 * failure and the caller falls back to file-based encryption.
 */
class KeychainStore
{
public:
    static bool isAvailable();
    static bool store(const QString &key, const QString &password);
    static QString retrieve(const QString &key);
    static bool remove(const QString &key);

    // Marker used in servers.json to indicate "stored in the keychain".
    static QString markerPrefix() { return QStringLiteral("kc:"); }
};

#endif // KEYCHAINSTORE_H
