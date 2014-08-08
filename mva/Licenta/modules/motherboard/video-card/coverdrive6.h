#ifndef COVERDRIVE6_H
#define COVERDRIVE6_H

#include "ioverdrive.h"

class COverdrive6: public IOverdrive
{
private:
    HINSTANCE m_hDll;
    double m_pTemp[10];
    double m_pFanRPM[10];
    double m_pFanPerc[10];

    ADLOD6ThermalControllerCaps* m_pThermalControllerCaps;

    ADL_OVERDRIVE6_FANSPEED_GET ADL_Overdrive6_FanSpeed_Get;
    ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS ADL_Overdrive6_ThermalController_Caps;
    ADL_OVERDRIVE6_TEMPERATURE_GET ADL_Overdrive6_Temperature_Get;
    ADL_OVERDRIVE6_CAPABILITIES_GET ADL_Overdrive6_Capabilities_Get;
    ADL_OVERDRIVE6_STATEINFO_GET ADL_Overdrive6_StateInfo_Get;
    ADL_OVERDRIVE6_CURRENTSTATUS_GET ADL_Overdrive6_CurrentStatus_Get;
    ADL_OVERDRIVE6_POWERCONTROL_CAPS ADL_Overdrive6_PowerControl_Caps;
    ADL_OVERDRIVE6_POWERCONTROLINFO_GET ADL_Overdrive6_PowerControlInfo_Get;
    ADL_OVERDRIVE6_POWERCONTROL_GET ADL_Overdrive6_PowerControl_Get;
    ADL_OVERDRIVE6_FANSPEED_SET ADL_Overdrive6_FanSpeed_Set;
    ADL_OVERDRIVE6_STATE_SET ADL_Overdrive6_State_Set;
    ADL_OVERDRIVE6_POWERCONTROL_SET ADL_Overdrive6_PowerControl_Set;

public:
    explicit COverdrive6(HINSTANCE hDll);
    virtual ~COverdrive6();

    virtual double *OnGetTemperatures(int nAdapterIndex);
    virtual double *OnGetFanSpeedsRPM(int nAdapterIndex);
    virtual double *OnGetFanSpeedsPercentage(int nAdapterIndex);
    virtual double OnGetEngineClock(int nAdapterIndex);
    virtual double OnGetMemoryClock(int nAdapterIndex);
    virtual double OnGetCoreVoltage(int nAdapterIndex);
};

#endif // COVERDRIVE6_H
