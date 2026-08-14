#ifndef SNIPPETDIALOG_H
#define SNIPPETDIALOG_H

#include <QDialog>
#include <QListWidget>

class QLineEdit;

class SnippetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SnippetDialog(QWidget *parent = nullptr);

signals:
    void executeRequested(const QString &command);

private slots:
    void onAddSnippet();
    void onEditSnippet();
    void onDeleteSnippet();
    void onExecuteSnippet();

private:
    void refreshList();
    QString selectedName() const;

    QListWidget *m_listWidget;
};

#endif // SNIPPETDIALOG_H
