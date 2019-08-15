/****************************************************************************
** Meta object code from reading C++ file 'mainheader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainheader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainheader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainHeader_t {
    QByteArrayData data[14];
    char stringdata0[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainHeader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainHeader_t qt_meta_stringdata_MainHeader = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainHeader"
QT_MOC_LITERAL(1, 11, 14), // "SigGetFileList"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5), // "index"
QT_MOC_LITERAL(4, 33, 11), // "SigToUpload"
QT_MOC_LITERAL(5, 45, 13), // "SigGetMyShare"
QT_MOC_LITERAL(6, 59, 12), // "SigGetShares"
QT_MOC_LITERAL(7, 72, 17), // "SigGetTransRecord"
QT_MOC_LITERAL(8, 90, 20), // "on_tbtn_exit_clicked"
QT_MOC_LITERAL(9, 111, 24), // "on_tbtn_filelist_clicked"
QT_MOC_LITERAL(10, 136, 22), // "on_tbtn_upload_clicked"
QT_MOC_LITERAL(11, 159, 23), // "on_tbtn_myshare_clicked"
QT_MOC_LITERAL(12, 183, 26), // "on_tbtn_share_list_clicked"
QT_MOC_LITERAL(13, 210, 21) // "on_tbtn_trans_clicked"

    },
    "MainHeader\0SigGetFileList\0\0index\0"
    "SigToUpload\0SigGetMyShare\0SigGetShares\0"
    "SigGetTransRecord\0on_tbtn_exit_clicked\0"
    "on_tbtn_filelist_clicked\0"
    "on_tbtn_upload_clicked\0on_tbtn_myshare_clicked\0"
    "on_tbtn_share_list_clicked\0"
    "on_tbtn_trans_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainHeader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       5,    1,   75,    2, 0x06 /* Public */,
       6,    1,   78,    2, 0x06 /* Public */,
       7,    1,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   84,    2, 0x08 /* Private */,
       9,    0,   85,    2, 0x08 /* Private */,
      10,    0,   86,    2, 0x08 /* Private */,
      11,    0,   87,    2, 0x08 /* Private */,
      12,    0,   88,    2, 0x08 /* Private */,
      13,    0,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainHeader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainHeader *_t = static_cast<MainHeader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SigGetFileList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->SigToUpload((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SigGetMyShare((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SigGetShares((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->SigGetTransRecord((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_tbtn_exit_clicked(); break;
        case 6: _t->on_tbtn_filelist_clicked(); break;
        case 7: _t->on_tbtn_upload_clicked(); break;
        case 8: _t->on_tbtn_myshare_clicked(); break;
        case 9: _t->on_tbtn_share_list_clicked(); break;
        case 10: _t->on_tbtn_trans_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainHeader::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainHeader::SigGetFileList)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainHeader::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainHeader::SigToUpload)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainHeader::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainHeader::SigGetMyShare)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainHeader::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainHeader::SigGetShares)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MainHeader::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainHeader::SigGetTransRecord)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject MainHeader::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainHeader.data,
      qt_meta_data_MainHeader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainHeader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainHeader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainHeader.stringdata0))
        return static_cast<void*>(const_cast< MainHeader*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainHeader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MainHeader::SigGetFileList(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainHeader::SigToUpload(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainHeader::SigGetMyShare(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainHeader::SigGetShares(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainHeader::SigGetTransRecord(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
