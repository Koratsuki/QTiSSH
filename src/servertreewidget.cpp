#include "servertreewidget.h"
#include "servermanager.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QHeaderView>

ServerTreeWidget::ServerTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
    , m_serverManager(nullptr)
    , m_folderManager(nullptr)
{
    setHeaderLabel("Servers");
    setDragDropMode(QAbstractItemView::InternalMove);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    
    // Hide the header
    header()->hide();
    
    setupContextMenus();
}

void ServerTreeWidget::setServerManager(ServerManager *serverManager)
{
    if (m_serverManager) {
        disconnect(m_serverManager, nullptr, this, nullptr);
    }
    
    m_serverManager = serverManager;
    
    if (m_serverManager) {
        connect(m_serverManager, &ServerManager::serversChanged, this, &ServerTreeWidget::onServersChanged);
    }
    
    refreshTree();
}

void ServerTreeWidget::setFolderManager(FolderManager *folderManager)
{
    if (m_folderManager) {
        disconnect(m_folderManager, nullptr, this, nullptr);
    }
    
    m_folderManager = folderManager;
    
    if (m_folderManager) {
        connect(m_folderManager, &FolderManager::foldersChanged, this, &ServerTreeWidget::onFoldersChanged);
    }
    
    refreshTree();
}

void ServerTreeWidget::refreshTree()
{
    buildTree();
}

ServerConfig ServerTreeWidget::getSelectedServer() const
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != ItemType::Server) {
        return ServerConfig();
    }
    
    QString serverId = getItemServerId(item);
    if (m_serverManager) {
        return m_serverManager->getServer(serverId);
    }
    
    return ServerConfig();
}

QString ServerTreeWidget::getSelectedFolderId() const
{
    QTreeWidgetItem *item = currentItem();
    if (!item || getItemType(item) != ItemType::Folder) {
        return QString();
    }
    
    return getItemFolderId(item);
}

ItemType ServerTreeWidget::getSelectedItemType() const
{
    QTreeWidgetItem *item = currentItem();
    if (!item) {
        return ItemType::Folder; // Default
    }
    
    return getItemType(item);
}

void ServerTreeWidget::selectServer(const QString &serverId)
{
    QTreeWidgetItem *item = findServerItem(serverId);
    if (item) {
        setCurrentItem(item);
    }
}

void ServerTreeWidget::selectFolder(const QString &folderId)
{
    QTreeWidgetItem *item = findFolderItem(folderId);
    if (item) {
        setCurrentItem(item);
    }
}

void ServerTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());
    
    if (!item) {
        // Empty area - show empty context menu
        m_emptyContextMenu->exec(event->globalPos());
    } else {
        ItemType type = getItemType(item);
        if (type == ItemType::Folder) {
            m_folderContextMenu->exec(event->globalPos());
        } else {
            m_serverContextMenu->exec(event->globalPos());
        }
    }
}

void ServerTreeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());
    if (!item) {
        return;
    }
    
    ItemType type = getItemType(item);
    if (type == ItemType::Server) {
        ServerConfig server = getSelectedServer();
        if (server.isValid()) {
            emit serverDoubleClicked(server);
        }
    } else if (type == ItemType::Folder) {
        QString folderId = getSelectedFolderId();
        if (!folderId.isEmpty()) {
            emit folderDoubleClicked(folderId);
        }
    }
}

void ServerTreeWidget::dropEvent(QDropEvent *event)
{
    QTreeWidgetItem *targetItem = itemAt(event->pos());
    QTreeWidgetItem *sourceItem = currentItem();
    
    if (!sourceItem) {
        event->ignore();
        return;
    }
    
    ItemType sourceType = getItemType(sourceItem);
    
    // Only allow moving servers for now
    if (sourceType != ItemType::Server) {
        event->ignore();
        return;
    }
    
    QString serverId = getItemServerId(sourceItem);
    QString targetFolderId;
    
    if (targetItem) {
        ItemType targetType = getItemType(targetItem);
        if (targetType == ItemType::Folder) {
            targetFolderId = getItemFolderId(targetItem);
        } else {
            // Dropped on a server - get its parent folder
            QTreeWidgetItem *parentItem = targetItem->parent();
            if (parentItem) {
                targetFolderId = getItemFolderId(parentItem);
            }
        }
    }
    
    emit moveServerRequested(serverId, targetFolderId);
    event->accept();
}

void ServerTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() == this) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ServerTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());
    if (item) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ServerTreeWidget::onCreateFolder()
{
    QTreeWidgetItem *item = currentItem();
    QString parentFolderId;
    
    if (item && getItemType(item) == ItemType::Folder) {
        parentFolderId = getItemFolderId(item);
    }
    
    emit createFolderRequested(parentFolderId);
}

void ServerTreeWidget::onRenameFolder()
{
    QString folderId = getSelectedFolderId();
    if (!folderId.isEmpty()) {
        emit renameFolderRequested(folderId);
    }
}

void ServerTreeWidget::onDeleteFolder()
{
    QString folderId = getSelectedFolderId();
    if (!folderId.isEmpty()) {
        emit deleteFolderRequested(folderId);
    }
}

void ServerTreeWidget::onAddServer()
{
    QTreeWidgetItem *item = currentItem();
    QString folderId;
    
    if (item && getItemType(item) == ItemType::Folder) {
        folderId = getItemFolderId(item);
    }
    
    emit addServerRequested(folderId);
}

void ServerTreeWidget::onEditServer()
{
    QString serverId = getItemServerId(currentItem());
    if (!serverId.isEmpty()) {
        emit editServerRequested(serverId);
    }
}

void ServerTreeWidget::onDeleteServer()
{
    QString serverId = getItemServerId(currentItem());
    if (!serverId.isEmpty()) {
        emit deleteServerRequested(serverId);
    }
}

void ServerTreeWidget::onFoldersChanged()
{
    refreshTree();
}

void ServerTreeWidget::onServersChanged()
{
    refreshTree();
}

void ServerTreeWidget::setupContextMenus()
{
    // Folder context menu
    m_folderContextMenu = new QMenu(this);
    m_createFolderAction = m_folderContextMenu->addAction("📁 Create Folder");
    m_addServerToFolderAction = m_folderContextMenu->addAction("🖥️ Add Server");
    m_folderContextMenu->addSeparator();
    m_renameFolderAction = m_folderContextMenu->addAction("✏️ Rename");
    m_deleteFolderAction = m_folderContextMenu->addAction("🗑️ Delete");
    
    connect(m_createFolderAction, &QAction::triggered, this, &ServerTreeWidget::onCreateFolder);
    connect(m_addServerToFolderAction, &QAction::triggered, this, &ServerTreeWidget::onAddServer);
    connect(m_renameFolderAction, &QAction::triggered, this, &ServerTreeWidget::onRenameFolder);
    connect(m_deleteFolderAction, &QAction::triggered, this, &ServerTreeWidget::onDeleteFolder);
    
    // Server context menu
    m_serverContextMenu = new QMenu(this);
    m_editServerAction = m_serverContextMenu->addAction("✏️ Edit");
    m_deleteServerAction = m_serverContextMenu->addAction("🗑️ Delete");
    
    connect(m_editServerAction, &QAction::triggered, this, &ServerTreeWidget::onEditServer);
    connect(m_deleteServerAction, &QAction::triggered, this, &ServerTreeWidget::onDeleteServer);
    
    // Empty area context menu
    m_emptyContextMenu = new QMenu(this);
    m_addServerAction = m_emptyContextMenu->addAction("🖥️ Add Server");
    m_emptyContextMenu->addSeparator();
    QAction *createRootFolderAction = m_emptyContextMenu->addAction("📁 Create Folder");
    
    connect(m_addServerAction, &QAction::triggered, this, &ServerTreeWidget::onAddServer);
    connect(createRootFolderAction, &QAction::triggered, this, &ServerTreeWidget::onCreateFolder);
}

void ServerTreeWidget::buildTree()
{
    clear();
    
    if (!m_folderManager || !m_serverManager) {
        return;
    }
    
    // Add root folders
    QStringList rootFolders = m_folderManager->getRootFolders();
    for (const QString &folderId : rootFolders) {
        addFolderToTree(folderId);
    }
    
    // Add servers that don't belong to any folder
    addRootServers();
    
    expandAll();
}

void ServerTreeWidget::addFolderToTree(const QString &folderId, QTreeWidgetItem *parentItem)
{
    if (!m_folderManager->folderExists(folderId)) {
        return;
    }
    
    FolderNode folder = m_folderManager->getFolder(folderId);
    
    QTreeWidgetItem *folderItem = new QTreeWidgetItem();
    folderItem->setText(0, "📁 " + folder.name);
    setItemData(folderItem, ItemType::Folder, folderId);
    
    if (parentItem) {
        parentItem->addChild(folderItem);
    } else {
        addTopLevelItem(folderItem);
    }
    
    // Add child folders
    QStringList childFolders = m_folderManager->getChildFolders(folderId);
    for (const QString &childId : childFolders) {
        addFolderToTree(childId, folderItem);
    }
    
    // Add servers in this folder
    addServersToFolder(folderId, folderItem);
}

void ServerTreeWidget::addServersToFolder(const QString &folderId, QTreeWidgetItem *folderItem)
{
    if (!m_serverManager) {
        return;
    }
    
    QStringList serverIds = m_serverManager->getServerIds();
    for (const QString &serverId : serverIds) {
        ServerConfig server = m_serverManager->getServer(serverId);
        if (server.group() == folderId) {
            QTreeWidgetItem *serverItem = new QTreeWidgetItem();
            serverItem->setText(0, "🖥️ " + server.alias());
            setItemData(serverItem, ItemType::Server, serverId);
            folderItem->addChild(serverItem);
        }
    }
}

void ServerTreeWidget::addRootServers(QTreeWidgetItem *rootItem)
{
    if (!m_serverManager) {
        return;
    }
    
    QStringList serverIds = m_serverManager->getServerIds();
    for (const QString &serverId : serverIds) {
        ServerConfig server = m_serverManager->getServer(serverId);
        if (server.group().isEmpty()) {
            QTreeWidgetItem *serverItem = new QTreeWidgetItem();
            serverItem->setText(0, "🖥️ " + server.alias());
            setItemData(serverItem, ItemType::Server, serverId);
            
            if (rootItem) {
                rootItem->addChild(serverItem);
            } else {
                addTopLevelItem(serverItem);
            }
        }
    }
}

QTreeWidgetItem *ServerTreeWidget::findFolderItem(const QString &folderId) const
{
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = topLevelItem(i);
        if (getItemType(item) == ItemType::Folder && getItemFolderId(item) == folderId) {
            return item;
        }
        
        // Search recursively
        QTreeWidgetItemIterator it(item);
        while (*it) {
            if (getItemType(*it) == ItemType::Folder && getItemFolderId(*it) == folderId) {
                return *it;
            }
            ++it;
        }
    }
    return nullptr;
}

QTreeWidgetItem *ServerTreeWidget::findServerItem(const QString &serverId) const
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if (getItemType(*it) == ItemType::Server && getItemServerId(*it) == serverId) {
            return *it;
        }
        ++it;
    }
    return nullptr;
}

QString ServerTreeWidget::getItemFolderId(QTreeWidgetItem *item) const
{
    if (!item) {
        return QString();
    }
    return item->data(0, Qt::UserRole + 1).toString();
}

QString ServerTreeWidget::getItemServerId(QTreeWidgetItem *item) const
{
    if (!item) {
        return QString();
    }
    return item->data(0, Qt::UserRole + 1).toString();
}

ItemType ServerTreeWidget::getItemType(QTreeWidgetItem *item) const
{
    if (!item) {
        return ItemType::Folder;
    }
    return static_cast<ItemType>(item->data(0, Qt::UserRole).toInt());
}

void ServerTreeWidget::setItemData(QTreeWidgetItem *item, ItemType type, const QString &id)
{
    if (!item) {
        return;
    }
    item->setData(0, Qt::UserRole, static_cast<int>(type));
    item->setData(0, Qt::UserRole + 1, id);
}
