/****************************************************************************
** Meta object code from reading C++ file 'network_devices_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Licenta/gui/headers/network_devices_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'network_devices_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CNetworkDevicesWidget_t {
    QByteArrayData data[11];
    char stringdata[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CNetworkDevicesWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CNetworkDevicesWidget_t qt_meta_stringdata_CNetworkDevicesWidget = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 12),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 8),
QT_MOC_LITERAL(4, 45, 39),
QT_MOC_LITERAL(5, 85, 29),
QT_MOC_LITERAL(6, 115, 5),
QT_MOC_LITERAL(7, 121, 26),
QT_MOC_LITERAL(8, 148, 19),
QT_MOC_LITERAL(9, 168, 6),
QT_MOC_LITERAL(10, 175, 39)
    },
    "CNetworkDevicesWidget\0OnShowWidget\0\0"
    "QWidget*\0OnRequestNetworkDeviceInformationSignal\0"
    "OnNetworkNamesItemClickedSlot\0Index\0"
    "OnSetNetworkNamesTreeModel\0"
    "QStandardItemModel*\0pModel\0"
    "OnSetNetworkDeviceInformationsTreeModel\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CNetworkDevicesWidget[] = {

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
       5,    1,   45,    2, 0x08,
       7,    1,   48,    2, 0x08,
      10,    1,   51,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void CNetworkDevicesWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CNetworkDevicesWidget *_t = static_cast<CNetworkDevicesWidget *>(_o);
        switch (_id) {
        case 0: _t->OnShowWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 1: _t->OnRequestNetworkDeviceInformationSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->OnNetworkNamesItemClickedSlot((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 3: _t->OnSetNetworkNamesTreeModel((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        case 4: _t->OnSetNetworkDeviceInformationsTreeModel((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStandardItemModel* >(); break;
            }
            break;
        case 4:
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
            typedef void (CNetworkDevicesWidget::*_t)(QWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CNetworkDevicesWidget::OnShowWidget)) {
                *result = 0;
            }
        }
        {
            typedef void (CNetworkDevicesWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CNetworkDevicesWidget::OnRequestNetworkDeviceInformationSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CNetworkDevicesWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CNetworkDevicesWidget.data,
      qt_meta_data_CNetworkDevicesWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *CNetworkDevicesWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CNetworkDevicesWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CNetworkDevicesWidget.stringdata))
        return static_cast<void*>(const_cast< CNetworkDevicesWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CNetworkDevicesWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CNetworkDevicesWidget::OnShowWidget(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CNetworkDevicesWidget::OnRequestNetworkDeviceInformationSignal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
