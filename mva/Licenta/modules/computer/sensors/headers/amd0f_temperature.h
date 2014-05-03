#ifndef AMD0F_TEMPERATURE_H
#define AMD0F_TEMPERATURE_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"


class CAmd0FCpuSensor : public ICPUSensor
{
private:
    CSTKDriverWrapper *m_pDriver;

public:
    CAmd0FCpuSensor();
    virtual ~CAmd0FCpuSensor();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double *GetTemps();
    virtual QString GetCpuName();
    virtual QString GetMicroArchitecture();
    virtual int GetNumberOfCores();
};

#endif // AMD0F_TEMPERATURE_H
