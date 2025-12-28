/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "servertreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAddServer;
    QAction *actionQuit;
    QAction *actionAbout;
    QAction *actionOpen_new_tab;
    QAction *actionClose_current_tab;
    QAction *actionSet_tab_title;
    QAction *actionDetach;
    QAction *actionDuplicate_tab;
    QAction *actionWrite_commands_in_all_Terminals;
    QAction *actionCopy;
    QAction *actionCopy_2;
    QAction *actionPaste;
    QAction *actionUndo;
    QAction *actionUndo_2;
    QAction *actionRedo;
    QAction *actionNew_session;
    QAction *actionLoad;
    QAction *actionSincle_Terminal;
    QAction *actionTwo_terminals_mode_Left_split;
    QAction *actionTwo_terminals_mode_Right_split;
    QAction *action4_Terminals_mode;
    QAction *actionConfiguration;
    QAction *actionImport_configuration;
    QAction *actionExport_configuration;
    QAction *actionReset_configuration;
    QAction *actionDocumentation;
    QAction *actionCheck_for_updates;
    QAction *actionOpenMainApp;
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QSplitter *mainSplitter;
    QWidget *leftPanel;
    QVBoxLayout *leftLayout;
    QLabel *serverListLabel;
    ServerTreeWidget *serverTree;
    QVBoxLayout *buttonLayout;
    QHBoxLayout *buttonRow1;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QHBoxLayout *buttonRow2;
    QPushButton *connectButton;
    QPushButton *connectSftpButton;
    QTabWidget *tabWidget;
    QWidget *welcomeTab;
    QVBoxLayout *welcomeLayout;
    QLabel *welcomeLabel;
    QHBoxLayout *welcomeButtonLayout;
    QSpacerItem *leftSpacer;
    QPushButton *openMainAppButton;
    QSpacerItem *rightSpacer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuTerminal;
    QMenu *menuSessions;
    QMenu *menuUser_sessions;
    QMenu *menuSettings;
    QMenu *menuEdit;
    QMenu *menuView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 700);
        actionAddServer = new QAction(MainWindow);
        actionAddServer->setObjectName("actionAddServer");
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionOpen_new_tab = new QAction(MainWindow);
        actionOpen_new_tab->setObjectName("actionOpen_new_tab");
        actionClose_current_tab = new QAction(MainWindow);
        actionClose_current_tab->setObjectName("actionClose_current_tab");
        actionSet_tab_title = new QAction(MainWindow);
        actionSet_tab_title->setObjectName("actionSet_tab_title");
        actionDetach = new QAction(MainWindow);
        actionDetach->setObjectName("actionDetach");
        actionDuplicate_tab = new QAction(MainWindow);
        actionDuplicate_tab->setObjectName("actionDuplicate_tab");
        actionWrite_commands_in_all_Terminals = new QAction(MainWindow);
        actionWrite_commands_in_all_Terminals->setObjectName("actionWrite_commands_in_all_Terminals");
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName("actionCopy");
        actionCopy_2 = new QAction(MainWindow);
        actionCopy_2->setObjectName("actionCopy_2");
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName("actionPaste");
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName("actionUndo");
        actionUndo_2 = new QAction(MainWindow);
        actionUndo_2->setObjectName("actionUndo_2");
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName("actionRedo");
        actionNew_session = new QAction(MainWindow);
        actionNew_session->setObjectName("actionNew_session");
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName("actionLoad");
        actionSincle_Terminal = new QAction(MainWindow);
        actionSincle_Terminal->setObjectName("actionSincle_Terminal");
        actionTwo_terminals_mode_Left_split = new QAction(MainWindow);
        actionTwo_terminals_mode_Left_split->setObjectName("actionTwo_terminals_mode_Left_split");
        actionTwo_terminals_mode_Right_split = new QAction(MainWindow);
        actionTwo_terminals_mode_Right_split->setObjectName("actionTwo_terminals_mode_Right_split");
        action4_Terminals_mode = new QAction(MainWindow);
        action4_Terminals_mode->setObjectName("action4_Terminals_mode");
        actionConfiguration = new QAction(MainWindow);
        actionConfiguration->setObjectName("actionConfiguration");
        actionImport_configuration = new QAction(MainWindow);
        actionImport_configuration->setObjectName("actionImport_configuration");
        actionExport_configuration = new QAction(MainWindow);
        actionExport_configuration->setObjectName("actionExport_configuration");
        actionReset_configuration = new QAction(MainWindow);
        actionReset_configuration->setObjectName("actionReset_configuration");
        actionDocumentation = new QAction(MainWindow);
        actionDocumentation->setObjectName("actionDocumentation");
        actionCheck_for_updates = new QAction(MainWindow);
        actionCheck_for_updates->setObjectName("actionCheck_for_updates");
        actionOpenMainApp = new QAction(MainWindow);
        actionOpenMainApp->setObjectName("actionOpenMainApp");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainSplitter = new QSplitter(centralwidget);
        mainSplitter->setObjectName("mainSplitter");
        mainSplitter->setOrientation(Qt::Orientation::Horizontal);
        leftPanel = new QWidget(mainSplitter);
        leftPanel->setObjectName("leftPanel");
        leftPanel->setMinimumSize(QSize(250, 0));
        leftPanel->setMaximumSize(QSize(300, 16777215));
        leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setObjectName("leftLayout");
        leftLayout->setContentsMargins(0, 0, 0, 0);
        serverListLabel = new QLabel(leftPanel);
        serverListLabel->setObjectName("serverListLabel");
        serverListLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; font-size: 14px; padding: 5px;"));

        leftLayout->addWidget(serverListLabel);

        serverTree = new ServerTreeWidget(leftPanel);
        serverTree->setObjectName("serverTree");

        leftLayout->addWidget(serverTree);

        buttonLayout = new QVBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        buttonRow1 = new QHBoxLayout();
        buttonRow1->setObjectName("buttonRow1");
        addButton = new QPushButton(leftPanel);
        addButton->setObjectName("addButton");

        buttonRow1->addWidget(addButton);

        editButton = new QPushButton(leftPanel);
        editButton->setObjectName("editButton");

        buttonRow1->addWidget(editButton);

        deleteButton = new QPushButton(leftPanel);
        deleteButton->setObjectName("deleteButton");

        buttonRow1->addWidget(deleteButton);


        buttonLayout->addLayout(buttonRow1);

        buttonRow2 = new QHBoxLayout();
        buttonRow2->setObjectName("buttonRow2");
        connectButton = new QPushButton(leftPanel);
        connectButton->setObjectName("connectButton");

        buttonRow2->addWidget(connectButton);

        connectSftpButton = new QPushButton(leftPanel);
        connectSftpButton->setObjectName("connectSftpButton");

        buttonRow2->addWidget(connectSftpButton);


        buttonLayout->addLayout(buttonRow2);


        leftLayout->addLayout(buttonLayout);

        mainSplitter->addWidget(leftPanel);
        tabWidget = new QTabWidget(mainSplitter);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(true);
        welcomeTab = new QWidget();
        welcomeTab->setObjectName("welcomeTab");
        welcomeLayout = new QVBoxLayout(welcomeTab);
        welcomeLayout->setObjectName("welcomeLayout");
        welcomeLabel = new QLabel(welcomeTab);
        welcomeLabel->setObjectName("welcomeLabel");
        welcomeLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; color: #666;"));
        welcomeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        welcomeLayout->addWidget(welcomeLabel);

        welcomeButtonLayout = new QHBoxLayout();
        welcomeButtonLayout->setObjectName("welcomeButtonLayout");
        leftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        welcomeButtonLayout->addItem(leftSpacer);

        openMainAppButton = new QPushButton(welcomeTab);
        openMainAppButton->setObjectName("openMainAppButton");
        openMainAppButton->setStyleSheet(QString::fromUtf8("QPushButton { font-size: 12px; padding: 8px 16px; }"));

        welcomeButtonLayout->addWidget(openMainAppButton);

        rightSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        welcomeButtonLayout->addItem(rightSpacer);


        welcomeLayout->addLayout(welcomeButtonLayout);

        tabWidget->addTab(welcomeTab, QString());
        mainSplitter->addWidget(tabWidget);

        mainLayout->addWidget(mainSplitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        menuTerminal = new QMenu(menubar);
        menuTerminal->setObjectName("menuTerminal");
        menuSessions = new QMenu(menubar);
        menuSessions->setObjectName("menuSessions");
        menuUser_sessions = new QMenu(menuSessions);
        menuUser_sessions->setObjectName("menuUser_sessions");
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName("menuSettings");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuTerminal->menuAction());
        menubar->addAction(menuSessions->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpenMainApp);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionDocumentation);
        menuHelp->addAction(actionCheck_for_updates);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuTerminal->addAction(actionOpen_new_tab);
        menuTerminal->addAction(actionClose_current_tab);
        menuTerminal->addSeparator();
        menuTerminal->addAction(actionSet_tab_title);
        menuTerminal->addAction(actionDetach);
        menuTerminal->addSeparator();
        menuTerminal->addAction(actionDuplicate_tab);
        menuTerminal->addAction(actionWrite_commands_in_all_Terminals);
        menuSessions->addAction(actionNew_session);
        menuSessions->addSeparator();
        menuSessions->addAction(menuUser_sessions->menuAction());
        menuUser_sessions->addAction(actionLoad);
        menuSettings->addAction(actionConfiguration);
        menuSettings->addSeparator();
        menuSettings->addAction(actionImport_configuration);
        menuSettings->addAction(actionExport_configuration);
        menuSettings->addSeparator();
        menuSettings->addAction(actionReset_configuration);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCopy_2);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionUndo_2);
        menuEdit->addAction(actionRedo);
        menuView->addAction(actionSincle_Terminal);
        menuView->addAction(actionTwo_terminals_mode_Left_split);
        menuView->addAction(actionTwo_terminals_mode_Right_split);
        menuView->addAction(action4_Terminals_mode);
        menuView->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QTiSSH - SSH Connection Manager", nullptr));
        actionAddServer->setText(QCoreApplication::translate("MainWindow", "&Add Server", nullptr));
#if QT_CONFIG(shortcut)
        actionAddServer->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionQuit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "&About", nullptr));
        actionOpen_new_tab->setText(QCoreApplication::translate("MainWindow", "Open new tab", nullptr));
        actionClose_current_tab->setText(QCoreApplication::translate("MainWindow", "Close current tab", nullptr));
        actionSet_tab_title->setText(QCoreApplication::translate("MainWindow", "Set tab title", nullptr));
        actionDetach->setText(QCoreApplication::translate("MainWindow", "Detach", nullptr));
        actionDuplicate_tab->setText(QCoreApplication::translate("MainWindow", "Duplicate current tab", nullptr));
        actionWrite_commands_in_all_Terminals->setText(QCoreApplication::translate("MainWindow", "Write commands on all Terminals", nullptr));
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
        actionCopy_2->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionUndo_2->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        actionNew_session->setText(QCoreApplication::translate("MainWindow", "New session", nullptr));
        actionLoad->setText(QCoreApplication::translate("MainWindow", "Load...", nullptr));
        actionSincle_Terminal->setText(QCoreApplication::translate("MainWindow", "Single terminal mode", nullptr));
        actionTwo_terminals_mode_Left_split->setText(QCoreApplication::translate("MainWindow", "2 terminals mode (Left split)", nullptr));
        actionTwo_terminals_mode_Right_split->setText(QCoreApplication::translate("MainWindow", "2 terminals mode (Right split)", nullptr));
        action4_Terminals_mode->setText(QCoreApplication::translate("MainWindow", "4 Terminals mode", nullptr));
        actionConfiguration->setText(QCoreApplication::translate("MainWindow", "Configuration", nullptr));
        actionImport_configuration->setText(QCoreApplication::translate("MainWindow", "Import configuration", nullptr));
        actionExport_configuration->setText(QCoreApplication::translate("MainWindow", "Export configuration", nullptr));
        actionReset_configuration->setText(QCoreApplication::translate("MainWindow", "Reset configuration", nullptr));
        actionDocumentation->setText(QCoreApplication::translate("MainWindow", "Documentation", nullptr));
        actionCheck_for_updates->setText(QCoreApplication::translate("MainWindow", "Check for updates", nullptr));
        actionOpenMainApp->setText(QCoreApplication::translate("MainWindow", "&Open Main Application", nullptr));
#if QT_CONFIG(shortcut)
        actionOpenMainApp->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        serverListLabel->setText(QCoreApplication::translate("MainWindow", "Servers & Folders", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        editButton->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "SSH Terminal", nullptr));
        connectSftpButton->setText(QCoreApplication::translate("MainWindow", "SFTP Browser", nullptr));
        welcomeLabel->setText(QCoreApplication::translate("MainWindow", "Welcome to QTiSSH!\n"
"\n"
"Select a server from the list on the left\n"
"and click 'Connect' or double-click to start an SSH session.\n"
"\n"
"Click 'Add' to add a new server.\n"
"\n"
"Or click the button below to open the main tabbed interface:", nullptr));
        openMainAppButton->setText(QCoreApplication::translate("MainWindow", "Open Main Application", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(welcomeTab), QCoreApplication::translate("MainWindow", "Welcome", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
        menuTerminal->setTitle(QCoreApplication::translate("MainWindow", "Terminal", nullptr));
        menuSessions->setTitle(QCoreApplication::translate("MainWindow", "Sessions", nullptr));
        menuUser_sessions->setTitle(QCoreApplication::translate("MainWindow", "User sessions", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
