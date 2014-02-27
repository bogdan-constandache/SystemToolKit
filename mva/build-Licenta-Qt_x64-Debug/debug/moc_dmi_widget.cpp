/****************************************************************************
** Meta object code from reading C++ file 'dmi_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Licenta/gui/headers/dmi_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dmi_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CDMIWidget_t {
    QByteArrayData data[12];
    char stringdata[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CDMIWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CDMIWidget_t qt_meta_stringdata_CDMIWidget = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 24),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 13),
QT_MOC_LITERAL(4, 51, 12),
QT_MOC_LITERAL(5, 64, 8),
QT_MOC_LITERAL(6, 73, 19),
QT_MOC_LITERAL(7, 93, 19),
QT_MOC_LITERAL(8, 113, 6),
QT_MOC_LITERAL(9, 120, 24),
QT_MOC_LITERAL(10, 145, 18),
QT_MOC_LITERAL(11, 164, 5)
    },
    "CDMIWidget\0OnRequestPropertiesModel\0"
    "\0DMIModuleType\0OnShowWidget\0QWidget*\0"
    "OnSetItemsTreeModel\0QStandardItemModel*\0"
    "pModel\0OnSetPropertiesTreeModel\0"
    "OnItemsTreeClicked\0index\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CDMIWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x05,
       4,    1,   42,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       6,    1,   45,    2, 0x08,
       9,    1,   48,    2, 0x08,
      10,    1,   51,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QModelIndex,   11,

       0        // eod
};

void CDMIWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CDMIWidget *_t = static_cast<CDMIWidget *>(_o);
        switch (_id) {
        case 0: _t->OnRequestPropertiesModel((*reinterpret_cast< DMIModuleType(*)>(_a[1]))); break;
        case 1: _t->OnShowWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 2: _t->OnSetItemsTreeModel((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        case 3: _t->OnSetPropertiesTreeModel((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        case 4: _t->OnItemsTreeClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStandardItemModel* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStandardItemModel* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CDMIWidget::*_t)(DMIModuleType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CDMIWidget::OnRequestPropertiesModel)) {
                *result = 0;
            }
        }
        {
            typedef void (CDMIWidget::*_t)(QWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CDMIWidget::OnShowWidget)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CDMIWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CDMIWidget.data,
      qt_meta_data_CDMIWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *CDMIWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CDMIWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDMIWidget.stringdata))
        return static_cast<void*>(const_cast< CDMIWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CDMIWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CDMIWidget::OnRequestPropertiesModel(DMIModuleType _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CDMIWidget::OnShowWidget(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
