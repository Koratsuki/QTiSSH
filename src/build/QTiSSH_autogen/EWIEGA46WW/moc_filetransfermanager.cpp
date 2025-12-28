/****************************************************************************
** Meta object code from reading C++ file 'filetransfermanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../filetransfermanager.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filetransfermanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_FileTransferManager_t {
    uint offsetsAndSizes[38];
    char stringdata0[20];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[9];
    char stringdata5[16];
    char stringdata6[11];
    char stringdata7[24];
    char stringdata8[8];
    char stringdata9[22];
    char stringdata10[15];
    char stringdata11[7];
    char stringdata12[17];
    char stringdata13[8];
    char stringdata14[13];
    char stringdata15[19];
    char stringdata16[26];
    char stringdata17[24];
    char stringdata18[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_FileTransferManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_FileTransferManager_t qt_meta_stringdata_FileTransferManager = {
    {
        QT_MOC_LITERAL(0, 19),  // "FileTransferManager"
        QT_MOC_LITERAL(20, 13),  // "transferAdded"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 13),  // "FileTransfer*"
        QT_MOC_LITERAL(49, 8),  // "transfer"
        QT_MOC_LITERAL(58, 15),  // "transferRemoved"
        QT_MOC_LITERAL(74, 10),  // "transferId"
        QT_MOC_LITERAL(85, 23),  // "transferProgressChanged"
        QT_MOC_LITERAL(109, 7),  // "percent"
        QT_MOC_LITERAL(117, 21),  // "transferStatusChanged"
        QT_MOC_LITERAL(139, 14),  // "TransferStatus"
        QT_MOC_LITERAL(154, 6),  // "status"
        QT_MOC_LITERAL(161, 16),  // "transferFinished"
        QT_MOC_LITERAL(178, 7),  // "success"
        QT_MOC_LITERAL(186, 12),  // "queueChanged"
        QT_MOC_LITERAL(199, 18),  // "onTransferFinished"
        QT_MOC_LITERAL(218, 25),  // "onTransferProgressChanged"
        QT_MOC_LITERAL(244, 23),  // "onTransferStatusChanged"
        QT_MOC_LITERAL(268, 12)   // "processQueue"
    },
    "FileTransferManager",
    "transferAdded",
    "",
    "FileTransfer*",
    "transfer",
    "transferRemoved",
    "transferId",
    "transferProgressChanged",
    "percent",
    "transferStatusChanged",
    "TransferStatus",
    "status",
    "transferFinished",
    "success",
    "queueChanged",
    "onTransferFinished",
    "onTransferProgressChanged",
    "onTransferStatusChanged",
    "processQueue"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_FileTransferManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       5,    1,   77,    2, 0x06,    3 /* Public */,
       7,    2,   80,    2, 0x06,    5 /* Public */,
       9,    2,   85,    2, 0x06,    8 /* Public */,
      12,    2,   90,    2, 0x06,   11 /* Public */,
      14,    0,   95,    2, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    1,   96,    2, 0x08,   15 /* Private */,
      16,    1,   99,    2, 0x08,   17 /* Private */,
      17,    1,  102,    2, 0x08,   19 /* Private */,
      18,    0,  105,    2, 0x08,   21 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    8,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10,    6,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    6,   13,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject FileTransferManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FileTransferManager.offsetsAndSizes,
    qt_meta_data_FileTransferManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_FileTransferManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FileTransferManager, std::true_type>,
        // method 'transferAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<FileTransfer *, std::false_type>,
        // method 'transferRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'transferProgressChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'transferStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<TransferStatus, std::false_type>,
        // method 'transferFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'queueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTransferFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onTransferProgressChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTransferStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<TransferStatus, std::false_type>,
        // method 'processQueue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void FileTransferManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileTransferManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->transferAdded((*reinterpret_cast< std::add_pointer_t<FileTransfer*>>(_a[1]))); break;
        case 1: _t->transferRemoved((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->transferProgressChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->transferStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<TransferStatus>>(_a[2]))); break;
        case 4: _t->transferFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 5: _t->queueChanged(); break;
        case 6: _t->onTransferFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->onTransferProgressChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->onTransferStatusChanged((*reinterpret_cast< std::add_pointer_t<TransferStatus>>(_a[1]))); break;
        case 9: _t->processQueue(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< FileTransfer* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FileTransferManager::*)(FileTransfer * );
            if (_t _q_method = &FileTransferManager::transferAdded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FileTransferManager::*)(const QString & );
            if (_t _q_method = &FileTransferManager::transferRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FileTransferManager::*)(const QString & , int );
            if (_t _q_method = &FileTransferManager::transferProgressChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FileTransferManager::*)(const QString & , TransferStatus );
            if (_t _q_method = &FileTransferManager::transferStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FileTransferManager::*)(const QString & , bool );
            if (_t _q_method = &FileTransferManager::transferFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FileTransferManager::*)();
            if (_t _q_method = &FileTransferManager::queueChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *FileTransferManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileTransferManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FileTransferManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FileTransferManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void FileTransferManager::transferAdded(FileTransfer * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileTransferManager::transferRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileTransferManager::transferProgressChanged(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileTransferManager::transferStatusChanged(const QString & _t1, TransferStatus _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FileTransferManager::transferFinished(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FileTransferManager::queueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
