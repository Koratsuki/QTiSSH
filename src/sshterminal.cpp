#include "sshterminal.h"
#include "ui_sshterminal.h"
#include "passwordmanager.h"
#include "commandhistorymanager.h"
#include "sessionlogger.h"
#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QScrollBar>
#include <QTextCursor>
#include <QRegularExpression>
#include <QDateTime>
#include <QDir>
#include <csignal>

SSHTerminal::SSHTerminal(const ServerConfig &config, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SSHTerminal)
    , m_config(config)
    , m_process(new QProcess(this))
    , m_connected(false)
    , m_waitingForPassword(false)
    , m_userClosed(false)
    , m_reconnectScheduled(false)
    , m_reconnectAttempts(0)
    , m_terminal(new VT100Terminal(this))
    , m_inEscapeSequence(false)
    , m_sizeSyncTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // Replace the legacy terminal widget with the new VT100 emulator
    ui->terminal->hide();
    ui->input->hide();
    ui->verticalLayout->insertWidget(0, m_terminal);
    
    connect(m_process, &QProcess::readyReadStandardOutput, this, &SSHTerminal::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &SSHTerminal::onReadyReadStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SSHTerminal::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &SSHTerminal::onProcessError);
    connect(ui->input, &QLineEdit::returnPressed, this, &SSHTerminal::onInputReturnPressed);
    
    // Connect new terminal input
    connect(m_terminal, &VT100Terminal::keyPressed, this, &SSHTerminal::onTerminalKeyPressed);
    connect(m_terminal, &VT100Terminal::terminalSizeChanged, this, &SSHTerminal::onTerminalSizeChanged);
    
    // Keep retrying to sync the remote terminal size after connect. The remote
    // shell may not be ready when the first attempt is made, and connection
    // detection based on the prompt ($#>) can be missed with custom prompts.
    m_sizeSyncTimer->setInterval(500);
    connect(m_sizeSyncTimer, &QTimer::timeout, this, &SSHTerminal::onSizeSyncTimeout);
}

SSHTerminal::~SSHTerminal()
{
    m_userClosed = true;
    if (m_process->state() == QProcess::Running) {
        m_process->terminate();
        m_process->waitForFinished(1000);
        if (m_process->state() == QProcess::Running) {
            m_process->kill();
        }
    }
    stopSessionLog();
    delete ui;
}

void SSHTerminal::connectToServer()
{
    if (m_connected) {
        return;
    }

    startSessionLog();
    QString sshCommand = buildSSHCommand();
    ui->terminal->appendPlainText(QString("Connecting to %1@%2:%3...")
                                .arg(m_config.username())
                                .arg(m_config.host())
                                .arg(m_config.port()));

    QStringList args;
    args << "-p" << QString::number(m_config.port());
    
    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        args << "-i" << m_config.keyPath();
    }
    
    // Host key verification
    if (!m_config.strictHostKeyChecking()) {
        args << "-o" << "StrictHostKeyChecking=no";
        args << "-o" << "UserKnownHostsFile=/dev/null";
    } else {
        args << "-o" << "StrictHostKeyChecking=yes";
    }

    // Jump host
    if (!m_config.jumpHost().isEmpty()) {
        args << "-J" << m_config.jumpHost();
    }

    // SSH agent forwarding
    if (m_config.forwardAgent() || m_config.authType() == AuthType::SSHAgent) {
        args << "-o" << "ForwardAgent=yes";
    }

    // Custom SSH options (from profile + server)
    args << m_config.sshOptionArgs();

    // Force TERM variable on the server
    args << "-o" << "SetEnv=TERM=xterm-256color";
    args << "-t" << "-t";

    // Tunnels
    args << buildTunnelArguments();
    
    args << QString("%1@%2").arg(m_config.username()).arg(m_config.host());

    // Set TERM environment variable
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("TERM", "xterm-256color");
    m_process->setProcessEnvironment(env);

    m_process->start("ssh", args);

    // Retry sending the terminal size until the remote shell accepts it
    m_sizeSyncTimer->start();
    
    if (m_config.authType() == AuthType::Password && !m_config.password().isEmpty()) {
        // Don't auto-answer if the password is still encrypted (master password locked)
        if (!m_config.password().startsWith(PasswordManager::instance().storagePrefix())) {
            m_waitingForPassword = true;
        }
    }
}

QStringList SSHTerminal::buildTunnelArguments() const
{
    QStringList args;
    if (m_config.tunnels().isEmpty()) {
        return args;
    }
    
    // Tunnels are stored one per line: "L:8080:localhost:80", "R:5432:db:5432" or "D:1080".
    // Legacy entries without a type prefix are treated as local forwarding.
    QStringList tunnelList = m_config.tunnels().split(QRegularExpression("[\\s,;]+"), Qt::SkipEmptyParts);
    for (const QString &tunnel : tunnelList) {
        QString spec = tunnel;
        QString flag = "-L";
        if (spec.startsWith("L:", Qt::CaseInsensitive)) {
            flag = "-L";
            spec = spec.mid(2);
        } else if (spec.startsWith("R:", Qt::CaseInsensitive)) {
            flag = "-R";
            spec = spec.mid(2);
        } else if (spec.startsWith("D:", Qt::CaseInsensitive)) {
            flag = "-D";
            spec = spec.mid(2);
        }
        if (!spec.isEmpty()) {
            args << flag << spec;
        }
    }
    return args;
}

void SSHTerminal::disconnectFromServer()
{
    m_userClosed = true;
    m_reconnectScheduled = false;
    m_sizeSyncTimer->stop();
    if (m_process->state() == QProcess::Running) {
        m_process->write("exit\n");
        m_process->waitForFinished(1000);
        if (m_process->state() == QProcess::Running) {
            m_process->terminate();
        }
    }
    m_connected = false;
    stopSessionLog();
    emit connectionStateChanged(false);
}

QString SSHTerminal::buildSSHCommand()
{
    QString cmd = QString("ssh -p %1").arg(m_config.port());
    
    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        cmd += QString(" -i %1").arg(m_config.keyPath());
    }
    
    if (!m_config.strictHostKeyChecking()) {
        cmd += " -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null";
    }

    if (!m_config.jumpHost().isEmpty()) {
        cmd += " -J " + m_config.jumpHost();
    }

    if (m_config.forwardAgent() || m_config.authType() == AuthType::SSHAgent) {
        cmd += " -o ForwardAgent=yes";
    }

    if (!m_config.tunnels().isEmpty()) {
        QStringList tunnelArgs = buildTunnelArguments();
        for (int i = 0; i + 1 < tunnelArgs.size(); i += 2) {
            cmd += QString(" %1 %2").arg(tunnelArgs[i], tunnelArgs[i + 1]);
        }
    }

    cmd += QString(" %1@%2").arg(m_config.username()).arg(m_config.host());
    return cmd;
}

void SSHTerminal::sendCommand(const QString &command)
{
    if (m_process->state() == QProcess::Running) {
        m_process->write(command.toUtf8() + "\n");
        addCommandToHistory(command);
    }
}

void SSHTerminal::executeCommand(const QString &command)
{
    if (command.isEmpty()) {
        return;
    }
    sendCommand(command);
}

QString SSHTerminal::currentTypedLine() const
{
    return QString::fromUtf8(m_inputBuffer);
}

void SSHTerminal::addCommandToHistory(const QString &command)
{
    if (command.trimmed().isEmpty()) {
        return;
    }
    CommandHistoryManager::instance().add(m_config.id(), command.trimmed());
}

void SSHTerminal::onReadyReadStandardOutput()
{
    QByteArray data = m_process->readAllStandardOutput();
    QString output = QString::fromUtf8(data);
    
    m_outputBuffer += output;
    
    // Check for password prompt
    if (m_waitingForPassword && 
        (output.contains("password:", Qt::CaseInsensitive) || 
         output.contains("Password:", Qt::CaseInsensitive))) {
        m_waitingForPassword = false;
        if (!m_config.password().isEmpty()) {
            m_process->write(m_config.password().toUtf8() + "\n");
        }
    }
    
    // Check if connection established
    if (!m_connected && (output.contains("$") || output.contains("#") || output.contains(">"))) {
        m_connected = true;
        m_reconnectAttempts = 0;
        emit connectionStateChanged(true);
        // Sync terminal size once the shell is at a prompt
        sendTerminalSize();
    }
    
    writeLog(output);
    m_terminal->writeData(data);
}

void SSHTerminal::onReadyReadStandardError()
{
    QByteArray data = m_process->readAllStandardError();
    QString error = QString::fromUtf8(data);
    
    writeLog(error);
    
    // Some SSH output goes to stderr that's not actually errors
    if (error.contains("password:", Qt::CaseInsensitive)) {
        m_terminal->writeData(data);
        if (m_waitingForPassword && !m_config.password().isEmpty()) {
            m_waitingForPassword = false;
            m_process->write(m_config.password().toUtf8() + "\n");
        }
    } else {
        m_terminal->writeData(data);
    }
}

void SSHTerminal::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_sizeSyncTimer->stop();
    m_connected = false;
    emit connectionStateChanged(false);
    stopSessionLog();

    if (exitStatus == QProcess::CrashExit) {
        ui->terminal->appendPlainText("\n\nConnection crashed!");
    } else {
        ui->terminal->appendPlainText(QString("\n\nConnection closed (exit code: %1)").arg(exitCode));
    }

    if (m_config.autoReconnect() && !m_userClosed && !m_reconnectScheduled) {
        scheduleAutoReconnect();
    }
}

void SSHTerminal::onProcessError(QProcess::ProcessError error)
{
    QString errorMsg;
    switch (error) {
        case QProcess::FailedToStart:
            errorMsg = "Failed to start SSH. Make sure 'ssh' is installed and in your PATH.";
            break;
        case QProcess::Crashed:
            errorMsg = "SSH process crashed.";
            break;
        case QProcess::Timedout:
            errorMsg = "SSH process timed out.";
            break;
        case QProcess::WriteError:
            errorMsg = "Error writing to SSH process.";
            break;
        case QProcess::ReadError:
            errorMsg = "Error reading from SSH process.";
            break;
        default:
            errorMsg = "Unknown SSH error occurred.";
            break;
    }
    
    ui->terminal->appendPlainText("ERROR: " + errorMsg);
    emit errorOccurred(errorMsg);
}

void SSHTerminal::onInputReturnPressed()
{
    QString command = ui->input->text();
    if (!command.isEmpty()) {
        sendCommand(command);
        ui->terminal->appendPlainText("> " + command);
        ui->input->clear();
    }
}

void SSHTerminal::focusTerminal()
{
    m_terminal->setFocus();
}

void SSHTerminal::onTerminalKeyPressed(const QByteArray &data)
{
    if (m_process->state() == QProcess::Running) {
        m_process->write(data);
    }
    
    // Capture the line being typed for per-server command history.
    for (int i = 0; i < data.size(); ++i) {
        char c = data.at(i);
        if (c == '\r' || c == '\n') {
            QString command = QString::fromUtf8(m_inputBuffer);
            m_inputBuffer.clear();
            if (!command.trimmed().isEmpty()) {
                addCommandToHistory(command);
            }
        } else if (c == '\x7f' || c == '\b') {
            QString line = QString::fromUtf8(m_inputBuffer);
            if (!line.isEmpty()) {
                line.chop(1);
            }
            m_inputBuffer = line.toUtf8();
        } else if (c == '\x1b') {
            m_inEscapeSequence = true;
        } else if (m_inEscapeSequence) {
            if (c >= 0x40 && c <= 0x7e) {
                m_inEscapeSequence = false;
            }
        } else if (c >= 0x20 && c != 0x7f) {
            m_inputBuffer.append(c);
        }
    }
}

void SSHTerminal::onTerminalSizeChanged(int rows, int columns)
{
    Q_UNUSED(rows)
    Q_UNUSED(columns)
    sendTerminalSize();
}

void SSHTerminal::onSizeSyncTimeout()
{
    sendTerminalSize();
}

void SSHTerminal::sendTerminalSize()
{
    if (m_process->state() != QProcess::Running) {
        return;
    }
    if (m_terminal->useAlternateBuffer()) {
        return;
    }
    int rows = m_terminal->terminalRows();
    int cols = m_terminal->terminalColumns();
    if (rows <= 0 || cols <= 0) {
        return;
    }
    if (rows == m_lastSentRows && cols == m_lastSentCols) {
        return;
    }
    m_lastSentRows = rows;
    m_lastSentCols = cols;
    QByteArray cmd = QString("stty rows %1 cols %2\n").arg(rows).arg(cols).toUtf8();
    m_process->write(cmd);
}

void SSHTerminal::startSessionLog()
{
    stopSessionLog();
    m_sessionLogPath = SessionLogger::startSession(m_config);
}

void SSHTerminal::stopSessionLog()
{
    if (!m_sessionLogPath.isEmpty()) {
        SessionLogger::closeSession(m_sessionLogPath);
        m_sessionLogPath.clear();
    }
}

void SSHTerminal::writeLog(const QString &text)
{
    if (!m_sessionLogPath.isEmpty()) {
        SessionLogger::append(m_sessionLogPath, text);
    }
}

void SSHTerminal::scheduleAutoReconnect()
{
    if (m_reconnectAttempts >= 3) {
        ui->terminal->appendPlainText(tr("Auto-reconnect limit reached. Giving up."));
        return;
    }
    m_reconnectAttempts++;
    m_reconnectScheduled = true;
    ui->terminal->appendPlainText(
        tr("\nReconnecting in 3 seconds (attempt %1/3)...").arg(m_reconnectAttempts));

    QTimer::singleShot(3000, this, [this]() {
        m_reconnectScheduled = false;
        if (!m_userClosed && !m_connected && m_process->state() != QProcess::Running) {
            connectToServer();
        }
    });
}

void SSHTerminal::copy()
{
    m_terminal->copy();
}

void SSHTerminal::paste()
{
    // Send clipboard contents directly to the SSH process; the remote
    // application (e.g. the shell) echoes it back into the terminal.
    QString text = QApplication::clipboard()->text();
    if (!text.isEmpty() && m_process->state() == QProcess::Running) {
        m_process->write(text.toUtf8());
    }
}

void SSHTerminal::setTerminalFont(const QFont &font)
{
    m_terminal->setFont(font);
}

void SSHTerminal::setCursorStyle(VT100Terminal::CursorStyle style)
{
    m_terminal->setCursorStyle(style);
}

void SSHTerminal::setTerminalColors(const QColor &foreground, const QColor &background)
{
    m_terminal->setDefaultColors(foreground, background);
}
