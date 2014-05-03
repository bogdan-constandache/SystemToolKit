#ifndef AMD10_TEMPERATURE_H
#define AMD10_TEMPERATURE_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"


class CAmd10CpuSensor : public ICPUSensor
{
private:
    CSTKDriverWrapper *m_pDriver;

public:
    CAmd10CpuSensor();
    virtual ~CAmd10CpuSensor();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double *GetTemps();
    virtual QString GetCpuName();
    virtual QString GetMicroArchitecture();
    virtual int GetNumberOfCores();
};


#endif // AMD10_TEMPERATURE_H
