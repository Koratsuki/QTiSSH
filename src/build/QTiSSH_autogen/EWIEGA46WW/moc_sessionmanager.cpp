/****************************************************************************
** Meta object code from reading C++ file 'sessionmanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../sessionmanager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sessionmanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_SessionManager_t {
    uint offsetsAndSizes[40];
    char stringdata0[15];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[8];
    char stringdata5[15];
    char stringdata6[10];
    char stringdata7[21];
    char stringdata8[11];
    char stringdata9[11];
    char stringdata10[20];
    char stringdata11[6];
    char stringdata12[19];
    char stringdata13[22];
    char stringdata14[13];
    char stringdata15[9];
    char stringdata16[9];
    char stringdata17[15];
    char stringdata18[6];
    char stringdata19[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SessionManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SessionManager_t qt_meta_stringdata_SessionManager = {
    {
        QT_MOC_LITERAL(0, 14),  // "SessionManager"
        QT_MOC_LITERAL(15, 14),  // "sessionCreated"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 8),  // "Session*"
        QT_MOC_LITERAL(40, 7),  // "session"
        QT_MOC_LITERAL(48, 14),  // "sessionRemoved"
        QT_MOC_LITERAL(63, 9),  // "sessionId"
        QT_MOC_LITERAL(73, 20),  // "activeSessionChanged"
        QT_MOC_LITERAL(94, 10),  // "newSession"
        QT_MOC_LITERAL(105, 10),  // "oldSession"
        QT_MOC_LITERAL(116, 19),  // "sessionCountChanged"
        QT_MOC_LITERAL(136, 5),  // "count"
        QT_MOC_LITERAL(142, 18),  // "allSessionsRemoved"
        QT_MOC_LITERAL(161, 21),  // "onSessionStateChanged"
        QT_MOC_LITERAL(183, 12),  // "SessionState"
        QT_MOC_LITERAL(196, 8),  // "newState"
        QT_MOC_LITERAL(205, 8),  // "oldState"
        QT_MOC_LITERAL(214, 14),  // "onSessionError"
        QT_MOC_LITERAL(229, 5),  // "error"
        QT_MOC_LITERAL(235, 15)   // "onSessionClosed"
    },
    "SessionManager",
    "sessionCreated",
    "",
    "Session*",
    "session",
    "sessionRemoved",
    "sessionId",
    "activeSessionChanged",
    "newSession",
    "oldSession",
    "sessionCountChanged",
    "count",
    "allSessionsRemoved",
    "onSessionStateChanged",
    "SessionState",
    "newState",
    "oldState",
    "onSessionError",
    "error",
    "onSessionClosed"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SessionManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       5,    1,   65,    2, 0x06,    3 /* Public */,
       7,    2,   68,    2, 0x06,    5 /* Public */,
      10,    1,   73,    2, 0x06,    8 /* Public */,
      12,    0,   76,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    2,   77,    2, 0x08,   11 /* Private */,
      17,    1,   82,    2, 0x08,   14 /* Private */,
      19,    0,   85,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QUuid,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    8,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 14,   15,   16,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SessionManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SessionManager.offsetsAndSizes,
    qt_meta_data_SessionManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SessionManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SessionManager, std::true_type>,
        // method 'sessionCreated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Session *, std::false_type>,
        // method 'sessionRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUuid &, std::false_type>,
        // method 'activeSessionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Session *, std::false_type>,
        QtPrivate::TypeAndForceComplete<Session *, std::false_type>,
        // method 'sessionCountChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'allSessionsRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSessionStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SessionState, std::false_type>,
        QtPrivate::TypeAndForceComplete<SessionState, std::false_type>,
        // method 'onSessionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onSessionClosed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SessionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SessionManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sessionCreated((*reinterpret_cast< std::add_pointer_t<Session*>>(_a[1]))); break;
        case 1: _t->sessionRemoved((*reinterpret_cast< std::add_pointer_t<QUuid>>(_a[1]))); break;
        case 2: _t->activeSessionChanged((*reinterpret_cast< std::add_pointer_t<Session*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Session*>>(_a[2]))); break;
        case 3: _t->sessionCountChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->allSessionsRemoved(); break;
        case 5: _t->onSessionStateChanged((*reinterpret_cast< std::add_pointer_t<SessionState>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<SessionState>>(_a[2]))); break;
        case 6: _t->onSessionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->onSessionClosed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Session* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Session* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SessionManager::*)(Session * );
            if (_t _q_method = &SessionManager::sessionCreated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SessionManager::*)(const QUuid & );
            if (_t _q_method = &SessionManager::sessionRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SessionManager::*)(Session * , Session * );
            if (_t _q_method = &SessionManager::activeSessionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SessionManager::*)(int );
            if (_t _q_method = &SessionManager::sessionCountChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SessionManager::*)();
            if (_t _q_method = &SessionManager::allSessionsRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *SessionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SessionManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SessionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SessionManager::sessionCreated(Session * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SessionManager::sessionRemoved(const QUuid & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SessionManager::activeSessionChanged(Session * _t1, Session * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SessionManager::sessionCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SessionManager::allSessionsRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
