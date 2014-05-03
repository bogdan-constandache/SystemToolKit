#ifndef FACTORY_BOARD_SENSOR_H
#define FACTORY_BOARD_SENSOR_H

#include "it87.h"
#include "w836xx.h"

#define BASE_ADDRESS_REGISTER 0x60

#define IT87_REGISTER_PORT 0x2E
#define IT87_VALUE_PORT 0x2F
#define IT87_CHIP_VERSION_REGISTER 0x22
#define IT87_ENVIRONMENT_CONTROLLER 0x04

#define W836_REGISTER_PORT 0x4E
#define W836_VALUE_PORT 0x4F

class FactoryBoardSensor
{
private:
    CSTKDriverWrapper *m_pDriver;
    ISensor *m_pBoardSensor;

    USHORT ReadWordFromPort(BYTE bRegisterPort, BYTE bValuePort, BYTE bRegister);
    BYTE ReadByteFromPort(BYTE bRegisterPort, BYTE bValuePort, BYTE bRegister);

    bool DetectIT87XXSensor();
    bool DetectW836XXSensor();

public:
    FactoryBoardSensor();
    ~FactoryBoardSensor();

    ISensor *GetBoardSensor();
    int DestroySensor();
};

#endif // FACTORY_BOARD_SENSOR_H
