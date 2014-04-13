#ifndef W836XX_H
#define W836XX_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"

class CW836XX : public ISensor
{
private:
    USHORT usAddress;
    USHORT usAddressReg;
    USHORT usDataReg;

    Chip m_eChip;

    CSTKDriverWrapper *m_pDriver;

public:
    CW836XX(Chip, USHORT);
    virtual ~CW836XX();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double* GetTemps();
    virtual double* GetFanSpeeds();
    virtual double* GetVoltages();
    virtual QString GetChipName();
};

#endif // W836XX_H
