# -------------------------------------------------
#
# Project name: QTiSSH
# Authors: Leslie León & Enier González
# 
# -------------------------------------------------
TEMPLATE = app
TARGET = QTiSSH
VERSION = v0.1.4

# -------------------------------------------------
# Basic configuration
# -------------------------------------------------
QT += core gui widgets
CONFIG += c++17 release
# CONFIG += debug

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
    src/servertreewidget.cpp

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
    src/servertreewidget.h

FORMS += \
    src/mainwindow.ui \
    src/add_server.ui \
    src/sshterminal.ui \
    src/sftpbrowser.ui
