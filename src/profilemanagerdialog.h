#ifndef PROFILEMANAGERDIALOG_H
#define PROFILEMANAGERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>

/**
 * @brief Add/remove named SSH option profiles.
 */
class ProfileManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileManagerDialog(QWidget *parent = nullptr);

private slots:
    void onAdd();
    void onEdit();
    void onRemove();

private:
    void refreshList();

    QListWidget *m_list;
};

#endif // PROFILEMANAGERDIALOG_H
