#ifndef QUICKCOMMANDSDIALOG_H
#define QUICKCOMMANDSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>

/**
 * @brief Dialog with predefined commands for common tasks.
 *
 * Double-clicking (or selecting and clicking Execute) runs the command in the
 * active SSH terminal.
 */
class QuickCommandsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QuickCommandsDialog(QWidget *parent = nullptr);

signals:
    void executeRequested(const QString &command);

private slots:
    void onExecute();
    void onDoubleClicked(QListWidgetItem *item);
    void onSearchChanged(const QString &text);

private:
    void populate();

    QListWidget *m_list;
    QLineEdit *m_search;
};

#endif // QUICKCOMMANDSDIALOG_H
