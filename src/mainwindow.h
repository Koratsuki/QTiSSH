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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SSHTerminal;
class SFTPBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    
    void setupUI();
    void connectToServer(const ServerConfig &config);
    ServerConfig getSelectedServer();
};
#endif // MAINWINDOW_H
