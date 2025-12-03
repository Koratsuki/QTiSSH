#include "sshterminal.h"
#include "ui_sshterminal.h"
#include <QFont>
#include <QScrollBar>
#include <QTextCursor>

SSHTerminal::SSHTerminal(const ServerConfig &config, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SSHTerminal)
    , m_config(config)
    , m_process(new QProcess(this))
    , m_connected(false)
    , m_waitingForPassword(false)
{
    ui->setupUi(this);
    
    connect(m_process, &QProcess::readyReadStandardOutput, this, &SSHTerminal::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &SSHTerminal::onReadyReadStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SSHTerminal::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &SSHTerminal::onProcessError);
    connect(ui->input, &QLineEdit::returnPressed, this, &SSHTerminal::onInputReturnPressed);
}

SSHTerminal::~SSHTerminal()
{
    if (m_process->state() == QProcess::Running) {
        m_process->terminate();
        m_process->waitForFinished(1000);
        if (m_process->state() == QProcess::Running) {
            m_process->kill();
        }
    }
    delete ui;
}

void SSHTerminal::connectToServer()
{
    if (m_connected) {
        return;
    }

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
    
    // Disable strict host key checking for easier testing (can be made configurable)
    args << "-o" << "StrictHostKeyChecking=no";
    args << "-o" << "UserKnownHostsFile=/dev/null";
    
    args << QString("%1@%2").arg(m_config.username()).arg(m_config.host());

    m_process->start("ssh", args);
    
    if (m_config.authType() == AuthType::Password && !m_config.password().isEmpty()) {
        m_waitingForPassword = true;
    }
}

void SSHTerminal::disconnectFromServer()
{
    if (m_process->state() == QProcess::Running) {
        m_process->write("exit\n");
        m_process->waitForFinished(1000);
        if (m_process->state() == QProcess::Running) {
            m_process->terminate();
        }
    }
    m_connected = false;
    emit connectionStateChanged(false);
}

QString SSHTerminal::buildSSHCommand()
{
    QString cmd = QString("ssh -p %1").arg(m_config.port());
    
    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        cmd += QString(" -i %1").arg(m_config.keyPath());
    }
    
    cmd += QString(" %1@%2").arg(m_config.username()).arg(m_config.host());
    return cmd;
}

void SSHTerminal::sendCommand(const QString &command)
{
    if (m_process->state() == QProcess::Running) {
        m_process->write(command.toUtf8() + "\n");
    }
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
        emit connectionStateChanged(true);
    }
    
    ui->terminal->appendPlainText(output);
    
    // Auto-scroll to bottom
    QScrollBar *scrollBar = ui->terminal->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void SSHTerminal::onReadyReadStandardError()
{
    QByteArray data = m_process->readAllStandardError();
    QString error = QString::fromUtf8(data);
    
    // Some SSH output goes to stderr that's not actually errors
    if (error.contains("password:", Qt::CaseInsensitive)) {
        ui->terminal->appendPlainText(error);
        if (m_waitingForPassword && !m_config.password().isEmpty()) {
            m_waitingForPassword = false;
            m_process->write(m_config.password().toUtf8() + "\n");
        }
    } else {
        ui->terminal->appendPlainText("ERROR: " + error);
    }
}

void SSHTerminal::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_connected = false;
    emit connectionStateChanged(false);
    
    if (exitStatus == QProcess::CrashExit) {
        ui->terminal->appendPlainText("\n\nConnection crashed!");
    } else {
        ui->terminal->appendPlainText(QString("\n\nConnection closed (exit code: %1)").arg(exitCode));
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
