#ifndef MAINTABWINDOW_H
#define MAINTABWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include "sessionmanager.h"
#include "servermanager.h"
#include "foldermanager.h"
#include "servertreewidget.h"
#include "serverconfig.h"

class QTreeWidgetItem;

/**
 * @brief Modern tabbed main window for the terminal application
 * 
 * This is the main application window that provides:
 * - Tabbed interface for multiple sessions
 * - Session management sidebar
 * - Modern UI with themes
 * - Connection management
 */
class MainTabWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainTabWindow(QWidget *parent = nullptr);
    ~MainTabWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Session management
    void onNewSession();
    void onNewSSHSession();
    void onNewLocalShell();
    void onNewTelnetSession();
    void onCloseSession();
    void onCloseAllSessions();
    void onSessionCreated(Session *session);
    void onSessionRemoved(const QUuid &sessionId);
    void onActiveSessionChanged(Session *newSession, Session *oldSession);
    
    // Server management (from MainWindow)
    void onAddServerClicked();
    void onEditServerClicked();
    void onDeleteServerClicked();
    void onConnectClicked();
    void onConnectSftpClicked();
    void onServerDoubleClicked(const ServerConfig &server);
    void onServersChanged();
    void onAboutClicked();
    
    // Folder management slots
    void onCreateFolderRequested(const QString &parentFolderId);
    void onRenameFolderRequested(const QString &folderId);
    void onDeleteFolderRequested(const QString &folderId);
    void onAddServerToFolderRequested(const QString &folderId);
    void onEditServerRequested(const QString &serverId);
    void onDeleteServerRequested(const QString &serverId);
    void onMoveServerRequested(const QString &serverId, const QString &newFolderId);
    
    // Tab management
    void onTabChanged(int index);
    void onTabCloseRequested(int index);
    void onTabContextMenu(const QPoint &pos);
    
    // Session tree management
    void onSessionTreeItemClicked(QTreeWidgetItem *item, int column);
    void onSessionTreeContextMenu(const QPoint &pos);
    void onSessionTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    
    // UI actions
    void onAbout();
    void onPreferences();
    void onToggleSessionPanel();
    void onToggleFullScreen();
    
    // Session state updates
    void onSessionStateChanged(SessionState newState, SessionState oldState);
    void onSessionNameChanged(const QString &newName);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupSessionPanel();
    void setupTabWidget();
    void setupConnections();
    void setupTheme();
    
    // Session management
    void addSessionTab(Session *session);
    void removeSessionTab(Session *session);
    void updateSessionTab(Session *session);
    void updateSessionTree();
    void addSessionToTree(Session *session);
    void removeSessionFromTree(Session *session);
    void updateSessionInTree(Session *session);
    QTreeWidgetItem* findSessionTreeItem(Session *session);
    
    // UI helpers
    void updateWindowTitle();
    void updateStatusBar();
    void showConnectionDialog(SessionType type);
    
    // Core components
    SessionManager *m_sessionManager;
    ServerManager *m_serverManager;
    FolderManager *m_folderManager;
    
    // UI components
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    
    // Session panel (left side)
    QWidget *m_sessionPanel;
    QTreeWidget *m_sessionTree;
    QPushButton *m_newSessionButton;
    QPushButton *m_newSSHButton;
    QPushButton *m_newLocalButton;
    QPushButton *m_newTelnetButton;
    
    // Server management panel (integrated from MainWindow)
    ServerTreeWidget *m_serverTree;
    QPushButton *m_addButton;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QPushButton *m_connectButton;
    QPushButton *m_connectSftpButton;
    
    // Tab widget (right side)
    QTabWidget *m_tabWidget;
    
    // Menu and toolbar
    QMenuBar *m_menuBar;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QLabel *m_statusLabel;
    QLabel *m_sessionCountLabel;
    
    // Actions
    QAction *m_newSessionAction;
    QAction *m_newSSHAction;
    QAction *m_newLocalAction;
    QAction *m_newTelnetAction;
    QAction *m_closeSessionAction;
    QAction *m_closeAllAction;
    QAction *m_preferencesAction;
    QAction *m_aboutAction;
    QAction *m_exitAction;
    QAction *m_toggleSessionPanelAction;
    QAction *m_fullScreenAction;
    
    // State
    QMap<Session*, int> m_sessionTabMap;  // Maps sessions to tab indices
    QMap<Session*, QTreeWidgetItem*> m_sessionTreeMap;  // Maps sessions to tree items
    bool m_sessionPanelVisible;
};

#endif // MAINTABWINDOW_H
