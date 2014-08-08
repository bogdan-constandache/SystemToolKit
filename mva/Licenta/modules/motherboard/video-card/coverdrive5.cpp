#include "coverdrive5.h"

COverdrive5::COverdrive5(HINSTANCE hDll) : m_hDll(NULL)
{
    m_hDll = hDll;
    ZeroMemory(m_pTemp, sizeof(m_pTemp));
    ZeroMemory(m_pFanRPM, sizeof(m_pFanRPM));
    ZeroMemory(m_pFanPerc, sizeof(m_pFanPerc));

    ADL_Overdrive5_ThermalDevices_Enum = (ADL_OVERDRIVE5_THERMALDEVICES_ENUM) GetProcAddress (m_hDll, "ADL_Overdrive5_ThermalDevices_Enum");
    ADL_Overdrive5_ODParameters_Get = (ADL_OVERDRIVE5_ODPARAMETERS_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_ODParameters_Get");
    ADL_Overdrive5_Temperature_Get = (ADL_OVERDRIVE5_TEMPERATURE_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_Temperature_Get");
    ADL_Overdrive5_FanSpeed_Get = (ADL_OVERDRIVE5_FANSPEED_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_FanSpeed_Get");
    ADL_Overdrive5_FanSpeedInfo_Get = (ADL_OVERDRIVE5_FANSPEEDINFO_GET ) GetProcAddress (m_hDll, "ADL_Overdrive5_FanSpeedInfo_Get");
    ADL_Overdrive5_ODPerformanceLevels_Get = (ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_ODPerformanceLevels_Get");
    ADL_Overdrive5_CurrentActivity_Get = (ADL_OVERDRIVE5_CURRENTACTIVITY_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_CurrentActivity_Get");
    ADL_Overdrive5_FanSpeed_Set = (ADL_OVERDRIVE5_FANSPEED_SET) GetProcAddress (m_hDll, "ADL_Overdrive5_FanSpeed_Set");
    ADL_Overdrive5_ODPerformanceLevels_Set = (ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET) GetProcAddress(m_hDll, "ADL_Overdrive5_ODPerformanceLevels_Set");
    ADL_Overdrive5_PowerControl_Caps = (ADL_OVERDRIVE5_POWERCONTROL_CAPS) GetProcAddress (m_hDll, "ADL_Overdrive5_PowerControl_Caps");
    ADL_Overdrive5_PowerControlInfo_Get = (ADL_OVERDRIVE5_POWERCONTROLINFO_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_PowerControlInfo_Get");
    ADL_Overdrive5_PowerControl_Get = (ADL_OVERDRIVE5_POWERCONTROL_GET) GetProcAddress (m_hDll, "ADL_Overdrive5_PowerControl_Get");
    ADL_Overdrive5_PowerControl_Set = (ADL_OVERDRIVE5_POWERCONTROL_SET) GetProcAddress (m_hDll, "ADL_Overdrive5_PowerControl_Set");
    ADL_Overdrive6_State_Set = (ADL_OVERDRIVE6_STATE_SET) GetProcAddress (m_hDll, "ADL_Overdrive6_State_Set");

    if( NULL == ADL_Overdrive5_ThermalDevices_Enum ||
            NULL == ADL_Overdrive5_Temperature_Get ||
            NULL == ADL_Overdrive5_FanSpeedInfo_Get ||
            NULL == ADL_Overdrive5_ODPerformanceLevels_Get ||
            NULL == ADL_Overdrive5_ODParameters_Get ||
            NULL == ADL_Overdrive5_CurrentActivity_Get ||
            NULL == ADL_Overdrive5_FanSpeed_Set ||
            NULL == ADL_Overdrive5_ODPerformanceLevels_Set ||
            NULL == ADL_Overdrive5_PowerControl_Caps ||
            NULL == ADL_Overdrive5_PowerControlInfo_Get ||
            NULL == ADL_Overdrive5_PowerControl_Get ||
            NULL == ADL_Overdrive5_PowerControl_Set)
    {
        return;
    }
}

COverdrive5::~COverdrive5()
{
    m_hDll = NULL;
}

double *COverdrive5::OnGetTemperatures(int nAdapterIndex)
{
    int ADL_Err = ADL_ERR;

    ZeroMemory(m_pTemp, sizeof(m_pTemp));

    ADLThermalControllerInfo termalControllerInfo = {0};
    termalControllerInfo.iSize = sizeof (ADLThermalControllerInfo);

    for (int iThermalControllerIndex = 0; iThermalControllerIndex < 10; iThermalControllerIndex++)
    {
        ADL_Err = ADL_Overdrive5_ThermalDevices_Enum(nAdapterIndex, iThermalControllerIndex, &termalControllerInfo);

        if (ADL_Err == ADL_WARNING_NO_DATA)
            return 0;

        if(termalControllerInfo.iThermalDomain != ADL_DL_THERMAL_DOMAIN_GPU)
            return 0;

        ADLTemperature adlTemperature = {0};
        adlTemperature.iSize = sizeof (ADLTemperature);
        if( ADL_OK != ADL_Overdrive5_Temperature_Get(nAdapterIndex, iThermalControllerIndex, &adlTemperature))
            return 0;

        // The temperature is returned in millidegrees Celsius.
        int temperatureInDegreesCelsius = adlTemperature.iTemperature / 1000;

        m_pTemp[iThermalControllerIndex] = temperatureInDegreesCelsius;
    }

    return m_pTemp;
}

double *COverdrive5::OnGetFanSpeedsRPM(int nAdapterIndex)
{
    int ADL_Err = ADL_ERR;

    ZeroMemory(m_pFanRPM, sizeof(m_pFanRPM));

    ADLThermalControllerInfo termalControllerInfo = {0};
    termalControllerInfo.iSize = sizeof (ADLThermalControllerInfo);

    for (int iThermalControllerIndex = 0; iThermalControllerIndex < 10; iThermalControllerIndex++)
    {
        ADL_Err = ADL_Overdrive5_ThermalDevices_Enum (nAdapterIndex, iThermalControllerIndex, &termalControllerInfo);

        if (ADL_Err == ADL_WARNING_NO_DATA)
            return 0;

        if (termalControllerInfo.iThermalDomain != ADL_DL_THERMAL_DOMAIN_GPU)
            return 0;

        ADLFanSpeedInfo fanSpeedInfo = {0};
        fanSpeedInfo.iSize = sizeof (ADLFanSpeedInfo);
        if( ADL_OK != ADL_Overdrive5_FanSpeedInfo_Get (nAdapterIndex, iThermalControllerIndex, &fanSpeedInfo) )
            return 0;

        int fanSpeedReportingMethod = 0;
        fanSpeedReportingMethod = ((fanSpeedInfo.iFlags & ADL_DL_FANCTRL_SUPPORTS_RPM_READ) == ADL_DL_FANCTRL_SUPPORTS_RPM_READ )? ADL_DL_FANCTRL_SPEED_TYPE_RPM : ADL_DL_FANCTRL_SPEED_TYPE_PERCENT;

        ADLFanSpeedValue fanSpeedValue = {0};
        fanSpeedValue.iSpeedType = fanSpeedReportingMethod;
        if( ADL_OK != ADL_Overdrive5_FanSpeed_Get (nAdapterIndex, iThermalControllerIndex, &fanSpeedValue) )
            return 0;

        if (fanSpeedReportingMethod == ADL_DL_FANCTRL_SPEED_TYPE_RPM)
            m_pFanRPM[iThermalControllerIndex] = fanSpeedValue.iFanSpeed;
    }

    return m_pFanRPM;
}

double *COverdrive5::OnGetFanSpeedsPercentage(int nAdapterIndex)
{
    int ADL_Err = ADL_ERR;

    ZeroMemory(m_pFanPerc, sizeof(m_pFanPerc));

    ADLThermalControllerInfo termalControllerInfo = {0};
    termalControllerInfo.iSize = sizeof (ADLThermalControllerInfo);

    for (int iThermalControllerIndex = 0; iThermalControllerIndex < 10; iThermalControllerIndex++)
    {
        ADL_Err = ADL_Overdrive5_ThermalDevices_Enum (nAdapterIndex, iThermalControllerIndex, &termalControllerInfo);

        if (ADL_Err == ADL_WARNING_NO_DATA)
            return 0;

        if (termalControllerInfo.iThermalDomain != ADL_DL_THERMAL_DOMAIN_GPU)
            return 0;

        ADLFanSpeedInfo fanSpeedInfo = {0};
        fanSpeedInfo.iSize = sizeof (ADLFanSpeedInfo);
        if( ADL_OK != ADL_Overdrive5_FanSpeedInfo_Get (nAdapterIndex, iThermalControllerIndex, &fanSpeedInfo) )
            return 0;

        int fanSpeedReportingMethod = 0;
        fanSpeedReportingMethod = ((fanSpeedInfo.iFlags & ADL_DL_FANCTRL_SUPPORTS_RPM_READ) == ADL_DL_FANCTRL_SUPPORTS_RPM_READ )? ADL_DL_FANCTRL_SPEED_TYPE_RPM : ADL_DL_FANCTRL_SPEED_TYPE_PERCENT;

        ADLFanSpeedValue fanSpeedValue = {0};
        fanSpeedValue.iSpeedType = fanSpeedReportingMethod;
        if( ADL_OK != ADL_Overdrive5_FanSpeed_Get (nAdapterIndex, iThermalControllerIndex, &fanSpeedValue) )
            return 0;

        if (fanSpeedReportingMethod == ADL_DL_FANCTRL_SPEED_TYPE_PERCENT)
            m_pFanPerc[iThermalControllerIndex] = fanSpeedValue.iFanSpeed;
    }

    return m_pFanPerc;
}

double COverdrive5::OnGetEngineClock(int nAdapterIndex)
{
    ADLPMActivity activity = {0};
    activity.iSize = sizeof (ADLPMActivity);
    if (ADL_OK != ADL_Overdrive5_CurrentActivity_Get (nAdapterIndex, &activity))
        return 0;

    return activity.iEngineClock / 100;
}

double COverdrive5::OnGetMemoryClock(int nAdapterIndex)
{
    ADLPMActivity activity = {0};
    activity.iSize = sizeof (ADLPMActivity);
    if (ADL_OK != ADL_Overdrive5_CurrentActivity_Get (nAdapterIndex, &activity))
        return 0;

    return activity.iMemoryClock / 100;
}

double COverdrive5::OnGetCoreVoltage(int nAdapterIndex)
{
    ADLPMActivity activity = {0};
    activity.iSize = sizeof (ADLPMActivity);
    if (ADL_OK != ADL_Overdrive5_CurrentActivity_Get (nAdapterIndex, &activity))
        return 0;

    return activity.iVddc;
}
