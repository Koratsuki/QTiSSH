#include "sessionmanager.h"
#include "terminalsession.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

SessionManager::SessionManager(QObject *parent)
    : QObject(parent)
    , m_activeSession(nullptr)
{
    // Initialize session counters
    m_sessionCounters[SessionType::SSH] = 0;
    m_sessionCounters[SessionType::LocalShell] = 0;
    m_sessionCounters[SessionType::Telnet] = 0;
    m_sessionCounters[SessionType::Serial] = 0;
    m_sessionCounters[SessionType::RDP] = 0;
    m_sessionCounters[SessionType::VNC] = 0;
}

SessionManager::~SessionManager()
{
    removeAllSessions();
}

Session* SessionManager::createSession(SessionType type)
{
    QString name = generateUniqueSessionName(type);
    return createSession(type, name);
}

Session* SessionManager::createSession(SessionType type, const QString &name)
{
    Session *session = nullptr;
    
    // Create appropriate session type
    switch (type) {
    case SessionType::SSH:
    case SessionType::LocalShell:
    case SessionType::Telnet:
        // For now, all terminal-based sessions use TerminalSession
        session = new TerminalSession(type, this);
        break;
    case SessionType::Serial:
        // TODO: Implement SerialSession
        session = new TerminalSession(type, this);
        break;
    case SessionType::RDP:
        // TODO: Implement RDPSession
        qWarning() << "RDP sessions not yet implemented";
        return nullptr;
    case SessionType::VNC:
        // TODO: Implement VNCSession
        qWarning() << "VNC sessions not yet implemented";
        return nullptr;
    default:
        qWarning() << "Unknown session type:" << static_cast<int>(type);
        return nullptr;
    }
    
    if (session) {
        session->setSessionName(name);
        m_sessions[session->sessionId()] = session;
        connectSessionSignals(session);
        
        // Increment counter for this session type
        m_sessionCounters[type]++;
        
        emit sessionCreated(session);
        emit sessionCountChanged(m_sessions.size());
        
        qDebug() << "Created session:" << name << "ID:" << session->sessionId();
    }
    
    return session;
}

bool SessionManager::removeSession(const QUuid &sessionId)
{
    Session *session = getSession(sessionId);
    return removeSession(session);
}

bool SessionManager::removeSession(Session *session)
{
    if (!session || !m_sessions.contains(session->sessionId())) {
        return false;
    }
    
    QUuid sessionId = session->sessionId();
    
    // If this is the active session, clear it
    if (m_activeSession == session) {
        m_activeSession = nullptr;
        emit activeSessionChanged(nullptr, session);
    }
    
    // Disconnect signals and remove from map
    disconnectSessionSignals(session);
    m_sessions.remove(sessionId);
    
    // Delete the session
    session->deleteLater();
    
    emit sessionRemoved(sessionId);
    emit sessionCountChanged(m_sessions.size());
    
    qDebug() << "Removed session ID:" << sessionId;
    return true;
}

void SessionManager::removeAllSessions()
{
    if (m_sessions.isEmpty()) {
        return;
    }
    
    // Clear active session
    if (m_activeSession) {
        m_activeSession = nullptr;
        emit activeSessionChanged(nullptr, m_activeSession);
    }
    
    // Remove all sessions
    for (auto it = m_sessions.begin(); it != m_sessions.end(); ++it) {
        Session *session = it.value();
        disconnectSessionSignals(session);
        session->deleteLater();
    }
    
    m_sessions.clear();
    
    // Reset counters
    for (auto &counter : m_sessionCounters) {
        counter = 0;
    }
    
    emit allSessionsRemoved();
    emit sessionCountChanged(0);
    
    qDebug() << "Removed all sessions";
}

Session* SessionManager::getSession(const QUuid &sessionId) const
{
    return m_sessions.value(sessionId, nullptr);
}

QList<Session*> SessionManager::getAllSessions() const
{
    return m_sessions.values();
}

QList<Session*> SessionManager::getSessionsByType(SessionType type) const
{
    QList<Session*> result;
    for (Session *session : m_sessions.values()) {
        if (session->sessionType() == type) {
            result.append(session);
        }
    }
    return result;
}

QList<Session*> SessionManager::getConnectedSessions() const
{
    QList<Session*> result;
    for (Session *session : m_sessions.values()) {
        if (session->isConnected()) {
            result.append(session);
        }
    }
    return result;
}

int SessionManager::getSessionCount() const
{
    return m_sessions.size();
}

int SessionManager::getConnectedSessionCount() const
{
    return getConnectedSessions().size();
}

Session* SessionManager::getActiveSession() const
{
    return m_activeSession;
}

void SessionManager::setActiveSession(Session *session)
{
    if (m_activeSession != session) {
        Session *oldSession = m_activeSession;
        m_activeSession = session;
        emit activeSessionChanged(session, oldSession);
        
        if (session) {
            qDebug() << "Active session changed to:" << session->sessionName();
        } else {
            qDebug() << "Active session cleared";
        }
    }
}

void SessionManager::setActiveSession(const QUuid &sessionId)
{
    Session *session = getSession(sessionId);
    setActiveSession(session);
}

QList<Session*> SessionManager::findSessionsByName(const QString &name) const
{
    QList<Session*> result;
    for (Session *session : m_sessions.values()) {
        if (session->sessionName().contains(name, Qt::CaseInsensitive)) {
            result.append(session);
        }
    }
    return result;
}

Session* SessionManager::findSessionByName(const QString &name) const
{
    for (Session *session : m_sessions.values()) {
        if (session->sessionName() == name) {
            return session;
        }
    }
    return nullptr;
}

bool SessionManager::saveSessionsToFile(const QString &filePath) const
{
    QVariantList sessionsData = saveAllSessions();
    
    QJsonDocument doc = QJsonDocument::fromVariant(sessionsData);
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }
    
    file.write(doc.toJson());
    return true;
}

bool SessionManager::loadSessionsFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isArray()) {
        qWarning() << "Invalid session file format";
        return false;
    }
    
    return loadAllSessions(doc.toVariant().toList());
}

QVariantList SessionManager::saveAllSessions() const
{
    QVariantList result;
    for (Session *session : m_sessions.values()) {
        result.append(session->saveSession());
    }
    return result;
}

bool SessionManager::loadAllSessions(const QVariantList &sessionsData)
{
    // TODO: Implement session restoration
    // This would recreate sessions from saved data
    Q_UNUSED(sessionsData)
    qDebug() << "Session restoration not yet implemented";
    return false;
}

void SessionManager::connectSessionSignals(Session *session)
{
    if (!session) return;
    
    connect(session, &Session::sessionStateChanged,
            this, &SessionManager::onSessionStateChanged);
    connect(session, &Session::sessionError,
            this, &SessionManager::onSessionError);
    connect(session, &Session::sessionClosed,
            this, &SessionManager::onSessionClosed);
}

void SessionManager::disconnectSessionSignals(Session *session)
{
    if (!session) return;
    
    disconnect(session, &Session::sessionStateChanged,
               this, &SessionManager::onSessionStateChanged);
    disconnect(session, &Session::sessionError,
               this, &SessionManager::onSessionError);
    disconnect(session, &Session::sessionClosed,
               this, &SessionManager::onSessionClosed);
}

QString SessionManager::generateUniqueSessionName(SessionType type) const
{
    QString baseName = SessionUtils::sessionTypeToString(type);
    int counter = m_sessionCounters.value(type, 0) + 1;
    
    QString name;
    do {
        name = QString("%1 %2").arg(baseName).arg(counter);
        counter++;
    } while (findSessionByName(name) != nullptr);
    
    return name;
}

void SessionManager::onSessionStateChanged(SessionState newState, SessionState oldState)
{
    Q_UNUSED(newState)
    Q_UNUSED(oldState)
    
    Session *session = qobject_cast<Session*>(sender());
    if (session) {
        qDebug() << "Session" << session->sessionName() 
                 << "state changed from" << SessionUtils::sessionStateToString(oldState)
                 << "to" << SessionUtils::sessionStateToString(newState);
    }
}

void SessionManager::onSessionError(const QString &error)
{
    Session *session = qobject_cast<Session*>(sender());
    if (session) {
        qWarning() << "Session" << session->sessionName() << "error:" << error;
    }
}

void SessionManager::onSessionClosed()
{
    Session *session = qobject_cast<Session*>(sender());
    if (session) {
        qDebug() << "Session" << session->sessionName() << "closed";
        // Optionally auto-remove closed sessions
        // removeSession(session);
    }
}
