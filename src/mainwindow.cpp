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
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serverManager(new ServerManager(this))
    , m_folderManager(new FolderManager(this))
{
    ui->setupUi(this);
    setupUI();
    setupMenuBar();
    
    connect(m_serverManager, &ServerManager::serversChanged, this, &MainWindow::onServersChanged);
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
    
    // Left panel - Server tree
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    QLabel *serverListLabel = new QLabel("Servers & Folders", this);
    serverListLabel->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
    leftLayout->addWidget(serverListLabel);
    
    m_serverTree = new ServerTreeWidget(this);
    m_serverTree->setServerManager(m_serverManager);
    m_serverTree->setFolderManager(m_folderManager);
    
    // Connect tree widget signals
    connect(m_serverTree, &ServerTreeWidget::serverDoubleClicked, this, &MainWindow::onServerDoubleClicked);
    connect(m_serverTree, &ServerTreeWidget::createFolderRequested, this, &MainWindow::onCreateFolderRequested);
    connect(m_serverTree, &ServerTreeWidget::renameFolderRequested, this, &MainWindow::onRenameFolderRequested);
    connect(m_serverTree, &ServerTreeWidget::deleteFolderRequested, this, &MainWindow::onDeleteFolderRequested);
    connect(m_serverTree, &ServerTreeWidget::addServerRequested, this, &MainWindow::onAddServerToFolderRequested);
    connect(m_serverTree, &ServerTreeWidget::editServerRequested, this, &MainWindow::onEditServerRequested);
    connect(m_serverTree, &ServerTreeWidget::deleteServerRequested, this, &MainWindow::onDeleteServerRequested);
    connect(m_serverTree, &ServerTreeWidget::moveServerRequested, this, &MainWindow::onMoveServerRequested);
    
    leftLayout->addWidget(m_serverTree);
    
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

void MainWindow::onServerDoubleClicked(const ServerConfig &server)
{
    connectToServer(server);
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
    // Tree widget will automatically refresh when servers change
}

ServerConfig MainWindow::getSelectedServer()
{
    return m_serverTree->getSelectedServer();
}

void MainWindow::onCreateFolderRequested(const QString &parentFolderId)
{
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("Create Folder"), 
                                               tr("Folder name:"), QLineEdit::Normal, 
                                               QString(), &ok);
    if (ok && !folderName.isEmpty()) {
        m_folderManager->createFolder(folderName, parentFolderId);
    }
}

void MainWindow::onRenameFolderRequested(const QString &folderId)
{
    if (!m_folderManager->folderExists(folderId)) {
        return;
    }
    
    FolderNode folder = m_folderManager->getFolder(folderId);
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Rename Folder"), 
                                            tr("New folder name:"), QLineEdit::Normal, 
                                            folder.name, &ok);
    if (ok && !newName.isEmpty() && newName != folder.name) {
        m_folderManager->renameFolder(folderId, newName);
    }
}

void MainWindow::onDeleteFolderRequested(const QString &folderId)
{
    if (!m_folderManager->folderExists(folderId)) {
        return;
    }
    
    FolderNode folder = m_folderManager->getFolder(folderId);
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              tr("Confirm Delete"),
                                                              tr("Are you sure you want to delete folder '%1'?\n\nThis will also delete all subfolders.").arg(folder.name),
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_folderManager->deleteFolder(folderId);
    }
}

void MainWindow::onAddServerToFolderRequested(const QString &folderId)
{
    add_Server dialog(this);
    dialog.setFolderManager(m_folderManager);
    dialog.setDefaultFolder(folderId);
    
    if (dialog.exec() == QDialog::Accepted) {
        ServerConfig config = dialog.getServerConfig();
        m_serverManager->addServer(config);
    }
}

void MainWindow::onEditServerRequested(const QString &serverId)
{
    ServerConfig server = m_serverManager->getServer(serverId);
    if (!server.isValid()) {
        return;
    }
    
    add_Server dialog(this);
    dialog.setFolderManager(m_folderManager);
    dialog.setServerConfig(server);
    
    if (dialog.exec() == QDialog::Accepted) {
        ServerConfig updatedConfig = dialog.getServerConfig();
        m_serverManager->updateServer(serverId, updatedConfig);
    }
}

void MainWindow::onDeleteServerRequested(const QString &serverId)
{
    ServerConfig server = m_serverManager->getServer(serverId);
    if (!server.isValid()) {
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, 
                                                              tr("Confirm Delete"),
                                                              tr("Are you sure you want to delete server '%1'?").arg(server.alias()),
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_serverManager->removeServer(serverId);
    }
}

void MainWindow::onMoveServerRequested(const QString &serverId, const QString &newFolderId)
{
    ServerConfig server = m_serverManager->getServer(serverId);
    if (!server.isValid()) {
        return;
    }
    
    server.setGroup(newFolderId);
    m_serverManager->updateServer(serverId, server);
}
