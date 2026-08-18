# -------------------------------------------------
#
# Project name: QTiSSH
# Authors: Leslie León & Enier González
# 
# -------------------------------------------------
TEMPLATE = app
TARGET = QTiSSH
VERSION = v0.1.5

# -------------------------------------------------
# Basic configuration
# -------------------------------------------------
QT += core gui widgets network dbus charts
CONFIG += c++17 release
# CONFIG += debug

# Include paths
INCLUDEPATH += src

# OpenSSL for credential encryption
LIBS += -lssl -lcrypto

unix:!macx: LIBS += -lX11

# -------------------------------------------------
# Source files, headers and UI
# -------------------------------------------------
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/add_server.cpp \
    src/serverconfig.cpp \
    src/servermanager.cpp \
    src/sshterminal.cpp \
    src/filetransfer.cpp \
    src/filetransfermanager.cpp \
    src/sftpconnection.cpp \
    src/sftpbrowser.cpp \
    src/foldermanager.cpp \
    src/servertreewidget.cpp \
    src/terminalchar.cpp \
    src/terminalscreen.cpp \
    src/vt100parser.cpp \
    src/vt100terminal.cpp \
    src/thememanager.cpp \
    src/settingsmanager.cpp \
    src/settingsdialog.cpp \
    src/snippetmanager.cpp \
    src/snippetdialog.cpp \
    src/passwordmanager.cpp \
    src/commandhistorymanager.cpp \
    src/commandhistorydialog.cpp \
    src/profilemanager.cpp \
    src/profilemanagerdialog.cpp \
    src/sessionlogger.cpp \
    src/recentconnectionsmanager.cpp \
    src/terminalsplitwidget.cpp \
    src/globalhotkeymanager.cpp \
    src/keychainstore.cpp \
    src/quickcommandsdialog.cpp \
    src/metricbutton.cpp \
        metricbutton.h \
        monitoringdialog.cpp \
    src/servermonitoringbar.cpp \
    src/networkdiscoverydialog.cpp \
    src/connectionlogsdialog.cpp \
    src/remoteeditor.cpp \
    src/applockmanager.cpp \
    src/applockdialog.cpp

HEADERS += \
    src/mainwindow.h \
    src/add_server.h \
    src/serverconfig.h \
    src/servermanager.h \
    src/sshterminal.h \
    src/filetransfer.h \
    src/filetransfermanager.h \
    src/sftpconnection.h \
    src/sftpbrowser.h \
    src/foldermanager.h \
    src/servertreewidget.h \
    src/terminalchar.h \
    src/terminalscreen.h \
    src/vt100parser.h \
    src/vt100terminal.h \
    src/thememanager.h \
    src/settingsmanager.h \
    src/settingsdialog.h \
    src/snippetmanager.h \
    src/snippetdialog.h \
    src/passwordmanager.h \
    src/commandhistorymanager.h \
    src/commandhistorydialog.h \
    src/profilemanager.h \
    src/profilemanagerdialog.h \
    src/sessionlogger.h \
    src/recentconnectionsmanager.h \
    src/terminalsplitwidget.h \
    src/globalhotkeymanager.h \
    src/keychainstore.h \
    src/quickcommandsdialog.h \
    src/monitoringdialog.h \
    src/servermonitoringbar.h \
    src/networkdiscoverydialog.h \
    src/connectionlogsdialog.h \
    src/remoteeditor.h \
    src/applockmanager.h \
    src/applockdialog.h

FORMS += \
    src/mainwindow.ui \
    src/add_server.ui \
    src/sshterminal.ui \
    src/sftpbrowser.ui

TRANSLATIONS += \
    src/i18n/qtissh_es.ts

RESOURCES += \
    src/i18n/translations.qrc
