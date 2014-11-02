#ifndef AMD10_TEMPERATURE_H
#define AMD10_TEMPERATURE_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"

#define FAMILY_10H_CONTROL_DEVICE_ID 0x1203
#define FAMILY_11H_CONTROL_DEVICE_ID 0x1303
#define FAMILY_12H_CONTROL_DEVICE_ID 0x1703
#define FAMILY_14H_CONTROL_DEVICE_ID 0x1703
#define FAMILY_15H_MODEL_00_CONTROL_DEVICE_ID 0x1603
#define FAMILY_15H_MODEL_10_CONTROL_DEVICE_ID 0x1403
#define FAMILY_16H_MODEL_00__CONTROL_DEVICE_ID 0x1533

#define CONTROL_FUNCTION 3

#define REPORTED_TEMPERATURE_CONTROL_REGISTER 0xA4

class CAmd10CpuSensor : public ICPUSensor
{
private:
    CSTKDriverWrapper *m_pDriver;

    double *m_pTemps;

    int m_nFamily, m_nModel, m_nStepping, m_nCoreCount;
    int m_nMultiplier;
    double m_dOffset;
    BYTE m_bThermalCoreSelectionCPU0, m_bThermalCoreSelectionCPU1;
    AMDMicroArchitecture m_eMicroArch;

    USHORT m_usControlDeviceID;
    UINT m_uiControlAddress;

    UINT GetControllAddress(int nFunction, USHORT usDeviceID);

public:
    explicit CAmd10CpuSensor(int nFamily, int nModel, int nStepping, int nCoreCount);
    virtual ~CAmd10CpuSensor();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double *GetTemps();
    virtual QString GetCpuName();
    virtual QString GetMicroArchitecture();
    virtual int GetNumberOfCores();
    virtual int GetMultiplier();
};


#endif // AMD10_TEMPERATURE_H
