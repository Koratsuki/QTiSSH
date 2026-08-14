#include "networkdiscoverydialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QHostAddress>
#include <QThread>
#include <QTime>

static QString findLocalNetwork(QHostAddress &firstAddress, int &prefix)
{
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            (iface.flags() & QNetworkInterface::IsLoopBack)) {
            continue;
        }
        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            QHostAddress addr = entry.ip();
            if (addr.protocol() != QAbstractSocket::IPv4Protocol) {
                continue;
            }
            const int bits = entry.prefixLength();
            if (bits <= 0 || bits >= 31) {
                continue;
            }
            firstAddress = addr;
            prefix = bits;
            quint32 raw = addr.toIPv4Address();
            quint32 mask = (bits == 0) ? 0 : (0xFFFFFFFFu << (32 - bits));
            quint32 network = raw & mask;
            return QHostAddress(network).toString();
        }
    }
    return QString();
}

NetworkDiscoveryDialog::NetworkDiscoveryDialog(QWidget *parent)
    : QDialog(parent)
    , m_thread(nullptr)
{
    setWindowTitle(tr("Network Discovery - SSH Servers"));
    setMinimumSize(460, 400);

    auto *layout = new QVBoxLayout(this);

    m_statusLabel = new QLabel(this);
    layout->addWidget(m_statusLabel);

    m_list = new QListWidget(this);
    layout->addWidget(m_list);

    m_progress = new QProgressBar(this);
    m_progress->setVisible(false);
    layout->addWidget(m_progress);

    auto *buttons = new QHBoxLayout();
    m_scanButton = new QPushButton(tr("Scan Network"), this);
    m_connectButton = new QPushButton(tr("Quick Connect"), this);
    m_addButton = new QPushButton(tr("Add to Servers..."), this);
    auto *closeButton = new QPushButton(tr("Close"), this);
    m_connectButton->setEnabled(false);
    m_addButton->setEnabled(false);
    buttons->addWidget(m_scanButton);
    buttons->addWidget(m_connectButton);
    buttons->addWidget(m_addButton);
    buttons->addStretch();
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(m_scanButton, &QPushButton::clicked, this, &NetworkDiscoveryDialog::startScan);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_connectButton, &QPushButton::clicked, this, &NetworkDiscoveryDialog::onConnect);
    connect(m_addButton, &QPushButton::clicked, this, &NetworkDiscoveryDialog::onAddServer);
    connect(m_list, &QListWidget::itemSelectionChanged, this, [this]() {
        bool has = m_list->currentItem() != nullptr;
        m_connectButton->setEnabled(has);
        m_addButton->setEnabled(has);
    });
}

void NetworkDiscoveryDialog::setupList()
{
    m_list->clear();
    for (const DiscoveredHost &host : m_results) {
        if (!host.open) {
            continue;
        }
        auto *item = new QListWidgetItem(host.address);
        item->setData(Qt::UserRole, host.address);
        m_list->addItem(item);
    }
    m_statusLabel->setText(
        tr("%1 host(s) with SSH open.").arg(m_list->count()));
}

void NetworkDiscoveryDialog::startScan()
{
    QHostAddress firstAddress;
    int prefix = 0;
    QString network = findLocalNetwork(firstAddress, prefix);
    if (network.isEmpty()) {
        QMessageBox::warning(this, tr("Network Discovery"),
                             tr("Could not determine a local IPv4 network."));
        return;
    }

    if (m_thread) {
        m_thread->quit();
        m_thread->wait();
        delete m_thread;
        m_thread = nullptr;
    }

    const QString currentUser = qEnvironmentVariable("USER", "root");
    m_thread = new QThread(this);
    auto *worker = new DiscoveryWorker(network, prefix, currentUser);
    worker->moveToThread(m_thread);

    connect(m_thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(worker, &DiscoveryWorker::progress, this, &NetworkDiscoveryDialog::onScanProgress);
    connect(worker, &DiscoveryWorker::finished, this, &NetworkDiscoveryDialog::onScanFinished);

    m_scanButton->setEnabled(false);
    m_progress->setVisible(true);
    m_statusLabel->setText(tr("Scanning %1/%2 ...").arg(network, QString::number(prefix)));

    m_thread->start();
    QMetaObject::invokeMethod(worker, "run", Qt::QueuedConnection);
}

void NetworkDiscoveryDialog::onScanProgress(int scanned, int total)
{
    m_progress->setMaximum(qMax(total, 1));
    m_progress->setValue(scanned);
}

void NetworkDiscoveryDialog::onScanFinished()
{
    if (m_thread) {
        m_thread->quit();
        m_thread->wait();
        m_thread = nullptr;
    }
    m_progress->setVisible(false);
    m_scanButton->setEnabled(true);
    setupList();
}

void NetworkDiscoveryDialog::onConnect()
{
    QListWidgetItem *item = m_list->currentItem();
    if (!item) {
        return;
    }
    const QString currentUser = qEnvironmentVariable("USER", "root");
    emit connectRequested(QHostAddress(item->data(Qt::UserRole).toString()), currentUser);
    accept();
}

void NetworkDiscoveryDialog::onAddServer()
{
    QListWidgetItem *item = m_list->currentItem();
    if (!item) {
        return;
    }
    emit addServerRequested(QHostAddress(item->data(Qt::UserRole).toString()));
    accept();
}

// ---------------------------------------------------------------------------
// Worker
// ---------------------------------------------------------------------------

DiscoveryWorker::DiscoveryWorker(const QString &network, int prefix, const QString &currentUser)
    : m_network(network)
    , m_prefix(prefix)
    , m_currentUser(currentUser)
{
    Q_UNUSED(m_currentUser)
}

void DiscoveryWorker::run()
{
    QList<DiscoveredHost> results;

    quint32 raw = QHostAddress(m_network).toIPv4Address();
    quint32 mask = (m_prefix == 0) ? 0 : (0xFFFFFFFFu << (32 - m_prefix));
    quint32 network = raw & mask;
    quint32 broadcast = network | ~mask;
    quint32 count = broadcast - network;

    int scanned = 0;
    const int total = (count > 1000) ? 1000 : int(count);

    for (quint32 i = 1; i <= total; ++i) {
        quint32 host = network + i;
        // Skip the network/broadcast addresses and common gateway.
        if (host == network || host == broadcast || host == network + 1) {
            continue;
        }
        QString address = QHostAddress(host).toString();

        DiscoveredHost result;
        result.address = address;
        result.open = false;

        QTcpSocket socket;
        socket.connectToHost(address, 22);
        if (socket.waitForConnected(200)) {
            result.open = true;
            socket.disconnectFromHost();
        }
        results.append(result);

        ++scanned;
        if (scanned % 10 == 0) {
            emit progress(scanned, total);
        }
    }

    emit progress(total, total);
    emit finished(results);
}
