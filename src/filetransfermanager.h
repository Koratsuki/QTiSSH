#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include <QObject>
#include <QQueue>
#include <QList>
#include <QTimer>
#include "filetransfer.h"

class FileTransferManager : public QObject
{
    Q_OBJECT

public:
    explicit FileTransferManager(QObject *parent = nullptr);
    
    // Transfer management
    QString addTransfer(const QString &localPath, const QString &remotePath,
                       TransferType type, const QString &serverId);
    void removeTransfer(const QString &transferId);
    void cancelTransfer(const QString &transferId);
    void pauseTransfer(const QString &transferId);
    void resumeTransfer(const QString &transferId);
    
    // Queue management
    void startQueue();
    void pauseQueue();
    void clearQueue();
    
    // Getters
    QList<FileTransfer*> getAllTransfers() const;
    QList<FileTransfer*> getActiveTransfers() const;
    QList<FileTransfer*> getQueuedTransfers() const;
    FileTransfer* getTransfer(const QString &transferId) const;
    
    // Settings
    void setMaxConcurrentTransfers(int max) { m_maxConcurrentTransfers = max; }
    int maxConcurrentTransfers() const { return m_maxConcurrentTransfers; }

signals:
    void transferAdded(FileTransfer *transfer);
    void transferRemoved(const QString &transferId);
    void transferProgressChanged(const QString &transferId, int percent);
    void transferStatusChanged(const QString &transferId, TransferStatus status);
    void transferFinished(const QString &transferId, bool success);
    void queueChanged();

private slots:
    void onTransferFinished(bool success);
    void onTransferProgressChanged(int percent);
    void onTransferStatusChanged(TransferStatus status);
    void processQueue();

private:
    QList<FileTransfer*> m_transfers;
    QQueue<FileTransfer*> m_queue;
    QTimer *m_queueTimer;
    int m_maxConcurrentTransfers;
    bool m_queuePaused;
    
    void startNextTransfer();
    int getActiveTransferCount() const;
};

#endif // FILETRANSFERMANAGER_H

