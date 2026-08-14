#include "monitoringdialog.h"
#include "passwordmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QFile>
#include <QUuid>
#include <QProcessEnvironment>

MonitoringDialog::MonitoringDialog(const ServerConfig &config, QWidget *parent)
    : QDialog(parent)
    , m_config(config)
    , m_process(new QProcess(this))
{
    setWindowTitle(tr("Server Monitoring - %1 (%2)")
                       .arg(config.alias(), QString("%1@%2").arg(config.username(), config.host())));
    setMinimumSize(560, 420);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("CPU, memory and disk usage:"), this));

    m_output = new QPlainTextEdit(this);
    m_output->setReadOnly(true);
    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    m_output->setFont(mono);
    layout->addWidget(m_output);

    auto *buttons = new QHBoxLayout();
    m_refreshButton = new QPushButton(tr("Refresh"), this);
    auto *closeButton = new QPushButton(tr("Close"), this);
    buttons->addStretch();
    buttons->addWidget(m_refreshButton);
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(m_refreshButton, &QPushButton::clicked, this, &MonitoringDialog::refresh);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &MonitoringDialog::onReadyRead);
    connect(m_process, &QProcess::readyReadStandardError, this, &MonitoringDialog::onReadyRead);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MonitoringDialog::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred, this, &MonitoringDialog::onProcessError);

    refresh();
}

MonitoringDialog::~MonitoringDialog()
{
    if (m_process->state() == QProcess::Running) {
        m_process->terminate();
        m_process->waitForFinished(1000);
    }
    cleanupAskPass();
}

void MonitoringDialog::refresh()
{
    m_refreshButton->setEnabled(false);
    m_output->clear();
    startProcess();
}

void MonitoringDialog::startProcess()
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

    QString command = "uptime; echo '==== Memory ===='; free -m; "
                      "echo '==== Disk ===='; df -h; "
                      "echo '==== Top Processes ===='; top -bn1 | head -25";
    args << QString("%1@%2").arg(m_config.username(), m_config.host()) << command;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    setupAskPass();
    if (!m_askPassPath.isEmpty()) {
        env.insert("SSH_ASKPASS", m_askPassPath);
        env.insert("SSH_ASKPASS_REQUIRE", "force");
        env.insert("DISPLAY", ":0");
    }
    m_process->setProcessEnvironment(env);
    m_process->start("ssh", args);
}

void MonitoringDialog::setupAskPass()
{
    cleanupAskPass();
    QString pwd = m_config.password();
    if (m_config.authType() != AuthType::Password || pwd.isEmpty()
        || pwd.startsWith(PasswordManager::instance().storagePrefix())) {
        return;
    }

    QString escaped = pwd;
    escaped.replace("'", "'\\''");

    m_askPassPath = QDir::temp().filePath(
        "qtissh-askpass-" + QUuid::createUuid().toString(QUuid::WithoutBraces));
    QFile file(m_askPassPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write("#!/bin/sh\n");
        file.write("echo '" + escaped.toUtf8() + "'\n");
        file.close();
        QFile::setPermissions(m_askPassPath,
                              QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
    }
}

void MonitoringDialog::cleanupAskPass()
{
    if (!m_askPassPath.isEmpty()) {
        QFile::remove(m_askPassPath);
        m_askPassPath.clear();
    }
}

void MonitoringDialog::onReadyRead()
{
    m_output->appendPlainText(QString::fromUtf8(m_process->readAllStandardOutput()));
    m_output->appendPlainText(QString::fromUtf8(m_process->readAllStandardError()));
}

void MonitoringDialog::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)
    m_refreshButton->setEnabled(true);
    cleanupAskPass();
    if (exitCode != 0) {
        m_output->appendPlainText(tr("\n[Command finished with exit code %1]").arg(exitCode));
    }
}

void MonitoringDialog::onProcessError(QProcess::ProcessError error)
{
    m_refreshButton->setEnabled(true);
    cleanupAskPass();
    QString msg;
    switch (error) {
    case QProcess::FailedToStart:
        msg = tr("Failed to start ssh. Make sure it is installed and in PATH.");
        break;
    case QProcess::Crashed:
        msg = tr("SSH process crashed.");
        break;
    default:
        msg = tr("SSH error: %1").arg(int(error));
        break;
    }
    m_output->appendPlainText(tr("\n[ERROR] %1").arg(msg));
}
