#ifndef NETWORKDISCOVERYDIALOG_H
#define NETWORKDISCOVERYDIALOG_H

#include <QDialog>
#include <QThread>
#include <QListWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QHostAddress>
#include <QMutex>

struct DiscoveredHost {
    QString address;
    bool open;
};

/**
 * @brief Scans the local subnet for hosts with an open SSH port (22).
 *
 * The scan runs in a worker thread to keep the UI responsive.
 */
class NetworkDiscoveryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NetworkDiscoveryDialog(QWidget *parent = nullptr);

signals:
    void connectRequested(const QHostAddress &host, const QString &username);
    void addServerRequested(const QHostAddress &host);

private slots:
    void onScanFinished();
    void onScanProgress(int scanned, int total);
    void startScan();
    void onConnect();
    void onAddServer();

private:
    void setupList();

    QListWidget *m_list;
    QProgressBar *m_progress;
    QLabel *m_statusLabel;
    QPushButton *m_scanButton;
    QPushButton *m_connectButton;
    QPushButton *m_addButton;
    QThread *m_thread;
    QList<DiscoveredHost> m_results;
};

/**
 * @brief Worker that probes port 22 on every address in a subnet.
 */
class DiscoveryWorker : public QObject
{
    Q_OBJECT
public:
    DiscoveryWorker(const QString &network, int prefix, const QString &currentUser);

public slots:
    void run();

signals:
    void progress(int scanned, int total);
    void finished(QList<DiscoveredHost> results);

private:
    QString m_network;
    int m_prefix;
    QString m_currentUser;
};

#endif // NETWORKDISCOVERYDIALOG_H
