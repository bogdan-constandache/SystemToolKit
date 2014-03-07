/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Licenta/gui/headers/main_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata[550];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 29),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 41),
QT_MOC_LITERAL(4, 84, 32),
QT_MOC_LITERAL(5, 117, 33),
QT_MOC_LITERAL(6, 151, 27),
QT_MOC_LITERAL(7, 179, 31),
QT_MOC_LITERAL(8, 211, 28),
QT_MOC_LITERAL(9, 240, 30),
QT_MOC_LITERAL(10, 271, 24),
QT_MOC_LITERAL(11, 296, 36),
QT_MOC_LITERAL(12, 333, 37),
QT_MOC_LITERAL(13, 371, 35),
QT_MOC_LITERAL(14, 407, 32),
QT_MOC_LITERAL(15, 440, 22),
QT_MOC_LITERAL(16, 463, 5),
QT_MOC_LITERAL(17, 469, 22),
QT_MOC_LITERAL(18, 492, 19),
QT_MOC_LITERAL(19, 512, 6),
QT_MOC_LITERAL(20, 519, 12),
QT_MOC_LITERAL(21, 532, 8),
QT_MOC_LITERAL(22, 541, 7)
    },
    "MainWindow\0OnComputerDMIOptClickedSignal\0"
    "\0OnComputerPowerManagementOptClickedSignal\0"
    "OnHddInformationOptClickedSignal\0"
    "OnOperatingSystemOptClickedSignal\0"
    "OnProcessesOptClickedSignal\0"
    "OnSystemDriversOptClickedSignal\0"
    "OnStorageATAOptClickedSignal\0"
    "OnStorageSmartOptClickedSignal\0"
    "OnSmbiosOptClickedSignal\0"
    "OnApplicationManagerOptClickedSignal\0"
    "OnStartupApplicationsOptClickedSignal\0"
    "OnActiveConnectionsOptClickedSignal\0"
    "OnNetworkDevicesOptClickedSignal\0"
    "OnItemsTreeClickedSlot\0index\0"
    "OnPopulateMenuTreeSlot\0QStandardItemModel*\0"
    "pModel\0OnShowWidget\0QWidget*\0pWidget\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x05,
       3,    0,   95,    2, 0x05,
       4,    0,   96,    2, 0x05,
       5,    0,   97,    2, 0x05,
       6,    0,   98,    2, 0x05,
       7,    0,   99,    2, 0x05,
       8,    0,  100,    2, 0x05,
       9,    0,  101,    2, 0x05,
      10,    0,  102,    2, 0x05,
      11,    0,  103,    2, 0x05,
      12,    0,  104,    2, 0x05,
      13,    0,  105,    2, 0x05,
      14,    0,  106,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
      15,    1,  107,    2, 0x08,
      17,    1,  110,    2, 0x08,
      20,    1,  113,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,   16,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->OnComputerDMIOptClickedSignal(); break;
        case 1: _t->OnComputerPowerManagementOptClickedSignal(); break;
        case 2: _t->OnHddInformationOptClickedSignal(); break;
        case 3: _t->OnOperatingSystemOptClickedSignal(); break;
        case 4: _t->OnProcessesOptClickedSignal(); break;
        case 5: _t->OnSystemDriversOptClickedSignal(); break;
        case 6: _t->OnStorageATAOptClickedSignal(); break;
        case 7: _t->OnStorageSmartOptClickedSignal(); break;
        case 8: _t->OnSmbiosOptClickedSignal(); break;
        case 9: _t->OnApplicationManagerOptClickedSignal(); break;
        case 10: _t->OnStartupApplicationsOptClickedSignal(); break;
        case 11: _t->OnActiveConnectionsOptClickedSignal(); break;
        case 12: _t->OnNetworkDevicesOptClickedSignal(); break;
        case 13: _t->OnItemsTreeClickedSlot((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 14: _t->OnPopulateMenuTreeSlot((*reinterpret_cast< QStandardItemModel*(*)>(_a[1]))); break;
        case 15: _t->OnShowWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QStandardItemModel* >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnComputerDMIOptClickedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnComputerPowerManagementOptClickedSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnHddInformationOptClickedSignal)) {
                *result = 2;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnOperatingSystemOptClickedSignal)) {
                *result = 3;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnProcessesOptClickedSignal)) {
                *result = 4;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnSystemDriversOptClickedSignal)) {
                *result = 5;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnStorageATAOptClickedSignal)) {
                *result = 6;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnStorageSmartOptClickedSignal)) {
                *result = 7;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnSmbiosOptClickedSignal)) {
                *result = 8;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnApplicationManagerOptClickedSignal)) {
                *result = 9;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnStartupApplicationsOptClickedSignal)) {
                *result = 10;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnActiveConnectionsOptClickedSignal)) {
                *result = 11;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::OnNetworkDevicesOptClickedSignal)) {
                *result = 12;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::OnComputerDMIOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MainWindow::OnComputerPowerManagementOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MainWindow::OnHddInformationOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MainWindow::OnOperatingSystemOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void MainWindow::OnProcessesOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MainWindow::OnSystemDriversOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void MainWindow::OnStorageATAOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void MainWindow::OnStorageSmartOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void MainWindow::OnSmbiosOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void MainWindow::OnApplicationManagerOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void MainWindow::OnStartupApplicationsOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void MainWindow::OnActiveConnectionsOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void MainWindow::OnNetworkDevicesOptClickedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}
QT_END_MOC_NAMESPACE
