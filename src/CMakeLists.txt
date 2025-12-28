cmake_minimum_required(VERSION 3.16)

project(QTiSSH VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        maintabwindow.cpp
        maintabwindow.h
        sessionmanager.h
        sessionmanager.cpp
        session.h
        session.cpp
        add_server.h
        add_server.cpp
        add_server.ui
        serverconfig.h
        serverconfig.cpp
        servermanager.h
        servermanager.cpp
        sshterminal.h
        sshterminal.cpp
        filetransfer.h
        filetransfer.cpp
        filetransfermanager.h
        filetransfermanager.cpp
        sftpconnection.h
        sftpconnection.cpp
        sftpbrowser.h
        sftpbrowser.cpp
        foldermanager.h
        foldermanager.cpp
        servertreewidget.h
        servertreewidget.cpp
        terminalsession.h
        terminalsession.cpp
        vt100parser.h
        vt100parser.cpp
        vt100terminal.h
        vt100terminal.cpp
        terminalscreen.h
        terminalscreen.cpp
        terminalchar.h
        terminalchar.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QTiSSH
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QTiSSH APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QTiSSH SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QTiSSH
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QTiSSH PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QTiSSH)
endif()
set_target_properties(QTiSSH PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QTiSSH
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTiSSH)
endif()
