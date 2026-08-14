#ifndef REMOTEEDITOR_H
#define REMOTEEDITOR_H

#include <QWidget>
#include <QProcess>
#include "serverconfig.h"

class QPlainTextEdit;
class QLabel;
class QPushButton;
class QTimer;

/**
 * @brief Edits a remote file with automatic upload/sync.
 *
 * The remote file is downloaded to a temporary location, edited locally, and
 * re-uploaded on save (Ctrl+S). A timer polls the remote modification time so
 * external changes are detected and reloaded when the local buffer is clean.
 */
class RemoteEditor : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteEditor(const ServerConfig &config, const QString &remotePath,
                          QWidget *parent = nullptr);

    QString remotePath() const { return m_remotePath; }
    QString serverId() const { return m_config.id(); }

public slots:
    void save();

private slots:
    void onDownloadFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onUploadFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onSyncTimeout();
    void onTextChanged();

private:
    bool runSyncCommand(const QStringList &args, QByteArray *output, QByteArray *errors);
    void download();
    void upload();
    void refreshFromRemote();
    void setBusy(bool busy);
    void setupAskPass(QProcessEnvironment &env);
    void cleanupAskPass();
    QStringList buildScpArgs(bool download) const;
    QStringList buildSshBaseArgs() const;

    ServerConfig m_config;
    QString m_remotePath;
    QString m_localPath;
    QPlainTextEdit *m_editor;
    QLabel *m_statusLabel;
    QPushButton *m_saveButton;
    QTimer *m_syncTimer;
    QProcess *m_process;
    qint64 m_remoteMtime;
    bool m_localModified;
    bool m_busy;
    QString m_askPassPath;
};

#endif // REMOTEEDITOR_H
