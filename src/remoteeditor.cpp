#include "remoteeditor.h"
#include "passwordmanager.h"
#include <QPlainTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QShortcut>
#include <QKeySequence>
#include <QMessageBox>
#include <QDateTime>
#include <QUuid>

RemoteEditor::RemoteEditor(const ServerConfig &config, const QString &remotePath,
                           QWidget *parent)
    : QWidget(parent)
    , m_config(config)
    , m_remotePath(remotePath)
    , m_localModified(false)
    , m_busy(false)
{
    setWindowTitle(tr("Remote File: %1 - %2").arg(config.alias(), remotePath));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_editor = new QPlainTextEdit(this);
    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    m_editor->setFont(mono);
    layout->addWidget(m_editor);

    auto *bar = new QHBoxLayout();
    m_saveButton = new QPushButton(tr("Save (Ctrl+S)"), this);
    m_statusLabel = new QLabel(tr("Downloading..."), this);
    bar->addWidget(m_saveButton);
    bar->addWidget(m_statusLabel, 1);
    layout->addLayout(bar);

    m_saveButton->setEnabled(false);

    connect(m_saveButton, &QPushButton::clicked, this, &RemoteEditor::save);
    connect(m_editor, &QPlainTextEdit::textChanged, this, &RemoteEditor::onTextChanged);

    auto *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(saveShortcut, &QShortcut::activated, this, &RemoteEditor::save);

    m_syncTimer = new QTimer(this);
    m_syncTimer->setInterval(4000);
    connect(m_syncTimer, &QTimer::timeout, this, &RemoteEditor::onSyncTimeout);

    m_localPath = QDir::temp().filePath(
        "qtissh-remote-" + QUuid::createUuid().toString(QUuid::WithoutBraces));

    download();
}

QStringList RemoteEditor::buildSshBaseArgs() const
{
    QStringList args;
    args << "-p" << QString::number(m_config.port());

    if (m_config.authType() == AuthType::PublicKey && !m_config.keyPath().isEmpty()) {
        args << "-i" << m_config.keyPath();
    }
    if (!m_config.strictHostKeyChecking()) {
        args << "-o" << "StrictHostKeyChecking=no";
        args << "-o" << "UserKnownHostsFile=/dev/null";
    }
    args << m_config.sshOptionArgs();

    return args;
}

QStringList RemoteEditor::buildScpArgs(bool download) const
{
    QStringList args = buildSshBaseArgs();
    args.removeAll("-p");
    args.prepend("-P" + QString::number(m_config.port()));

    QString remote = QString("%1@%2:%3")
                         .arg(m_config.username(), m_config.host(), m_remotePath);
    if (download) {
        args << remote << m_localPath;
    } else {
        args << m_localPath << remote;
    }
    return args;
}

void RemoteEditor::setupAskPass(QProcessEnvironment &env)
{
    if (m_config.authType() != AuthType::Password) {
        return;
    }
    QString pwd = m_config.password();
    if (pwd.isEmpty() || pwd.startsWith(PasswordManager::instance().storagePrefix())) {
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
        env.insert("SSH_ASKPASS", m_askPassPath);
        env.insert("SSH_ASKPASS_REQUIRE", "force");
        env.insert("DISPLAY", env.value("DISPLAY", ":0"));
    }
}

void RemoteEditor::cleanupAskPass()
{
    if (!m_askPassPath.isEmpty()) {
        QFile::remove(m_askPassPath);
        m_askPassPath.clear();
    }
}

bool RemoteEditor::runSyncCommand(const QStringList &args, QByteArray *output, QByteArray *errors)
{
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    setupAskPass(env);
    process.setProcessEnvironment(env);
    process.start(args.first(), args.mid(1));
    if (!process.waitForFinished(30000)) {
        process.kill();
        cleanupAskPass();
        return false;
    }
    if (output) {
        *output = process.readAllStandardOutput();
    }
    if (errors) {
        *errors = process.readAllStandardError();
    }
    cleanupAskPass();
    return process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0;
}

void RemoteEditor::download()
{
    setBusy(true);
    m_statusLabel->setText(tr("Downloading %1...").arg(m_remotePath));

    m_process = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    setupAskPass(env);
    m_process->setProcessEnvironment(env);

    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &RemoteEditor::onDownloadFinished);
    m_process->start("scp", buildScpArgs(true));
}

void RemoteEditor::onDownloadFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    cleanupAskPass();
    setBusy(false);

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {
        m_statusLabel->setText(tr("Download failed."));
        m_saveButton->setEnabled(false);
        QMessageBox::warning(this, tr("Download Failed"),
                             tr("Could not download %1:\n%2")
                                 .arg(m_remotePath, QString::fromUtf8(m_process->readAllStandardError())));
        m_process->deleteLater();
        return;
    }

    QFile file(m_localPath);
    if (file.open(QIODevice::ReadOnly)) {
        m_editor->setPlainText(QString::fromUtf8(file.readAll()));
        file.close();
    }

    // Query the remote modification time.
    QStringList args = buildSshBaseArgs();
    args << QString("%1@%2").arg(m_config.username(), m_config.host());
    args << QString("stat -c %%Y -- %1").arg(QString(m_remotePath).replace("'", "'\\''"));
    QByteArray out;
    if (runSyncCommand(args, &out, nullptr)) {
        m_remoteMtime = QString::fromUtf8(out).trimmed().toLongLong();
    }

    m_statusLabel->setText(tr("Ready - %1").arg(m_remotePath));
    m_saveButton->setEnabled(true);
    m_syncTimer->start();
    m_process->deleteLater();
}

void RemoteEditor::upload()
{
    setBusy(true);
    m_statusLabel->setText(tr("Uploading %1...").arg(m_remotePath));

    QFile file(m_localPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(m_editor->toPlainText().toUtf8());
        file.close();
    }

    m_process = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    setupAskPass(env);
    m_process->setProcessEnvironment(env);

    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &RemoteEditor::onUploadFinished);
    m_process->start("scp", buildScpArgs(false));
}

void RemoteEditor::onUploadFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    cleanupAskPass();
    setBusy(false);

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {
        m_statusLabel->setText(tr("Upload failed."));
        QMessageBox::warning(this, tr("Upload Failed"),
                             tr("Could not upload %1:\n%2")
                                 .arg(m_remotePath, QString::fromUtf8(m_process->readAllStandardError())));
        m_process->deleteLater();
        return;
    }

    m_localModified = false;
    m_statusLabel->setText(tr("Saved - %1").arg(QDateTime::currentDateTime().toString("HH:mm:ss")));

    QStringList args = buildSshBaseArgs();
    args << QString("%1@%2").arg(m_config.username(), m_config.host());
    args << QString("stat -c %%Y -- %1").arg(QString(m_remotePath).replace("'", "'\\''"));
    QByteArray out;
    if (runSyncCommand(args, &out, nullptr)) {
        m_remoteMtime = QString::fromUtf8(out).trimmed().toLongLong();
    }
    m_process->deleteLater();
}

void RemoteEditor::refreshFromRemote()
{
    QStringList args = buildSshBaseArgs();
    args << QString("%1@%2").arg(m_config.username(), m_config.host());
    args << QString("stat -c %%Y -- %1").arg(QString(m_remotePath).replace("'", "'\\''"));
    QByteArray out;
    if (!runSyncCommand(args, &out, nullptr)) {
        return;
    }

    qint64 mtime = QString::fromUtf8(out).trimmed().toLongLong();
    if (mtime == 0 || mtime == m_remoteMtime) {
        return;
    }
    m_remoteMtime = mtime;

    if (m_localModified) {
        m_statusLabel->setText(tr("File changed on server; saving will overwrite."));
        return;
    }

    setBusy(true);
    m_statusLabel->setText(tr("Reloading from server..."));
    m_process = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    setupAskPass(env);
    m_process->setProcessEnvironment(env);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
                cleanupAskPass();
                setBusy(false);
                if (exitStatus == QProcess::NormalExit && exitCode == 0) {
                    QFile file(m_localPath);
                    if (file.open(QIODevice::ReadOnly)) {
                        m_editor->setPlainText(QString::fromUtf8(file.readAll()));
                        file.close();
                    }
                    m_statusLabel->setText(tr("Reloaded from server - %1").arg(m_remotePath));
                } else {
                    m_statusLabel->setText(tr("Reload failed."));
                }
                m_process->deleteLater();
            });
    m_process->start("scp", buildScpArgs(true));
}

void RemoteEditor::onSyncTimeout()
{
    if (m_busy) {
        return;
    }
    refreshFromRemote();
}

void RemoteEditor::onTextChanged()
{
    if (!m_busy) {
        m_localModified = true;
    }
}

void RemoteEditor::save()
{
    if (m_busy) {
        return;
    }
    upload();
}

void RemoteEditor::setBusy(bool busy)
{
    m_busy = busy;
    m_saveButton->setEnabled(!busy);
    if (busy) {
        m_syncTimer->stop();
    } else if (m_remoteMtime != 0) {
        m_syncTimer->start();
    }
}
