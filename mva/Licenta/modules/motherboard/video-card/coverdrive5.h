#ifndef COVERDRIVE5_H
#define COVERDRIVE5_H

#include "ioverdrive.h"

class COverdrive5 : public IOverdrive
{
private:
    HINSTANCE m_hDll;
    double m_pTemp[10];
    double m_pFanRPM[10];
    double m_pFanPerc[10];

    ADL_OVERDRIVE5_THERMALDEVICES_ENUM		ADL_Overdrive5_ThermalDevices_Enum;
    ADL_OVERDRIVE5_ODPARAMETERS_GET			ADL_Overdrive5_ODParameters_Get;
    ADL_OVERDRIVE5_TEMPERATURE_GET			ADL_Overdrive5_Temperature_Get;
    ADL_OVERDRIVE5_FANSPEED_GET				ADL_Overdrive5_FanSpeed_Get;
    ADL_OVERDRIVE5_FANSPEEDINFO_GET			ADL_Overdrive5_FanSpeedInfo_Get;
    ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET	ADL_Overdrive5_ODPerformanceLevels_Get;
    ADL_OVERDRIVE5_CURRENTACTIVITY_GET		ADL_Overdrive5_CurrentActivity_Get;
    ADL_OVERDRIVE5_FANSPEED_SET				ADL_Overdrive5_FanSpeed_Set;
    ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET  ADL_Overdrive5_ODPerformanceLevels_Set;
    ADL_OVERDRIVE5_POWERCONTROL_CAPS		ADL_Overdrive5_PowerControl_Caps;
    ADL_OVERDRIVE5_POWERCONTROLINFO_GET		ADL_Overdrive5_PowerControlInfo_Get;
    ADL_OVERDRIVE5_POWERCONTROL_GET			ADL_Overdrive5_PowerControl_Get;
    ADL_OVERDRIVE5_POWERCONTROL_SET			ADL_Overdrive5_PowerControl_Set;
    ADL_OVERDRIVE6_STATE_SET				ADL_Overdrive6_State_Set;

public:
    explicit COverdrive5(HINSTANCE hDll);
    virtual ~COverdrive5();

    virtual double *OnGetTemperatures(int nAdapterIndex);
    virtual double *OnGetFanSpeedsRPM(int nAdapterIndex);
    virtual double *OnGetFanSpeedsPercentage(int nAdapterIndex);
    virtual double OnGetEngineClock(int nAdapterIndex);
    virtual double OnGetMemoryClock(int nAdapterIndex);
    virtual double OnGetCoreVoltage(int nAdapterIndex);
};

#endif // COVERDRIVE5_H
