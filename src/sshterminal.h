#ifndef SSHTERMINAL_H
#define SSHTERMINAL_H

#include <QWidget>
#include <QProcess>
#include "serverconfig.h"

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

signals:
    void connectionStateChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onInputReturnPressed();

private:
    void sendCommand(const QString &command);
    QString buildSSHCommand();

    Ui::SSHTerminal *ui;
    ServerConfig m_config;
    QProcess *m_process;
    bool m_connected;
    bool m_waitingForPassword;
    QString m_outputBuffer;
};

#endif // SSHTERMINAL_H
