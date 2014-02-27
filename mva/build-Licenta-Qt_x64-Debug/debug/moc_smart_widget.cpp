/****************************************************************************
** Meta object code from reading C++ file 'smart_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Licenta/gui/headers/smart_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smart_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CSmartWidget_t {
    QByteArrayData data[9];
    char stringdata[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CSmartWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CSmartWidget_t qt_meta_stringdata_CSmartWidget = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 12),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 8),
QT_MOC_LITERAL(4, 36, 24),
QT_MOC_LITERAL(5, 61, 22),
QT_MOC_LITERAL(6, 84, 19),
QT_MOC_LITERAL(7, 104, 24),
QT_MOC_LITERAL(8, 129, 18)
    },
    "CSmartWidget\0OnShowWidget\0\0QWidget*\0"
    "OnRequestHDDInformations\0"
    "OnSetHDDItemsTreeModel\0QStandardItemModel*\0"
    "OnSetPropertiesTreeModel\0OnItemsTreeClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSmartWidget[] = {

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
       8,    1,   51,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::QModelIndex,    2,

       0        // eod
};

void CSmartWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSmartWidget *_t = static_cast<CSmartWidget *>(_o);
        switch (_id) {
        case 0: _t->OnShowWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 1: _t->OnRequestHDDInformations((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->OnSetHDDItemsTreeModel((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        case 3: _t->OnSetPropertiesTreeModel((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        case 4: _t->OnItemsTreeClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
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
            typedef void (CSmartWidget::*_t)(QWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSmartWidget::OnShowWidget)) {
                *result = 0;
            }
        }
        {
            typedef void (CSmartWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSmartWidget::OnRequestHDDInformations)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CSmartWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSmartWidget.data,
      qt_meta_data_CSmartWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *CSmartWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSmartWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSmartWidget.stringdata))
        return static_cast<void*>(const_cast< CSmartWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSmartWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CSmartWidget::OnShowWidget(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSmartWidget::OnRequestHDDInformations(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
