/****************************************************************************
** Meta object code from reading C++ file 'view_adapter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Licenta/gui/view_adapter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view_adapter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ViewAdapter_t {
    QByteArrayData data[8];
    char stringdata[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ViewAdapter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ViewAdapter_t qt_meta_stringdata_ViewAdapter = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 18),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 15),
QT_MOC_LITERAL(4, 48, 16),
QT_MOC_LITERAL(5, 65, 13),
QT_MOC_LITERAL(6, 79, 22),
QT_MOC_LITERAL(7, 102, 20)
    },
    "ViewAdapter\0OnInitializeSignal\0\0"
    "OnDestroySignal\0OnInitializeSlot\0"
    "OnDestroySlot\0OnCreateMainWindowSlot\0"
    "OnShowMainWindowSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ViewAdapter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x05,
       3,    0,   45,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,   46,    2, 0x0a,
       5,    0,   47,    2, 0x0a,
       6,    0,   48,    2, 0x0a,
       7,    0,   49,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ViewAdapter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ViewAdapter *_t = static_cast<ViewAdapter *>(_o);
        switch (_id) {
        case 0: _t->OnInitializeSignal(); break;
        case 1: _t->OnDestroySignal(); break;
        case 2: _t->OnInitializeSlot(); break;
        case 3: _t->OnDestroySlot(); break;
        case 4: _t->OnCreateMainWindowSlot(); break;
        case 5: _t->OnShowMainWindowSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ViewAdapter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ViewAdapter::OnInitializeSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (ViewAdapter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ViewAdapter::OnDestroySignal)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ViewAdapter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ViewAdapter.data,
      qt_meta_data_ViewAdapter,  qt_static_metacall, 0, 0}
};


const QMetaObject *ViewAdapter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ViewAdapter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ViewAdapter.stringdata))
        return static_cast<void*>(const_cast< ViewAdapter*>(this));
    return QObject::qt_metacast(_clname);
}

int ViewAdapter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ViewAdapter::OnInitializeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ViewAdapter::OnDestroySignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
