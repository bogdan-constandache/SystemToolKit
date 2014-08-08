#include "coverdrive6.h"

COverdrive6::COverdrive6(HINSTANCE hDll): m_hDll(NULL)
{
    m_hDll = hDll;
    ZeroMemory(m_pTemp, sizeof(m_pTemp));
    ZeroMemory(m_pFanRPM, sizeof(m_pFanRPM));
    ZeroMemory(m_pFanPerc, sizeof(m_pFanPerc));

    ADL_Overdrive6_FanSpeed_Get = (ADL_OVERDRIVE6_FANSPEED_GET) GetProcAddress (m_hDll,"ADL_Overdrive6_FanSpeed_Get");
    ADL_Overdrive6_ThermalController_Caps = (ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS) GetProcAddress (m_hDll, "ADL_Overdrive6_ThermalController_Caps");
    ADL_Overdrive6_Temperature_Get = (ADL_OVERDRIVE6_TEMPERATURE_GET) GetProcAddress (m_hDll, "ADL_Overdrive6_Temperature_Get");
    ADL_Overdrive6_Capabilities_Get = (ADL_OVERDRIVE6_CAPABILITIES_GET) GetProcAddress (m_hDll, "ADL_Overdrive6_Capabilities_Get");
    ADL_Overdrive6_StateInfo_Get = (ADL_OVERDRIVE6_STATEINFO_GET) GetProcAddress (m_hDll, "ADL_Overdrive6_StateInfo_Get");
    ADL_Overdrive6_CurrentStatus_Get = (ADL_OVERDRIVE6_CURRENTSTATUS_GET) GetProcAddress (m_hDll, "ADL_Overdrive6_CurrentStatus_Get");
    ADL_Overdrive6_PowerControl_Caps = (ADL_OVERDRIVE6_POWERCONTROL_CAPS) GetProcAddress (m_hDll, "ADL_Overdrive6_PowerControl_Caps");
    ADL_Overdrive6_PowerControlInfo_Get = (ADL_OVERDRIVE6_POWERCONTROLINFO_GET) GetProcAddress (m_hDll, "ADL_Overdrive6_PowerControlInfo_Get");
    ADL_Overdrive6_PowerControl_Get = (ADL_OVERDRIVE6_POWERCONTROL_GET) GetProcAddress (m_hDll, "ADL_Overdrive6_PowerControl_Get");
    ADL_Overdrive6_FanSpeed_Set  = (ADL_OVERDRIVE6_FANSPEED_SET) GetProcAddress (m_hDll, "ADL_Overdrive6_FanSpeed_Set");
    ADL_Overdrive6_State_Set = (ADL_OVERDRIVE6_STATE_SET) GetProcAddress (m_hDll, "ADL_Overdrive6_State_Set");
    ADL_Overdrive6_PowerControl_Set = (ADL_OVERDRIVE6_POWERCONTROL_SET) GetProcAddress (m_hDll, "ADL_Overdrive6_PowerControl_Set");


    if (NULL == ADL_Overdrive6_FanSpeed_Get ||
        NULL == ADL_Overdrive6_ThermalController_Caps ||
        NULL == ADL_Overdrive6_Temperature_Get ||
        NULL == ADL_Overdrive6_Capabilities_Get ||
        NULL == ADL_Overdrive6_StateInfo_Get ||
        NULL == ADL_Overdrive6_CurrentStatus_Get ||
        NULL == ADL_Overdrive6_PowerControl_Caps ||
        NULL == ADL_Overdrive6_PowerControlInfo_Get ||
        NULL == ADL_Overdrive6_PowerControl_Get ||
        NULL == ADL_Overdrive6_FanSpeed_Set ||
        NULL == ADL_Overdrive6_State_Set ||
        NULL == ADL_Overdrive6_PowerControl_Set)
    {
       return;
    }

    m_pThermalControllerCaps = new ADLOD6ThermalControllerCaps;
    ZeroMemory(m_pThermalControllerCaps, sizeof(ADLOD6ThermalControllerCaps));
}

COverdrive6::~COverdrive6()
{
    delete m_pThermalControllerCaps;
    m_pThermalControllerCaps = NULL;

    m_hDll = NULL;
}

double *COverdrive6::OnGetTemperatures(int nAdapterIndex)
{
    int nTemperature = 0;
    ZeroMemory(m_pThermalControllerCaps, sizeof(ADLOD6ThermalControllerCaps));

    if( ADL_OK != ADL_Overdrive6_ThermalController_Caps(nAdapterIndex, m_pThermalControllerCaps) )
       return 0;

    if( ADL_OD6_TCCAPS_THERMAL_CONTROLLER != (m_pThermalControllerCaps->iCapabilities & ADL_OD6_TCCAPS_THERMAL_CONTROLLER) )
        return 0;

    if( ADL_OK != ADL_Overdrive6_Temperature_Get(nAdapterIndex, &nTemperature) )
        return 0;

    m_pTemp[0] = nTemperature / 1000;

    return m_pTemp;
}

double *COverdrive6::OnGetFanSpeedsRPM(int nAdapterIndex)
{
    ADLOD6FanSpeedInfo fanSpeedInfo = {0};
    ZeroMemory(m_pThermalControllerCaps, sizeof(ADLOD6ThermalControllerCaps));

    if( ADL_OK != ADL_Overdrive6_ThermalController_Caps(nAdapterIndex, m_pThermalControllerCaps) )
       return 0;

    if( ADL_OD6_TCCAPS_FANSPEED_CONTROL != (m_pThermalControllerCaps->iCapabilities & ADL_OD6_TCCAPS_FANSPEED_CONTROL) )
        return 0;

    if( ADL_OD6_TCCAPS_FANSPEED_RPM_READ != (m_pThermalControllerCaps->iCapabilities & ADL_OD6_TCCAPS_FANSPEED_RPM_READ) )
        return 0;

    if( ADL_OK != ADL_Overdrive6_FanSpeed_Get(nAdapterIndex, &fanSpeedInfo) )
        return 0;

    if( ADL_OD6_FANSPEED_TYPE_RPM == (fanSpeedInfo.iSpeedType & ADL_OD6_FANSPEED_TYPE_RPM) )
    {
        m_pFanRPM[0] = fanSpeedInfo.iFanSpeedRPM;
        return m_pFanRPM;
    }

    return 0;
}

double *COverdrive6::OnGetFanSpeedsPercentage(int nAdapterIndex)
{
    ADLOD6FanSpeedInfo fanSpeedInfo = {0};
    ZeroMemory(m_pThermalControllerCaps, sizeof(ADLOD6ThermalControllerCaps));

    if( ADL_OK != ADL_Overdrive6_ThermalController_Caps(nAdapterIndex, m_pThermalControllerCaps) )
       return 0;

    if( ADL_OD6_TCCAPS_FANSPEED_CONTROL != (m_pThermalControllerCaps->iCapabilities & ADL_OD6_TCCAPS_FANSPEED_CONTROL) )
        return 0;

    if( ADL_OD6_TCCAPS_FANSPEED_PERCENT_READ != (m_pThermalControllerCaps->iCapabilities & ADL_OD6_TCCAPS_FANSPEED_PERCENT_READ) )
        return 0;

    if( ADL_OK != ADL_Overdrive6_FanSpeed_Get(nAdapterIndex, &fanSpeedInfo) )
        return 0;

    if( ADL_OD6_FANSPEED_TYPE_PERCENT == (fanSpeedInfo.iSpeedType & ADL_OD6_FANSPEED_TYPE_PERCENT) )
    {
        m_pFanPerc[0] = fanSpeedInfo.iFanSpeedPercent;
        return m_pFanPerc;
    }

    return 0;
}

double COverdrive6::OnGetEngineClock(int nAdapterIndex)
{
    ADLOD6CurrentStatus currentStatus = {0};

    if( ADL_OK != ADL_Overdrive6_CurrentStatus_Get(nAdapterIndex, &currentStatus) )
        return 0;

    return currentStatus.iEngineClock / 100;
}

double COverdrive6::OnGetMemoryClock(int nAdapterIndex)
{
    ADLOD6CurrentStatus currentStatus = {0};

    if( ADL_OK != ADL_Overdrive6_CurrentStatus_Get(nAdapterIndex, &currentStatus) )
        return 0;

    return currentStatus.iMemoryClock / 100;
}

double COverdrive6::OnGetCoreVoltage(int nAdapterIndex)
{
    int nPowerCaps = 0;
    int nPowerCtrlDefault = 0;
    int nPowerCtrlCurrent = 0;
    ADLOD6PowerControlInfo powerControlInfo = {0};

    if( ADL_OK != ADL_Overdrive6_PowerControl_Caps(nAdapterIndex, &nPowerCaps) )
        return 0;

    if( !nPowerCaps )
        return 0;

    if( ADL_OK != ADL_Overdrive6_PowerControlInfo_Get(nAdapterIndex, &powerControlInfo) )
        return 0;

    if( ADL_OK != ADL_Overdrive6_PowerControl_Get(nAdapterIndex, &nPowerCtrlCurrent, &nPowerCtrlDefault) )
        return 0;

    return nPowerCtrlCurrent;
}
