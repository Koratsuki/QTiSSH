#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTabWidget>
#include "servermanager.h"
#include "foldermanager.h"
#include "servertreewidget.h"
#include "serverconfig.h"
#include "thememanager.h"
#include <QLineEdit>
#include <QToolButton>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SSHTerminal;
class SFTPBrowser;
class SnippetDialog;
class TerminalSplitWidget;
class QSystemTrayIcon;
class QMenu;
class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAddServerClicked();
    void onEditServerClicked();
    void onDeleteServerClicked();
    void onConnectClicked();
    void onConnectSftpClicked();
    void onServerDoubleClicked(const ServerConfig &server);
    void onTabCloseRequested(int index);
    void onServersChanged();
    void onAboutClicked();
    void onThemeToggleClicked();
    void onSearchTextChanged(const QString &text);
    void onOptionsClicked();
    void onCopyClicked();
    void onPasteClicked();
    void onCutClicked();
    void onSnippetsClicked();
    void onCommandHistoryClicked();
    void onExportServersClicked();
    void onImportServersClicked();
    void onSetMasterPasswordClicked();
    void onUnlockPasswordsClicked();
    void onRemoveMasterPasswordClicked();
    void onSetAppPasswordClicked();
    void onChangeAppPasswordClicked();
    void onRemoveAppPasswordClicked();
    void onLockApplicationClicked();
    void onLanguageSelected(const QString &lang);
    void onExecuteSnippet(const QString &command);
    void onExecuteHistoryCommand(const QString &command);
    void onQuickCommandsClicked();
    void onMonitoringClicked();
    void onNetworkDiscoveryClicked();
    void onConnectionLogsClicked();
    void onProfilesClicked();
    void onSplitHorizontalClicked();
    void onSplitVerticalClicked();
    void onNewTerminalClicked();
    void onOpenInExternalTerminal();
    void onQuickConnectClicked();
    void onEditRemoteFileClicked();
    void onRecentConnectionTriggered(const QString &serverId);
    void onClearRecentClicked();
    void onQuickConnectGlobal();
    void onToggleWindowGlobal();
    void onDiscoveryConnect(const QHostAddress &host, const QString &username);
    void onDiscoveryAddServer(const QHostAddress &host);
    
    // Folder management slots
    void onCreateFolderRequested(const QString &parentFolderId);
    void onRenameFolderRequested(const QString &folderId);
    void onDeleteFolderRequested(const QString &folderId);
    void onAddServerToFolderRequested(const QString &folderId);
    void onEditServerRequested(const QString &serverId);
    void onDeleteServerRequested(const QString &serverId);
    void onMoveServerRequested(const QString &serverId, const QString &newFolderId);

private:
    Ui::MainWindow *ui;
    ServerManager *m_serverManager;
    FolderManager *m_folderManager;
    ServerTreeWidget *m_serverTree;
    QTabWidget *m_tabWidget;
    QLineEdit *m_searchBar;
    QToolButton *m_themeButton;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_recentMenu;
    bool m_closing;

    void setupUI();
    void setupSecurityMenu(QMenu *editMenu);
    void setupToolsMenu(QMenu *editMenu);
    void setupLanguageMenu(QMenu *editMenu);
    void setupRecentMenu(QMenu *fileMenu);
    void setupTray();
    void setupExternalTerminalMenu(QMenu *fileMenu);
    void setupSplitMenu(QMenu *editMenu);
    void rebuildRecentMenu();
    void connectToServer(const ServerConfig &config);
    void openSftpTab(const ServerConfig &config);
    void openRemoteEditor(const ServerConfig &config, const QString &remotePath);
    void connectToServerById(const QString &serverId);
    void splitCurrentTab(Qt::Orientation orientation);
    ServerConfig getSelectedServer();
    SSHTerminal *currentTerminal() const;
    void promptUnlockPasswords();
    void updateTerminalTabTitle(QWidget *tabWidget, const QString &baseTitle);
    void writeSession();
    void restoreSession();
    void promptRestoreSession();
    void registerGlobalHotkeys();
    QHostAddress parseHost(const QString &host) const;
};
#endif // MAINWINDOW_H
