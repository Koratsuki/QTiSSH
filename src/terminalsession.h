#ifndef TERMINALSESSION_H
#define TERMINALSESSION_H

#include "session.h"
#include "vt100terminal.h"
#include <QProcess>
#include <QTcpSocket>

/**
 * @brief Terminal-based session implementation
 * 
 * This class integrates the VT100Terminal with various connection types
 * like SSH, local shell, telnet, etc.
 */
class TerminalSession : public Session
{
    Q_OBJECT

public:
    explicit TerminalSession(SessionType type, QObject *parent = nullptr);
    ~TerminalSession();

    // Session interface implementation
    void connect() override;
    void disconnect() override;
    bool isConnected() const override;
    
    // UI integration
    QWidget* getDisplayWidget() override;
    QString getStatusText() const override;
    QIcon getSessionIcon() const override;
    
    // Terminal-specific methods
    VT100Terminal* getTerminal() const { return m_terminal; }
    void sendData(const QByteArray &data);
    void sendText(const QString &text);
    
    // Connection configuration
    void setConnectionHost(const QString &host);
    void setConnectionPort(int port);
    void setConnectionUser(const QString &user);
    void setConnectionPassword(const QString &password);
    void setConnectionCommand(const QString &command);
    
    QString connectionHost() const { return m_host; }
    int connectionPort() const { return m_port; }
    QString connectionUser() const { return m_user; }
    QString connectionCommand() const { return m_command; }

    // Session persistence
    QVariantMap saveSession() const override;
    bool loadSession(const QVariantMap &data) override;

private slots:
    void onTerminalKeyPressed(const QByteArray &data);
    void onTerminalBell();
    void onProcessStarted();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onProcessReadyRead();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void onSocketReadyRead();

private:
    void setupTerminal();
    void setupConnections();
    void startLocalShell();
    void startSSHConnection();
    void startTelnetConnection();
    void cleanupConnection();
    void updateStatusText();
    
    // UI components
    VT100Terminal *m_terminal;
    
    // Connection handling
    QProcess *m_process;        // For local shell and SSH via external process
    QTcpSocket *m_socket;       // For telnet and raw connections
    
    // Connection parameters
    QString m_host;
    int m_port;
    QString m_user;
    QString m_password;
    QString m_command;
    
    // State tracking
    bool m_isConnected;
    QString m_statusText;
};

#endif // TERMINALSESSION_H
