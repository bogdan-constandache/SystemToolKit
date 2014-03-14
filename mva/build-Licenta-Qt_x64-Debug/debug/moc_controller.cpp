/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Licenta/controller/controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Controller_t {
    QByteArrayData data[24];
    char stringdata[631];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Controller_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Controller_t qt_meta_stringdata_Controller = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 15),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 27),
QT_MOC_LITERAL(4, 56, 39),
QT_MOC_LITERAL(5, 96, 30),
QT_MOC_LITERAL(6, 127, 31),
QT_MOC_LITERAL(7, 159, 25),
QT_MOC_LITERAL(8, 185, 29),
QT_MOC_LITERAL(9, 215, 26),
QT_MOC_LITERAL(10, 242, 28),
QT_MOC_LITERAL(11, 271, 22),
QT_MOC_LITERAL(12, 294, 34),
QT_MOC_LITERAL(13, 329, 35),
QT_MOC_LITERAL(14, 365, 33),
QT_MOC_LITERAL(15, 399, 30),
QT_MOC_LITERAL(16, 430, 19),
QT_MOC_LITERAL(17, 450, 21),
QT_MOC_LITERAL(18, 472, 26),
QT_MOC_LITERAL(19, 499, 13),
QT_MOC_LITERAL(20, 513, 26),
QT_MOC_LITERAL(21, 540, 24),
QT_MOC_LITERAL(22, 565, 26),
QT_MOC_LITERAL(23, 592, 37)
    },
    "Controller\0StartController\0\0"
    "OnComputerDMIOptClickedSlot\0"
    "OnComputerPowerManagementOptClickedSlot\0"
    "OnHddInformationOptClickedSlot\0"
    "OnOperatingSystemOptClickedSlot\0"
    "OnProcessesOptClickedSlot\0"
    "OnSystemDriversOptClickedSlot\0"
    "OnStorageATAOptClickedSlot\0"
    "OnStorageSmartOptClickedSlot\0"
    "OnSmbiosOptClickedSlot\0"
    "OnApplicationManagerOptClickedSlot\0"
    "OnStartupApplicationsOptClickedSlot\0"
    "OnActiveConnectionsOptClickedSlot\0"
    "OnNetworkDevicesOptClickedSlot\0"
    "OnCPUOptClickedSlot\0OnCPUIDOptClickedSlot\0"
    "OnRequestDMIItemProperties\0DMIModuleType\0"
    "OnRequestATAItemProperties\0"
    "OnRequestSMARTProperties\0"
    "OnUninstallApplicationSlot\0"
    "OnRequestNetworkDeviceInfomationsSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x0a,
       3,    0,  120,    2, 0x0a,
       4,    0,  121,    2, 0x0a,
       5,    0,  122,    2, 0x0a,
       6,    0,  123,    2, 0x0a,
       7,    0,  124,    2, 0x0a,
       8,    0,  125,    2, 0x0a,
       9,    0,  126,    2, 0x0a,
      10,    0,  127,    2, 0x0a,
      11,    0,  128,    2, 0x0a,
      12,    0,  129,    2, 0x0a,
      13,    0,  130,    2, 0x0a,
      14,    0,  131,    2, 0x0a,
      15,    0,  132,    2, 0x0a,
      16,    0,  133,    2, 0x0a,
      17,    0,  134,    2, 0x0a,
      18,    1,  135,    2, 0x0a,
      20,    1,  138,    2, 0x0a,
      21,    1,  141,    2, 0x0a,
      22,    0,  144,    2, 0x0a,
      23,    1,  145,    2, 0x0a,

 // slots: parameters
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Controller *_t = static_cast<Controller *>(_o);
        switch (_id) {
        case 0: _t->StartController(); break;
        case 1: _t->OnComputerDMIOptClickedSlot(); break;
        case 2: _t->OnComputerPowerManagementOptClickedSlot(); break;
        case 3: _t->OnHddInformationOptClickedSlot(); break;
        case 4: _t->OnOperatingSystemOptClickedSlot(); break;
        case 5: _t->OnProcessesOptClickedSlot(); break;
        case 6: _t->OnSystemDriversOptClickedSlot(); break;
        case 7: _t->OnStorageATAOptClickedSlot(); break;
        case 8: _t->OnStorageSmartOptClickedSlot(); break;
        case 9: _t->OnSmbiosOptClickedSlot(); break;
        case 10: _t->OnApplicationManagerOptClickedSlot(); break;
        case 11: _t->OnStartupApplicationsOptClickedSlot(); break;
        case 12: _t->OnActiveConnectionsOptClickedSlot(); break;
        case 13: _t->OnNetworkDevicesOptClickedSlot(); break;
        case 14: _t->OnCPUOptClickedSlot(); break;
        case 15: _t->OnCPUIDOptClickedSlot(); break;
        case 16: _t->OnRequestDMIItemProperties((*reinterpret_cast< DMIModuleType(*)>(_a[1]))); break;
        case 17: _t->OnRequestATAItemProperties((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->OnRequestSMARTProperties((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->OnUninstallApplicationSlot(); break;
        case 20: _t->OnRequestNetworkDeviceInfomationsSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Controller::staticMetaObject = {
    { &AbstractController::staticMetaObject, qt_meta_stringdata_Controller.data,
      qt_meta_data_Controller,  qt_static_metacall, 0, 0}
};


const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Controller.stringdata))
        return static_cast<void*>(const_cast< Controller*>(this));
    return AbstractController::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
