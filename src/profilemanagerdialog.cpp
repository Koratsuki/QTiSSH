#include "profilemanagerdialog.h"
#include "profilemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>

ProfileManagerDialog::ProfileManagerDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Connection Profiles"));
    setMinimumSize(420, 360);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr(
        "Profiles are named sets of SSH options (one per line) that can be "
        "selected when adding or editing a server."), this));

    m_list = new QListWidget(this);
    layout->addWidget(m_list);

    auto *buttons = new QHBoxLayout();
    auto *addButton = new QPushButton(tr("Add..."), this);
    auto *editButton = new QPushButton(tr("Edit..."), this);
    auto *removeButton = new QPushButton(tr("Remove"), this);
    auto *closeButton = new QPushButton(tr("Close"), this);
    buttons->addWidget(addButton);
    buttons->addWidget(editButton);
    buttons->addWidget(removeButton);
    buttons->addStretch();
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(addButton, &QPushButton::clicked, this, &ProfileManagerDialog::onAdd);
    connect(editButton, &QPushButton::clicked, this, &ProfileManagerDialog::onEdit);
    connect(removeButton, &QPushButton::clicked, this, &ProfileManagerDialog::onRemove);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

    refreshList();
}

void ProfileManagerDialog::refreshList()
{
    m_list->clear();
    ProfileManager &pm = ProfileManager::instance();
    const QStringList names = pm.profileNames();
    for (const QString &name : names) {
        auto *item = new QListWidgetItem(name);
        item->setToolTip(pm.optionsFor(name));
        m_list->addItem(item);
    }
}

void ProfileManagerDialog::onAdd()
{
    bool ok = false;
    QString name = QInputDialog::getText(this, tr("Add Profile"),
                                         tr("Profile name:"), QLineEdit::Normal,
                                         QString(), &ok);
    if (!ok || name.trimmed().isEmpty()) {
        return;
    }
    if (ProfileManager::instance().exists(name.trimmed())) {
        QMessageBox::warning(this, tr("Add Profile"), tr("A profile with that name already exists."));
        return;
    }
    bool ok2 = false;
    QString options = QInputDialog::getMultiLineText(
        this, tr("Add Profile"),
        tr("SSH options (one per line, e.g. Compression=yes):"),
        QString(), &ok2);
    if (!ok2) {
        return;
    }
    ProfileManager::instance().addProfile(name.trimmed(), options.trimmed());
    refreshList();
}

void ProfileManagerDialog::onEdit()
{
    QListWidgetItem *item = m_list->currentItem();
    if (!item) {
        return;
    }
    QString name = item->text();
    bool ok = false;
    QString options = QInputDialog::getMultiLineText(
        this, tr("Edit Profile"),
        tr("SSH options (one per line):"),
        ProfileManager::instance().optionsFor(name), &ok);
    if (ok) {
        ProfileManager::instance().updateProfile(name, options.trimmed());
        item->setToolTip(ProfileManager::instance().optionsFor(name));
    }
}

void ProfileManagerDialog::onRemove()
{
    QListWidgetItem *item = m_list->currentItem();
    if (!item) {
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Remove Profile"),
        tr("Remove profile '%1'? Servers that use it will fall back to their "
           "own options.").arg(item->text()));
    if (reply != QMessageBox::Yes) {
        return;
    }
    ProfileManager::instance().removeProfile(item->text());
    refreshList();
}
