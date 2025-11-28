#include "sftpbrowser.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QHeaderView>
#include <QFrame>

SFTPBrowser::SFTPBrowser(const ServerConfig &config, QWidget *parent)
    : QWidget(parent)
    , m_config(config)
    , m_sftpConnection(new SFTPConnection(config, this))
    , m_transferManager(new FileTransferManager(this))
{
    setupUI();
    setupConnections();
    setupDragDrop();
}

SFTPBrowser::~SFTPBrowser()
{
    if (isConnected()) {
        disconnectFromServer();
    }
}

void SFTPBrowser::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Setup toolbar
    setupToolbar();
    mainLayout->addWidget(m_toolbar);
    
    // Main splitter (horizontal: browsers | transfer queue)
    m_mainSplitter = new QSplitter(Qt::Vertical, this);
    
    // Browser splitter (horizontal: local | remote)
    m_browserSplitter = new QSplitter(Qt::Horizontal, this);
    
    setupLocalBrowser();
    setupRemoteBrowser();
    setupTransferQueue();
    
    m_browserSplitter->addWidget(m_localPanel);
    m_browserSplitter->addWidget(m_remotePanel);
    m_browserSplitter->setStretchFactor(0, 1);
    m_browserSplitter->setStretchFactor(1, 1);
    
    m_mainSplitter->addWidget(m_browserSplitter);
    m_mainSplitter->addWidget(m_transferQueue);
    m_mainSplitter->setStretchFactor(0, 3);
    m_mainSplitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(m_mainSplitter);
    setLayout(mainLayout);
}

void SFTPBrowser::setupToolbar()
{
    m_toolbar = new QWidget(this);
    QHBoxLayout *toolbarLayout = new QHBoxLayout(m_toolbar);
    
    m_connectButton = new QPushButton("Connect", this);
    m_uploadButton = new QPushButton("⬆️ Upload", this);
    m_downloadButton = new QPushButton("⬇️ Download", this);
    m_deleteButton = new QPushButton("🗑️ Delete", this);
    m_newFolderButton = new QPushButton("📁 New Folder", this);
    m_refreshButton = new QPushButton("🔄 Refresh", this);
    
    m_uploadButton->setEnabled(false);
    m_downloadButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_newFolderButton->setEnabled(false);
    m_refreshButton->setEnabled(false);
    
    toolbarLayout->addWidget(m_connectButton);
    
    // Add separator
    QFrame *separator1 = new QFrame();
    separator1->setFrameShape(QFrame::VLine);
    separator1->setFrameShadow(QFrame::Sunken);
    separator1->setMaximumWidth(2);
    toolbarLayout->addWidget(separator1);
    
    toolbarLayout->addWidget(m_uploadButton);
    toolbarLayout->addWidget(m_downloadButton);
    
    // Add separator
    QFrame *separator2 = new QFrame();
    separator2->setFrameShape(QFrame::VLine);
    separator2->setFrameShadow(QFrame::Sunken);
    separator2->setMaximumWidth(2);
    toolbarLayout->addWidget(separator2);
    
    toolbarLayout->addWidget(m_deleteButton);
    toolbarLayout->addWidget(m_newFolderButton);
    
    // Add separator
    QFrame *separator3 = new QFrame();
    separator3->setFrameShape(QFrame::VLine);
    separator3->setFrameShadow(QFrame::Sunken);
    separator3->setMaximumWidth(2);
    toolbarLayout->addWidget(separator3);
    
    toolbarLayout->addWidget(m_refreshButton);
    toolbarLayout->addStretch();
    
    m_toolbar->setLayout(toolbarLayout);
}

void SFTPBrowser::setupLocalBrowser()
{
    m_localPanel = new QWidget(this);
    QVBoxLayout *localLayout = new QVBoxLayout(m_localPanel);
    
    m_localPathLabel = new QLabel("Local Files", this);
    m_localPathLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    localLayout->addWidget(m_localPathLabel);
    
    m_localTreeView = new QTreeView(this);
    m_localModel = new QFileSystemModel(this);
    m_localModel->setRootPath(QDir::homePath());
    m_localTreeView->setModel(m_localModel);
    m_localTreeView->setRootIndex(m_localModel->index(QDir::homePath()));
    
    // Hide size, type, and date columns for cleaner look
    m_localTreeView->hideColumn(1);
    m_localTreeView->hideColumn(2);
    m_localTreeView->hideColumn(3);
    
    m_localTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_localTreeView->setDragEnabled(true);
    m_localTreeView->setDragDropMode(QAbstractItemView::DragOnly);
    
    localLayout->addWidget(m_localTreeView);
    m_localPanel->setLayout(localLayout);
}

void SFTPBrowser::setupRemoteBrowser()
{
    m_remotePanel = new QWidget(this);
    QVBoxLayout *remoteLayout = new QVBoxLayout(m_remotePanel);
    
    m_remotePathLabel = new QLabel("Remote Files - Not Connected", this);
    m_remotePathLabel->setStyleSheet("font-weight: bold; padding: 5px;");
    remoteLayout->addWidget(m_remotePathLabel);
    
    m_remoteListWidget = new QListWidget(this);
    m_remoteListWidget->setEnabled(false);
    m_remoteListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_remoteListWidget->setAcceptDrops(true);
    m_remoteListWidget->setDropIndicatorShown(true);
    
    remoteLayout->addWidget(m_remoteListWidget);
    m_remotePanel->setLayout(remoteLayout);
}

void SFTPBrowser::setupTransferQueue()
{
    m_transferQueue = new TransferQueueWidget(m_transferManager, this);
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
    connect(m_connectButton, &QPushButton::clicked, this, &SFTPBrowser::connectToServer);
    connect(m_uploadButton, &QPushButton::clicked, this, &SFTPBrowser::onUploadClicked);
    connect(m_downloadButton, &QPushButton::clicked, this, &SFTPBrowser::onDownloadClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &SFTPBrowser::onDeleteRemoteClicked);
    connect(m_newFolderButton, &QPushButton::clicked, this, &SFTPBrowser::onCreateRemoteFolderClicked);
    connect(m_refreshButton, &QPushButton::clicked, this, &SFTPBrowser::refreshRemoteDirectory);
    
    // Local browser signals
    connect(m_localTreeView, &QTreeView::doubleClicked, this, &SFTPBrowser::onLocalFileDoubleClicked);
    
    // Remote browser signals
    connect(m_remoteListWidget, &QListWidget::itemDoubleClicked, this, &SFTPBrowser::onRemoteItemDoubleClicked);
    connect(m_remoteListWidget, &QListWidget::itemSelectionChanged, this, &SFTPBrowser::onRemoteItemSelectionChanged);
    connect(m_remoteListWidget, &QListWidget::customContextMenuRequested, this, &SFTPBrowser::showRemoteContextMenu);
}

void SFTPBrowser::setupDragDrop()
{
    setAcceptDrops(true);
    m_remoteListWidget->setAcceptDrops(true);
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
        m_connectButton->setText("Connecting...");
        m_connectButton->setEnabled(false);
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
    m_connectButton->setText("Disconnect");
    m_connectButton->setEnabled(true);
    m_uploadButton->setEnabled(true);
    m_downloadButton->setEnabled(true);
    m_deleteButton->setEnabled(true);
    m_newFolderButton->setEnabled(true);
    m_refreshButton->setEnabled(true);
    m_remoteListWidget->setEnabled(true);
    
    updateConnectionState();
    emit connectionStateChanged(true);
}

void SFTPBrowser::onSftpDisconnected()
{
    m_connectButton->setText("Connect");
    m_connectButton->setEnabled(true);
    m_uploadButton->setEnabled(false);
    m_downloadButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_newFolderButton->setEnabled(false);
    m_refreshButton->setEnabled(false);
    m_remoteListWidget->setEnabled(false);
    m_remoteListWidget->clear();
    
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
    m_remotePathLabel->setText(QString("Remote Files - %1").arg(path));
}

void SFTPBrowser::updateRemoteFileList(const QList<RemoteFileInfo> &files)
{
    m_remoteListWidget->clear();
    
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
        
        m_remoteListWidget->addItem(item);
    }
}

void SFTPBrowser::updateConnectionState()
{
    QString status = isConnected() ? 
        QString("Connected to %1@%2").arg(m_config.username()).arg(m_config.host()) :
        "Not Connected";
    
    if (!isConnected()) {
        m_remotePathLabel->setText("Remote Files - " + status);
    }
}

void SFTPBrowser::onLocalFileDoubleClicked(const QModelIndex &index)
{
    if (m_localModel->isDir(index)) {
        // Navigate to directory
        m_localTreeView->setRootIndex(index);
        m_localPathLabel->setText("Local Files - " + m_localModel->filePath(index));
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
    bool hasSelection = !m_remoteListWidget->selectedItems().isEmpty();
    m_downloadButton->setEnabled(hasSelection && isConnected());
    m_deleteButton->setEnabled(hasSelection && isConnected());
}

void SFTPBrowser::showRemoteContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = m_remoteListWidget->itemAt(pos);
    if (!item || !isConnected()) return;
    
    QMenu contextMenu(this);
    contextMenu.addAction("Download", this, &SFTPBrowser::onDownloadClicked);
    contextMenu.addAction("Delete", this, &SFTPBrowser::onDeleteRemoteClicked);
    contextMenu.addSeparator();
    contextMenu.addAction("Refresh", this, &SFTPBrowser::refreshRemoteDirectory);
    
    contextMenu.exec(m_remoteListWidget->mapToGlobal(pos));
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
    
    QList<QListWidgetItem*> selectedItems = m_remoteListWidget->selectedItems();
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
    
    QList<QListWidgetItem*> selectedItems = m_remoteListWidget->selectedItems();
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

#include "sftpbrowser.moc"
