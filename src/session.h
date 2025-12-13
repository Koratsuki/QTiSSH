#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

/**
 * @brief Session types supported by the application
 */
enum class SessionType {
    SSH,
    LocalShell,
    Telnet,
    Serial,
    RDP,
    VNC
};

/**
 * @brief Session state enumeration
 */
enum class SessionState {
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Error
};

/**
 * @brief Base class for all session types
 * 
 * This abstract class defines the interface for all session types
 * and provides common functionality like state management, metadata, etc.
 */
class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(SessionType type, QObject *parent = nullptr);
    virtual ~Session();

    // Session identification
    QUuid sessionId() const { return m_sessionId; }
    QString sessionName() const { return m_sessionName; }
    void setSessionName(const QString &name);
    
    // Session type and state
    SessionType sessionType() const { return m_sessionType; }
    SessionState sessionState() const { return m_sessionState; }
    
    // Session metadata
    QDateTime createdAt() const { return m_createdAt; }
    QDateTime lastActiveAt() const { return m_lastActiveAt; }
    QString lastError() const { return m_lastError; }
    
    // Session control - pure virtual methods
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void reconnect();
    virtual bool isConnected() const = 0;
    
    // UI integration
    virtual QWidget* getDisplayWidget() = 0;
    virtual QString getStatusText() const = 0;
    virtual QIcon getSessionIcon() const = 0;
    
    // Session persistence
    virtual QVariantMap saveSession() const;
    virtual bool loadSession(const QVariantMap &data);

signals:
    void sessionStateChanged(SessionState newState, SessionState oldState);
    void sessionNameChanged(const QString &newName);
    void sessionError(const QString &error);
    void sessionClosed();
    void activityDetected();

protected:
    void setState(SessionState newState);
    void setError(const QString &error);
    void updateLastActivity();
    
    // Session data
    QUuid m_sessionId;
    QString m_sessionName;
    SessionType m_sessionType;
    SessionState m_sessionState;
    QDateTime m_createdAt;
    QDateTime m_lastActiveAt;
    QString m_lastError;
};

/**
 * @brief Helper functions for session types
 */
class SessionUtils
{
public:
    static QString sessionTypeToString(SessionType type);
    static SessionType sessionTypeFromString(const QString &typeStr);
    static QString sessionStateToString(SessionState state);
    static QIcon getIconForSessionType(SessionType type);
};

#endif // SESSION_H
