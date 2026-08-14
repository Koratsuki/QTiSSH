#include "commandhistorydialog.h"
#include "commandhistorymanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>

CommandHistoryDialog::CommandHistoryDialog(const QString &serverId, const QString &serverLabel,
                                           QWidget *parent)
    : QDialog(parent)
    , m_serverId(serverId)
    , m_listWidget(new QListWidget(this))
{
    setWindowTitle(tr("Command History — %1").arg(serverLabel));
    resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Commands executed on this server (double-click to re-run)."), this));
    layout->addWidget(m_listWidget);

    QStringList commands = CommandHistoryManager::instance().commands(serverId);
    m_listWidget->addItems(commands);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *executeButton = new QPushButton(tr("Execute"), this);
    QPushButton *clearButton = new QPushButton(tr("Clear History"), this);
    buttonLayout->addWidget(executeButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);

    connect(executeButton, &QPushButton::clicked, this, &CommandHistoryDialog::onExecute);
    connect(clearButton, &QPushButton::clicked, this, &CommandHistoryDialog::onClear);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this,
            [this](QListWidgetItem*) { onExecute(); });

    if (m_listWidget->count() > 0) {
        m_listWidget->setCurrentRow(0);
    }
}

void CommandHistoryDialog::onExecute()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item) {
        QMessageBox::information(this, tr("Command History"), tr("Select a command to execute."));
        return;
    }
    emit executeRequested(item->text());
}

void CommandHistoryDialog::onClear()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Clear History"), tr("Clear the command history for this server?"));
    if (reply == QMessageBox::Yes) {
        CommandHistoryManager::instance().clear(m_serverId);
        m_listWidget->clear();
    }
}
