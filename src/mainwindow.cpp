#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add_server.h"
#include "sshterminal.h"
#include "sftpbrowser.h"
#include "terminalsplitwidget.h"
#include "settingsdialog.h"
#include "vt100terminal.h"
#include "settingsmanager.h"
#include "snippetdialog.h"
#include "snippetmanager.h"
#include "commandhistorydialog.h"
#include "passwordmanager.h"
#include "applockmanager.h"
#include "applockdialog.h"
#include "recentconnectionsmanager.h"
#include "sessionlogger.h"
#include "connectionlogsdialog.h"
#include "quickcommandsdialog.h"
#include "monitoringdialog.h"
#include "networkdiscoverydialog.h"
#include "profilemanagerdialog.h"
#include "globalhotkeymanager.h"
#include "remoteeditor.h"
#include <QComboBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <QTimer>
#include <QDir>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QActionGroup>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serverManager(new ServerManager(this))
    , m_folderManager(new FolderManager(this))
    , m_trayIcon(nullptr)
    , m_recentMenu(nullptr)
    , m_closing(false)
{
    ui->setupUi(this);
    setupUI();
    
    connect(m_serverManager, &ServerManager::serversChanged, this, &MainWindow::onServersChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSession();

    if (SettingsManager::instance().minimizeToTray() && m_trayIcon
        && QSystemTrayIcon::isSystemTrayAvailable()) {
        hide();
        event->ignore();
        return;
    }

    m_closing = true;
    event->accept();
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

    editMenu->addSeparator();
    QAction *snippetsAction = editMenu->addAction(tr("&Snippets..."));
    connect(snippetsAction, &QAction::triggered, this, &MainWindow::onSnippetsClicked);

    QAction *historyAction = editMenu->addAction(tr("Command &History..."));
    connect(historyAction, &QAction::triggered, this, &MainWindow::onCommandHistoryClicked);

    setupSplitMenu(editMenu);
    setupToolsMenu(editMenu);
    setupSecurityMenu(editMenu);
    setupLanguageMenu(editMenu);

    // Add Import/Export to File menu
    QMenu *fileMenu = ui->menuFile;
    fileMenu->insertSeparator(ui->actionAddServer);
    QAction *importAction = fileMenu->addAction(tr("&Import Servers..."));
    connect(importAction, &QAction::triggered, this, &MainWindow::onImportServersClicked);
    QAction *exportAction = fileMenu->addAction(tr("&Export Servers..."));
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportServersClicked);
    fileMenu->insertSeparator(ui->actionQuit);

    QAction *quickConnectAction = fileMenu->addAction(tr("Quick &Connect..."));
    quickConnectAction->setShortcut(QKeySequence("Ctrl+K"));
    connect(quickConnectAction, &QAction::triggered, this, &MainWindow::onQuickConnectClicked);

    QAction *editRemoteAction = fileMenu->addAction(tr("Edit Remote &File..."));
    connect(editRemoteAction, &QAction::triggered, this, &MainWindow::onEditRemoteFileClicked);

    setupRecentMenu(fileMenu);
    setupExternalTerminalMenu(fileMenu);
    fileMenu->insertSeparator(ui->actionQuit);

    // Prompt for master password on startup if one is set
    if (PasswordManager::instance().hasMasterPassword()
        && !PasswordManager::instance().isUnlocked()) {
        QTimer::singleShot(0, this, [this]() { promptUnlockPasswords(); });
    }

    setupTray();
    registerGlobalHotkeys();
    promptRestoreSession();
}

void MainWindow::setupSecurityMenu(QMenu *editMenu)
{
    QMenu *securityMenu = editMenu->addMenu(tr("&Security"));

    QAction *setMasterAction = securityMenu->addAction(tr("Set Master Password..."));
    connect(setMasterAction, &QAction::triggered, this, &MainWindow::onSetMasterPasswordClicked);

    QAction *unlockAction = securityMenu->addAction(tr("Unlock Passwords..."));
    connect(unlockAction, &QAction::triggered, this, &MainWindow::onUnlockPasswordsClicked);

    securityMenu->addSeparator();
    QAction *removeMasterAction = securityMenu->addAction(tr("Remove Master Password..."));
    connect(removeMasterAction, &QAction::triggered, this, &MainWindow::onRemoveMasterPasswordClicked);

    // Application lock (global password to enter the app)
    securityMenu->addSeparator();
    QMenu *appLockMenu = securityMenu->addMenu(tr("&Application Lock"));

    QAction *setAppLockAction = appLockMenu->addAction(tr("Set Application Password..."));
    connect(setAppLockAction, &QAction::triggered, this, &MainWindow::onSetAppPasswordClicked);

    QAction *changeAppLockAction = appLockMenu->addAction(tr("Change Application Password..."));
    connect(changeAppLockAction, &QAction::triggered, this, &MainWindow::onChangeAppPasswordClicked);

    QAction *removeAppLockAction = appLockMenu->addAction(tr("Remove Application Password..."));
    connect(removeAppLockAction, &QAction::triggered, this, &MainWindow::onRemoveAppPasswordClicked);

    appLockMenu->addSeparator();
    QAction *lockNowAction = appLockMenu->addAction(tr("Lock Application Now"));
    connect(lockNowAction, &QAction::triggered, this, &MainWindow::onLockApplicationClicked);

    const bool lockEnabled = AppLockManager::instance().isEnabled();
    setAppLockAction->setEnabled(!lockEnabled);
    changeAppLockAction->setEnabled(lockEnabled);
    removeAppLockAction->setEnabled(lockEnabled);
    lockNowAction->setEnabled(lockEnabled);
}

void MainWindow::setupLanguageMenu(QMenu *editMenu)
{
    QMenu *languageMenu = editMenu->addMenu(tr("&Language"));

    QActionGroup *group = new QActionGroup(languageMenu);
    group->setExclusive(true);

    QAction *enAction = languageMenu->addAction(tr("English"));
    enAction->setCheckable(true);
    enAction->setData(QStringLiteral("en"));
    group->addAction(enAction);

    QAction *esAction = languageMenu->addAction(QStringLiteral("Español"));
    esAction->setCheckable(true);
    esAction->setData(QStringLiteral("es"));
    group->addAction(esAction);

    const QString current = SettingsManager::instance().language();
    (current == QLatin1String("es") ? esAction : enAction)->setChecked(true);

    connect(group, &QActionGroup::triggered, this, [this](QAction *action) {
        onLanguageSelected(action->data().toString());
    });
}

void MainWindow::onLanguageSelected(const QString &lang)
{
    if (lang.isEmpty() || lang == SettingsManager::instance().language()) {
        return;
    }
    SettingsManager::instance().setLanguage(lang);
    SettingsManager::instance().save();

    // Restart the application so the new language is applied everywhere.
    QProcess::startDetached(QApplication::applicationFilePath(),
                            QApplication::arguments().mid(1));
    QApplication::quit();
}

void MainWindow::setupToolsMenu(QMenu *editMenu)
{
    QMenu *toolsMenu = editMenu->addMenu(tr("&Tools"));

    QAction *quickCommandsAction = toolsMenu->addAction(tr("Quick &Commands..."));
    connect(quickCommandsAction, &QAction::triggered, this, &MainWindow::onQuickCommandsClicked);

    QAction *monitoringAction = toolsMenu->addAction(tr("Server &Monitoring..."));
    connect(monitoringAction, &QAction::triggered, this, &MainWindow::onMonitoringClicked);

    QAction *discoveryAction = toolsMenu->addAction(tr("Network &Discovery..."));
    connect(discoveryAction, &QAction::triggered, this, &MainWindow::onNetworkDiscoveryClicked);

    QAction *logsAction = toolsMenu->addAction(tr("Connection &Logs..."));
    connect(logsAction, &QAction::triggered, this, &MainWindow::onConnectionLogsClicked);

    QAction *profilesAction = toolsMenu->addAction(tr("Connection &Profiles..."));
    connect(profilesAction, &QAction::triggered, this, &MainWindow::onProfilesClicked);
}

void MainWindow::setupSplitMenu(QMenu *editMenu)
{
    QMenu *splitMenu = editMenu->addMenu(tr("Split"));

    QAction *horizontalAction = splitMenu->addAction(tr("Split &Horizontally"));
    horizontalAction->setShortcut(QKeySequence("Ctrl+Shift+E"));
    connect(horizontalAction, &QAction::triggered, this, &MainWindow::onSplitHorizontalClicked);

    QAction *verticalAction = splitMenu->addAction(tr("Split &Vertically"));
    verticalAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    connect(verticalAction, &QAction::triggered, this, &MainWindow::onSplitVerticalClicked);

    QAction *newTerminalAction = splitMenu->addAction(tr("&New Terminal (same server)"));
    newTerminalAction->setShortcut(QKeySequence("Ctrl+Shift+T"));
    connect(newTerminalAction, &QAction::triggered, this, &MainWindow::onNewTerminalClicked);
}

void MainWindow::setupRecentMenu(QMenu *fileMenu)
{
    m_recentMenu = fileMenu->addMenu(tr("&Recent Connections"));
    rebuildRecentMenu();
}

void MainWindow::rebuildRecentMenu()
{
    if (!m_recentMenu) {
        return;
    }
    m_recentMenu->clear();

    const QList<RecentConnection> recent =
        RecentConnectionsManager::instance().recent();
    for (const RecentConnection &rc : recent) {
        QAction *action = m_recentMenu->addAction(
            QString("%1  (%2)")
                .arg(rc.alias, rc.lastConnected.toString("yyyy-MM-dd HH:mm")));
        connect(action, &QAction::triggered, this, [this, rc]() {
            onRecentConnectionTriggered(rc.serverId);
        });
    }

    if (recent.isEmpty()) {
        m_recentMenu->addAction(tr("(No recent connections)"))->setEnabled(false);
    }

    m_recentMenu->addSeparator();
    QAction *clearAction = m_recentMenu->addAction(tr("Clear Recent"));
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearRecentClicked);
}

void MainWindow::setupExternalTerminalMenu(QMenu *fileMenu)
{
    QMenu *externalMenu = fileMenu->addMenu(tr("Open in External Terminal"));

    QAction *windowsTerminalAction = externalMenu->addAction(tr("Windows Terminal"));
    windowsTerminalAction->setEnabled(false);
#ifdef Q_OS_WIN
    windowsTerminalAction->setEnabled(true);
    connect(windowsTerminalAction, &QAction::triggered, this,
            [this]() { onOpenInExternalTerminal(); });
#endif

    QAction *iterm2Action = externalMenu->addAction(tr("iTerm2"));
    iterm2Action->setEnabled(false);
#ifdef Q_OS_MACOS
    iterm2Action->setEnabled(true);
    connect(iterm2Action, &QAction::triggered, this,
            [this]() { onOpenInExternalTerminal(); });
#endif

#ifdef Q_OS_LINUX
    QAction *linuxTerminalAction = externalMenu->addAction(tr("System Terminal"));
    connect(linuxTerminalAction, &QAction::triggered, this,
            [this]() { onOpenInExternalTerminal(); });
#endif
}

void MainWindow::setupTray()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }

    QIcon icon = windowIcon();
    if (icon.isNull()) {
        icon = QIcon::fromTheme("utilities-terminal");
    }

    m_trayIcon = new QSystemTrayIcon(icon, this);
    m_trayIcon->setToolTip(tr("QTiSSH"));

    QMenu *menu = new QMenu(this);
    QAction *showAction = menu->addAction(tr("Show / Hide Window"));
    connect(showAction, &QAction::triggered, this, &MainWindow::onToggleWindowGlobal);
    QAction *quitAction = menu->addAction(tr("Quit"));
    connect(quitAction, &QAction::triggered, this, [this]() {
        m_closing = true;
        m_trayIcon->hide();
        close();
    });
    m_trayIcon->setContextMenu(menu);
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger
                    || reason == QSystemTrayIcon::DoubleClick) {
                    onToggleWindowGlobal();
                }
            });
}

void MainWindow::registerGlobalHotkeys()
{
    SettingsManager &sm = SettingsManager::instance();
    GlobalHotkeyManager &ghm = GlobalHotkeyManager::instance();
    ghm.init(this);
    connect(&ghm, &GlobalHotkeyManager::quickConnectPressed,
            this, &MainWindow::onQuickConnectGlobal);
    connect(&ghm, &GlobalHotkeyManager::toggleWindowPressed,
            this, &MainWindow::onToggleWindowGlobal);
    ghm.registerSequences(sm.globalQuickConnect(), sm.globalToggleWindow());
}

void MainWindow::onQuickConnectGlobal()
{
    QTimer::singleShot(0, this, &MainWindow::onQuickConnectClicked);
}

void MainWindow::onToggleWindowGlobal()
{
    if (isVisible() && !isMinimized()) {
        hide();
    } else {
        showNormal();
        raise();
        activateWindow();
    }
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
    
    SettingsManager &sm = SettingsManager::instance();
    dialog.setTerminalFont(sm.terminalFont());
    dialog.setCursorStyle(sm.cursorStyle());
    dialog.setForegroundColor(sm.terminalForeground());
    dialog.setBackgroundColor(sm.terminalBackground());
    dialog.setMinimizeToTray(sm.minimizeToTray());
    dialog.setGlobalQuickConnect(sm.globalQuickConnect());
    dialog.setGlobalToggleWindow(sm.globalToggleWindow());
    
    if (dialog.exec() == QDialog::Accepted) {
        QFont font = dialog.terminalFont();
        int fontSize = dialog.fontSize();
        if (fontSize > 0) {
            font.setPointSize(fontSize);
        }
        VT100Terminal::CursorStyle style = static_cast<VT100Terminal::CursorStyle>(dialog.cursorStyle());
        QColor fg = dialog.foregroundColor();
        QColor bg = dialog.backgroundColor();
        
        // Save to settings
        sm.setTerminalFont(font);
        sm.setTerminalFontSize(fontSize);
        sm.setCursorStyle(style);
        sm.setTerminalForeground(fg);
        sm.setTerminalBackground(bg);
        sm.setMinimizeToTray(dialog.minimizeToTray());
        sm.setGlobalQuickConnect(dialog.globalQuickConnect());
        sm.setGlobalToggleWindow(dialog.globalToggleWindow());
        sm.save();

        registerGlobalHotkeys();
        
        // Apply to all active terminals
        for (int i = 0; i < m_tabWidget->count(); ++i) {
            QWidget *widget = m_tabWidget->widget(i);
            if (auto *split = qobject_cast<TerminalSplitWidget*>(widget)) {
                split->applySettings(font, style, fg, bg);
            } else if (auto *terminal = qobject_cast<SSHTerminal*>(widget)) {
                terminal->setTerminalFont(font);
                terminal->setCursorStyle(style);
                terminal->setTerminalColors(fg, bg);
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

    openSftpTab(server);
}

void MainWindow::onServerDoubleClicked(const ServerConfig &server)
{
    connectToServer(server);
}

void MainWindow::connectToServer(const ServerConfig &config)
{
    auto *split = new TerminalSplitWidget(config, this);
    split->applySettings(SettingsManager::instance().terminalFont(),
                         SettingsManager::instance().cursorStyle(),
                         SettingsManager::instance().terminalForeground(),
                         SettingsManager::instance().terminalBackground());

    QString baseTitle = QString("%1 (%2)").arg(config.alias()).arg(config.host());
    int tabIndex = m_tabWidget->addTab(split, "⏳ " + baseTitle);
    m_tabWidget->setCurrentIndex(tabIndex);
    updateTerminalTabTitle(split, baseTitle);

    for (SSHTerminal *terminal : split->terminals()) {
        connect(terminal, &SSHTerminal::connectionStateChanged, this,
                [this, split, baseTitle]() { updateTerminalTabTitle(split, baseTitle); });
    }
    connect(split, &TerminalSplitWidget::activeTerminalChanged, this,
            [this, split, baseTitle]() { updateTerminalTabTitle(split, baseTitle); });

    RecentConnectionsManager::instance().add(config.id(), config.alias());
    rebuildRecentMenu();
    split->connectToServer();
}

void MainWindow::openSftpTab(const ServerConfig &config)
{
    SFTPBrowser *sftpBrowser = new SFTPBrowser(config, this);

    QString tabTitle = QString("📁 %1 SFTP").arg(config.alias());
    int tabIndex = m_tabWidget->addTab(sftpBrowser, tabTitle);
    m_tabWidget->setCurrentIndex(tabIndex);

    connect(sftpBrowser, &SFTPBrowser::connectionStateChanged, this,
            [this, sftpBrowser, tabTitle](bool connected) {
                int index = m_tabWidget->indexOf(sftpBrowser);
                if (index != -1) {
                    m_tabWidget->setTabText(index,
                        (connected ? "🟢 " : "🔴 ") + tabTitle);
                }
            });

    sftpBrowser->connectToServer();
}

void MainWindow::connectToServerById(const QString &serverId)
{
    ServerConfig config = m_serverManager->getServer(serverId);
    if (config.isValid()) {
        connectToServer(config);
    } else {
        QMessageBox::warning(this, tr("Connect"),
                             tr("The server for this connection no longer exists."));
    }
}

void MainWindow::onTabCloseRequested(int index)
{
    // Don't close the welcome tab
    if (index == 0) {
        return;
    }

    QWidget *widget = m_tabWidget->widget(index);

    bool isConnected = false;
    if (auto *split = qobject_cast<TerminalSplitWidget*>(widget)) {
        for (SSHTerminal *terminal : split->terminals()) {
            if (terminal->isConnected()) {
                isConnected = true;
                break;
            }
        }
    } else if (auto *terminal = qobject_cast<SSHTerminal*>(widget)) {
        isConnected = terminal->isConnected();
    } else if (auto *sftpBrowser = qobject_cast<SFTPBrowser*>(widget)) {
        isConnected = sftpBrowser->isConnected();
    }

    if (isConnected) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  tr("Close Connection"),
                                                                  tr("This connection is still active. Are you sure you want to close it?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }

        if (auto *split = qobject_cast<TerminalSplitWidget*>(widget)) {
            for (SSHTerminal *terminal : split->terminals()) {
                terminal->disconnectFromServer();
            }
        } else if (auto *terminal = qobject_cast<SSHTerminal*>(widget)) {
            terminal->disconnectFromServer();
        } else if (auto *sftpBrowser = qobject_cast<SFTPBrowser*>(widget)) {
            sftpBrowser->disconnectFromServer();
        }
    }

    m_tabWidget->removeTab(index);
    delete widget;
}

void MainWindow::writeSession()
{
    QJsonArray tabs;
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        QWidget *widget = m_tabWidget->widget(i);

        if (auto *split = qobject_cast<TerminalSplitWidget*>(widget)) {
            SSHTerminal *terminal = split->activeTerminal();
            if (terminal) {
                QJsonObject tab;
                tab["type"] = "terminal";
                tab["serverId"] = terminal->getServerConfig().id();
                tab["split"] = QString::number(split->count());
                tabs.append(tab);
            }
        } else if (auto *sftp = qobject_cast<SFTPBrowser*>(widget)) {
            QJsonObject tab;
            tab["type"] = "sftp";
            tab["serverId"] = sftp->getServerConfig().id();
            tabs.append(tab);
        }
    }

    QJsonObject root;
    root["tabs"] = tabs;

    const QString dir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
                        + "/QTiSSH";
    QDir().mkpath(dir);
    QFile file(dir + "/session.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    }
}

void MainWindow::restoreSession()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
                         + "/QTiSSH/session.json";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray tabs = doc.object()["tabs"].toArray();
    for (const QJsonValue &value : tabs) {
        QJsonObject tab = value.toObject();
        QString type = tab["type"].toString();
        QString serverId = tab["serverId"].toString();
        ServerConfig config = m_serverManager->getServer(serverId);
        if (!config.isValid()) {
            continue;
        }

        if (type == "sftp") {
            openSftpTab(config);
        } else {
            connectToServer(config);
            int splits = tab["split"].toInt();
            if (splits > 1) {
                if (auto *split = qobject_cast<TerminalSplitWidget*>(m_tabWidget->currentWidget())) {
                    for (int s = 1; s < splits; ++s) {
                        split->splitTerminal(Qt::Vertical);
                    }
                    split->applySettings(SettingsManager::instance().terminalFont(),
                                         SettingsManager::instance().cursorStyle(),
                                         SettingsManager::instance().terminalForeground(),
                                         SettingsManager::instance().terminalBackground());
                }
            }
        }
    }

    QFile::remove(path);
}

void MainWindow::promptRestoreSession()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
                         + "/QTiSSH/session.json";
    if (!QFile::exists(path)) {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Restore Session"),
        tr("A previous session was found. Restore the open tabs?"));
    if (reply == QMessageBox::Yes) {
        restoreSession();
    } else {
        QFile::remove(path);
    }
}

void MainWindow::updateTerminalTabTitle(QWidget *tabWidget, const QString &baseTitle)
{
    int index = m_tabWidget->indexOf(tabWidget);
    if (index < 0) {
        return;
    }

    SSHTerminal *terminal = nullptr;
    if (auto *split = qobject_cast<TerminalSplitWidget*>(tabWidget)) {
        terminal = split->activeTerminal();
    } else {
        terminal = qobject_cast<SSHTerminal*>(tabWidget);
    }

    bool connected = terminal && terminal->isConnected();
    m_tabWidget->setTabText(index, (connected ? "🟢 " : "🔴 ") + baseTitle);
}

void MainWindow::splitCurrentTab(Qt::Orientation orientation)
{
    QWidget *current = m_tabWidget->currentWidget();
    auto *split = qobject_cast<TerminalSplitWidget*>(current);
    if (!split) {
        QMessageBox::information(this, tr("Split"),
                                 tr("Open an SSH terminal tab first to split it."));
        return;
    }

    split->splitTerminal(orientation);
    split->applySettings(SettingsManager::instance().terminalFont(),
                         SettingsManager::instance().cursorStyle(),
                         SettingsManager::instance().terminalForeground(),
                         SettingsManager::instance().terminalBackground());

    SSHTerminal *configTerminal = split->activeTerminal();
    QString baseTitle = configTerminal
        ? QString("%1 (%2)").arg(configTerminal->getServerConfig().alias())
                             .arg(configTerminal->getServerConfig().host())
        : QString();

    for (SSHTerminal *terminal : split->terminals()) {
        connect(terminal, &SSHTerminal::connectionStateChanged, this,
                [this, split, baseTitle]() { updateTerminalTabTitle(split, baseTitle); });
    }
    updateTerminalTabTitle(split, baseTitle);
}

void MainWindow::onSplitHorizontalClicked()
{
    splitCurrentTab(Qt::Horizontal);
}

void MainWindow::onSplitVerticalClicked()
{
    splitCurrentTab(Qt::Vertical);
}

void MainWindow::onNewTerminalClicked()
{
    splitCurrentTab(Qt::Vertical);
}

void MainWindow::onOpenInExternalTerminal()
{
    ServerConfig server = getSelectedServer();
    if (!server.isValid()) {
        QMessageBox::information(this, tr("External Terminal"),
                                 tr("Select a server in the list first."));
        return;
    }

    QString sshBinary = "ssh";
    QStringList args;
    args << "-o" << "StrictHostKeyChecking=no";
    if (!server.port() || server.port() != 22) {
        args << "-p" << QString::number(server.port());
    }
    args << QString("%1@%2").arg(server.username()).arg(server.host());

    QStringList launch;
#ifdef Q_OS_WIN
    launch << "wt.exe" << "-d" << "~" << sshBinary;
#elif defined(Q_OS_MACOS)
    args.prepend(sshBinary);
    launch << "open" << "-a" << "iTerm" << "--args";
#else
    launch << "x-terminal-emulator";
#endif
    launch << args;

    if (!QProcess::startDetached(launch.first(), args.mid(1))) {
        QMessageBox::warning(this, tr("External Terminal"),
                             tr("Failed to launch the external terminal."));
    }
}

void MainWindow::onQuickConnectClicked()
{
    bool ok = false;
    QString text = QInputDialog::getText(
        this, tr("Quick Connect"),
        tr("Host (optionally user@host):"), QLineEdit::Normal,
        QString(), &ok);
    if (!ok || text.trimmed().isEmpty()) {
        return;
    }

    QString host = text.trimmed();
    QString username = QDir::home().dirName();

    int at = host.indexOf('@');
    if (at >= 0) {
        username = host.left(at);
        host = host.mid(at + 1);
    }

    ServerConfig config;
    config.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
    config.setAlias(host);
    config.setHost(host);
    config.setUsername(username);
    config.setPort(22);
    config.setGroup("root");

    connectToServer(config);
}

void MainWindow::onEditRemoteFileClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Edit Remote File"));

    auto *layout = new QVBoxLayout(&dialog);
    auto *form = new QFormLayout();
    auto *serverCombo = new QComboBox(&dialog);
    for (const ServerConfig &server : m_serverManager->getAllServers()) {
        if (server.isValid()) {
            serverCombo->addItem(QString("%1 (%2)").arg(server.alias(), server.host()),
                                 server.id());
        }
    }
    auto *pathEdit = new QLineEdit(&dialog);
    pathEdit->setPlaceholderText("/home/user/file.txt");
    form->addRow(tr("Server:"), serverCombo);
    form->addRow(tr("Remote path:"), pathEdit);
    layout->addLayout(form);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Open"));
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString remotePath = pathEdit->text().trimmed();
    if (remotePath.isEmpty()) {
        QMessageBox::warning(this, tr("Edit Remote File"), tr("Please enter a remote path."));
        return;
    }

    QString serverId = serverCombo->currentData().toString();
    ServerConfig server = m_serverManager->getServer(serverId);
    if (!server.isValid()) {
        QMessageBox::warning(this, tr("Edit Remote File"), tr("Please select a server."));
        return;
    }

    openRemoteEditor(server, remotePath);
}

void MainWindow::openRemoteEditor(const ServerConfig &config, const QString &remotePath)
{
    auto *editor = new RemoteEditor(config, remotePath, this);
    int index = m_tabWidget->addTab(editor, QString("✏️ %1").arg(config.alias()));
    m_tabWidget->setCurrentIndex(index);
}

void MainWindow::onRecentConnectionTriggered(const QString &serverId)
{
    connectToServerById(serverId);
}

void MainWindow::onClearRecentClicked()
{
    RecentConnectionsManager::instance().clear();
    rebuildRecentMenu();
}

void MainWindow::onQuickCommandsClicked()
{
    SSHTerminal *terminal = currentTerminal();
    if (!terminal) {
        QMessageBox::information(this, tr("Quick Commands"),
                                 tr("Open an SSH terminal tab first."));
        return;
    }

    QuickCommandsDialog dialog(this);
    connect(&dialog, &QuickCommandsDialog::executeRequested, terminal,
            [terminal](const QString &command) {
                terminal->executeCommand(command);
            });
    dialog.exec();
}

void MainWindow::onMonitoringClicked()
{
    ServerConfig server = getSelectedServer();
    if (!server.isValid()) {
        QMessageBox::information(this, tr("Server Monitoring"),
                                 tr("Select a server in the list first."));
        return;
    }

    MonitoringDialog dialog(server, this);
    dialog.exec();
}

void MainWindow::onNetworkDiscoveryClicked()
{
    NetworkDiscoveryDialog dialog(this);
    connect(&dialog, &NetworkDiscoveryDialog::connectRequested,
            this, &MainWindow::onDiscoveryConnect);
    connect(&dialog, &NetworkDiscoveryDialog::addServerRequested,
            this, &MainWindow::onDiscoveryAddServer);
    dialog.exec();
}

void MainWindow::onConnectionLogsClicked()
{
    ConnectionLogsDialog dialog(this);
    dialog.exec();
}

void MainWindow::onProfilesClicked()
{
    ProfileManagerDialog dialog(this);
    dialog.exec();
}

void MainWindow::onDiscoveryConnect(const QHostAddress &host, const QString &username)
{
    ServerConfig config;
    config.setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
    config.setAlias(host.toString());
    config.setHost(host.toString());
    config.setUsername(username);
    config.setPort(22);
    config.setGroup("root");

    connectToServer(config);
}

void MainWindow::onDiscoveryAddServer(const QHostAddress &host)
{
    ServerConfig config;
    config.setAlias(host.toString());
    config.setHost(host.toString());
    config.setUsername(QDir::home().dirName());
    config.setPort(22);

    add_Server dialog(this);
    dialog.setFolderManager(m_folderManager);
    dialog.setServerConfig(config);
    if (dialog.exec() == QDialog::Accepted) {
        m_serverManager->addServer(dialog.getServerConfig());
    }
}

QHostAddress MainWindow::parseHost(const QString &host) const
{
    QHostAddress address;
    if (address.setAddress(host)) {
        return address;
    }
    return QHostAddress();
}

SSHTerminal *MainWindow::currentTerminal() const
{
    QWidget *widget = m_tabWidget->currentWidget();
    if (auto *split = qobject_cast<TerminalSplitWidget*>(widget)) {
        return split->activeTerminal();
    }
    return qobject_cast<SSHTerminal*>(widget);
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

void MainWindow::onSnippetsClicked()
{
    SnippetDialog dialog(this);
    connect(&dialog, &SnippetDialog::executeRequested, this, &MainWindow::onExecuteSnippet);
    dialog.exec();
}

void MainWindow::onCommandHistoryClicked()
{
    SSHTerminal *terminal = currentTerminal();
    if (!terminal) {
        QMessageBox::information(this, tr("Command History"),
                                 tr("Open an SSH terminal tab first."));
        return;
    }

    ServerConfig config = terminal->getServerConfig();
    QString label = QString("%1 (%2)").arg(config.alias(), config.host());
    CommandHistoryDialog dialog(config.id(), label, this);
    connect(&dialog, &CommandHistoryDialog::executeRequested,
            this, &MainWindow::onExecuteHistoryCommand);
    dialog.exec();
}

void MainWindow::onExportServersClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Export Servers"),
                                                QDir::homePath() + "/qtissh-servers.json",
                                                tr("JSON Files (*.json)"));
    if (path.isEmpty()) {
        return;
    }

    if (m_serverManager->exportToFile(path)) {
        QMessageBox::information(this, tr("Export"),
                                 tr("Servers exported to %1").arg(path));
    } else {
        QMessageBox::warning(this, tr("Export"), tr("Failed to export servers."));
    }
}

void MainWindow::onImportServersClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Import Servers"),
                                                QDir::homePath(),
                                                tr("JSON Files (*.json)"));
    if (path.isEmpty()) {
        return;
    }

    int count = m_serverManager->importFromFile(path);
    QMessageBox::information(this, tr("Import"),
                             tr("Imported %1 server(s).").arg(count));
}

void MainWindow::promptUnlockPasswords()
{
    PasswordManager &pm = PasswordManager::instance();
    if (!pm.hasMasterPassword() || pm.isUnlocked()) {
        return;
    }

    bool ok = false;
    QString password = QInputDialog::getText(this, tr("Master Password"),
                                             tr("Enter the master password to unlock stored credentials:"),
                                             QLineEdit::Password, QString(), &ok);
    if (!ok) {
        return;
    }

    if (!pm.unlock(password)) {
        QMessageBox::warning(this, tr("Unlock"), tr("Incorrect master password."));
        return;
    }

    m_serverManager->loadServers();
    m_serverManager->saveServers();
    emit m_serverManager->serversChanged();
    QMessageBox::information(this, tr("Unlock"), tr("Credentials unlocked."));
}

void MainWindow::onSetMasterPasswordClicked()
{
    PasswordManager &pm = PasswordManager::instance();
    if (pm.hasMasterPassword()) {
        QMessageBox::information(this, tr("Master Password"),
                                 tr("A master password is already set. Remove it first to change it."));
        return;
    }

    bool ok = false;
    QString password = QInputDialog::getText(this, tr("Set Master Password"),
                                             tr("Enter a master password to encrypt stored credentials:"),
                                             QLineEdit::Password, QString(), &ok);
    if (!ok || password.isEmpty()) {
        return;
    }

    bool ok2 = false;
    QString confirm = QInputDialog::getText(this, tr("Set Master Password"),
                                            tr("Confirm the master password:"),
                                            QLineEdit::Password, QString(), &ok2);
    if (!ok2 || confirm != password) {
        QMessageBox::warning(this, tr("Set Master Password"),
                             tr("Passwords do not match or confirmation was cancelled."));
        return;
    }

    if (pm.setMasterPassword(password)) {
        m_serverManager->encryptAllPasswords();
        m_serverManager->saveServers();
        QMessageBox::information(this, tr("Set Master Password"),
                                 tr("Master password set. Credentials are now encrypted."));
    }
}

void MainWindow::onUnlockPasswordsClicked()
{
    promptUnlockPasswords();
}

void MainWindow::onRemoveMasterPasswordClicked()
{
    PasswordManager &pm = PasswordManager::instance();
    if (!pm.hasMasterPassword()) {
        QMessageBox::information(this, tr("Master Password"),
                                 tr("No master password is currently set."));
        return;
    }

    if (!pm.isUnlocked()) {
        promptUnlockPasswords();
    }

    if (!pm.isUnlocked()) {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Remove Master Password"),
        tr("This will store all passwords in plaintext. Continue?"));
    if (reply != QMessageBox::Yes) {
        return;
    }

    m_serverManager->decryptAllPasswords();
    m_serverManager->saveServers();
    pm.removeMasterPassword();
    QMessageBox::information(this, tr("Remove Master Password"),
                             tr("Master password removed. Credentials are stored in plaintext."));
}

void MainWindow::onSetAppPasswordClicked()
{
    AppLockManager &lock = AppLockManager::instance();
    if (lock.isEnabled()) {
        QMessageBox::information(this, tr("Application Lock"),
                                 tr("An application password is already set. Use "
                                    "'Change Application Password' to update it."));
        return;
    }

    bool ok = false;
    QString password = QInputDialog::getText(
        this, tr("Set Application Password"),
        tr("Set a password required to open QTiSSH:"),
        QLineEdit::Password, QString(), &ok);
    if (!ok || password.isEmpty()) {
        return;
    }

    bool ok2 = false;
    QString confirm = QInputDialog::getText(
        this, tr("Set Application Password"),
        tr("Confirm the application password:"),
        QLineEdit::Password, QString(), &ok2);
    if (!ok2 || confirm != password) {
        QMessageBox::warning(this, tr("Set Application Password"),
                             tr("The passwords do not match."));
        return;
    }

    if (!lock.setPassword(password)) {
        QMessageBox::warning(this, tr("Set Application Password"),
                             tr("Could not save the application password."));
        return;
    }

    QMessageBox::information(this, tr("Application Lock"),
                             tr("Application password set. QTiSSH will now require "
                                "it to open."));
}

void MainWindow::onChangeAppPasswordClicked()
{
    AppLockManager &lock = AppLockManager::instance();
    if (!lock.isEnabled()) {
        QMessageBox::information(this, tr("Application Lock"),
                                 tr("No application password is currently set."));
        return;
    }

    bool ok = false;
    QString current = QInputDialog::getText(
        this, tr("Change Application Password"),
        tr("Enter the current application password:"),
        QLineEdit::Password, QString(), &ok);
    if (!ok || !lock.verify(current)) {
        QMessageBox::warning(this, tr("Change Application Password"),
                             tr("Incorrect current password."));
        return;
    }

    bool ok2 = false;
    QString newPassword = QInputDialog::getText(
        this, tr("Change Application Password"),
        tr("Enter the new application password:"),
        QLineEdit::Password, QString(), &ok2);
    if (!ok2 || newPassword.isEmpty()) {
        return;
    }

    bool ok3 = false;
    QString confirm = QInputDialog::getText(
        this, tr("Change Application Password"),
        tr("Confirm the new application password:"),
        QLineEdit::Password, QString(), &ok3);
    if (!ok3 || confirm != newPassword) {
        QMessageBox::warning(this, tr("Change Application Password"),
                             tr("The passwords do not match."));
        return;
    }

    if (!lock.setPassword(newPassword)) {
        QMessageBox::warning(this, tr("Change Application Password"),
                             tr("Could not save the new password."));
        return;
    }

    QMessageBox::information(this, tr("Application Lock"),
                             tr("Application password updated."));
}

void MainWindow::onRemoveAppPasswordClicked()
{
    AppLockManager &lock = AppLockManager::instance();
    if (!lock.isEnabled()) {
        QMessageBox::information(this, tr("Application Lock"),
                                 tr("No application password is currently set."));
        return;
    }

    bool ok = false;
    QString current = QInputDialog::getText(
        this, tr("Remove Application Password"),
        tr("Enter the application password to confirm removal:"),
        QLineEdit::Password, QString(), &ok);
    if (!ok || !lock.verify(current)) {
        QMessageBox::warning(this, tr("Remove Application Password"),
                             tr("Incorrect password."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Remove Application Password"),
        tr("Remove the application password? QTiSSH will no longer ask for it."));
    if (reply != QMessageBox::Yes) {
        return;
    }

    lock.remove();
    QMessageBox::information(this, tr("Application Lock"),
                             tr("Application password removed."));
}

void MainWindow::onLockApplicationClicked()
{
    AppLockManager &lock = AppLockManager::instance();
    if (!lock.isEnabled()) {
        return;
    }

    hide();
    if (!AppLockDialog::prompt(this)) {
        qApp->quit();
        return;
    }
    showNormal();
    raise();
    activateWindow();
}

void MainWindow::onExecuteSnippet(const QString &command)
{
    SSHTerminal *terminal = currentTerminal();
    if (!terminal) {
        QMessageBox::information(this, tr("Snippets"),
                                 tr("Open an SSH terminal tab to execute the snippet."));
        return;
    }
    terminal->executeCommand(command);
}

void MainWindow::onExecuteHistoryCommand(const QString &command)
{
    SSHTerminal *terminal = currentTerminal();
    if (!terminal) {
        return;
    }
    terminal->executeCommand(command);
}
