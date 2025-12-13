#include "terminalsession.h"
#include <QDebug>
#include <QStandardPaths>

TerminalSession::TerminalSession(SessionType type, QObject *parent)
    : Session(type, parent)
    , m_terminal(nullptr)
    , m_process(nullptr)
    , m_socket(nullptr)
    , m_port(22)  // Default SSH port
    , m_isConnected(false)
{
    setupTerminal();
    setupConnections();
    
    // Set default values based on session type
    switch (type) {
    case SessionType::SSH:
        m_port = 22;
        m_statusText = "SSH - Disconnected";
        break;
    case SessionType::Telnet:
        m_port = 23;
        m_statusText = "Telnet - Disconnected";
        break;
    case SessionType::LocalShell:
        m_statusText = "Local Shell - Disconnected";
        break;
    default:
        m_statusText = "Terminal - Disconnected";
        break;
    }
}

TerminalSession::~TerminalSession()
{
    cleanupConnection();
    delete m_terminal;
}

void TerminalSession::setupTerminal()
{
    m_terminal = new VT100Terminal();
    
    // Configure terminal for session use
    m_terminal->setScrollbackLines(1000);
    m_terminal->setCursorBlinking(true);
    
    // Set a nice monospace font
    QFont terminalFont("Consolas", 10);
    if (!terminalFont.exactMatch()) {
        terminalFont = QFont("Courier New", 10);
    }
    m_terminal->setFont(terminalFont);
}

void TerminalSession::setupConnections()
{
    if (m_terminal) {
        connect(m_terminal, &VT100Terminal::keyPressed,
                this, &TerminalSession::onTerminalKeyPressed);
        connect(m_terminal, &VT100Terminal::bell,
                this, &TerminalSession::onTerminalBell);
    }
}

void TerminalSession::connect()
{
    if (m_isConnected) {
        return;
    }
    
    setState(SessionState::Connecting);
    
    switch (sessionType()) {
    case SessionType::LocalShell:
        startLocalShell();
        break;
    case SessionType::SSH:
        startSSHConnection();
        break;
    case SessionType::Telnet:
        startTelnetConnection();
        break;
    default:
        setError("Unsupported session type");
        break;
    }
}

void TerminalSession::disconnect()
{
    if (!m_isConnected) {
        return;
    }
    
    cleanupConnection();
    setState(SessionState::Disconnected);
    m_isConnected = false;
    updateStatusText();
}

bool TerminalSession::isConnected() const
{
    return m_isConnected;
}

QWidget* TerminalSession::getDisplayWidget()
{
    return m_terminal;
}

QString TerminalSession::getStatusText() const
{
    return m_statusText;
}

QIcon TerminalSession::getSessionIcon() const
{
    return SessionUtils::getIconForSessionType(sessionType());
}

void TerminalSession::sendData(const QByteArray &data)
{
    if (!m_isConnected) {
        return;
    }
    
    if (m_process && m_process->state() == QProcess::Running) {
        m_process->write(data);
    } else if (m_socket && m_socket->state() == QTcpSocket::ConnectedState) {
        m_socket->write(data);
    }
    
    updateLastActivity();
}

void TerminalSession::sendText(const QString &text)
{
    sendData(text.toUtf8());
}

void TerminalSession::setConnectionHost(const QString &host)
{
    m_host = host;
}

void TerminalSession::setConnectionPort(int port)
{
    m_port = port;
}

void TerminalSession::setConnectionUser(const QString &user)
{
    m_user = user;
}

void TerminalSession::setConnectionPassword(const QString &password)
{
    m_password = password;
}

void TerminalSession::setConnectionCommand(const QString &command)
{
    m_command = command;
}

QVariantMap TerminalSession::saveSession() const
{
    QVariantMap data = Session::saveSession();
    data["host"] = m_host;
    data["port"] = m_port;
    data["user"] = m_user;
    data["command"] = m_command;
    // Note: We don't save passwords for security
    return data;
}

bool TerminalSession::loadSession(const QVariantMap &data)
{
    if (!Session::loadSession(data)) {
        return false;
    }
    
    m_host = data.value("host").toString();
    m_port = data.value("port", 22).toInt();
    m_user = data.value("user").toString();
    m_command = data.value("command").toString();
    
    return true;
}

void TerminalSession::startLocalShell()
{
    if (m_process) {
        delete m_process;
    }
    
    m_process = new QProcess(this);
    
    connect(m_process, &QProcess::started,
            this, &TerminalSession::onProcessStarted);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &TerminalSession::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred,
            this, &TerminalSession::onProcessError);
    connect(m_process, &QProcess::readyRead,
            this, &TerminalSession::onProcessReadyRead);
    
    // Start appropriate shell based on platform
#ifdef Q_OS_WIN
    QString shell = "cmd.exe";
    QStringList args;
#else
    QString shell = qEnvironmentVariable("SHELL", "/bin/bash");
    QStringList args;
    if (!m_command.isEmpty()) {
        args << "-c" << m_command;
    }
#endif
    
    m_process->start(shell, args);
    
    if (!m_process->waitForStarted(3000)) {
        setError("Failed to start local shell");
    }
}

void TerminalSession::startSSHConnection()
{
    if (m_host.isEmpty()) {
        setError("SSH host not specified");
        return;
    }
    
    if (m_process) {
        delete m_process;
    }
    
    m_process = new QProcess(this);
    
    connect(m_process, &QProcess::started,
            this, &TerminalSession::onProcessStarted);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &TerminalSession::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred,
            this, &TerminalSession::onProcessError);
    connect(m_process, &QProcess::readyRead,
            this, &TerminalSession::onProcessReadyRead);
    
    // Build SSH command
    QStringList args;
    args << "-o" << "StrictHostKeyChecking=no";  // For demo purposes
    
    if (m_port != 22) {
        args << "-p" << QString::number(m_port);
    }
    
    QString target = m_host;
    if (!m_user.isEmpty()) {
        target = m_user + "@" + m_host;
    }
    args << target;
    
    if (!m_command.isEmpty()) {
        args << m_command;
    }
    
    m_process->start("ssh", args);
    
    if (!m_process->waitForStarted(3000)) {
        setError("Failed to start SSH process");
    }
}

void TerminalSession::startTelnetConnection()
{
    if (m_host.isEmpty()) {
        setError("Telnet host not specified");
        return;
    }
    
    if (m_socket) {
        delete m_socket;
    }
    
    m_socket = new QTcpSocket(this);
    
    connect(m_socket, &QTcpSocket::connected,
            this, &TerminalSession::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected,
            this, &TerminalSession::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::errorOccurred,
            this, &TerminalSession::onSocketError);
    connect(m_socket, &QTcpSocket::readyRead,
            this, &TerminalSession::onSocketReadyRead);
    
    m_socket->connectToHost(m_host, m_port);
}

void TerminalSession::cleanupConnection()
{
    if (m_process) {
        if (m_process->state() != QProcess::NotRunning) {
            m_process->kill();
            m_process->waitForFinished(3000);
        }
        m_process->deleteLater();
        m_process = nullptr;
    }
    
    if (m_socket) {
        if (m_socket->state() != QTcpSocket::UnconnectedState) {
            m_socket->disconnectFromHost();
            m_socket->waitForDisconnected(3000);
        }
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

void TerminalSession::updateStatusText()
{
    QString typeStr = SessionUtils::sessionTypeToString(sessionType());
    QString stateStr = SessionUtils::sessionStateToString(sessionState());
    
    if (sessionState() == SessionState::Connected && !m_host.isEmpty()) {
        m_statusText = QString("%1 - %2:%3 - %4").arg(typeStr, m_host, QString::number(m_port), stateStr);
    } else {
        m_statusText = QString("%1 - %2").arg(typeStr, stateStr);
    }
}

// Slot implementations
void TerminalSession::onTerminalKeyPressed(const QByteArray &data)
{
    sendData(data);
}

void TerminalSession::onTerminalBell()
{
    // Handle terminal bell - could play sound, flash window, etc.
    qDebug() << "Terminal bell in session:" << sessionName();
}

void TerminalSession::onProcessStarted()
{
    m_isConnected = true;
    setState(SessionState::Connected);
    updateStatusText();
    
    qDebug() << "Process started for session:" << sessionName();
}

void TerminalSession::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)
    
    m_isConnected = false;
    setState(SessionState::Disconnected);
    updateStatusText();
    
    if (m_terminal) {
        m_terminal->writeData("\r\n[Process finished]\r\n");
    }
    
    qDebug() << "Process finished for session:" << sessionName();
}

void TerminalSession::onProcessError(QProcess::ProcessError error)
{
    QString errorStr;
    switch (error) {
    case QProcess::FailedToStart:
        errorStr = "Failed to start process";
        break;
    case QProcess::Crashed:
        errorStr = "Process crashed";
        break;
    case QProcess::Timedout:
        errorStr = "Process timed out";
        break;
    default:
        errorStr = "Unknown process error";
        break;
    }
    
    setError(errorStr);
    m_isConnected = false;
    updateStatusText();
}

void TerminalSession::onProcessReadyRead()
{
    if (m_process && m_terminal) {
        QByteArray data = m_process->readAll();
        m_terminal->writeData(data);
        updateLastActivity();
    }
}

void TerminalSession::onSocketConnected()
{
    m_isConnected = true;
    setState(SessionState::Connected);
    updateStatusText();
    
    qDebug() << "Socket connected for session:" << sessionName();
}

void TerminalSession::onSocketDisconnected()
{
    m_isConnected = false;
    setState(SessionState::Disconnected);
    updateStatusText();
    
    if (m_terminal) {
        m_terminal->writeData("\r\n[Connection closed]\r\n");
    }
    
    qDebug() << "Socket disconnected for session:" << sessionName();
}

void TerminalSession::onSocketError(QAbstractSocket::SocketError error)
{
    QString errorStr = m_socket ? m_socket->errorString() : "Socket error";
    setError(errorStr);
    m_isConnected = false;
    updateStatusText();
}

void TerminalSession::onSocketReadyRead()
{
    if (m_socket && m_terminal) {
        QByteArray data = m_socket->readAll();
        m_terminal->writeData(data);
        updateLastActivity();
    }
}
