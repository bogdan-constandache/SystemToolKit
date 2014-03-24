#ifndef IT87_H
#define IT87_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"

#define VENDOR_ID                       0x90
#define ADDRESS_REGISTER_OFFSET         0x05
#define DATA_REGISTER_OFFSET            0x06
#define CONFIGURATION_REGISTER          0x00
#define TEMP_BASE_REGISTER              0x29
#define VENDOR_ID_REGISTER              0x58
#define FAN_SPEED_DIVISOR_REGISTER      0x0B
#define VOLTAGE_BASE_REGISTER           0x20

static BYTE FAN_SPEED_REG[] = {0x0D, 0x0E, 0x0F, 0x80, 0x82};
static BYTE FAN_SPEED_EXT_REG[] = {0x18, 0x19, 0x1A, 0x81, 0x83};

class CIT87 : public ISensor
{
    USHORT usAddress;
    USHORT usAddressReg;
    USHORT usDataReg;
    Chip eChip;
    CSTKDriverWrapper *m_pDriver;

    double m_pVoltages[9];
    double m_pTemps[3];
    double m_pFans[5];

    double dVoltageGain;

    bool bHas16BitFanCounter;

public:
    CIT87(Chip, USHORT);
    virtual ~CIT87();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double* GetTemps();
    virtual double* GetFanSpeeds();
    virtual double* GetVoltages();
    virtual QString GetChipName();
};


#endif // IT87_H
