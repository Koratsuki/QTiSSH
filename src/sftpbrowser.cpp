#include "sftpbrowser.h"
#include "ui_sftpbrowser.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QHeaderView>
#include <QFrame>
#include <QDir>
#include <QListWidgetItem>

SFTPBrowser::SFTPBrowser(const ServerConfig &config, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SFTPBrowser)
    , m_config(config)
    , m_sftpConnection(new SFTPConnection(config, this))
    , m_transferManager(new FileTransferManager(this))
{
    ui->setupUi(this);
    
    // Setup local file browser model
    m_localModel = new QFileSystemModel(this);
    m_localModel->setRootPath(QDir::homePath());
    ui->localTreeView->setModel(m_localModel);
    ui->localTreeView->setRootIndex(m_localModel->index(QDir::homePath()));
    
    // Hide size, type, and date columns for cleaner look
    ui->localTreeView->hideColumn(1);
    ui->localTreeView->hideColumn(2);
    ui->localTreeView->hideColumn(3);
    
    // Create and add transfer queue widget to the container
    TransferQueueWidget *transferQueue = new TransferQueueWidget(m_transferManager, this);
    ui->transferQueueLayout->addWidget(transferQueue);
    
    // Set splitter stretch factors
    ui->mainSplitter->setStretchFactor(0, 3);  // Browser area
    ui->mainSplitter->setStretchFactor(1, 1);  // Transfer queue
    ui->browserSplitter->setStretchFactor(0, 1);  // Local browser
    ui->browserSplitter->setStretchFactor(1, 1);  // Remote browser
    
    setupConnections();
    setupDragDrop();
}

SFTPBrowser::~SFTPBrowser()
{
    if (isConnected()) {
        disconnectFromServer();
    }
    delete ui;
}

void SFTPBrowser::setupConnections()
{
    // SFTP connection signals
    connect(m_sftpConnection, &SFTPConnection::connected, this, &SFTPBrowser::onSftpConnected);
    connect(m_sftpConnection, &SFTPConnection::disconnected, this, &SFTPBrowser::onSftpDisconnected);
    connect(m_sftpConnection, &SFTPConnection::connectionError, this, &SFTPBrowser::onSftpConnectionError);
    connect(m_sftpConnection, &SFTPConnection::directoryListed, this, &SFTPBrowser::onSftpDirectoryListed);
    connect(m_sftpConnection, &SFTPConnection::directoryChanged, this, &SFTPBrowser::onSftpDirectoryChanged);
    
    // UI signals
    connect(ui->connectButton, &QPushButton::clicked, this, &SFTPBrowser::connectToServer);
    connect(ui->uploadButton, &QPushButton::clicked, this, &SFTPBrowser::onUploadClicked);
    connect(ui->downloadButton, &QPushButton::clicked, this, &SFTPBrowser::onDownloadClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &SFTPBrowser::onDeleteRemoteClicked);
    connect(ui->newFolderButton, &QPushButton::clicked, this, &SFTPBrowser::onCreateRemoteFolderClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &SFTPBrowser::refreshRemoteDirectory);
    
    // Local browser signals
    connect(ui->localTreeView, &QTreeView::doubleClicked, this, &SFTPBrowser::onLocalFileDoubleClicked);
    
    // Remote browser signals
    connect(ui->remoteListWidget, &QListWidget::itemDoubleClicked, this, &SFTPBrowser::onRemoteItemDoubleClicked);
    connect(ui->remoteListWidget, &QListWidget::itemSelectionChanged, this, &SFTPBrowser::onRemoteItemSelectionChanged);
    connect(ui->remoteListWidget, &QListWidget::customContextMenuRequested, this, &SFTPBrowser::showRemoteContextMenu);
}

void SFTPBrowser::setupDragDrop()
{
    setAcceptDrops(true);
    ui->remoteListWidget->setAcceptDrops(true);
}

bool SFTPBrowser::isConnected() const
{
    return m_sftpConnection->isConnected();
}

void SFTPBrowser::connectToServer()
{
    if (isConnected()) {
        disconnectFromServer();
    } else {
        ui->connectButton->setText("Connecting...");
        ui->connectButton->setEnabled(false);
        m_sftpConnection->connectToServer();
    }
}

void SFTPBrowser::disconnectFromServer()
{
    m_sftpConnection->disconnect();
}

void SFTPBrowser::refreshRemoteDirectory()
{
    if (isConnected()) {
        m_sftpConnection->listDirectory();
    }
}

void SFTPBrowser::onSftpConnected()
{
    ui->connectButton->setText("Disconnect");
    ui->connectButton->setEnabled(true);
    ui->uploadButton->setEnabled(true);
    ui->downloadButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->newFolderButton->setEnabled(true);
    ui->refreshButton->setEnabled(true);
    ui->remoteListWidget->setEnabled(true);
    
    updateConnectionState();
    emit connectionStateChanged(true);
}

void SFTPBrowser::onSftpDisconnected()
{
    ui->connectButton->setText("Connect");
    ui->connectButton->setEnabled(true);
    ui->uploadButton->setEnabled(false);
    ui->downloadButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->newFolderButton->setEnabled(false);
    ui->refreshButton->setEnabled(false);
    ui->remoteListWidget->setEnabled(false);
    ui->remoteListWidget->clear();
    
    updateConnectionState();
    emit connectionStateChanged(false);
}

void SFTPBrowser::onSftpConnectionError(const QString &error)
{
    QMessageBox::warning(this, "Connection Error", error);
    onSftpDisconnected();
    emit errorOccurred(error);
}

void SFTPBrowser::onSftpDirectoryListed(const QList<RemoteFileInfo> &files)
{
    updateRemoteFileList(files);
}

void SFTPBrowser::onSftpDirectoryChanged(const QString &path)
{
    ui->remotePathLabel->setText(QString("Remote Files - %1").arg(path));
}

void SFTPBrowser::updateRemoteFileList(const QList<RemoteFileInfo> &files)
{
    ui->remoteListWidget->clear();
    
    for (const RemoteFileInfo &fileInfo : files) {
        QListWidgetItem *item = new QListWidgetItem();
        
        QString icon = fileInfo.isDirectory ? "📁" : "📄";
        QString displayText = QString("%1 %2").arg(icon).arg(fileInfo.name);
        
        if (!fileInfo.isDirectory) {
            displayText += QString(" (%1 bytes)").arg(fileInfo.size);
        }
        
        item->setText(displayText);
        item->setData(Qt::UserRole, fileInfo.path);
        item->setData(Qt::UserRole + 1, fileInfo.isDirectory);
        item->setData(Qt::UserRole + 2, fileInfo.name);
        
        ui->remoteListWidget->addItem(item);
    }
}

void SFTPBrowser::updateConnectionState()
{
    QString status = isConnected() ? 
        QString("Connected to %1@%2").arg(m_config.username()).arg(m_config.host()) :
        "Not Connected";
    
    if (!isConnected()) {
        ui->remotePathLabel->setText("Remote Files - " + status);
    }
}

void SFTPBrowser::onLocalFileDoubleClicked(const QModelIndex &index)
{
    if (m_localModel->isDir(index)) {
        // Navigate to directory
        ui->localTreeView->setRootIndex(index);
        ui->localPathLabel->setText("Local Files - " + m_localModel->filePath(index));
    }
}

void SFTPBrowser::onRemoteItemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;
    
    bool isDirectory = item->data(Qt::UserRole + 1).toBool();
    QString path = item->data(Qt::UserRole).toString();
    
    if (isDirectory) {
        m_sftpConnection->changeDirectory(path);
    }
}

void SFTPBrowser::onRemoteItemSelectionChanged()
{
    // Enable/disable buttons based on selection
    bool hasSelection = !ui->remoteListWidget->selectedItems().isEmpty();
    ui->downloadButton->setEnabled(hasSelection && isConnected());
    ui->deleteButton->setEnabled(hasSelection && isConnected());
}

void SFTPBrowser::showRemoteContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = ui->remoteListWidget->itemAt(pos);
    if (!item || !isConnected()) return;
    
    QMenu contextMenu(this);
    contextMenu.addAction("Download", this, &SFTPBrowser::onDownloadClicked);
    contextMenu.addAction("Delete", this, &SFTPBrowser::onDeleteRemoteClicked);
    contextMenu.addSeparator();
    contextMenu.addAction("Refresh", this, &SFTPBrowser::refreshRemoteDirectory);
    
    contextMenu.exec(ui->remoteListWidget->mapToGlobal(pos));
}

void SFTPBrowser::onUploadClicked()
{
    if (!isConnected()) return;
    
    QStringList files = QFileDialog::getOpenFileNames(this, "Select Files to Upload");
    if (files.isEmpty()) return;
    
    QString remotePath = m_sftpConnection->currentRemotePath();
    
    for (const QString &file : files) {
        QFileInfo fileInfo(file);
        QString remoteFile = remotePath + "/" + fileInfo.fileName();
        m_transferManager->addTransfer(file, remoteFile, TransferType::Upload, m_config.id());
    }
}

void SFTPBrowser::onDownloadClicked()
{
    if (!isConnected()) return;
    
    QList<QListWidgetItem*> selectedItems = ui->remoteListWidget->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QString localDir = QFileDialog::getExistingDirectory(this, "Select Download Directory");
    if (localDir.isEmpty()) return;
    
    for (QListWidgetItem *item : selectedItems) {
        QString remotePath = item->data(Qt::UserRole).toString();
        QString fileName = item->data(Qt::UserRole + 2).toString();
        QString localPath = localDir + "/" + fileName;
        
        m_transferManager->addTransfer(localPath, remotePath, TransferType::Download, m_config.id());
    }
}

void SFTPBrowser::onDeleteRemoteClicked()
{
    if (!isConnected()) return;
    
    QList<QListWidgetItem*> selectedItems = ui->remoteListWidget->selectedItems();
    if (selectedItems.isEmpty()) return;
    
    QStringList fileNames;
    for (QListWidgetItem *item : selectedItems) {
        fileNames << item->data(Qt::UserRole + 2).toString();
    }
    
    int ret = QMessageBox::question(this, "Confirm Delete",
                                   QString("Are you sure you want to delete %1 item(s)?\n%2")
                                   .arg(fileNames.size())
                                   .arg(fileNames.join(", ")));
    
    if (ret == QMessageBox::Yes) {
        for (QListWidgetItem *item : selectedItems) {
            QString remotePath = item->data(Qt::UserRole).toString();
            bool isDirectory = item->data(Qt::UserRole + 1).toBool();
            
            if (isDirectory) {
                m_sftpConnection->removeDirectory(remotePath);
            } else {
                m_sftpConnection->deleteFile(remotePath);
            }
        }
    }
}

void SFTPBrowser::onCreateRemoteFolderClicked()
{
    if (!isConnected()) return;
    
    bool ok;
    QString folderName = QInputDialog::getText(this, "Create Folder",
                                              "Folder name:", QLineEdit::Normal,
                                              "", &ok);
    
    if (ok && !folderName.isEmpty()) {
        m_sftpConnection->createDirectory(folderName);
    }
}

// TransferQueueWidget implementation
TransferQueueWidget::TransferQueueWidget(FileTransferManager *manager, QWidget *parent)
    : QWidget(parent)
    , m_transferManager(manager)
{
    setupUI();
    
    // Connect to transfer manager signals
    connect(m_transferManager, &FileTransferManager::transferAdded, this, &TransferQueueWidget::onTransferAdded);
    connect(m_transferManager, &FileTransferManager::transferRemoved, this, &TransferQueueWidget::onTransferRemoved);
    connect(m_transferManager, &FileTransferManager::transferProgressChanged, this, &TransferQueueWidget::onTransferProgressChanged);
    connect(m_transferManager, &FileTransferManager::transferStatusChanged, this, &TransferQueueWidget::onTransferStatusChanged);
}

void TransferQueueWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_statusLabel = new QLabel("Transfer Queue", this);
    m_statusLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    layout->addWidget(m_statusLabel);
    
    m_transferList = new QListWidget(this);
    m_transferList->setMaximumHeight(150);
    layout->addWidget(m_transferList);
    
    setLayout(layout);
}

void TransferQueueWidget::onTransferAdded(FileTransfer *transfer)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, transfer->id());
    m_transferList->addItem(item);
    updateTransferItem(transfer->id());
}

void TransferQueueWidget::onTransferRemoved(const QString &transferId)
{
    for (int i = 0; i < m_transferList->count(); ++i) {
        QListWidgetItem *item = m_transferList->item(i);
        if (item->data(Qt::UserRole).toString() == transferId) {
            delete m_transferList->takeItem(i);
            break;
        }
    }
}

void TransferQueueWidget::onTransferProgressChanged(const QString &transferId, int percent)
{
    updateTransferItem(transferId);
}

void TransferQueueWidget::onTransferStatusChanged(const QString &transferId, TransferStatus status)
{
    updateTransferItem(transferId);
}

void TransferQueueWidget::updateTransferItem(const QString &transferId)
{
    FileTransfer *transfer = m_transferManager->getTransfer(transferId);
    if (!transfer) return;
    
    for (int i = 0; i < m_transferList->count(); ++i) {
        QListWidgetItem *item = m_transferList->item(i);
        if (item->data(Qt::UserRole).toString() == transferId) {
            QString icon = getTransferTypeIcon(transfer->type());
            QString status = getStatusText(transfer->status());
            QString progress = transfer->status() == TransferStatus::InProgress ? 
                QString(" [%1%]").arg(transfer->progressPercent()) : "";
            
            item->setText(QString("%1 %2 %3%4").arg(icon).arg(transfer->fileName()).arg(status).arg(progress));
            break;
        }
    }
}

QString TransferQueueWidget::getStatusText(TransferStatus status)
{
    switch (status) {
        case TransferStatus::Queued: return "Queued";
        case TransferStatus::InProgress: return "Transferring";
        case TransferStatus::Completed: return "✅ Completed";
        case TransferStatus::Failed: return "❌ Failed";
        case TransferStatus::Cancelled: return "⏹️ Cancelled";
        default: return "Unknown";
    }
}

QString TransferQueueWidget::getTransferTypeIcon(TransferType type)
{
    return type == TransferType::Upload ? "⬆️" : "⬇️";
}

// Missing SFTPBrowser method implementations
void SFTPBrowser::onLocalDirectoryChanged(const QModelIndex &index)
{
    // Handle local directory change
    if (m_localModel && index.isValid()) {
        QString path = m_localModel->filePath(index);
        if (m_localModel->isDir(index)) {
            // Directory changed, could update current path display
            // For now, just accept the change
        }
    }
}

void SFTPBrowser::onRefreshClicked()
{
    // Refresh the remote directory listing
    if (m_sftpConnection && m_sftpConnection->isConnected()) {
        QString currentPath = m_sftpConnection->currentRemotePath();
        if (currentPath.isEmpty()) {
            currentPath = "/";
        }
        m_sftpConnection->listDirectory(currentPath);
    }
}

void SFTPBrowser::onLocalFilesDropped(const QList<QString> &files)
{
    // Handle files dropped onto the local file browser
    // This could be used for drag-and-drop upload functionality
    if (m_sftpConnection && m_sftpConnection->isConnected() && !files.isEmpty()) {
        QString currentRemotePath = m_sftpConnection->currentRemotePath();
        if (currentRemotePath.isEmpty()) {
            currentRemotePath = "/";
        }
        
        for (const QString &filePath : files) {
            QFileInfo fileInfo(filePath);
            if (fileInfo.exists()) {
                // Queue file for upload to current remote directory
                QString remotePath = currentRemotePath + "/" + fileInfo.fileName();
                m_transferManager->addTransfer(filePath, remotePath, TransferType::Upload, m_config.id());
            }
        }
    }
}

#include "sftpbrowser.moc"
