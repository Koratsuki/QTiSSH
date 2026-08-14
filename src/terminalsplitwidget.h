#ifndef TERMINALSPLITWIDGET_H
#define TERMINALSPLITWIDGET_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <QDateTime>
#include <QProcess>
#include "serverconfig.h"
#include "vt100terminal.h"
#include "servermonitoringbar.h"

class SSHTerminal;
class QSplitter;
class ServerMonitoringBar;

/**
 * @brief Container that hosts one or more SSH terminals side by side.
 *
 * Terminals share the same connection settings; "splitting" opens an extra
 * terminal to the same server. The most recently focused terminal is exposed
 * through activeTerminal().
 */
class TerminalSplitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TerminalSplitWidget(const ServerConfig &config, QWidget *parent = nullptr);
    ~TerminalSplitWidget();

    SSHTerminal *activeTerminal() const;
    SSHTerminal *terminalAt(int index) const;
    QList<SSHTerminal*> terminals() const { return m_terminals; }
    int count() const { return m_terminals.size(); }
    ServerMonitoringBar *monitoringBar() const { return m_monitoringBar; }

    void splitTerminal(Qt::Orientation orientation);
    void closeActiveTerminal();
    void closeTerminal(SSHTerminal *terminal);
    void connectToServer();

    void applySettings(const QFont &font,
                       VT100Terminal::CursorStyle style,
                       const QColor &foreground,
                       const QColor &background);

signals:
    void activeTerminalChanged(SSHTerminal *terminal);
    void allClosed();

private slots:
    void fetchMetrics();
    void onMetricsProcessFinished(int exitCode, int exitStatus);
    void onMetricsProcessError(int error);
    void onMetricsReadyRead();
    void onTerminalConnectionChanged(bool connected);

private:
    SSHTerminal *createTerminal();
    void setActive(SSHTerminal *terminal);
    void startMetricsProcess();
    void setupMetricsAskPass();
    void cleanupMetricsAskPass();
    ServerMetrics parseMetricsOutput(const QString &output);
    
    QSplitter *m_splitter;
    QVBoxLayout *m_mainLayout;
    QList<SSHTerminal*> m_terminals;
    SSHTerminal *m_activeTerminal;
    ServerConfig m_config;
    ServerMonitoringBar *m_monitoringBar;
    QProcess *m_metricsProcess;
    QString m_metricsOutputBuffer;
    QString m_metricsAskPassPath;
};

#endif // TERMINALSPLITWIDGET_H
