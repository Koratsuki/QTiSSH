#ifndef CONNECTIONLOGSDIALOG_H
#define CONNECTIONLOGSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QLabel>
#include <QPushButton>

/**
 * @brief Browse and inspect SSH session logs.
 */
class ConnectionLogsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionLogsDialog(QWidget *parent = nullptr);

private slots:
    void onSelectionChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onDelete();

private:
    void refreshList();

    QListWidget *m_list;
    QPlainTextEdit *m_viewer;
    QLabel *m_infoLabel;
    QPushButton *m_deleteButton;
};

#endif // CONNECTIONLOGSDIALOG_H
