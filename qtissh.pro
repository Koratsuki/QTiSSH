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
# Archivos fuente, cabeceras y UI
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

# -------------------------------------------------
# Configuración de directorios de salida
# -------------------------------------------------
# Carpeta donde se generarán los archivos moc, ui y obj
MOC_DIR = $$PWD/build/moc       # Archivos generados por el meta-object compiler
UI_DIR  = $$PWD/build/ui        # Archivos generados de los .ui a .h
OBJECTS_DIR = $$PWD/build/obj   # Archivos objeto intermedios

# Carpeta donde se guardará el binario final
DESTDIR = $$PWD/build
