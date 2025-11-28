#ifndef SFTPBROWSER_H
#define SFTPBROWSER_H

#include <QWidget>
#include <QSplitter>
#include <QTreeView>
#include <QListWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QMenu>
#include "serverconfig.h"
#include "sftpconnection.h"
#include "filetransfermanager.h"

class TransferQueueWidget;

class SFTPBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit SFTPBrowser(const ServerConfig &config, QWidget *parent = nullptr);
    ~SFTPBrowser();

    ServerConfig getServerConfig() const { return m_config; }
    bool isConnected() const;

public slots:
    void connectToServer();
    void disconnectFromServer();
    void refreshRemoteDirectory();

signals:
    void connectionStateChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    // Connection slots
    void onSftpConnected();
    void onSftpDisconnected();
    void onSftpConnectionError(const QString &error);
    void onSftpDirectoryListed(const QList<RemoteFileInfo> &files);
    void onSftpDirectoryChanged(const QString &path);

    // Local file browser slots
    void onLocalDirectoryChanged(const QModelIndex &index);
    void onLocalFileDoubleClicked(const QModelIndex &index);

    // Remote file browser slots
    void onRemoteItemDoubleClicked(QListWidgetItem *item);
    void onRemoteItemSelectionChanged();
    void showRemoteContextMenu(const QPoint &pos);

    // Transfer slots
    void onUploadClicked();
    void onDownloadClicked();
    void onDeleteRemoteClicked();
    void onCreateRemoteFolderClicked();
    void onRefreshClicked();

    // Drag and drop
    void onLocalFilesDropped(const QList<QString> &files);

private:
    ServerConfig m_config;
    SFTPConnection *m_sftpConnection;
    FileTransferManager *m_transferManager;

    // UI Components
    QSplitter *m_mainSplitter;
    QSplitter *m_browserSplitter;
    
    // Local file browser
    QWidget *m_localPanel;
    QTreeView *m_localTreeView;
    QFileSystemModel *m_localModel;
    QLabel *m_localPathLabel;
    
    // Remote file browser
    QWidget *m_remotePanel;
    QListWidget *m_remoteListWidget;
    QStandardItemModel *m_remoteModel;
    QLabel *m_remotePathLabel;
    QPushButton *m_connectButton;
    QPushButton *m_refreshButton;
    
    // Transfer queue
    TransferQueueWidget *m_transferQueue;
    
    // Toolbar
    QWidget *m_toolbar;
    QPushButton *m_uploadButton;
    QPushButton *m_downloadButton;
    QPushButton *m_deleteButton;
    QPushButton *m_newFolderButton;
    
    void setupUI();
    void setupLocalBrowser();
    void setupRemoteBrowser();
    void setupTransferQueue();
    void setupToolbar();
    void setupConnections();
    
    void updateRemoteFileList(const QList<RemoteFileInfo> &files);
    void updateConnectionState();
    QString getSelectedLocalPath() const;
    QString getSelectedRemotePath() const;
    QStringList getSelectedLocalFiles() const;
    
    // Drag and drop support
    void setupDragDrop();
};

// Transfer queue widget
class TransferQueueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransferQueueWidget(FileTransferManager *manager, QWidget *parent = nullptr);

private slots:
    void onTransferAdded(FileTransfer *transfer);
    void onTransferRemoved(const QString &transferId);
    void onTransferProgressChanged(const QString &transferId, int percent);
    void onTransferStatusChanged(const QString &transferId, TransferStatus status);

private:
    FileTransferManager *m_transferManager;
    QListWidget *m_transferList;
    QLabel *m_statusLabel;
    
    void setupUI();
    void updateTransferItem(const QString &transferId);
    QString getStatusText(TransferStatus status);
    QString getTransferTypeIcon(TransferType type);
};

#endif // SFTPBROWSER_H
