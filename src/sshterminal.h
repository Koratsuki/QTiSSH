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

    void copy();
    void paste();
    void setTerminalFont(const QFont &font);
    void setCursorStyle(VT100Terminal::CursorStyle style);

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

    Ui::SSHTerminal *ui;
    ServerConfig m_config;
    QProcess *m_process;
    VT100Terminal *m_terminal;
    bool m_connected;
    bool m_waitingForPassword;
    QString m_outputBuffer;
};

#endif // SSHTERMINAL_H
