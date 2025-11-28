#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add_server.h"
#include "sshterminal.h"
#include "sftpbrowser.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serverManager(new ServerManager(this))
{
    ui->setupUi(this);
    setupUI();
    setupMenuBar();
    
    connect(m_serverManager, &ServerManager::serversChanged, this, &MainWindow::onServersChanged);
    
    updateServerList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("QTiSSH - SSH Connection Manager");
    resize(1200, 700);

    // Create central widget with splitter
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Left panel - Server list
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    QLabel *serverListLabel = new QLabel("Saved Servers", this);
    serverListLabel->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    leftLayout->addWidget(serverListLabel);
    
    m_serverList = new QListWidget(this);
    m_serverList->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(m_serverList, &QListWidget::itemDoubleClicked, this, &MainWindow::onServerListDoubleClicked);
    leftLayout->addWidget(m_serverList);
    
    // Buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    
    // First row of buttons
    QHBoxLayout *buttonRow1 = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("Add", this);
    QPushButton *editButton = new QPushButton("Edit", this);
    QPushButton *deleteButton = new QPushButton("Delete", this);
    
    buttonRow1->addWidget(addButton);
    buttonRow1->addWidget(editButton);
    buttonRow1->addWidget(deleteButton);
    
    // Second row of buttons
    QHBoxLayout *buttonRow2 = new QHBoxLayout();
    QPushButton *connectButton = new QPushButton("SSH Terminal", this);
    QPushButton *connectSftpButton = new QPushButton("SFTP Browser", this);
    
    buttonRow2->addWidget(connectButton);
    buttonRow2->addWidget(connectSftpButton);
    
    buttonLayout->addLayout(buttonRow1);
    buttonLayout->addLayout(buttonRow2);
    
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddServerClicked);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditServerClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteServerClicked);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(connectSftpButton, &QPushButton::clicked, this, &MainWindow::onConnectSftpClicked);
    leftLayout->addLayout(buttonLayout);
    
    leftPanel->setLayout(leftLayout);
    leftPanel->setMaximumWidth(300);
    leftPanel->setMinimumWidth(250);
    
    // Right panel - Tabs for connections
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    
    // Add welcome tab
    QWidget *welcomeWidget = new QWidget(this);
    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeWidget);
    QLabel *welcomeLabel = new QLabel("Welcome to QTiSSH!\n\n"
                                      "Select a server from the list on the left\n"
                                      "and click 'Connect' or double-click to start an SSH session.\n\n"
                                      "Click 'Add' to add a new server.", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 14px; color: #666;");
    welcomeLayout->addWidget(welcomeLabel);
    welcomeWidget->setLayout(welcomeLayout);
    m_tabWidget->addTab(welcomeWidget, "Welcome");
    
    splitter->addWidget(leftPanel);
    splitter->addWidget(m_tabWidget);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    
    QAction *addServerAction = new QAction(tr("&Add Server"), this);
    addServerAction->setShortcut(QKeySequence::New);
    connect(addServerAction, &QAction::triggered, this, &MainWindow::onAddServerClicked);
    fileMenu->addAction(addServerAction);
    
    fileMenu->addSeparator();
    
    QAction *quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);
    fileMenu->addAction(quitAction);
    
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About QTiSSH"),
                          tr("QTiSSH v0.1\n\nA Qt-based SSH Connection Manager\n\n"
                             "Features:\n"
                             "- Multiple simultaneous SSH connections\n"
                             "- Password and public key authentication\n"
                             "- Server configuration management\n"));
    });
    helpMenu->addAction(aboutAction);
}

void MainWindow::updateServerList()
{
    m_serverList->clear();
    
    QList<ServerConfig> servers = m_serverManager->getAllServers();
    for (const auto &server : servers) {
        QString displayText = QString("%1 (%2@%3)")
                                .arg(server.alias())
                                .arg(server.username())
                                .arg(server.host());
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, server.id());
        m_serverList->addItem(item);
    }
}

void MainWindow::onAddServerClicked()
{
    add_Server dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ServerConfig config = dialog.getServerConfig();
        m_serverManager->addServer(config);
    }
}

void MainWindow::onEditServerClicked()
{
    ServerConfig server = getSelectedServer();
    if (!server.isValid()) {
        QMessageBox::warning(this, tr("No Selection"), tr("Please select a server to edit."));
        return;
    }
    
    add_Server dialog(this);
    dialog.setWindowTitle("Edit Server");
    dialog.setServerConfig(server);
    
    if (dialog.exec() == QDialog::Accepted) {
        ServerConfig updatedConfig = dialog.getServerConfig();
        m_serverManager->updateServer(server.id(), updatedConfig);
    }
}

void MainWindow::onDeleteServerClicked()
{
    ServerConfig server = getSelectedServer();
    if (!server.isValid()) {
        QMessageBox::warning(this, tr("No Selection"), tr("Please select a server to delete."));
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              tr("Confirm Delete"),
                                                              tr("Are you sure you want to delete '%1'?").arg(server.alias()),
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_serverManager->removeServer(server.id());
    }
}

void MainWindow::onConnectClicked()
{
    ServerConfig server = getSelectedServer();
    if (!server.isValid()) {
        QMessageBox::warning(this, tr("No Selection"), tr("Please select a server to connect to."));
        return;
    }
    
    connectToServer(server);
}

void MainWindow::onConnectSftpClicked()
{
    ServerConfig server = getSelectedServer();
    if (!server.isValid()) {
        QMessageBox::warning(this, tr("No Selection"), tr("Please select a server to connect to."));
        return;
    }
    
    // Create new SFTP browser tab
    SFTPBrowser *sftpBrowser = new SFTPBrowser(server, this);
    
    QString tabTitle = QString("📁 %1 SFTP").arg(server.alias());
    int tabIndex = m_tabWidget->addTab(sftpBrowser, tabTitle);
    m_tabWidget->setCurrentIndex(tabIndex);
    
    // Update tab title on connection state change
    connect(sftpBrowser, &SFTPBrowser::connectionStateChanged, this, [this, sftpBrowser, tabTitle](bool connected) {
        int index = m_tabWidget->indexOf(sftpBrowser);
        if (index != -1) {
            if (connected) {
                m_tabWidget->setTabText(index, "🟢 " + tabTitle);
            } else {
                m_tabWidget->setTabText(index, "🔴 " + tabTitle);
            }
        }
    });
    
    // Auto-connect to server
    sftpBrowser->connectToServer();
}

void MainWindow::onServerListDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    onConnectClicked();
}

void MainWindow::connectToServer(const ServerConfig &config)
{
    // Create new terminal tab
    SSHTerminal *terminal = new SSHTerminal(config, this);
    
    QString tabTitle = QString("%1 (%2)").arg(config.alias()).arg(config.host());
    int tabIndex = m_tabWidget->addTab(terminal, tabTitle);
    m_tabWidget->setCurrentIndex(tabIndex);
    
    // Connect to the server
    terminal->connectToServer();
    
    // Update tab title on connection state change
    connect(terminal, &SSHTerminal::connectionStateChanged, this, [this, terminal, tabTitle](bool connected) {
        int index = m_tabWidget->indexOf(terminal);
        if (index != -1) {
            if (connected) {
                m_tabWidget->setTabText(index, "🟢 " + tabTitle);
            } else {
                m_tabWidget->setTabText(index, "🔴 " + tabTitle);
            }
        }
    });
}

void MainWindow::onTabCloseRequested(int index)
{
    // Don't close the welcome tab
    if (index == 0) {
        return;
    }
    
    QWidget *widget = m_tabWidget->widget(index);
    SSHTerminal *terminal = qobject_cast<SSHTerminal*>(widget);
    SFTPBrowser *sftpBrowser = qobject_cast<SFTPBrowser*>(widget);
    
    bool isConnected = false;
    if (terminal && terminal->isConnected()) {
        isConnected = true;
    } else if (sftpBrowser && sftpBrowser->isConnected()) {
        isConnected = true;
    }
    
    if (isConnected) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  tr("Close Connection"),
                                                                  tr("This connection is still active. Are you sure you want to close it?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        
        if (terminal) {
            terminal->disconnectFromServer();
        } else if (sftpBrowser) {
            sftpBrowser->disconnectFromServer();
        }
    }
    
    m_tabWidget->removeTab(index);
    delete widget;
}

void MainWindow::onServersChanged()
{
    updateServerList();
}

ServerConfig MainWindow::getSelectedServer()
{
    QListWidgetItem *item = m_serverList->currentItem();
    if (!item) {
        return ServerConfig();
    }
    
    QString serverId = item->data(Qt::UserRole).toString();
    return m_serverManager->getServer(serverId);
}
