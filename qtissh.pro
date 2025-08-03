# -------------------------------------------------
#
# Project name: QTiSSH
# Authors: Leslie León & Enier González
# 
# -------------------------------------------------
TEMPLATE = app
TARGET = QTiSSH
VERSION = v0.0.1

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
    src/add_server.cpp

HEADERS += \
    src/mainwindow.h \
    src/add_server.h

FORMS += \
    src/mainwindow.ui \
    src/add_server.ui

