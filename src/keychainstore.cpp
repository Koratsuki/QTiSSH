#include "keychainstore.h"
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDBusArgument>
#include <QDBusVariant>
#include <QDBusMetaType>
#include <QVariantMap>
#include <QMap>
#include <QDBusMessage>

namespace {
const QString SERVICE = QStringLiteral("org.freedesktop.secrets");
const QString PATH = QStringLiteral("/org/freedesktop/secrets");
const QString IFACE = QStringLiteral("org.freedesktop.Secret.Service");

QMap<QString, QString> attributesFor(const QString &key)
{
    QMap<QString, QString> attrs;
    attrs.insert("application", "QTiSSH");
    attrs.insert("key", key);
    return attrs;
}

QString defaultCollectionPath()
{
    QDBusInterface service(SERVICE, PATH, IFACE);
    QDBusReply<QDBusObjectPath> reply = service.call("ReadAlias", "default");
    if (reply.isValid() && !reply.value().path().isEmpty()) {
        return reply.value().path();
    }

    // No default collection: try to create one.
    QVariantMap props;
    props.insert("org.freedesktop.Secret.Collection.Label", "QTiSSH");
    QDBusMessage msg = QDBusMessage::createMethodCall(
        SERVICE, PATH, IFACE, "CreateCollection");
    msg << QVariant::fromValue(props) << "default";
    QDBusMessage replyMsg = QDBusConnection::sessionBus().call(msg);
    if (replyMsg.type() == QDBusMessage::ErrorMessage) {
        return QString();
    }
    if (replyMsg.arguments().size() >= 1) {
        const QDBusObjectPath collection = replyMsg.arguments().at(0).value<QDBusObjectPath>();
        return collection.path();
    }
    return QString();
}
} // namespace

bool KeychainStore::isAvailable()
{
    static bool checked = false;
    static bool available = false;
    if (!checked) {
        checked = true;
        available = QDBusConnection::sessionBus().isConnected() &&
                    QDBusConnection::sessionBus().interface()->isServiceRegistered(SERVICE);
    }
    return available;
}

bool KeychainStore::store(const QString &key, const QString &password)
{
    if (!isAvailable() || key.isEmpty()) {
        return false;
    }

    const QString collection = defaultCollectionPath();
    if (collection.isEmpty()) {
        return false;
    }

    // Remove any existing item first (CreateItem with replace can also do this,
    // but explicit removal keeps the attribute list clean).
    remove(key);

    // Open a session (plain mode is enough for a local app).
    QDBusMessage openMsg = QDBusMessage::createMethodCall(SERVICE, PATH, IFACE, "OpenSession");
    openMsg << "plain" << QVariant::fromValue(QByteArray());
    QDBusMessage openReply = QDBusConnection::sessionBus().call(openMsg);
    if (openReply.type() == QDBusMessage::ErrorMessage || openReply.arguments().isEmpty()) {
        return false;
    }

    // Reply is (os): session object path + output parameters.
    QDBusArgument sessionArg = openReply.arguments().at(0).value<QDBusArgument>();
    sessionArg.beginStructure();
    QDBusObjectPath session;
    QByteArray output;
    sessionArg >> session;
    sessionArg >> output;
    sessionArg.endStructure();

    // Secret structure: (session, parameters, value)
    QDBusArgument secret;
    secret.beginStructure();
    secret << QDBusObjectPath(session);
    secret << QByteArray();
    secret << password.toUtf8();
    secret.endStructure();

    QVariantMap properties;
    properties.insert("org.freedesktop.Secret.Item.Label", QVariant("QTiSSH: " + key));
    properties.insert("org.freedesktop.Secret.Item.Attributes", QVariant::fromValue(attributesFor(key)));

    QDBusMessage msg = QDBusMessage::createMethodCall(
        SERVICE, collection, "org.freedesktop.Secret.Collection", "CreateItem");
    msg << QVariant::fromValue(properties)
        << QVariant::fromValue(secret)
        << true;
    QDBusMessage reply = QDBusConnection::sessionBus().call(msg);
    return reply.type() != QDBusMessage::ErrorMessage;
}

QString KeychainStore::retrieve(const QString &key)
{
    if (!isAvailable() || key.isEmpty()) {
        return QString();
    }

    const QString collection = defaultCollectionPath();
    if (collection.isEmpty()) {
        return QString();
    }

    QDBusMessage search = QDBusMessage::createMethodCall(
        SERVICE, collection, "org.freedesktop.Secret.Collection", "SearchItems");
    search << QVariant::fromValue(attributesFor(key));
    QDBusMessage searchReply = QDBusConnection::sessionBus().call(search);
    if (searchReply.type() == QDBusMessage::ErrorMessage || searchReply.arguments().isEmpty()) {
        return QString();
    }

    QList<QDBusObjectPath> paths = qdbus_cast<QList<QDBusObjectPath>>(
        searchReply.arguments().at(0));
    if (paths.isEmpty()) {
        return QString();
    }

    // Secret is a D-Bus property; query it through org.freedesktop.DBus.Properties.
    QDBusMessage getMsg = QDBusMessage::createMethodCall(
        SERVICE, paths.first().path(), "org.freedesktop.DBus.Properties", "Get");
    getMsg << "org.freedesktop.Secret.Item" << "Secret";
    QDBusMessage getReply = QDBusConnection::sessionBus().call(getMsg);
    if (getReply.type() == QDBusMessage::ErrorMessage || getReply.arguments().isEmpty()) {
        return QString();
    }

    QVariant variant = qvariant_cast<QDBusVariant>(getReply.arguments().at(0)).variant();
    QDBusArgument arg = qvariant_cast<QDBusArgument>(variant);
    arg.beginStructure();
    QDBusObjectPath session;
    QByteArray parameters;
    QByteArray secretValue;
    arg >> session;
    arg >> parameters;
    arg >> secretValue;
    arg.endStructure();

    return QString::fromUtf8(secretValue);
}

bool KeychainStore::remove(const QString &key)
{
    if (!isAvailable() || key.isEmpty()) {
        return false;
    }

    const QString collection = defaultCollectionPath();
    if (collection.isEmpty()) {
        return false;
    }

    QDBusMessage search = QDBusMessage::createMethodCall(
        SERVICE, collection, "org.freedesktop.Secret.Collection", "SearchItems");
    search << QVariant::fromValue(attributesFor(key));
    QDBusMessage searchReply = QDBusConnection::sessionBus().call(search);
    if (searchReply.type() == QDBusMessage::ErrorMessage || searchReply.arguments().isEmpty()) {
        return false;
    }

    QList<QDBusObjectPath> paths = qdbus_cast<QList<QDBusObjectPath>>(
        searchReply.arguments().at(0));
    if (paths.isEmpty()) {
        return false;
    }

    for (const QDBusObjectPath &path : paths) {
        QDBusMessage removeMsg = QDBusMessage::createMethodCall(
            SERVICE, collection, "org.freedesktop.Secret.Collection", "DeleteItem");
        removeMsg << QVariant::fromValue(path);
        QDBusConnection::sessionBus().call(removeMsg);
    }
    return true;
}
