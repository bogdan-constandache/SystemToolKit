#ifndef AMD0F_TEMPERATURE_H
#define AMD0F_TEMPERATURE_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"

#define AMD0F_DEVICE_ID 0x1103
#define AMD0F_THERM_STATUS_REGISTER 0xE4

class CAmd0FCpuSensor : public ICPUSensor
{
private:
    CSTKDriverWrapper *m_pDriver;

    double *m_pTemps;

    int m_nFamily, m_nModel, m_nStepping, m_nCoreCount;
    int m_nMultiplier;
    double m_dOffset;
    BYTE m_bThermalCoreSelectionCPU0, m_bThermalCoreSelectionCPU1;
    AMDMicroArchitecture m_eMicroArch;

    UINT GetControllAddress(int nFunction);

public:
    CAmd0FCpuSensor(int nFamily, int nModel, int nStepping, int nCoreCount);
    virtual ~CAmd0FCpuSensor();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double *GetTemps();
    virtual QString GetCpuName();
    virtual QString GetMicroArchitecture();
    virtual int GetNumberOfCores();
    virtual int GetMultiplier();
};

#endif // AMD0F_TEMPERATURE_H
