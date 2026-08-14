#ifndef COMMANDHISTORYDIALOG_H
#define COMMANDHISTORYDIALOG_H

#include <QDialog>
#include <QListWidget>

class CommandHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommandHistoryDialog(const QString &serverId, const QString &serverLabel,
                                  QWidget *parent = nullptr);

signals:
    void executeRequested(const QString &command);

private slots:
    void onExecute();
    void onClear();

private:
    QString m_serverId;
    QListWidget *m_listWidget;
};

#endif // COMMANDHISTORYDIALOG_H
