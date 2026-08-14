#include "applockdialog.h"
#include "applockmanager.h"
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QShowEvent>

AppLockDialog::AppLockDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("QTiSSH - Locked"));
    setModal(true);
    setMinimumWidth(360);

    auto *layout = new QVBoxLayout(this);

    auto *iconLabel = new QLabel(this);
    iconLabel->setPixmap(QIcon::fromTheme("dialog-password")
                             .pixmap(48, 48));
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    auto *titleLabel = new QLabel(tr("QTiSSH is locked"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    auto *hintLabel = new QLabel(tr("Enter the application password to continue:"), this);
    hintLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(hintLabel);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText(tr("Application password"));
    layout->addWidget(m_passwordEdit);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: #c0392b;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setVisible(false);
    layout->addWidget(m_errorLabel);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Unlock"));
    buttons->button(QDialogButtonBox::Cancel)->setText(tr("Quit"));
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &AppLockDialog::onSubmit);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &AppLockDialog::onSubmit);
}

void AppLockDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    m_passwordEdit->setFocus();
    m_passwordEdit->selectAll();
}

void AppLockDialog::onSubmit()
{
    if (AppLockManager::instance().verify(m_passwordEdit->text())) {
        accept();
        return;
    }

    m_errorLabel->setText(tr("Incorrect password. Try again."));
    m_errorLabel->setVisible(true);
    m_passwordEdit->clear();
    m_passwordEdit->setFocus();
}

bool AppLockDialog::prompt(QWidget *parent)
{
    AppLockDialog dialog(parent);
    return dialog.exec() == QDialog::Accepted;
}
