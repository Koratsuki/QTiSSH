/****************************************************************************
** Meta object code from reading C++ file 'terminalsession.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../terminalsession.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terminalsession.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_TerminalSession_t {
    uint offsetsAndSizes[38];
    char stringdata0[16];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[15];
    char stringdata5[17];
    char stringdata6[18];
    char stringdata7[9];
    char stringdata8[21];
    char stringdata9[11];
    char stringdata10[15];
    char stringdata11[23];
    char stringdata12[6];
    char stringdata13[19];
    char stringdata14[18];
    char stringdata15[21];
    char stringdata16[14];
    char stringdata17[29];
    char stringdata18[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_TerminalSession_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_TerminalSession_t qt_meta_stringdata_TerminalSession = {
    {
        QT_MOC_LITERAL(0, 15),  // "TerminalSession"
        QT_MOC_LITERAL(16, 20),  // "onTerminalKeyPressed"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 4),  // "data"
        QT_MOC_LITERAL(43, 14),  // "onTerminalBell"
        QT_MOC_LITERAL(58, 16),  // "onProcessStarted"
        QT_MOC_LITERAL(75, 17),  // "onProcessFinished"
        QT_MOC_LITERAL(93, 8),  // "exitCode"
        QT_MOC_LITERAL(102, 20),  // "QProcess::ExitStatus"
        QT_MOC_LITERAL(123, 10),  // "exitStatus"
        QT_MOC_LITERAL(134, 14),  // "onProcessError"
        QT_MOC_LITERAL(149, 22),  // "QProcess::ProcessError"
        QT_MOC_LITERAL(172, 5),  // "error"
        QT_MOC_LITERAL(178, 18),  // "onProcessReadyRead"
        QT_MOC_LITERAL(197, 17),  // "onSocketConnected"
        QT_MOC_LITERAL(215, 20),  // "onSocketDisconnected"
        QT_MOC_LITERAL(236, 13),  // "onSocketError"
        QT_MOC_LITERAL(250, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(279, 17)   // "onSocketReadyRead"
    },
    "TerminalSession",
    "onTerminalKeyPressed",
    "",
    "data",
    "onTerminalBell",
    "onProcessStarted",
    "onProcessFinished",
    "exitCode",
    "QProcess::ExitStatus",
    "exitStatus",
    "onProcessError",
    "QProcess::ProcessError",
    "error",
    "onProcessReadyRead",
    "onSocketConnected",
    "onSocketDisconnected",
    "onSocketError",
    "QAbstractSocket::SocketError",
    "onSocketReadyRead"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_TerminalSession[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x08,    1 /* Private */,
       4,    0,   77,    2, 0x08,    3 /* Private */,
       5,    0,   78,    2, 0x08,    4 /* Private */,
       6,    2,   79,    2, 0x08,    5 /* Private */,
      10,    1,   84,    2, 0x08,    8 /* Private */,
      13,    0,   87,    2, 0x08,   10 /* Private */,
      14,    0,   88,    2, 0x08,   11 /* Private */,
      15,    0,   89,    2, 0x08,   12 /* Private */,
      16,    1,   90,    2, 0x08,   13 /* Private */,
      18,    0,   93,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8,    7,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   12,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject TerminalSession::staticMetaObject = { {
    QMetaObject::SuperData::link<Session::staticMetaObject>(),
    qt_meta_stringdata_TerminalSession.offsetsAndSizes,
    qt_meta_data_TerminalSession,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_TerminalSession_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TerminalSession, std::true_type>,
        // method 'onTerminalKeyPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'onTerminalBell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProcessStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProcessFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>,
        // method 'onProcessError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ProcessError, std::false_type>,
        // method 'onProcessReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSocketConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSocketDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSocketError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'onSocketReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void TerminalSession::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TerminalSession *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onTerminalKeyPressed((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->onTerminalBell(); break;
        case 2: _t->onProcessStarted(); break;
        case 3: _t->onProcessFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 4: _t->onProcessError((*reinterpret_cast< std::add_pointer_t<QProcess::ProcessError>>(_a[1]))); break;
        case 5: _t->onProcessReadyRead(); break;
        case 6: _t->onSocketConnected(); break;
        case 7: _t->onSocketDisconnected(); break;
        case 8: _t->onSocketError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 9: _t->onSocketReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject *TerminalSession::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerminalSession::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TerminalSession.stringdata0))
        return static_cast<void*>(this);
    return Session::qt_metacast(_clname);
}

int TerminalSession::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Session::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
