#ifndef SERVERTREEWIDGET_H
#define SERVERTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include "serverconfig.h"
#include "foldermanager.h"

class ServerManager;

enum class ItemType {
    Folder,
    Server
};

class ServerTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit ServerTreeWidget(QWidget *parent = nullptr);
    
    void setServerManager(ServerManager *serverManager);
    void setFolderManager(FolderManager *folderManager);
    
    void refreshTree();
    ServerConfig getSelectedServer() const;
    QString getSelectedFolderId() const;
    ItemType getSelectedItemType() const;
    
    void selectServer(const QString &serverId);
    void selectFolder(const QString &folderId);
    void filterServers(const QString &filterText);

signals:
    void serverDoubleClicked(const ServerConfig &server);
    void folderDoubleClicked(const QString &folderId);
    void createFolderRequested(const QString &parentFolderId = QString());
    void renameFolderRequested(const QString &folderId);
    void deleteFolderRequested(const QString &folderId);
    void addServerRequested(const QString &folderId = QString());
    void editServerRequested(const QString &serverId);
    void deleteServerRequested(const QString &serverId);
    void moveServerRequested(const QString &serverId, const QString &newFolderId);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

private slots:
    void onCreateFolder();
    void onRenameFolder();
    void onDeleteFolder();
    void onAddServer();
    void onEditServer();
    void onDeleteServer();
    void onFoldersChanged();
    void onServersChanged();

private:
    ServerManager *m_serverManager;
    FolderManager *m_folderManager;
    
    QMenu *m_folderContextMenu;
    QMenu *m_serverContextMenu;
    QMenu *m_emptyContextMenu;
    
    QAction *m_createFolderAction;
    QAction *m_renameFolderAction;
    QAction *m_deleteFolderAction;
    QAction *m_addServerAction;
    QAction *m_editServerAction;
    QAction *m_deleteServerAction;
    QAction *m_addServerToFolderAction;
    
    void setupContextMenus();
    void buildTree();
    void addFolderToTree(const QString &folderId, QTreeWidgetItem *parentItem = nullptr);
    void addServersToFolder(const QString &folderId, QTreeWidgetItem *folderItem);
    void addRootServers(QTreeWidgetItem *rootItem = nullptr);
    
    QTreeWidgetItem *findFolderItem(const QString &folderId) const;
    QTreeWidgetItem *findServerItem(const QString &serverId) const;
    QString getItemFolderId(QTreeWidgetItem *item) const;
    QString getItemServerId(QTreeWidgetItem *item) const;
    ItemType getItemType(QTreeWidgetItem *item) const;
    
    void setItemData(QTreeWidgetItem *item, ItemType type, const QString &id);
};

#endif // SERVERTREEWIDGET_H
