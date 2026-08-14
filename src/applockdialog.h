#ifndef APPLOCKDIALOG_H
#define APPLOCKDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

/**
 * @brief Password prompt shown before the main window opens.
 *
 * Verifies the entered password against AppLockManager. Cancel/close quits the
 * application. prompt() returns true only when a valid password was provided.
 */
class AppLockDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AppLockDialog(QWidget *parent = nullptr);

    static bool prompt(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onSubmit();

private:
    QLineEdit *m_passwordEdit;
    QLabel *m_errorLabel;
};

#endif // APPLOCKDIALOG_H
