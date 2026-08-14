#include "snippetdialog.h"
#include "snippetmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>

SnippetDialog::SnippetDialog(QWidget *parent)
    : QDialog(parent)
    , m_listWidget(new QListWidget(this))
{
    setWindowTitle(tr("Snippets"));
    resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Save and execute common command sequences."), this));
    layout->addWidget(m_listWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton(tr("Add..."), this);
    QPushButton *editButton = new QPushButton(tr("Edit..."), this);
    QPushButton *deleteButton = new QPushButton(tr("Delete"), this);
    QPushButton *executeButton = new QPushButton(tr("Execute"), this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(executeButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    connect(addButton, &QPushButton::clicked, this, &SnippetDialog::onAddSnippet);
    connect(editButton, &QPushButton::clicked, this, &SnippetDialog::onEditSnippet);
    connect(deleteButton, &QPushButton::clicked, this, &SnippetDialog::onDeleteSnippet);
    connect(executeButton, &QPushButton::clicked, this, &SnippetDialog::onExecuteSnippet);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this,
            [this](QListWidgetItem*) { onExecuteSnippet(); });

    refreshList();
}

QString SnippetDialog::selectedName() const
{
    QListWidgetItem *item = m_listWidget->currentItem();
    return item ? item->data(Qt::UserRole).toString() : QString();
}

void SnippetDialog::refreshList()
{
    m_listWidget->clear();
    for (const Snippet &snippet : SnippetManager::instance().getAllSnippets()) {
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1 — %2").arg(snippet.name, snippet.command), m_listWidget);
        item->setData(Qt::UserRole, snippet.name);
    }
}

void SnippetDialog::onAddSnippet()
{
    bool nameOk = false;
    QString name = QInputDialog::getText(this, tr("Add Snippet"),
                                         tr("Snippet name:"), QLineEdit::Normal,
                                         QString(), &nameOk);
    if (!nameOk || name.trimmed().isEmpty()) {
        return;
    }

    bool cmdOk = false;
    QString command = QInputDialog::getMultiLineText(this, tr("Add Snippet"),
                                                     tr("Command:"), QString(), &cmdOk);
    if (!cmdOk || command.trimmed().isEmpty()) {
        return;
    }

    SnippetManager::instance().addSnippet(name.trimmed(), command);
    refreshList();
}

void SnippetDialog::onEditSnippet()
{
    QString name = selectedName();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Snippets"), tr("Select a snippet to edit."));
        return;
    }

    QString command = SnippetManager::instance().getSnippetCommand(name);
    bool cmdOk = false;
    QString newCommand = QInputDialog::getMultiLineText(this, tr("Edit Snippet"),
                                                        tr("Command:"), command, &cmdOk);
    if (cmdOk && !newCommand.trimmed().isEmpty()) {
        SnippetManager::instance().addSnippet(name, newCommand);
        refreshList();
    }
}

void SnippetDialog::onDeleteSnippet()
{
    QString name = selectedName();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Snippets"), tr("Select a snippet to delete."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Confirm Delete"), tr("Delete snippet '%1'?").arg(name));
    if (reply == QMessageBox::Yes) {
        SnippetManager::instance().removeSnippet(name);
        refreshList();
    }
}

void SnippetDialog::onExecuteSnippet()
{
    QString name = selectedName();
    if (name.isEmpty()) {
        QMessageBox::information(this, tr("Snippets"), tr("Select a snippet to execute."));
        return;
    }

    QString command = SnippetManager::instance().getSnippetCommand(name);
    if (!command.isEmpty()) {
        emit executeRequested(command);
    }
}
