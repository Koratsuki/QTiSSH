#ifndef MONITORINGDIALOG_H
#define MONITORINGDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QPlainTextEdit>
#include "serverconfig.h"

/**
 * @brief Displays basic system stats (CPU load, memory, disk) for a server.
 *
 * Runs "uptime; free -m; df -h; top -bn1" through ssh (password auth uses
 * the same SSH_ASKPASS trick as the other SSH subprocesses).
 */
class QPushButton;

class MonitoringDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MonitoringDialog(const ServerConfig &config, QWidget *parent = nullptr);
    ~MonitoringDialog();

private slots:
    void onReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void refresh();

private:
    void startProcess();
    void setupAskPass();
    void cleanupAskPass();

    ServerConfig m_config;
    QProcess *m_process;
    QPlainTextEdit *m_output;
    QPushButton *m_refreshButton;
    QString m_askPassPath;
};

#endif // MONITORINGDIALOG_H
