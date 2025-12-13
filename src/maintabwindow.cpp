#include "maintabwindow.h"
#include "terminalsession.h"
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QMenu>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDebug>

MainTabWindow::MainTabWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_sessionManager(nullptr)
    , m_centralWidget(nullptr)
    , m_mainSplitter(nullptr)
    , m_sessionPanel(nullptr)
    , m_sessionTree(nullptr)
    , m_tabWidget(nullptr)
    , m_sessionPanelVisible(true)
{
    setWindowTitle("QTiSSH Pro - Terminal Manager");
    setMinimumSize(1000, 600);
    resize(1400, 800);
    
    // Create session manager
    m_sessionManager = new SessionManager(this);
    
    setupUI();
    setupConnections();
    setupTheme();
    
    // Create a welcome local shell session
    onNewLocalShell();
}

MainTabWindow::~MainTabWindow()
{
    // SessionManager will be deleted automatically as a child
}

void MainTabWindow::setupUI()
{
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    
    // Create central widget with splitter
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    m_mainSplitter = new QSplitter(Qt::Horizontal, m_centralWidget);
    
    setupSessionPanel();
    setupTabWidget();
    
    // Add panels to splitter
    m_mainSplitter->addWidget(m_sessionPanel);
    m_mainSplitter->addWidget(m_tabWidget);
    
    // Set splitter proportions (20% for session panel, 80% for tabs)
    m_mainSplitter->setSizes({200, 800});
    m_mainSplitter->setCollapsible(0, true);  // Session panel can be collapsed
    m_mainSplitter->setCollapsible(1, false); // Tab widget cannot be collapsed
    
    // Layout central widget
    QHBoxLayout *centralLayout = new QHBoxLayout(m_centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->addWidget(m_mainSplitter);
}

void MainTabWindow::setupMenuBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    
    m_newSessionAction = new QAction("&New Session...", this);
    m_newSessionAction->setShortcut(QKeySequence::New);
    m_newSessionAction->setStatusTip("Create a new session");
    fileMenu->addAction(m_newSessionAction);
    
    fileMenu->addSeparator();
    
    m_newSSHAction = new QAction("New &SSH Session", this);
    m_newSSHAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    m_newSSHAction->setStatusTip("Create a new SSH session");
    fileMenu->addAction(m_newSSHAction);
    
    m_newLocalAction = new QAction("New &Local Shell", this);
    m_newLocalAction->setShortcut(QKeySequence("Ctrl+Shift+L"));
    m_newLocalAction->setStatusTip("Create a new local shell session");
    fileMenu->addAction(m_newLocalAction);
    
    m_newTelnetAction = new QAction("New &Telnet Session", this);
    m_newTelnetAction->setShortcut(QKeySequence("Ctrl+Shift+T"));
    m_newTelnetAction->setStatusTip("Create a new Telnet session");
    fileMenu->addAction(m_newTelnetAction);
    
    fileMenu->addSeparator();
    
    m_closeSessionAction = new QAction("&Close Session", this);
    m_closeSessionAction->setShortcut(QKeySequence::Close);
    m_closeSessionAction->setStatusTip("Close the current session");
    fileMenu->addAction(m_closeSessionAction);
    
    m_closeAllAction = new QAction("Close &All Sessions", this);
    m_closeAllAction->setShortcut(QKeySequence("Ctrl+Shift+W"));
    m_closeAllAction->setStatusTip("Close all sessions");
    fileMenu->addAction(m_closeAllAction);
    
    fileMenu->addSeparator();
    
    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Exit the application");
    fileMenu->addAction(m_exitAction);
    
    // View menu
    QMenu *viewMenu = menuBar()->addMenu("&View");
    
    m_toggleSessionPanelAction = new QAction("Toggle &Session Panel", this);
    m_toggleSessionPanelAction->setShortcut(QKeySequence("F9"));
    m_toggleSessionPanelAction->setCheckable(true);
    m_toggleSessionPanelAction->setChecked(true);
    m_toggleSessionPanelAction->setStatusTip("Show or hide the session panel");
    viewMenu->addAction(m_toggleSessionPanelAction);
    
    m_fullScreenAction = new QAction("&Full Screen", this);
    m_fullScreenAction->setShortcut(QKeySequence::FullScreen);
    m_fullScreenAction->setCheckable(true);
    m_fullScreenAction->setStatusTip("Toggle full screen mode");
    viewMenu->addAction(m_fullScreenAction);
    
    // Tools menu
    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
    
    m_preferencesAction = new QAction("&Preferences...", this);
    m_preferencesAction->setShortcut(QKeySequence::Preferences);
    m_preferencesAction->setStatusTip("Open preferences dialog");
    toolsMenu->addAction(m_preferencesAction);
    
    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    
    m_aboutAction = new QAction("&About QTiSSH Pro", this);
    m_aboutAction->setStatusTip("About this application");
    helpMenu->addAction(m_aboutAction);
}

void MainTabWindow::setupToolBar()
{
    m_toolBar = addToolBar("Main");
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    m_toolBar->addAction(m_newSSHAction);
    m_toolBar->addAction(m_newLocalAction);
    m_toolBar->addAction(m_newTelnetAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_closeSessionAction);
}

void MainTabWindow::setupStatusBar()
{
    m_statusLabel = new QLabel("Ready");
    m_sessionCountLabel = new QLabel("Sessions: 0");
    
    statusBar()->addWidget(m_statusLabel, 1);
    statusBar()->addPermanentWidget(m_sessionCountLabel);
}

void MainTabWindow::setupSessionPanel()
{
    m_sessionPanel = new QWidget;
    m_sessionPanel->setMinimumWidth(200);
    m_sessionPanel->setMaximumWidth(400);
    
    QVBoxLayout *panelLayout = new QVBoxLayout(m_sessionPanel);
    
    // Session tree
    QGroupBox *sessionsGroup = new QGroupBox("Active Sessions");
    QVBoxLayout *sessionsLayout = new QVBoxLayout(sessionsGroup);
    
    m_sessionTree = new QTreeWidget;
    m_sessionTree->setHeaderLabels({"Session", "Status"});
    m_sessionTree->setRootIsDecorated(false);
    m_sessionTree->setAlternatingRowColors(true);
    m_sessionTree->setContextMenuPolicy(Qt::CustomContextMenu);
    m_sessionTree->header()->setStretchLastSection(true);
    m_sessionTree->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    
    sessionsLayout->addWidget(m_sessionTree);
    panelLayout->addWidget(sessionsGroup);
    
    // Quick connect buttons
    QGroupBox *quickGroup = new QGroupBox("Quick Connect");
    QVBoxLayout *quickLayout = new QVBoxLayout(quickGroup);
    
    m_newSSHButton = new QPushButton("New SSH Session");
    m_newLocalButton = new QPushButton("New Local Shell");
    m_newTelnetButton = new QPushButton("New Telnet Session");
    
    quickLayout->addWidget(m_newSSHButton);
    quickLayout->addWidget(m_newLocalButton);
    quickLayout->addWidget(m_newTelnetButton);
    
    panelLayout->addWidget(quickGroup);
    panelLayout->addStretch();
}

void MainTabWindow::setupTabWidget()
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    m_tabWidget->setDocumentMode(true);
    m_tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Show a welcome message when no tabs are open
    QLabel *welcomeLabel = new QLabel("Welcome to QTiSSH Pro!\n\nCreate a new session to get started.");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("color: #666; font-size: 14px;");
    m_tabWidget->addTab(welcomeLabel, "Welcome");
}

void MainTabWindow::setupConnections()
{
    // Session manager connections
    connect(m_sessionManager, &SessionManager::sessionCreated,
            this, &MainTabWindow::onSessionCreated);
    connect(m_sessionManager, &SessionManager::sessionRemoved,
            this, &MainTabWindow::onSessionRemoved);
    connect(m_sessionManager, &SessionManager::activeSessionChanged,
            this, &MainTabWindow::onActiveSessionChanged);
    connect(m_sessionManager, &SessionManager::sessionCountChanged,
            this, [this](int count) {
                m_sessionCountLabel->setText(QString("Sessions: %1").arg(count));
                updateWindowTitle();
            });
    
    // Menu actions
    connect(m_newSessionAction, &QAction::triggered, this, &MainTabWindow::onNewSession);
    connect(m_newSSHAction, &QAction::triggered, this, &MainTabWindow::onNewSSHSession);
    connect(m_newLocalAction, &QAction::triggered, this, &MainTabWindow::onNewLocalShell);
    connect(m_newTelnetAction, &QAction::triggered, this, &MainTabWindow::onNewTelnetSession);
    connect(m_closeSessionAction, &QAction::triggered, this, &MainTabWindow::onCloseSession);
    connect(m_closeAllAction, &QAction::triggered, this, &MainTabWindow::onCloseAllSessions);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    connect(m_aboutAction, &QAction::triggered, this, &MainTabWindow::onAbout);
    connect(m_preferencesAction, &QAction::triggered, this, &MainTabWindow::onPreferences);
    connect(m_toggleSessionPanelAction, &QAction::triggered, this, &MainTabWindow::onToggleSessionPanel);
    connect(m_fullScreenAction, &QAction::triggered, this, &MainTabWindow::onToggleFullScreen);
    
    // Quick connect buttons
    connect(m_newSSHButton, &QPushButton::clicked, this, &MainTabWindow::onNewSSHSession);
    connect(m_newLocalButton, &QPushButton::clicked, this, &MainTabWindow::onNewLocalShell);
    connect(m_newTelnetButton, &QPushButton::clicked, this, &MainTabWindow::onNewTelnetSession);
    
    // Tab widget
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainTabWindow::onTabChanged);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &MainTabWindow::onTabCloseRequested);
    connect(m_tabWidget, &QTabWidget::customContextMenuRequested, this, &MainTabWindow::onTabContextMenu);
    
    // Session tree
    connect(m_sessionTree, &QTreeWidget::itemClicked, this, &MainTabWindow::onSessionTreeItemClicked);
    connect(m_sessionTree, &QTreeWidget::itemDoubleClicked, this, &MainTabWindow::onSessionTreeItemDoubleClicked);
    connect(m_sessionTree, &QTreeWidget::customContextMenuRequested, this, &MainTabWindow::onSessionTreeContextMenu);
}

void MainTabWindow::setupTheme()
{
    // Apply modern dark theme
    setStyleSheet(R"(
        QMainWindow {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        
        QTabWidget::pane {
            border: 1px solid #555;
            background-color: #2b2b2b;
        }
        
        QTabBar::tab {
            background-color: #3c3c3c;
            color: #ffffff;
            padding: 8px 16px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        
        QTabBar::tab:selected {
            background-color: #0078d4;
        }
        
        QTabBar::tab:hover {
            background-color: #4c4c4c;
        }
        
        QTreeWidget {
            background-color: #1e1e1e;
            color: #ffffff;
            border: 1px solid #555;
            alternate-background-color: #2a2a2a;
        }
        
        QTreeWidget::item:selected {
            background-color: #0078d4;
        }
        
        QTreeWidget::item:hover {
            background-color: #3c3c3c;
        }
        
        QPushButton {
            background-color: #0078d4;
            color: #ffffff;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #106ebe;
        }
        
        QPushButton:pressed {
            background-color: #005a9e;
        }
        
        QGroupBox {
            color: #ffffff;
            font-weight: bold;
            border: 2px solid #555;
            border-radius: 4px;
            margin-top: 1ex;
            padding-top: 10px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        
        QStatusBar {
            background-color: #1e1e1e;
            color: #ffffff;
            border-top: 1px solid #555;
        }
        
        QMenuBar {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        
        QMenuBar::item:selected {
            background-color: #0078d4;
        }
        
        QMenu {
            background-color: #2b2b2b;
            color: #ffffff;
            border: 1px solid #555;
        }
        
        QMenu::item:selected {
            background-color: #0078d4;
        }
        
        QToolBar {
            background-color: #2b2b2b;
            border: none;
            spacing: 4px;
        }
    )");
}

// Session management slots
void MainTabWindow::onNewSession()
{
    // Show a dialog to choose session type
    QStringList items;
    items << "SSH Session" << "Local Shell" << "Telnet Session";
    
    bool ok;
    QString item = QInputDialog::getItem(this, "New Session", "Session type:", items, 0, false, &ok);
    
    if (ok && !item.isEmpty()) {
        if (item == "SSH Session") {
            onNewSSHSession();
        } else if (item == "Local Shell") {
            onNewLocalShell();
        } else if (item == "Telnet Session") {
            onNewTelnetSession();
        }
    }
}

void MainTabWindow::onNewSSHSession()
{
    showConnectionDialog(SessionType::SSH);
}

void MainTabWindow::onNewLocalShell()
{
    Session *session = m_sessionManager->createSession(SessionType::LocalShell);
    if (session) {
        session->connect();  // Auto-connect local shell
    }
}

void MainTabWindow::onNewTelnetSession()
{
    showConnectionDialog(SessionType::Telnet);
}

void MainTabWindow::onCloseSession()
{
    int currentIndex = m_tabWidget->currentIndex();
    if (currentIndex >= 0) {
        onTabCloseRequested(currentIndex);
    }
}

void MainTabWindow::onCloseAllSessions()
{
    if (m_sessionManager->getSessionCount() == 0) {
        return;
    }
    
    int ret = QMessageBox::question(this, "Close All Sessions",
                                   "Are you sure you want to close all sessions?",
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_sessionManager->removeAllSessions();
    }
}

void MainTabWindow::onSessionCreated(Session *session)
{
    addSessionTab(session);
    addSessionToTree(session);
    
    // Connect to session signals for updates
    connect(session, &Session::sessionStateChanged,
            this, &MainTabWindow::onSessionStateChanged);
    connect(session, &Session::sessionNameChanged,
            this, &MainTabWindow::onSessionNameChanged);
    
    m_statusLabel->setText(QString("Created session: %1").arg(session->sessionName()));
}

void MainTabWindow::onSessionRemoved(const QUuid &sessionId)
{
    // Find and remove the session from UI
    for (auto it = m_sessionTabMap.begin(); it != m_sessionTabMap.end(); ++it) {
        if (it.key()->sessionId() == sessionId) {
            Session *session = it.key();
            removeSessionTab(session);
            removeSessionFromTree(session);
            break;
        }
    }
    
    m_statusLabel->setText("Session closed");
}

void MainTabWindow::onActiveSessionChanged(Session *newSession, Session *oldSession)
{
    Q_UNUSED(oldSession)
    
    if (newSession && m_sessionTabMap.contains(newSession)) {
        int tabIndex = m_sessionTabMap[newSession];
        m_tabWidget->setCurrentIndex(tabIndex);
    }
    
    updateWindowTitle();
}

// Tab management slots
void MainTabWindow::onTabChanged(int index)
{
    // Find session for this tab and set it as active
    for (auto it = m_sessionTabMap.begin(); it != m_sessionTabMap.end(); ++it) {
        if (it.value() == index) {
            m_sessionManager->setActiveSession(it.key());
            break;
        }
    }
}

void MainTabWindow::onTabCloseRequested(int index)
{
    // Find session for this tab and remove it
    for (auto it = m_sessionTabMap.begin(); it != m_sessionTabMap.end(); ++it) {
        if (it.value() == index) {
            Session *session = it.key();
            m_sessionManager->removeSession(session);
            break;
        }
    }
}

void MainTabWindow::onTabContextMenu(const QPoint &pos)
{
    int tabIndex = m_tabWidget->tabBar()->tabAt(pos);
    if (tabIndex < 0) return;
    
    QMenu menu;
    menu.addAction("Close Tab", [this, tabIndex]() {
        onTabCloseRequested(tabIndex);
    });
    menu.addAction("Close Other Tabs", [this, tabIndex]() {
        // Close all tabs except the specified one
        for (int i = m_tabWidget->count() - 1; i >= 0; --i) {
            if (i != tabIndex) {
                onTabCloseRequested(i);
            }
        }
    });
    menu.addAction("Close All Tabs", [this]() {
        onCloseAllSessions();
    });
    
    menu.exec(m_tabWidget->mapToGlobal(pos));
}

// Session tree slots
void MainTabWindow::onSessionTreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    
    // Find session for this item and set it as active
    for (auto it = m_sessionTreeMap.begin(); it != m_sessionTreeMap.end(); ++it) {
        if (it.value() == item) {
            m_sessionManager->setActiveSession(it.key());
            break;
        }
    }
}

void MainTabWindow::onSessionTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    
    // Find session and connect if not connected
    for (auto it = m_sessionTreeMap.begin(); it != m_sessionTreeMap.end(); ++it) {
        if (it.value() == item) {
            Session *session = it.key();
            if (!session->isConnected()) {
                session->connect();
            }
            break;
        }
    }
}

void MainTabWindow::onSessionTreeContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = m_sessionTree->itemAt(pos);
    if (!item) return;
    
    // Find session for this item
    Session *session = nullptr;
    for (auto it = m_sessionTreeMap.begin(); it != m_sessionTreeMap.end(); ++it) {
        if (it.value() == item) {
            session = it.key();
            break;
        }
    }
    
    if (!session) return;
    
    QMenu menu;
    
    if (session->isConnected()) {
        menu.addAction("Disconnect", [session]() {
            session->disconnect();
        });
    } else {
        menu.addAction("Connect", [session]() {
            session->connect();
        });
    }
    
    menu.addSeparator();
    menu.addAction("Rename Session", [this, session]() {
        bool ok;
        QString newName = QInputDialog::getText(this, "Rename Session", 
                                               "Session name:", QLineEdit::Normal,
                                               session->sessionName(), &ok);
        if (ok && !newName.isEmpty()) {
            session->setSessionName(newName);
        }
    });
    
    menu.addSeparator();
    menu.addAction("Close Session", [this, session]() {
        m_sessionManager->removeSession(session);
    });
    
    menu.exec(m_sessionTree->mapToGlobal(pos));
}

// UI action slots
void MainTabWindow::onAbout()
{
    QMessageBox::about(this, "About QTiSSH Pro",
                      "<h3>QTiSSH Pro</h3>"
                      "<p>A modern, open-source terminal manager</p>"
                      "<p>Version 1.0.0</p>"
                      "<p>Built with Qt and love ❤️</p>"
                      "<p>Features:</p>"
                      "<ul>"
                      "<li>VT100/ANSI terminal emulation</li>"
                      "<li>SSH, Telnet, and Local shell support</li>"
                      "<li>Tabbed interface</li>"
                      "<li>Session management</li>"
                      "<li>Modern dark theme</li>"
                      "</ul>");
}

void MainTabWindow::onPreferences()
{
    QMessageBox::information(this, "Preferences", "Preferences dialog not yet implemented.");
}

void MainTabWindow::onToggleSessionPanel()
{
    m_sessionPanelVisible = !m_sessionPanelVisible;
    m_sessionPanel->setVisible(m_sessionPanelVisible);
    m_toggleSessionPanelAction->setChecked(m_sessionPanelVisible);
}

void MainTabWindow::onToggleFullScreen()
{
    if (isFullScreen()) {
        showNormal();
        m_fullScreenAction->setChecked(false);
    } else {
        showFullScreen();
        m_fullScreenAction->setChecked(true);
    }
}

// Session state update slots
void MainTabWindow::onSessionStateChanged(SessionState newState, SessionState oldState)
{
    Q_UNUSED(oldState)
    
    Session *session = qobject_cast<Session*>(sender());
    if (session) {
        updateSessionTab(session);
        updateSessionInTree(session);
        
        QString stateStr = SessionUtils::sessionStateToString(newState);
        m_statusLabel->setText(QString("Session %1: %2").arg(session->sessionName(), stateStr));
    }
}

void MainTabWindow::onSessionNameChanged(const QString &newName)
{
    Q_UNUSED(newName)
    Session *session = qobject_cast<Session*>(sender());
    if (session) {
        updateSessionTab(session);
        updateSessionInTree(session);
        updateWindowTitle();
    }
}

// Session management implementation
void MainTabWindow::addSessionTab(Session *session)
{
    if (!session) return;
    
    QWidget *widget = session->getDisplayWidget();
    if (!widget) return;
    
    // Remove welcome tab if it exists
    if (m_tabWidget->count() == 1 && m_tabWidget->tabText(0) == "Welcome") {
        m_tabWidget->removeTab(0);
    }
    
    int tabIndex = m_tabWidget->addTab(widget, session->getSessionIcon(), session->sessionName());
    m_sessionTabMap[session] = tabIndex;
    
    // Set this tab as current
    m_tabWidget->setCurrentIndex(tabIndex);
}

void MainTabWindow::removeSessionTab(Session *session)
{
    if (!session || !m_sessionTabMap.contains(session)) return;
    
    int tabIndex = m_sessionTabMap[session];
    m_tabWidget->removeTab(tabIndex);
    m_sessionTabMap.remove(session);
    
    // Update tab indices for remaining sessions
    for (auto it = m_sessionTabMap.begin(); it != m_sessionTabMap.end(); ++it) {
        if (it.value() > tabIndex) {
            it.value()--;
        }
    }
    
    // Show welcome tab if no sessions remain
    if (m_tabWidget->count() == 0) {
        QLabel *welcomeLabel = new QLabel("Welcome to QTiSSH Pro!\n\nCreate a new session to get started.");
        welcomeLabel->setAlignment(Qt::AlignCenter);
        welcomeLabel->setStyleSheet("color: #666; font-size: 14px;");
        m_tabWidget->addTab(welcomeLabel, "Welcome");
    }
}

void MainTabWindow::updateSessionTab(Session *session)
{
    if (!session || !m_sessionTabMap.contains(session)) return;
    
    int tabIndex = m_sessionTabMap[session];
    m_tabWidget->setTabText(tabIndex, session->sessionName());
    m_tabWidget->setTabIcon(tabIndex, session->getSessionIcon());
}

void MainTabWindow::addSessionToTree(Session *session)
{
    if (!session) return;
    
    QTreeWidgetItem *item = new QTreeWidgetItem(m_sessionTree);
    item->setText(0, session->sessionName());
    item->setText(1, SessionUtils::sessionStateToString(session->sessionState()));
    item->setIcon(0, session->getSessionIcon());
    
    m_sessionTreeMap[session] = item;
}

void MainTabWindow::removeSessionFromTree(Session *session)
{
    if (!session || !m_sessionTreeMap.contains(session)) return;
    
    QTreeWidgetItem *item = m_sessionTreeMap[session];
    delete item;
    m_sessionTreeMap.remove(session);
}

void MainTabWindow::updateSessionInTree(Session *session)
{
    if (!session || !m_sessionTreeMap.contains(session)) return;
    
    QTreeWidgetItem *item = m_sessionTreeMap[session];
    item->setText(0, session->sessionName());
    item->setText(1, SessionUtils::sessionStateToString(session->sessionState()));
    item->setIcon(0, session->getSessionIcon());
}

QTreeWidgetItem* MainTabWindow::findSessionTreeItem(Session *session)
{
    return m_sessionTreeMap.value(session, nullptr);
}

// UI helpers
void MainTabWindow::updateWindowTitle()
{
    QString title = "QTiSSH Pro - Terminal Manager";
    
    Session *activeSession = m_sessionManager->getActiveSession();
    if (activeSession) {
        title += QString(" - %1").arg(activeSession->sessionName());
    }
    
    int sessionCount = m_sessionManager->getSessionCount();
    if (sessionCount > 0) {
        title += QString(" (%1 sessions)").arg(sessionCount);
    }
    
    setWindowTitle(title);
}

void MainTabWindow::showConnectionDialog(SessionType type)
{
    // Simple connection dialog for now
    QString typeStr = SessionUtils::sessionTypeToString(type);
    
    bool ok;
    QString host = QInputDialog::getText(this, QString("New %1 Session").arg(typeStr),
                                        "Host:", QLineEdit::Normal, "", &ok);
    
    if (ok && !host.isEmpty()) {
        Session *session = m_sessionManager->createSession(type);
        if (session) {
            TerminalSession *termSession = qobject_cast<TerminalSession*>(session);
            if (termSession) {
                termSession->setConnectionHost(host);
                
                // Get username for SSH
                if (type == SessionType::SSH) {
                    QString user = QInputDialog::getText(this, "SSH Connection",
                                                        "Username:", QLineEdit::Normal, "", &ok);
                    if (ok && !user.isEmpty()) {
                        termSession->setConnectionUser(user);
                    }
                }
                
                // Auto-connect
                session->connect();
            }
        }
    }
}

void MainTabWindow::closeEvent(QCloseEvent *event)
{
    if (m_sessionManager->getConnectedSessionCount() > 0) {
        int ret = QMessageBox::question(this, "Exit QTiSSH Pro",
                                       "There are active sessions. Are you sure you want to exit?",
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        
        if (ret == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    
    // Close all sessions
    m_sessionManager->removeAllSessions();
    
    event->accept();
}
