#include "terminalsplitwidget.h"
#include "sshterminal.h"
#include "passwordmanager.h"
#include "servermonitoringbar.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QApplication>
#include <QEvent>
#include <QProcess>
#include <QUuid>
#include <QDir>
#include <QFile>
#include <QProcessEnvironment>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TerminalSplitWidget::TerminalSplitWidget(const ServerConfig &config, QWidget *parent)
    : QWidget(parent)
    , m_splitter(new QSplitter(this))
    , m_mainLayout(new QVBoxLayout(this))
    , m_activeTerminal(nullptr)
    , m_config(config)
    , m_monitoringBar(new ServerMonitoringBar(config, this))
    , m_metricsProcess(nullptr)
{
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_splitter);
    m_mainLayout->addWidget(m_monitoringBar);

    SSHTerminal *terminal = createTerminal();
    m_splitter->addWidget(terminal);
    setActive(terminal);

    // Track the most recently focused terminal.
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget *, QWidget *now) {
        SSHTerminal *terminal = nullptr;
        QWidget *w = now;
        while (w) {
            terminal = qobject_cast<SSHTerminal*>(w);
            if (terminal) {
                break;
            }
            w = w->parentWidget();
        }
        if (terminal && m_terminals.contains(terminal) && terminal != m_activeTerminal) {
            setActive(terminal);
        }
    });

    // Connect monitoring bar signals
    connect(m_monitoringBar, &ServerMonitoringBar::metricsRequested, this, &TerminalSplitWidget::fetchMetrics);
    connect(m_monitoringBar, &ServerMonitoringBar::errorOccurred, this, [this](const QString &error) {
        qDebug() << "Monitoring error:" << error;
    });
    
    // Connect to terminal connection state changes
    connect(terminal, &SSHTerminal::connectionStateChanged, this, &TerminalSplitWidget::onTerminalConnectionChanged);
}

SSHTerminal *TerminalSplitWidget::createTerminal()
{
    SSHTerminal *terminal = new SSHTerminal(m_config, this);
    m_terminals.append(terminal);
    return terminal;
}

SSHTerminal *TerminalSplitWidget::activeTerminal() const
{
    return m_activeTerminal;
}

SSHTerminal *TerminalSplitWidget::terminalAt(int index) const
{
    return m_terminals.value(index, nullptr);
}

void TerminalSplitWidget::setActive(SSHTerminal *terminal)
{
    if (m_activeTerminal == terminal) {
        return;
    }
    m_activeTerminal = terminal;
    emit activeTerminalChanged(terminal);
    
    // Ensure the VT100Terminal gets focus for keyboard input
    if (terminal) {
        terminal->focusTerminal();
    }
}

void TerminalSplitWidget::splitTerminal(Qt::Orientation orientation)
{
    if (!m_activeTerminal) {
        return;
    }

    SSHTerminal *terminal = createTerminal();

    // Insert next to the active terminal.
    int index = m_splitter->indexOf(m_activeTerminal);
    m_splitter->insertWidget(index + 1, terminal);

    m_splitter->setOrientation(orientation);

    // Balance the split.
    QList<int> sizes;
    const int total = m_splitter->count();
    for (int i = 0; i < total; ++i) {
        sizes.append(1000 / total);
    }
    m_splitter->setSizes(sizes);

    // Connect to connection state changes for the new terminal
    connect(terminal, &SSHTerminal::connectionStateChanged, this, &TerminalSplitWidget::onTerminalConnectionChanged);

    terminal->connectToServer();
}

void TerminalSplitWidget::connectToServer()
{
    for (SSHTerminal *terminal : m_terminals) {
        terminal->connectToServer();
    }
}

void TerminalSplitWidget::closeTerminal(SSHTerminal *terminal)
{
    if (!terminal || !m_terminals.contains(terminal)) {
        return;
    }
    m_terminals.removeAll(terminal);
    m_splitter->indexOf(terminal);
    terminal->setParent(nullptr);
    terminal->setUserClosed();
    terminal->deleteLater();

    if (m_terminals.isEmpty()) {
        m_activeTerminal = nullptr;
        emit allClosed();
        return;
    }

    if (m_activeTerminal == terminal) {
        setActive(m_terminals.first());
    }
}

void TerminalSplitWidget::closeActiveTerminal()
{
    if (m_activeTerminal) {
        closeTerminal(m_activeTerminal);
    }
}

void TerminalSplitWidget::applySettings(const QFont &font,
                                        VT100Terminal::CursorStyle style,
                                        const QColor &foreground,
                                        const QColor &background)
{
    for (SSHTerminal *terminal : m_terminals) {
        terminal->setTerminalFont(font);
        terminal->setCursorStyle(style);
        terminal->setTerminalColors(foreground, background);
    }
}

void TerminalSplitWidget::fetchMetrics()
{
    if (m_metricsProcess && m_metricsProcess->state() == QProcess::Running) {
        return;
    }
    
    m_metricsProcess = new QProcess(this);
    connect(m_metricsProcess, &QProcess::readyReadStandardOutput, this, &TerminalSplitWidget::onMetricsReadyRead);
    connect(m_metricsProcess, &QProcess::readyReadStandardError, this, &TerminalSplitWidget::onMetricsReadyRead);
    connect(m_metricsProcess, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &TerminalSplitWidget::onMetricsProcessFinished);
    connect(m_metricsProcess, &QProcess::errorOccurred, this, &TerminalSplitWidget::onMetricsProcessError);
    
    m_metricsOutputBuffer.clear();
    setupMetricsAskPass();
    startMetricsProcess();
}

void TerminalSplitWidget::startMetricsProcess()
{
    QStringList args;
    args << "-p" << QString::number(m_config.port());
    
    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        args << "-i" << m_config.keyPath();
    }
    if (!m_config.strictHostKeyChecking()) {
        args << "-o" << "StrictHostKeyChecking=no";
        args << "-o" << "UserKnownHostsFile=/dev/null";
    } else {
        args << "-o" << "StrictHostKeyChecking=yes";
    }
    if (!m_config.jumpHost().isEmpty()) {
        args << "-J" << m_config.jumpHost();
    }
    if (m_config.forwardAgent() || m_config.authType() == AuthType::SSHAgent) {
        args << "-o" << "ForwardAgent=yes";
    }
    args << m_config.sshOptionArgs();
    
    QString command = R"SCRIPT(
        cpu=$(top -bn1 | grep "Cpu(s)" | awk '{print $2}' | sed 's/%us,//; s/%sy,//; s/%ni,//; s/%id,//; s/%wa,//; s/%hi,//; s/%si,//; s/%st,//' | awk '{sum+=$1} END {print sum}')
        mem_info=$(free -b | awk '/Mem:/ {print $2 "," $3}')
        net_info=$(cat /proc/net/dev | awk 'NR>2 {rx+=$2; tx+=$10} END {print rx "," tx}')
        up=$(uptime -p | sed 's/up //')
        users=$(who | wc -l)
        echo "CPU:$cpu"
        echo "MEM:$mem_info"
        echo "NET:$net_info"
        echo "UP:$up"
        echo "USERS:$users"
    )SCRIPT";
    
    args << QString("%1@%2").arg(m_config.username(), m_config.host()) << command;
    
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (!m_metricsAskPassPath.isEmpty()) {
        env.insert("SSH_ASKPASS", m_metricsAskPassPath);
        env.insert("SSH_ASKPASS_REQUIRE", "force");
        env.insert("DISPLAY", ":0");
    }
    m_metricsProcess->setProcessEnvironment(env);
    m_metricsProcess->start("ssh", args);
}

void TerminalSplitWidget::setupMetricsAskPass()
{
    cleanupMetricsAskPass();
    QString pwd = m_config.password();
    if (m_config.authType() != AuthType::Password || pwd.isEmpty()
        || pwd.startsWith(PasswordManager::instance().storagePrefix())) {
        return;
    }
    
    QString escaped = pwd;
    escaped.replace("'", "'\\''");
    
    m_metricsAskPassPath = QDir::temp().filePath(
        "qtissh-metrics-askpass-" + QUuid::createUuid().toString(QUuid::WithoutBraces));
    QFile file(m_metricsAskPassPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write("#!/bin/sh\n");
        file.write("echo '" + escaped.toUtf8() + "'\n");
        file.close();
        QFile::setPermissions(m_metricsAskPassPath,
                              QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
    }
}

void TerminalSplitWidget::cleanupMetricsAskPass()
{
    if (!m_metricsAskPassPath.isEmpty()) {
        QFile::remove(m_metricsAskPassPath);
        m_metricsAskPassPath.clear();
    }
}

void TerminalSplitWidget::onMetricsReadyRead()
{
    if (m_metricsProcess) {
        m_metricsOutputBuffer += QString::fromUtf8(m_metricsProcess->readAllStandardOutput());
        m_metricsOutputBuffer += QString::fromUtf8(m_metricsProcess->readAllStandardError());
    }
}

void TerminalSplitWidget::onMetricsProcessFinished(int exitCode, int exitStatus)
{
    Q_UNUSED(exitStatus)
    cleanupMetricsAskPass();
    
    if (exitCode == 0) {
        ServerMetrics metrics = parseMetricsOutput(m_metricsOutputBuffer);
        m_monitoringBar->updateMetrics(metrics);
    } else {
        qDebug() << "Metrics fetch failed with exit code:" << exitCode;
        qDebug() << "Output:" << m_metricsOutputBuffer;
    }
    
    m_metricsProcess->deleteLater();
    m_metricsProcess = nullptr;
}

void TerminalSplitWidget::onMetricsProcessError(int error)
{
    cleanupMetricsAskPass();
    QString msg;
    switch (static_cast<QProcess::ProcessError>(error)) {
        case QProcess::FailedToStart:
            msg = tr("Failed to start ssh for metrics. Make sure it is installed and in PATH.");
            break;
        case QProcess::Crashed:
            msg = tr("SSH metrics process crashed.");
            break;
        default:
            msg = tr("SSH metrics error: %1").arg(error);
            break;
    }
    qDebug() << "Metrics error:" << msg;
    emit m_monitoringBar->errorOccurred(msg);
    
    if (m_metricsProcess) {
        m_metricsProcess->deleteLater();
        m_metricsProcess = nullptr;
    }
}

ServerMetrics TerminalSplitWidget::parseMetricsOutput(const QString &output)
{
    ServerMetrics metrics;
    metrics.lastUpdate = QDateTime::currentDateTime();
    
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.startsWith("CPU:")) {
            bool ok;
            double val = trimmed.mid(4).toDouble(&ok);
            if (ok) metrics.cpuUsage = qBound(0.0, val, 100.0);
        } else if (trimmed.startsWith("MEM:")) {
            QStringList parts = trimmed.mid(4).split(',');
            if (parts.size() == 2) {
                metrics.memoryTotal = parts[0].toDouble() / 1024.0 / 1024.0;
                metrics.memoryUsed = parts[1].toDouble() / 1024.0 / 1024.0;
                if (metrics.memoryTotal > 0) {
                    metrics.memoryUsage = (metrics.memoryUsed / metrics.memoryTotal) * 100.0;
                }
            }
        } else if (trimmed.startsWith("NET:")) {
            QStringList parts = trimmed.mid(4).split(',');
            if (parts.size() == 2) {
                metrics.networkRx = parts[0].toDouble();
                metrics.networkTx = parts[1].toDouble();
            }
        } else if (trimmed.startsWith("UP:")) {
            metrics.uptime = trimmed.mid(3);
        } else if (trimmed.startsWith("USERS:")) {
            metrics.userCount = trimmed.mid(6).toInt();
        }
    }
    
    return metrics;
}

void TerminalSplitWidget::onTerminalConnectionChanged(bool connected)
{
    if (connected) {
        m_monitoringBar->startMonitoring(5000);
    } else {
        m_monitoringBar->stopMonitoring();
    }
}

TerminalSplitWidget::~TerminalSplitWidget()
{
    // Stop monitoring to prevent further metric requests
    if (m_monitoringBar) {
        m_monitoringBar->stopMonitoring();
    }
    
    // Clean up metrics process if running
    if (m_metricsProcess && m_metricsProcess->state() == QProcess::Running) {
        m_metricsProcess->terminate();
        m_metricsProcess->waitForFinished(1000);
    }
    cleanupMetricsAskPass();
}
