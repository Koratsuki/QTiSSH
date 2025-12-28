/****************************************************************************
** Meta object code from reading C++ file 'maintabwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../maintabwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maintabwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainTabWindow_t {
    uint offsetsAndSizes[116];
    char stringdata0[14];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[16];
    char stringdata5[19];
    char stringdata6[15];
    char stringdata7[19];
    char stringdata8[17];
    char stringdata9[9];
    char stringdata10[8];
    char stringdata11[17];
    char stringdata12[10];
    char stringdata13[23];
    char stringdata14[11];
    char stringdata15[11];
    char stringdata16[19];
    char stringdata17[20];
    char stringdata18[22];
    char stringdata19[17];
    char stringdata20[21];
    char stringdata21[22];
    char stringdata22[13];
    char stringdata23[7];
    char stringdata24[17];
    char stringdata25[15];
    char stringdata26[24];
    char stringdata27[15];
    char stringdata28[24];
    char stringdata29[9];
    char stringdata30[24];
    char stringdata31[29];
    char stringdata32[22];
    char stringdata33[9];
    char stringdata34[24];
    char stringdata35[22];
    char stringdata36[12];
    char stringdata37[13];
    char stringdata38[6];
    char stringdata39[20];
    char stringdata40[17];
    char stringdata41[4];
    char stringdata42[25];
    char stringdata43[17];
    char stringdata44[5];
    char stringdata45[7];
    char stringdata46[25];
    char stringdata47[31];
    char stringdata48[8];
    char stringdata49[14];
    char stringdata50[21];
    char stringdata51[19];
    char stringdata52[22];
    char stringdata53[13];
    char stringdata54[9];
    char stringdata55[9];
    char stringdata56[21];
    char stringdata57[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainTabWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainTabWindow_t qt_meta_stringdata_MainTabWindow = {
    {
        QT_MOC_LITERAL(0, 13),  // "MainTabWindow"
        QT_MOC_LITERAL(14, 12),  // "onNewSession"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 15),  // "onNewSSHSession"
        QT_MOC_LITERAL(44, 15),  // "onNewLocalShell"
        QT_MOC_LITERAL(60, 18),  // "onNewTelnetSession"
        QT_MOC_LITERAL(79, 14),  // "onCloseSession"
        QT_MOC_LITERAL(94, 18),  // "onCloseAllSessions"
        QT_MOC_LITERAL(113, 16),  // "onSessionCreated"
        QT_MOC_LITERAL(130, 8),  // "Session*"
        QT_MOC_LITERAL(139, 7),  // "session"
        QT_MOC_LITERAL(147, 16),  // "onSessionRemoved"
        QT_MOC_LITERAL(164, 9),  // "sessionId"
        QT_MOC_LITERAL(174, 22),  // "onActiveSessionChanged"
        QT_MOC_LITERAL(197, 10),  // "newSession"
        QT_MOC_LITERAL(208, 10),  // "oldSession"
        QT_MOC_LITERAL(219, 18),  // "onAddServerClicked"
        QT_MOC_LITERAL(238, 19),  // "onEditServerClicked"
        QT_MOC_LITERAL(258, 21),  // "onDeleteServerClicked"
        QT_MOC_LITERAL(280, 16),  // "onConnectClicked"
        QT_MOC_LITERAL(297, 20),  // "onConnectSftpClicked"
        QT_MOC_LITERAL(318, 21),  // "onServerDoubleClicked"
        QT_MOC_LITERAL(340, 12),  // "ServerConfig"
        QT_MOC_LITERAL(353, 6),  // "server"
        QT_MOC_LITERAL(360, 16),  // "onServersChanged"
        QT_MOC_LITERAL(377, 14),  // "onAboutClicked"
        QT_MOC_LITERAL(392, 23),  // "onCreateFolderRequested"
        QT_MOC_LITERAL(416, 14),  // "parentFolderId"
        QT_MOC_LITERAL(431, 23),  // "onRenameFolderRequested"
        QT_MOC_LITERAL(455, 8),  // "folderId"
        QT_MOC_LITERAL(464, 23),  // "onDeleteFolderRequested"
        QT_MOC_LITERAL(488, 28),  // "onAddServerToFolderRequested"
        QT_MOC_LITERAL(517, 21),  // "onEditServerRequested"
        QT_MOC_LITERAL(539, 8),  // "serverId"
        QT_MOC_LITERAL(548, 23),  // "onDeleteServerRequested"
        QT_MOC_LITERAL(572, 21),  // "onMoveServerRequested"
        QT_MOC_LITERAL(594, 11),  // "newFolderId"
        QT_MOC_LITERAL(606, 12),  // "onTabChanged"
        QT_MOC_LITERAL(619, 5),  // "index"
        QT_MOC_LITERAL(625, 19),  // "onTabCloseRequested"
        QT_MOC_LITERAL(645, 16),  // "onTabContextMenu"
        QT_MOC_LITERAL(662, 3),  // "pos"
        QT_MOC_LITERAL(666, 24),  // "onSessionTreeItemClicked"
        QT_MOC_LITERAL(691, 16),  // "QTreeWidgetItem*"
        QT_MOC_LITERAL(708, 4),  // "item"
        QT_MOC_LITERAL(713, 6),  // "column"
        QT_MOC_LITERAL(720, 24),  // "onSessionTreeContextMenu"
        QT_MOC_LITERAL(745, 30),  // "onSessionTreeItemDoubleClicked"
        QT_MOC_LITERAL(776, 7),  // "onAbout"
        QT_MOC_LITERAL(784, 13),  // "onPreferences"
        QT_MOC_LITERAL(798, 20),  // "onToggleSessionPanel"
        QT_MOC_LITERAL(819, 18),  // "onToggleFullScreen"
        QT_MOC_LITERAL(838, 21),  // "onSessionStateChanged"
        QT_MOC_LITERAL(860, 12),  // "SessionState"
        QT_MOC_LITERAL(873, 8),  // "newState"
        QT_MOC_LITERAL(882, 8),  // "oldState"
        QT_MOC_LITERAL(891, 20),  // "onSessionNameChanged"
        QT_MOC_LITERAL(912, 7)   // "newName"
    },
    "MainTabWindow",
    "onNewSession",
    "",
    "onNewSSHSession",
    "onNewLocalShell",
    "onNewTelnetSession",
    "onCloseSession",
    "onCloseAllSessions",
    "onSessionCreated",
    "Session*",
    "session",
    "onSessionRemoved",
    "sessionId",
    "onActiveSessionChanged",
    "newSession",
    "oldSession",
    "onAddServerClicked",
    "onEditServerClicked",
    "onDeleteServerClicked",
    "onConnectClicked",
    "onConnectSftpClicked",
    "onServerDoubleClicked",
    "ServerConfig",
    "server",
    "onServersChanged",
    "onAboutClicked",
    "onCreateFolderRequested",
    "parentFolderId",
    "onRenameFolderRequested",
    "folderId",
    "onDeleteFolderRequested",
    "onAddServerToFolderRequested",
    "onEditServerRequested",
    "serverId",
    "onDeleteServerRequested",
    "onMoveServerRequested",
    "newFolderId",
    "onTabChanged",
    "index",
    "onTabCloseRequested",
    "onTabContextMenu",
    "pos",
    "onSessionTreeItemClicked",
    "QTreeWidgetItem*",
    "item",
    "column",
    "onSessionTreeContextMenu",
    "onSessionTreeItemDoubleClicked",
    "onAbout",
    "onPreferences",
    "onToggleSessionPanel",
    "onToggleFullScreen",
    "onSessionStateChanged",
    "SessionState",
    "newState",
    "oldState",
    "onSessionNameChanged",
    "newName"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainTabWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  230,    2, 0x08,    1 /* Private */,
       3,    0,  231,    2, 0x08,    2 /* Private */,
       4,    0,  232,    2, 0x08,    3 /* Private */,
       5,    0,  233,    2, 0x08,    4 /* Private */,
       6,    0,  234,    2, 0x08,    5 /* Private */,
       7,    0,  235,    2, 0x08,    6 /* Private */,
       8,    1,  236,    2, 0x08,    7 /* Private */,
      11,    1,  239,    2, 0x08,    9 /* Private */,
      13,    2,  242,    2, 0x08,   11 /* Private */,
      16,    0,  247,    2, 0x08,   14 /* Private */,
      17,    0,  248,    2, 0x08,   15 /* Private */,
      18,    0,  249,    2, 0x08,   16 /* Private */,
      19,    0,  250,    2, 0x08,   17 /* Private */,
      20,    0,  251,    2, 0x08,   18 /* Private */,
      21,    1,  252,    2, 0x08,   19 /* Private */,
      24,    0,  255,    2, 0x08,   21 /* Private */,
      25,    0,  256,    2, 0x08,   22 /* Private */,
      26,    1,  257,    2, 0x08,   23 /* Private */,
      28,    1,  260,    2, 0x08,   25 /* Private */,
      30,    1,  263,    2, 0x08,   27 /* Private */,
      31,    1,  266,    2, 0x08,   29 /* Private */,
      32,    1,  269,    2, 0x08,   31 /* Private */,
      34,    1,  272,    2, 0x08,   33 /* Private */,
      35,    2,  275,    2, 0x08,   35 /* Private */,
      37,    1,  280,    2, 0x08,   38 /* Private */,
      39,    1,  283,    2, 0x08,   40 /* Private */,
      40,    1,  286,    2, 0x08,   42 /* Private */,
      42,    2,  289,    2, 0x08,   44 /* Private */,
      46,    1,  294,    2, 0x08,   47 /* Private */,
      47,    2,  297,    2, 0x08,   49 /* Private */,
      48,    0,  302,    2, 0x08,   52 /* Private */,
      49,    0,  303,    2, 0x08,   53 /* Private */,
      50,    0,  304,    2, 0x08,   54 /* Private */,
      51,    0,  305,    2, 0x08,   55 /* Private */,
      52,    2,  306,    2, 0x08,   56 /* Private */,
      56,    1,  311,    2, 0x08,   59 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QUuid,   12,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   33,   36,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::QPoint,   41,
    QMetaType::Void, 0x80000000 | 43, QMetaType::Int,   44,   45,
    QMetaType::Void, QMetaType::QPoint,   41,
    QMetaType::Void, 0x80000000 | 43, QMetaType::Int,   44,   45,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 53, 0x80000000 | 53,   54,   55,
    QMetaType::Void, QMetaType::QString,   57,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainTabWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainTabWindow.offsetsAndSizes,
    qt_meta_data_MainTabWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainTabWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainTabWindow, std::true_type>,
        // method 'onNewSession'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewSSHSession'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewLocalShell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewTelnetSession'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCloseSession'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCloseAllSessions'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSessionCreated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Session *, std::false_type>,
        // method 'onSessionRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUuid &, std::false_type>,
        // method 'onActiveSessionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Session *, std::false_type>,
        QtPrivate::TypeAndForceComplete<Session *, std::false_type>,
        // method 'onAddServerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditServerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteServerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConnectClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onConnectSftpClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onServerDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const ServerConfig &, std::false_type>,
        // method 'onServersChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAboutClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCreateFolderRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onRenameFolderRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onDeleteFolderRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onAddServerToFolderRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onEditServerRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onDeleteServerRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onMoveServerRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTabChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTabCloseRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTabContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onSessionTreeItemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSessionTreeContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onSessionTreeItemDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onAbout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPreferences'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onToggleSessionPanel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onToggleFullScreen'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSessionStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SessionState, std::false_type>,
        QtPrivate::TypeAndForceComplete<SessionState, std::false_type>,
        // method 'onSessionNameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void MainTabWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainTabWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onNewSession(); break;
        case 1: _t->onNewSSHSession(); break;
        case 2: _t->onNewLocalShell(); break;
        case 3: _t->onNewTelnetSession(); break;
        case 4: _t->onCloseSession(); break;
        case 5: _t->onCloseAllSessions(); break;
        case 6: _t->onSessionCreated((*reinterpret_cast< std::add_pointer_t<Session*>>(_a[1]))); break;
        case 7: _t->onSessionRemoved((*reinterpret_cast< std::add_pointer_t<QUuid>>(_a[1]))); break;
        case 8: _t->onActiveSessionChanged((*reinterpret_cast< std::add_pointer_t<Session*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Session*>>(_a[2]))); break;
        case 9: _t->onAddServerClicked(); break;
        case 10: _t->onEditServerClicked(); break;
        case 11: _t->onDeleteServerClicked(); break;
        case 12: _t->onConnectClicked(); break;
        case 13: _t->onConnectSftpClicked(); break;
        case 14: _t->onServerDoubleClicked((*reinterpret_cast< std::add_pointer_t<ServerConfig>>(_a[1]))); break;
        case 15: _t->onServersChanged(); break;
        case 16: _t->onAboutClicked(); break;
        case 17: _t->onCreateFolderRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->onRenameFolderRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->onDeleteFolderRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->onAddServerToFolderRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->onEditServerRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->onDeleteServerRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->onMoveServerRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 24: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->onTabCloseRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->onTabContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 27: _t->onSessionTreeItemClicked((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 28: _t->onSessionTreeContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 29: _t->onSessionTreeItemDoubleClicked((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 30: _t->onAbout(); break;
        case 31: _t->onPreferences(); break;
        case 32: _t->onToggleSessionPanel(); break;
        case 33: _t->onToggleFullScreen(); break;
        case 34: _t->onSessionStateChanged((*reinterpret_cast< std::add_pointer_t<SessionState>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<SessionState>>(_a[2]))); break;
        case 35: _t->onSessionNameChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Session* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Session* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainTabWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainTabWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainTabWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainTabWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
