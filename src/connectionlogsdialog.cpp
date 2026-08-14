#include "connectionlogsdialog.h"
#include "sessionlogger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMessageBox>

ConnectionLogsDialog::ConnectionLogsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Connection Logs"));
    setMinimumSize(720, 480);

    auto *layout = new QVBoxLayout(this);

    m_infoLabel = new QLabel(this);
    layout->addWidget(m_infoLabel);

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    m_list = new QListWidget(splitter);
    m_list->setMinimumWidth(260);
    m_viewer = new QPlainTextEdit(splitter);
    m_viewer->setReadOnly(true);
    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    m_viewer->setFont(mono);
    splitter->addWidget(m_list);
    splitter->addWidget(m_viewer);
    splitter->setStretchFactor(1, 1);
    layout->addWidget(splitter);

    auto *buttons = new QHBoxLayout();
    m_deleteButton = new QPushButton(tr("Delete Selected"), this);
    m_deleteButton->setEnabled(false);
    auto *closeButton = new QPushButton(tr("Close"), this);
    buttons->addWidget(m_deleteButton);
    buttons->addStretch();
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_deleteButton, &QPushButton::clicked, this, &ConnectionLogsDialog::onDelete);
    connect(m_list, &QListWidget::currentItemChanged, this, &ConnectionLogsDialog::onSelectionChanged);

    refreshList();
}

void ConnectionLogsDialog::refreshList()
{
    m_list->clear();
    m_viewer->clear();

    const QString dir = SessionLogger::logsDir();
    QDir d(dir);
    if (!d.exists()) {
        m_infoLabel->setText(tr("No session logs yet."));
        return;
    }

    QFileInfoList entries = d.entryInfoList({"*.log"}, QDir::Files, QDir::Time);
    for (const QFileInfo &fi : entries) {
        auto *item = new QListWidgetItem(
            QString("%1  (%2 KB, %3)")
                .arg(fi.fileName())
                .arg(fi.size() / 1024.0, 0, 'f', 1)
                .arg(fi.lastModified().toString("yyyy-MM-dd HH:mm")));
        item->setData(Qt::UserRole, fi.absoluteFilePath());
        m_list->addItem(item);
    }

    m_infoLabel->setText(tr("%1 log file(s) in %2").arg(m_list->count()).arg(dir));
}

void ConnectionLogsDialog::onSelectionChanged(QListWidgetItem *current, QListWidgetItem *)
{
    m_deleteButton->setEnabled(current != nullptr);
    if (!current) {
        m_viewer->clear();
        return;
    }
    QString path = current->data(Qt::UserRole).toString();
    QFile file(path);
    m_viewer->clear();
    if (file.open(QIODevice::ReadOnly)) {
        m_viewer->setPlainText(QString::fromUtf8(file.readAll()));
        file.close();
    }
}

void ConnectionLogsDialog::onDelete()
{
    QListWidgetItem *item = m_list->currentItem();
    if (!item) {
        return;
    }
    QString path = item->data(Qt::UserRole).toString();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Delete Log"), tr("Delete log file '%1'?").arg(item->text()));
    if (reply != QMessageBox::Yes) {
        return;
    }
    if (QFile::remove(path)) {
        refreshList();
    } else {
        QMessageBox::warning(this, tr("Delete Log"), tr("Could not delete the log file."));
    }
}
