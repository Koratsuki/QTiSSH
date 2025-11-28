#include "filetransfermanager.h"

FileTransferManager::FileTransferManager(QObject *parent)
    : QObject(parent)
    , m_queueTimer(new QTimer(this))
    , m_maxConcurrentTransfers(3)
    , m_queuePaused(false)
{
    m_queueTimer->setInterval(1000); // Check queue every second
    connect(m_queueTimer, &QTimer::timeout, this, &FileTransferManager::processQueue);
    m_queueTimer->start();
}

QString FileTransferManager::addTransfer(const QString &localPath, const QString &remotePath,
                                        TransferType type, const QString &serverId)
{
    FileTransfer *transfer = new FileTransfer(localPath, remotePath, type, serverId, this);
    
    // Connect signals
    connect(transfer, &FileTransfer::finished, this, &FileTransferManager::onTransferFinished);
    connect(transfer, &FileTransfer::progressChanged, this, &FileTransferManager::onTransferProgressChanged);
    connect(transfer, &FileTransfer::statusChanged, this, &FileTransferManager::onTransferStatusChanged);
    
    m_transfers.append(transfer);
    m_queue.enqueue(transfer);
    
    emit transferAdded(transfer);
    emit queueChanged();
    
    return transfer->id();
}

void FileTransferManager::removeTransfer(const QString &transferId)
{
    for (int i = 0; i < m_transfers.size(); ++i) {
        if (m_transfers[i]->id() == transferId) {
            FileTransfer *transfer = m_transfers[i];
            
            // Cancel if in progress
            if (transfer->status() == TransferStatus::InProgress) {
                transfer->cancel();
            }
            
            // Remove from queue if queued
            for (int j = 0; j < m_queue.size(); ++j) {
                if (m_queue[j]->id() == transferId) {
                    m_queue.removeAt(j);
                    break;
                }
            }
            
            m_transfers.removeAt(i);
            emit transferRemoved(transferId);
            emit queueChanged();
            
            transfer->deleteLater();
            return;
        }
    }
}

void FileTransferManager::cancelTransfer(const QString &transferId)
{
    FileTransfer *transfer = getTransfer(transferId);
    if (transfer) {
        transfer->cancel();
    }
}

void FileTransferManager::pauseTransfer(const QString &transferId)
{
    FileTransfer *transfer = getTransfer(transferId);
    if (transfer) {
        transfer->pause();
    }
}

void FileTransferManager::resumeTransfer(const QString &transferId)
{
    FileTransfer *transfer = getTransfer(transferId);
    if (transfer) {
        transfer->resume();
    }
}

void FileTransferManager::startQueue()
{
    m_queuePaused = false;
    processQueue();
}

void FileTransferManager::pauseQueue()
{
    m_queuePaused = true;
}

void FileTransferManager::clearQueue()
{
    // Cancel all active transfers
    for (FileTransfer *transfer : m_transfers) {
        if (transfer->status() == TransferStatus::InProgress) {
            transfer->cancel();
        }
    }
    
    // Clear queue
    m_queue.clear();
    
    // Remove all transfers
    for (FileTransfer *transfer : m_transfers) {
        transfer->deleteLater();
    }
    m_transfers.clear();
    
    emit queueChanged();
}

QList<FileTransfer*> FileTransferManager::getAllTransfers() const
{
    return m_transfers;
}

QList<FileTransfer*> FileTransferManager::getActiveTransfers() const
{
    QList<FileTransfer*> active;
    for (FileTransfer *transfer : m_transfers) {
        if (transfer->status() == TransferStatus::InProgress) {
            active.append(transfer);
        }
    }
    return active;
}

QList<FileTransfer*> FileTransferManager::getQueuedTransfers() const
{
    QList<FileTransfer*> queued;
    for (FileTransfer *transfer : m_transfers) {
        if (transfer->status() == TransferStatus::Queued) {
            queued.append(transfer);
        }
    }
    return queued;
}

FileTransfer* FileTransferManager::getTransfer(const QString &transferId) const
{
    for (FileTransfer *transfer : m_transfers) {
        if (transfer->id() == transferId) {
            return transfer;
        }
    }
    return nullptr;
}

void FileTransferManager::onTransferFinished(bool success)
{
    FileTransfer *transfer = qobject_cast<FileTransfer*>(sender());
    if (transfer) {
        emit transferFinished(transfer->id(), success);
        emit queueChanged();
        
        // Start next transfer in queue
        startNextTransfer();
    }
}

void FileTransferManager::onTransferProgressChanged(int percent)
{
    FileTransfer *transfer = qobject_cast<FileTransfer*>(sender());
    if (transfer) {
        emit transferProgressChanged(transfer->id(), percent);
    }
}

void FileTransferManager::onTransferStatusChanged(TransferStatus status)
{
    FileTransfer *transfer = qobject_cast<FileTransfer*>(sender());
    if (transfer) {
        emit transferStatusChanged(transfer->id(), status);
        emit queueChanged();
    }
}

void FileTransferManager::processQueue()
{
    if (m_queuePaused) {
        return;
    }
    
    startNextTransfer();
}

void FileTransferManager::startNextTransfer()
{
    if (getActiveTransferCount() >= m_maxConcurrentTransfers) {
        return;
    }
    
    if (m_queue.isEmpty()) {
        return;
    }
    
    FileTransfer *transfer = m_queue.dequeue();
    if (transfer && transfer->status() == TransferStatus::Queued) {
        transfer->start();
    }
}

int FileTransferManager::getActiveTransferCount() const
{
    int count = 0;
    for (FileTransfer *transfer : m_transfers) {
        if (transfer->status() == TransferStatus::InProgress) {
            count++;
        }
    }
    return count;
}

