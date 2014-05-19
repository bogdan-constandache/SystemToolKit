#ifndef IT87_H
#define IT87_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"

#define IT87_CHIP_VENDOR_ID                       0x90
#define IT87_CHIP_ADDRESS_REGISTER_OFFSET         0x05
#define IT87_CHIP_DATA_REGISTER_OFFSET            0x06
#define IT87_CHIP_CONFIGURATION_REGISTER          0x00
#define IT87_CHIP_TEMP_BASE_REGISTER              0x29
#define IT87_CHIP_VENDOR_ID_REGISTER              0x58
#define IT87_CHIP_FAN_SPEED_DIVISOR_REGISTER      0x0B
#define IT87_CHIP_VOLTAGE_BASE_REGISTER           0x20

#define IT87_CHIP_VCORE_REGISTER                  0x20
#define IT87_CHIP_DDR_REGISTER                    0x26
#define IT87_CHIP_3VSB_REGISTER                   0x27
#define IT87_CHIP_VBAT_REGISTER                   0x28


class CIT87 : public ISensor
{
    USHORT usAddress;
    USHORT usAddressReg;
    USHORT usDataReg;
    Chip eChip;
    CSTKDriverWrapper *m_pDriver;

    VoltageReading m_pVoltages[4];
    double m_pTemps[3];
    double m_pFans[5];

    double dVoltageGain;

    bool bHas16BitFanCounter;

    BYTE *m_pFanSpeedReg;
    BYTE *m_pFanSpeedExtReg;

public:
    CIT87(Chip, USHORT);
    virtual ~CIT87();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double* GetTemps();
    virtual double* GetFanSpeeds();
    virtual VoltageReading* GetVoltages();
    virtual QString GetChipName();
};


#endif // IT87_H
