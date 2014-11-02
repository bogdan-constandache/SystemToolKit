#ifndef INTEL_TEMPERATURE_H
#define INTEL_TEMPERATURE_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"


#define IA32_PERF_STATUS_MSR        0x0198
#define MSR_PLATFORM_INFO           0xCE
#define IA32_THERM_STATUS_MSR       0x019C
#define IA32_TEMPERATURE_TARGET     0x01A2


class CIntelCpuSensor : public ICPUSensor
{
private:
    CSTKDriverWrapper *m_pDriver;

    double *m_pTemps;
    double *m_pTjMax;

    IntelMicroArchitecture m_eMicroArch;
    int m_nFamily, m_nModel, m_nStepping, m_nCoreCount;
    int m_nMultiplier;
    QString m_qzCpuName;

    void InitializeTjMaxTable(int nValue);
    void InitializeTjMaxTableFromMSR();

public:
    CIntelCpuSensor(int nFamily, int nModel, int nStepping, int nCoreCount);
    virtual ~CIntelCpuSensor();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double *GetTemps();
    virtual QString GetCpuName();
    virtual QString GetMicroArchitecture();
    virtual int GetNumberOfCores();
    virtual int GetMultiplier();
};

#endif // INTEL_TEMPERATURE_H
