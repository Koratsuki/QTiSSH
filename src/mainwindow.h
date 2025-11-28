#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTabWidget>
#include "servermanager.h"
#include "serverconfig.h"

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
    void onServerListDoubleClicked(QListWidgetItem *item);
    void onTabCloseRequested(int index);
    void onServersChanged();

private:
    Ui::MainWindow *ui;
    ServerManager *m_serverManager;
    QListWidget *m_serverList;
    QTabWidget *m_tabWidget;
    
    void setupUI();
    void setupMenuBar();
    void updateServerList();
    void connectToServer(const ServerConfig &config);
    ServerConfig getSelectedServer();
};
#endif // MAINWINDOW_H
