#include "session.h"
#include <QIcon>
#include <QDebug>

Session::Session(SessionType type, QObject *parent)
    : QObject(parent)
    , m_sessionId(QUuid::createUuid())
    , m_sessionType(type)
    , m_sessionState(SessionState::Disconnected)
    , m_createdAt(QDateTime::currentDateTime())
    , m_lastActiveAt(QDateTime::currentDateTime())
{
    // Set default session name based on type
    m_sessionName = QString("%1 Session").arg(SessionUtils::sessionTypeToString(type));
}

Session::~Session()
{
    // Ensure proper cleanup
    if (isConnected()) {
        disconnect();
    }
}

void Session::setSessionName(const QString &name)
{
    if (m_sessionName != name) {
        m_sessionName = name;
        emit sessionNameChanged(name);
    }
}

void Session::reconnect()
{
    if (m_sessionState == SessionState::Connected) {
        disconnect();
    }
    connect();
}

void Session::setState(SessionState newState)
{
    if (m_sessionState != newState) {
        SessionState oldState = m_sessionState;
        m_sessionState = newState;
        updateLastActivity();
        emit sessionStateChanged(newState, oldState);
    }
}

void Session::setError(const QString &error)
{
    m_lastError = error;
    setState(SessionState::Error);
    emit sessionError(error);
}

void Session::updateLastActivity()
{
    m_lastActiveAt = QDateTime::currentDateTime();
    emit activityDetected();
}

QVariantMap Session::saveSession() const
{
    QVariantMap data;
    data["sessionId"] = m_sessionId.toString();
    data["sessionName"] = m_sessionName;
    data["sessionType"] = static_cast<int>(m_sessionType);
    data["createdAt"] = m_createdAt;
    data["lastActiveAt"] = m_lastActiveAt;
    return data;
}

bool Session::loadSession(const QVariantMap &data)
{
    if (!data.contains("sessionId") || !data.contains("sessionType")) {
        return false;
    }
    
    m_sessionId = QUuid::fromString(data["sessionId"].toString());
    m_sessionName = data.value("sessionName", m_sessionName).toString();
    m_sessionType = static_cast<SessionType>(data["sessionType"].toInt());
    m_createdAt = data.value("createdAt", m_createdAt).toDateTime();
    m_lastActiveAt = data.value("lastActiveAt", m_lastActiveAt).toDateTime();
    
    return true;
}

// SessionUtils implementation
QString SessionUtils::sessionTypeToString(SessionType type)
{
    switch (type) {
    case SessionType::SSH:
        return "SSH";
    case SessionType::LocalShell:
        return "Local Shell";
    case SessionType::Telnet:
        return "Telnet";
    case SessionType::Serial:
        return "Serial";
    case SessionType::RDP:
        return "RDP";
    case SessionType::VNC:
        return "VNC";
    default:
        return "Unknown";
    }
}

SessionType SessionUtils::sessionTypeFromString(const QString &typeStr)
{
    if (typeStr == "SSH") return SessionType::SSH;
    if (typeStr == "Local Shell") return SessionType::LocalShell;
    if (typeStr == "Telnet") return SessionType::Telnet;
    if (typeStr == "Serial") return SessionType::Serial;
    if (typeStr == "RDP") return SessionType::RDP;
    if (typeStr == "VNC") return SessionType::VNC;
    return SessionType::SSH; // Default
}

QString SessionUtils::sessionStateToString(SessionState state)
{
    switch (state) {
    case SessionState::Disconnected:
        return "Disconnected";
    case SessionState::Connecting:
        return "Connecting";
    case SessionState::Connected:
        return "Connected";
    case SessionState::Reconnecting:
        return "Reconnecting";
    case SessionState::Error:
        return "Error";
    default:
        return "Unknown";
    }
}

QIcon SessionUtils::getIconForSessionType(SessionType type)
{
    // For now, return empty icons - we'll add proper icons later
    switch (type) {
    case SessionType::SSH:
        return QIcon(":/icons/ssh.png");
    case SessionType::LocalShell:
        return QIcon(":/icons/terminal.png");
    case SessionType::Telnet:
        return QIcon(":/icons/telnet.png");
    case SessionType::Serial:
        return QIcon(":/icons/serial.png");
    case SessionType::RDP:
        return QIcon(":/icons/rdp.png");
    case SessionType::VNC:
        return QIcon(":/icons/vnc.png");
    default:
        return QIcon(":/icons/session.png");
    }
}
