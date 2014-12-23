#ifndef F718XX_H
#define F718XX_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"

#define F718_CHIP_ADDRESS_REGISTER_OFFSET       0x05
#define F718_CHIP_DATA_REGISTER_OFFSET          0x06
#define F718_CHIP_VOLTAGE_BASE_REGISTER         0x20
#define F718_CHIP_TEMPERATURE_CONFIG_REGISTER   0x69
#define F718_CHIP_TEMPERATURE_BASE_REGISTER     0x70


class CF718 : public ISensor
{
private:
    USHORT usAddress;
    Chip eChip;
    CRing0Wrapper *m_pDriver;

    VoltageReading m_pVoltages[9];
    double m_pTemps[3];
    double m_pFans[5];

    int m_nVoltageCount, m_nTempsCount, m_nFansCount;

    double dVoltageGain;

    bool bHas16BitFanCounter;

    BYTE *m_pFanSpeedReg;

    int ReadF718Byte(BYTE bRegister, BYTE *bOut);

public:
    CF718(Chip eChip, USHORT usAddress);
    virtual ~CF718();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double* GetTemps();
    virtual double* GetFanSpeeds();
    virtual VoltageReading* GetVoltages();
    virtual QString GetChipName();
};

#endif // F718XX_H
