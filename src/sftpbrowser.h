#ifndef SFTPBROWSER_H
#define SFTPBROWSER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QMenu>
#include "serverconfig.h"
#include "sftpconnection.h"
#include "filetransfermanager.h"

namespace Ui {
class SFTPBrowser;
}

class TransferQueueWidget;
class QTreeView;
class QListWidget;
class QListWidgetItem;
class QLabel;

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
    Ui::SFTPBrowser *ui;
    ServerConfig m_config;
    SFTPConnection *m_sftpConnection;
    FileTransferManager *m_transferManager;

    // Models
    QFileSystemModel *m_localModel;
    QStandardItemModel *m_remoteModel;
    
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
