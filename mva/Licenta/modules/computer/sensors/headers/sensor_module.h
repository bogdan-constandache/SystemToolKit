#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "it87.h"
#include "hardware_usage.h"

#define BASE_ADDRESS_REGISTER 0x60

#define IT87_REGISTER_PORT 0x2E
#define IT87_VALUE_PORT 0x2F
#define IT87_CHIP_VERSION_REGISTER 0x22
#define IT87_ENVIRONMENT_CONTROLLER 0x04

class CSensorModule
{
private:
    ISensor *m_pSensor;
    CSTKDriverWrapper *m_pDriver;
    Chip m_Chip;

    CHardwareUsage *m_pHardwareUsage;

    bool DetectIT87XXSensor();
    USHORT ReadWordFromPort(BYTE bRegister);

public:
    CSensorModule();
    ~CSensorModule();

    ISensor *DetectSensor();
    int DestroySensor();

    double GetCpuLoad();
    QString GetCpuName();
    double GetDiskLoad();
    MemoryStatus* GetMemoryStat();
};

#endif // SENSOR_MODULE_H
