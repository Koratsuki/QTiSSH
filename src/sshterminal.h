#ifndef SSHTERMINAL_H
#define SSHTERMINAL_H

#include <QProcess>
#include "serverconfig.h"
#include "vt100terminal.h"

namespace Ui {
class SSHTerminal;
}

class SSHTerminal : public QWidget
{
    Q_OBJECT

public:
    explicit SSHTerminal(const ServerConfig &config, QWidget *parent = nullptr);
    ~SSHTerminal();

    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const { return m_connected; }
    ServerConfig getServerConfig() const { return m_config; }
    void setUserClosed() { m_userClosed = true; }

    QString sessionLogPath() const { return m_sessionLogPath; }

    void copy();
    void paste();
    void setTerminalFont(const QFont &font);
    void setCursorStyle(VT100Terminal::CursorStyle style);
    void setTerminalColors(const QColor &foreground, const QColor &background);
    void executeCommand(const QString &command);
    QString currentTypedLine() const;

signals:
    void connectionStateChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onInputReturnPressed();
    void onTerminalKeyPressed(const QByteArray &data);
    void onTerminalSizeChanged(int rows, int columns);

private:
    void sendCommand(const QString &command);
    QString buildSSHCommand();
    QStringList buildTunnelArguments() const;
    void addCommandToHistory(const QString &command);
    void scheduleAutoReconnect();
    void writeLog(const QString &text);
    void startSessionLog();
    void stopSessionLog();

    Ui::SSHTerminal *ui;
    ServerConfig m_config;
    QProcess *m_process;
    VT100Terminal *m_terminal;
    bool m_connected;
    bool m_waitingForPassword;
    bool m_userClosed;
    bool m_reconnectScheduled;
    int m_reconnectAttempts;
    QString m_sessionLogPath;
    QString m_outputBuffer;
    QByteArray m_inputBuffer;
    bool m_inEscapeSequence;
};

#endif // SSHTERMINAL_H
