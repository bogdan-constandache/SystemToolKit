#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "it87.h"

#define IT87_REGISTER_PORT 0x2E
#define IT87_VALUE_PORT 0x2F
#define IT87_CHIP_VERSION_REGISTER 0x22

class CSensorModule
{
private:
    ISensor *m_pSensor;
    CSTKDriverWrapper *m_pDriver;
    Chip m_Chip;

public:
    CSensorModule();
    ~CSensorModule();

    ISensor *DetectSensor();
    int DestroySensor();
};

#endif // SENSOR_MODULE_H
