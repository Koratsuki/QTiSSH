#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add_server.h"
#include "sshterminal.h"
#include "sftpbrowser.h"
#include "settingsdialog.h"
#include "vt100terminal.h"
#include "settingsmanager.h"
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
    
    connect(m_serverManager, &ServerManager::serversChanged, this, &MainWindow::onServersChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Get references to UI elements created by Qt Designer
    m_serverTree = ui->serverTree;
    m_tabWidget = ui->tabWidget;
    
    // Set up the server tree widget
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
    
    // Connect button signals (buttons are already created in UI file)
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddServerClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::onEditServerClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteServerClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(ui->connectSftpButton, &QPushButton::clicked, this, &MainWindow::onConnectSftpClicked);

    // Create and add Search Bar and Theme Toggle to left panel
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText(tr("Search servers..."));
    m_searchBar->setClearButtonEnabled(true);
    
    m_themeButton = new QToolButton(this);
    m_themeButton->setText(tr("🌙"));
    m_themeButton->setToolTip(tr("Toggle Dark/Light Theme"));
    
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(m_searchBar);
    searchLayout->addWidget(m_themeButton);
    
    // Insert search layout above the server tree (which is at index 1 in the left panel's VBoxLayout)
    QVBoxLayout *leftLayout = qobject_cast<QVBoxLayout*>(ui->leftPanel->layout());
    if (leftLayout) {
        leftLayout->insertLayout(1, searchLayout);
    }
    
    connect(m_searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(m_themeButton, &QToolButton::clicked, this, &MainWindow::onThemeToggleClicked);
    
    // Connect tab widget signals
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    
    // Connect menu actions (defined in UI file)
    connect(ui->actionAddServer, &QAction::triggered, this, &MainWindow::onAddServerClicked);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAboutClicked);
    
    ui->mainSplitter->setStretchFactor(0, 0);
    ui->mainSplitter->setStretchFactor(1, 1);

    // Apply saved theme
    ThemeManager::instance().applyTheme(SettingsManager::instance().theme());

    // Force menu bar to be visible and non-native (inside the window)
    ui->menubar->setNativeMenuBar(false);
    this->setMenuBar(ui->menubar);
    ui->menubar->setVisible(true);

    // Create Edit Menu if it doesn't exist
    QMenu *editMenu = nullptr;
    for (QAction *action : ui->menubar->actions()) {
        if (action->menu() && action->text().contains("Edit")) {
            editMenu = action->menu();
            break;
        }
    }
    
    if (!editMenu) {
        editMenu = new QMenu(tr("&Edit"), this);
        // Try to insert before Help, otherwise just add
        if (ui->menuHelp) {
            ui->menubar->insertMenu(ui->menuHelp->menuAction(), editMenu);
        } else {
            ui->menubar->addMenu(editMenu);
        }
    }
    
    QAction *cutAction = editMenu->addAction(tr("Cu&t"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, this, &MainWindow::onCutClicked);

    QAction *copyAction = editMenu->addAction(tr("&Copy"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onCopyClicked);
    
    QAction *pasteAction = editMenu->addAction(tr("&Paste"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onPasteClicked);
    
    editMenu->addSeparator();
    
    QAction *optionsAction = editMenu->addAction(tr("&Options..."));
    connect(optionsAction, &QAction::triggered, this, &MainWindow::onOptionsClicked);
}

void MainWindow::onAboutClicked()
{
    QMessageBox::about(this, tr("About QTiSSH"),
                      tr("QTiSSH v0.1.4\n\nA Qt-based SSH Connection Manager\n\n"
                         "Features:\n"
                         "- Multiple simultaneous SSH connections\n"
                         "- Password and public key authentication\n"
                         "- Server configuration management\n"
                         "- SFTP file browser\n"
                         "- Folder organization"));
}

void MainWindow::onThemeToggleClicked()
{
    ThemeManager &tm = ThemeManager::instance();
    SettingsManager &sm = SettingsManager::instance();
    if (tm.currentTheme() == ThemeManager::Light) {
        tm.applyTheme(ThemeManager::Dark);
        sm.setTheme(ThemeManager::Dark);
        m_themeButton->setText(tr("☀️"));
    } else {
        tm.applyTheme(ThemeManager::Light);
        sm.setTheme(ThemeManager::Light);
        m_themeButton->setText(tr("🌙"));
    }
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    m_serverTree->filterServers(text);
}

void MainWindow::onCopyClicked()
{
    QWidget *current = m_tabWidget->currentWidget();
    SSHTerminal *terminal = qobject_cast<SSHTerminal*>(current);
    if (terminal) {
        terminal->copy();
    }
}

void MainWindow::onPasteClicked()
{
    QWidget *current = m_tabWidget->currentWidget();
    SSHTerminal *terminal = qobject_cast<SSHTerminal*>(current);
    if (terminal) {
        terminal->paste();
    }
}

void MainWindow::onCutClicked()
{
    // Cut is usually not applicable in a terminal, but we can copy
    onCopyClicked();
}

void MainWindow::onOptionsClicked()
{
    SettingsDialog dialog(this);
    
    // Initialize with current settings from active tab
    SSHTerminal *currentTerminal = qobject_cast<SSHTerminal*>(m_tabWidget->currentWidget());
    if (currentTerminal) {
        // We'd need getters in VT100Terminal for this to be perfect, 
        // but for now let's just use defaults or assume they exist.
    }
    
    if (dialog.exec() == QDialog::Accepted) {
        QFont font = dialog.terminalFont();
        VT100Terminal::CursorStyle style = static_cast<VT100Terminal::CursorStyle>(dialog.cursorStyle());
        
        // Save to settings
        SettingsManager &sm = SettingsManager::instance();
        sm.setTerminalFont(font);
        sm.setCursorStyle(style);
        
        // Apply to all active terminals
        for (int i = 0; i < m_tabWidget->count(); ++i) {
            SSHTerminal *terminal = qobject_cast<SSHTerminal*>(m_tabWidget->widget(i));
            if (terminal) {
                terminal->setTerminalFont(font);
                terminal->setCursorStyle(style);
            }
        }
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

void MainWindow::onServerDoubleClicked(const ServerConfig &server)
{
    connectToServer(server);
}

void MainWindow::connectToServer(const ServerConfig &config)
{
    // Create new terminal tab
    SSHTerminal *terminal = new SSHTerminal(config, this);
    
    // Apply saved terminal settings
    SettingsManager &sm = SettingsManager::instance();
    terminal->setTerminalFont(sm.terminalFont());
    terminal->setCursorStyle(sm.cursorStyle());
    
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
