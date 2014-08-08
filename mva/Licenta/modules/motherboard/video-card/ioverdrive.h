#ifndef IOVERDRIVE_H
#define IOVERDRIVE_H

#include <Windows.h>
#include "include/adl_sdk.h"

#define AMDVENDORID             (1002)
#define ADL_WARNING_NO_DATA      -100

// Definitions of the used function pointers. Add more if you use other ADL APIs
typedef int ( *ADL_MAIN_CONTROL_CREATE )(ADL_MAIN_MALLOC_CALLBACK, int );
typedef int ( *ADL_MAIN_CONTROL_DESTROY )();
typedef int ( *ADL_ADAPTER_NUMBEROFADAPTERS_GET ) ( int* );
typedef int ( *ADL_ADAPTER_ADAPTERINFO_GET ) ( LPAdapterInfo, int );
typedef int ( *ADL_ADAPTER_ACTIVE_GET ) ( int, int* );
typedef int ( *ADL_OVERDRIVE_CAPS ) (int iAdapterIndex, int *iSupported, int *iEnabled, int *iVersion);
typedef int ( *ADL_OVERDRIVE5_THERMALDEVICES_ENUM ) (int iAdapterIndex, int iThermalControllerIndex, ADLThermalControllerInfo *lpThermalControllerInfo);
typedef int ( *ADL_OVERDRIVE5_ODPARAMETERS_GET ) ( int  iAdapterIndex,  ADLODParameters *  lpOdParameters );
typedef int ( *ADL_OVERDRIVE5_TEMPERATURE_GET ) (int iAdapterIndex, int iThermalControllerIndex, ADLTemperature *lpTemperature);
typedef int ( *ADL_OVERDRIVE5_FANSPEED_GET ) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int ( *ADL_OVERDRIVE5_FANSPEEDINFO_GET ) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedInfo *lpFanSpeedInfo);
typedef int ( *ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET ) (int iAdapterIndex, int iDefault, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int ( *ADL_OVERDRIVE5_ODPARAMETERS_GET ) (int iAdapterIndex, ADLODParameters *lpOdParameters);
typedef int ( *ADL_OVERDRIVE5_CURRENTACTIVITY_GET ) (int iAdapterIndex, ADLPMActivity *lpActivity);
typedef int	( *ADL_OVERDRIVE5_FANSPEED_SET )(int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int ( *ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET ) (int iAdapterIndex, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int ( *ADL_OVERDRIVE5_POWERCONTROL_CAPS )(int iAdapterIndex,  int *lpSupported);
typedef int ( *ADL_OVERDRIVE5_POWERCONTROLINFO_GET )(int iAdapterIndex, ADLPowerControlInfo *lpPowerControlInfo);
typedef int ( *ADL_OVERDRIVE5_POWERCONTROL_GET )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int ( *ADL_OVERDRIVE5_POWERCONTROL_SET )(int iAdapterIndex, int iValue);
typedef int ( *ADL_OVERDRIVE6_FANSPEED_GET )(int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo);
typedef int ( *ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS )(int iAdapterIndex, ADLOD6ThermalControllerCaps *lpThermalControllerCaps);
typedef int ( *ADL_OVERDRIVE6_TEMPERATURE_GET )(int iAdapterIndex, int *lpTemperature);
typedef int ( *ADL_OVERDRIVE6_CAPABILITIES_GET ) (int iAdapterIndex, ADLOD6Capabilities *lpODCapabilities);
typedef int ( *ADL_OVERDRIVE6_STATEINFO_GET )(int iAdapterIndex, int iStateType, ADLOD6StateInfo *lpStateInfo);
typedef int	( *ADL_OVERDRIVE6_CURRENTSTATUS_GET )(int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus);
typedef int ( *ADL_OVERDRIVE6_POWERCONTROL_CAPS ) (int iAdapterIndex, int *lpSupported);
typedef int ( *ADL_OVERDRIVE6_POWERCONTROLINFO_GET )(int iAdapterIndex, ADLOD6PowerControlInfo *lpPowerControlInfo);
typedef int ( *ADL_OVERDRIVE6_POWERCONTROL_GET )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int ( *ADL_OVERDRIVE6_FANSPEED_SET )(int iAdapterIndex, ADLOD6FanSpeedValue *lpFanSpeedValue);
typedef int ( *ADL_OVERDRIVE6_STATE_SET )(int iAdapterIndex, int iStateType, ADLOD6StateInfo *lpStateInfo);
typedef int ( *ADL_OVERDRIVE6_POWERCONTROL_SET )(int iAdapterIndex, int iValue);

class IOverdrive
{
public:
    virtual ~IOverdrive() {}

    virtual double *OnGetTemperatures(int nAdapterIndex) = 0;
    virtual double *OnGetFanSpeedsRPM(int nAdapterIndex) = 0;
    virtual double *OnGetFanSpeedsPercentage(int nAdapterIndex) = 0;
    virtual double OnGetEngineClock(int nAdapterIndex) = 0;
    virtual double OnGetMemoryClock(int nAdapterIndex) = 0;
    virtual double OnGetCoreVoltage(int nAdapterIndex) = 0;
};

#endif // IOVERDRIVE_H
