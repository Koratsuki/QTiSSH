#include "quickcommandsdialog.h"
#include <QLabel>

struct QuickCommand {
    const char *name;
    const char *command;
};

static const QuickCommand kQuickCommands[] = {
    {"System information", "uname -a"},
    {"Uptime & load", "uptime"},
    {"Memory usage", "free -m"},
    {"Disk usage", "df -h"},
    {"Top processes", "top -bn1 | head -30"},
    {"Current directory", "pwd"},
    {"List files", "ls -lah"},
    {"Who is logged in", "who"},
    {"Network interfaces", "ip addr"},
    {"Open ports", "ss -tulpn"},
    {"Kernel logs (tail)", "dmesg | tail -20"},
    {"Current date", "date"},
    {"Clear screen", "clear"},
    {"Last logins", "last -n 10"},
    {"Logged in users", "w"},
    {"SSH connections", "ss -tn | grep ':22'"},
    {"Disk inodes", "df -ih"},
    {"CPU info", "lscpu"},
};

QuickCommandsDialog::QuickCommandsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Quick Commands"));
    setMinimumSize(380, 420);

    auto *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel(tr("Select a command to run in the active terminal:"), this));

    m_search = new QLineEdit(this);
    m_search->setPlaceholderText(tr("Search commands..."));
    m_search->setClearButtonEnabled(true);
    layout->addWidget(m_search);

    m_list = new QListWidget(this);
    layout->addWidget(m_list);

    auto *buttons = new QHBoxLayout();
    buttons->addStretch();
    auto *executeButton = new QPushButton(tr("Execute"), this);
    auto *closeButton = new QPushButton(tr("Close"), this);
    buttons->addWidget(executeButton);
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(executeButton, &QPushButton::clicked, this, &QuickCommandsDialog::onExecute);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_list, &QListWidget::itemDoubleClicked, this, &QuickCommandsDialog::onDoubleClicked);
    connect(m_search, &QLineEdit::textChanged, this, &QuickCommandsDialog::onSearchChanged);

    populate();
}

void QuickCommandsDialog::populate()
{
    m_list->clear();
    for (const QuickCommand &qc : kQuickCommands) {
        auto *item = new QListWidgetItem(qc.name);
        item->setData(Qt::UserRole, QLatin1String(qc.command));
        item->setToolTip(QLatin1String(qc.command));
        m_list->addItem(item);
    }
}

void QuickCommandsDialog::onSearchChanged(const QString &text)
{
    for (int i = 0; i < m_list->count(); ++i) {
        QListWidgetItem *item = m_list->item(i);
        item->setHidden(!text.isEmpty() &&
                        !item->text().contains(text, Qt::CaseInsensitive));
    }
}

void QuickCommandsDialog::onExecute()
{
    QListWidgetItem *item = m_list->currentItem();
    if (!item) {
        return;
    }
    emit executeRequested(item->data(Qt::UserRole).toString());
    accept();
}

void QuickCommandsDialog::onDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        emit executeRequested(item->data(Qt::UserRole).toString());
        accept();
    }
}
