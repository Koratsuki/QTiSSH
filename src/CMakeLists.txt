cmake_minimum_required(VERSION 3.16)

project(QTiSSH VERSION 0.1.4 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network DBus Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network DBus Charts)
find_package(OpenSSL REQUIRED)

# Translations: regenerate .qm from .ts at configure time when lrelease is available.
set(QTISSH_TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/i18n/qtissh_es.ts
)
find_program(LRELEASE_EXECUTABLE NAMES lrelease6 lrelease-qt6 lrelease)
if(LRELEASE_EXECUTABLE)
    foreach(_ts IN LISTS QTISSH_TS_FILES)
        get_filename_component(_ts_we ${_ts} NAME_WE)
        execute_process(
            COMMAND ${LRELEASE_EXECUTABLE} -silent ${_ts} -qm ${CMAKE_CURRENT_SOURCE_DIR}/i18n/${_ts_we}.qm
        )
    endforeach()
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        add_server.h
        add_server.cpp
        add_server.ui
        settingsdialog.h
        settingsdialog.cpp
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
        thememanager.h
        thememanager.cpp
        snippetmanager.h
        snippetmanager.cpp
        snippetdialog.h
        snippetdialog.cpp
        passwordmanager.h
        passwordmanager.cpp
        commandhistorymanager.h
        commandhistorymanager.cpp
        commandhistorydialog.h
        commandhistorydialog.cpp
        foldermanager.h
        foldermanager.cpp
        settingsmanager.h
        settingsmanager.cpp
        servertreewidget.cpp
        terminalchar.h
        terminalchar.cpp
        terminalscreen.h
        terminalscreen.cpp
        vt100parser.h
        vt100parser.cpp
        vt100terminal.h
        vt100terminal.cpp
        profilemanager.h
        profilemanager.cpp
        profilemanagerdialog.h
        profilemanagerdialog.cpp
        sessionlogger.h
        sessionlogger.cpp
        recentconnectionsmanager.h
        recentconnectionsmanager.cpp
        terminalsplitwidget.h
        terminalsplitwidget.cpp
        globalhotkeymanager.h
        globalhotkeymanager.cpp
        keychainstore.h
        keychainstore.cpp
        quickcommandsdialog.h
        quickcommandsdialog.cpp
        monitoringdialog.h
        monitoringdialog.cpp
        metricbutton.h
        metricbutton.cpp
        servermonitoringbar.h
        servermonitoringbar.cpp
        networkdiscoverydialog.h
        networkdiscoverydialog.cpp
        connectionlogsdialog.h
        connectionlogsdialog.cpp
        remoteeditor.h
        remoteeditor.cpp
        applockmanager.h
        applockmanager.cpp
        applockdialog.h
        applockdialog.cpp
        i18n/translations.qrc
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

target_link_libraries(QTiSSH PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::DBus Qt${QT_VERSION_MAJOR}::Charts OpenSSL::SSL OpenSSL::Crypto)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(QTiSSH PRIVATE X11::X11)
endif()
target_include_directories(QTiSSH PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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

# Desktop entry
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../packaging/qtissh.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

# Application icon (hicolor + pixmaps fallback)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../img/icon.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps
    RENAME qtissh.png)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../img/icon.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps
    RENAME qtissh.png)

# ---------------------------------------------------------------------------
# Packaging: DEB and RPM via CPack
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "qtissh")
set(CPACK_PACKAGE_VENDOR "Koratsuki")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt SSH Connection Manager")
set(CPACK_PACKAGE_DESCRIPTION
    "A modern, user-friendly SSH connection manager built with Qt. "
    "Manage multiple SSH connections with ease using a tabbed interface, "
    "including an SFTP file browser, terminal emulator, tunnels and more.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/Koratsuki/QTiSSH")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "Koratsuki")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Koratsuki <koratsuki@users.noreply.github.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libqt6core6 | libqt6core6t64 (>= 6.2), \
     libqt6gui6 | libqt6gui6t64 (>= 6.2), \
     libqt6widgets6 | libqt6widgets6t64 (>= 6.2), \
     libqt6network6 | libqt6network6t64 (>= 6.2), \
     libqt6dbus6 | libqt6dbus6t64 (>= 6.2), \
     libqt6charts6, \
     libssl3 | libssl3t64, \
     libx11-6, \
     openssh-client")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)

set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Internet")
set(CPACK_RPM_PACKAGE_REQUIRES
    "qt6-qtbase >= 6.2, qt6-qtbase-gui, qt6-qtcharts, \
     openssl-libs, libX11, openssh-clients")
set(CPACK_RPM_PACKAGE_AUTOREQ OFF)
set(CPACK_RPM_PACKAGE_AUTOPROV OFF)

include(CPack)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTiSSH)
endif()
