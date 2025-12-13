#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QUuid>
#include "session.h"

/**
 * @brief Manages all active sessions in the application
 * 
 * The SessionManager is responsible for:
 * - Creating and destroying sessions
 * - Tracking active sessions
 * - Session persistence and restoration
 * - Session lifecycle management
 */
class SessionManager : public QObject
{
    Q_OBJECT

public:
    explicit SessionManager(QObject *parent = nullptr);
    ~SessionManager();

    // Session creation and management
    Session* createSession(SessionType type);
    Session* createSession(SessionType type, const QString &name);
    bool removeSession(const QUuid &sessionId);
    bool removeSession(Session *session);
    void removeAllSessions();

    // Session access
    Session* getSession(const QUuid &sessionId) const;
    QList<Session*> getAllSessions() const;
    QList<Session*> getSessionsByType(SessionType type) const;
    QList<Session*> getConnectedSessions() const;
    int getSessionCount() const;
    int getConnectedSessionCount() const;

    // Active session management
    Session* getActiveSession() const;
    void setActiveSession(Session *session);
    void setActiveSession(const QUuid &sessionId);

    // Session persistence
    bool saveSessionsToFile(const QString &filePath) const;
    bool loadSessionsFromFile(const QString &filePath);
    QVariantList saveAllSessions() const;
    bool loadAllSessions(const QVariantList &sessionsData);

    // Session search and filtering
    QList<Session*> findSessionsByName(const QString &name) const;
    Session* findSessionByName(const QString &name) const;

signals:
    void sessionCreated(Session *session);
    void sessionRemoved(const QUuid &sessionId);
    void activeSessionChanged(Session *newSession, Session *oldSession);
    void sessionCountChanged(int count);
    void allSessionsRemoved();

private slots:
    void onSessionStateChanged(SessionState newState, SessionState oldState);
    void onSessionError(const QString &error);
    void onSessionClosed();

private:
    void connectSessionSignals(Session *session);
    void disconnectSessionSignals(Session *session);
    QString generateUniqueSessionName(SessionType type) const;

    // Session storage
    QMap<QUuid, Session*> m_sessions;
    Session *m_activeSession;
    
    // Session counters for unique naming
    QMap<SessionType, int> m_sessionCounters;
};

#endif // SESSIONMANAGER_H
