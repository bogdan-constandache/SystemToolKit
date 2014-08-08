#include "nvidia_card.h"

CNvidiaManager::CNvidiaManager()
    :m_pReport(NULL), m_pGPUInformations(NULL), m_pGPUUtilization(NULL), m_nvGpuCount(0)
{
    m_pReport = new NVidiaThermalReport;
    CHECK_ALLOCATION(m_pReport);

    m_pGPUInformations = new NVidiaGPUInformations;
    CHECK_ALLOCATION(m_pGPUInformations);

    m_pGPUUtilization = new NVidiaUtilizationReport;
    CHECK_ALLOCATION(m_pGPUUtilization);

    NvAPI_Status NvStatus = NvAPI_Initialize();

    if( NVAPI_OK != NvStatus )
        return;

    NvPhysicalGpuHandle m_nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS] = {0};
    m_nvGpuCount = 0;

    NvStatus = NvAPI_EnumPhysicalGPUs(m_nvGPUHandle, &m_nvGpuCount);
    if( NVAPI_OK != NvStatus )
        return;
}

CNvidiaManager::~CNvidiaManager()
{
    NvAPI_Status NvStatus = NvAPI_Unload();

    if( NVAPI_OK != NvStatus )
        return;

    SAFE_DELETE(m_pGPUUtilization);
    SAFE_DELETE(m_pGPUInformations);
    SAFE_DELETE(m_pReport);
}

QString CNvidiaManager::GetThermalTargetName(NV_THERMAL_TARGET nvThermTarget)
{
    switch(nvThermTarget)
    {
    case NVAPI_THERMAL_TARGET_NONE:
        return "";
    case NVAPI_THERMAL_TARGET_GPU:
        return "GPU";
    case NVAPI_THERMAL_TARGET_MEMORY:
        return "Memory";
    case NVAPI_THERMAL_TARGET_POWER_SUPPLY:
        return "Power supply";
    case NVAPI_THERMAL_TARGET_BOARD:
        return "Board";
    default:
        return "";
    }

    return "";
}

NVidiaThermalReport *CNvidiaManager::GetThermalReport(NvU32 nvIndex)
{
    NvAPI_Status NvStatus;

    NvPhysicalGpuHandle m_nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS] = {0};
    m_nvGpuCount = 0;

    NvStatus = NvAPI_EnumPhysicalGPUs(m_nvGPUHandle, &m_nvGpuCount);
    if( NVAPI_OK != NvStatus )
        return 0;
    if( nvIndex < 0 || nvIndex >= m_nvGpuCount )
        return 0;

    NV_GPU_THERMAL_SETTINGS nvThermalSettings[NVAPI_MAX_THERMAL_SENSORS_PER_GPU] = {0};

    for(NvU32 j = 0; j < NVAPI_MAX_THERMAL_SENSORS_PER_GPU; j++)
        nvThermalSettings[j].version = NV_GPU_THERMAL_SETTINGS_VER_2;

    NvStatus = NvAPI_GPU_GetThermalSettings(m_nvGPUHandle[nvIndex], NVAPI_THERMAL_TARGET_ALL,
                                            nvThermalSettings);
    if( NVAPI_OK != NvStatus )
        return 0;

    for(NvU32 j = 0; j < NVAPI_MAX_THERMAL_SENSORS_PER_GPU; j++)
    {
        for(NvU32 k = 0; k < NVAPI_MAX_THERMAL_SENSORS_PER_GPU; k++)
        {
            if( 0 != nvThermalSettings[j].sensor[k].currentTemp )
            {
                m_pReport->qzTarget = GetThermalTargetName(nvThermalSettings[j].sensor[k].target);
                m_pReport->qzTemp.sprintf("%dC", nvThermalSettings[j].sensor[k].currentTemp);
            }
        }
    }

    return m_pReport;
}

NvU32 CNvidiaManager::GetFanSpeedReport(NvU32 nvIndex)
{
    NvAPI_Status NvStatus;
    NvU32 nvValue = 0;

    NvPhysicalGpuHandle m_nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS] = {0};
    m_nvGpuCount = 0;

    NvStatus = NvAPI_EnumPhysicalGPUs(m_nvGPUHandle, &m_nvGpuCount);
    if( NVAPI_OK != NvStatus )
        return 0;

    if( nvIndex < 0 || nvIndex >= m_nvGpuCount )
        return 0;

    NvStatus = NvAPI_GPU_GetTachReading(m_nvGPUHandle[nvIndex], &nvValue);

    if( NVAPI_OK != NvStatus )
        return 0;

    return nvValue;
}

NVidiaGPUInformations *CNvidiaManager::GetGPUInformations(NvU32 nvIndex)
{
    NvAPI_Status NvStatus;
    NvU32 nvValue = 0;

    if( nvIndex < 0 || nvIndex >= m_nvGpuCount )
        return 0;

    NvPhysicalGpuHandle m_nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS] = {0};
    m_nvGpuCount = 0;

    NvStatus = NvAPI_EnumPhysicalGPUs(m_nvGPUHandle, &m_nvGpuCount);
    if( NVAPI_OK != NvStatus )
        return 0;

    NvStatus = NvAPI_GPU_GetGpuCoreCount(m_nvGPUHandle[nvIndex], &nvValue);
    if( NVAPI_OK != NvStatus )
        return 0;

    m_pGPUInformations->qzCoreCount = QString::number(nvValue);

    NvAPI_ShortString szName;
    NvStatus = NvAPI_GPU_GetFullName(m_nvGPUHandle[nvIndex], szName);
    if( NVAPI_OK != NvStatus )
        return 0;

    m_pGPUInformations->qzFullName = szName;

    NV_GPU_CLOCK_FREQUENCIES nvClocks = {0};
    nvClocks.version = NV_GPU_CLOCK_FREQUENCIES_VER_2;

    NvStatus = NvAPI_GPU_GetAllClockFrequencies(m_nvGPUHandle[nvIndex], &nvClocks);
    if( NVAPI_OK != NvStatus )
        return 0;

    m_pGPUInformations->qzGraphicsClock.sprintf("%dMHz", nvClocks.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000);
    m_pGPUInformations->qzMemoryClock.sprintf("%dMHz", nvClocks.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 2000);

    return m_pGPUInformations;
}

NVidiaUtilizationReport *CNvidiaManager::GetGPUUtilizationReport(NvU32 nvIndex)
{
    NvAPI_Status NvStatus;

    if( nvIndex < 0 || nvIndex >= m_nvGpuCount )
        return 0;

    NvPhysicalGpuHandle m_nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS] = {0};
    m_nvGpuCount = 0;

    NvStatus = NvAPI_EnumPhysicalGPUs(m_nvGPUHandle, &m_nvGpuCount);
    if( NVAPI_OK != NvStatus )
        return 0;

    NV_GPU_DYNAMIC_PSTATES_INFO_EX nvDynamicPStatesEx = {0};
    nvDynamicPStatesEx.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;

    NvStatus = NvAPI_GPU_GetDynamicPstatesInfoEx(m_nvGPUHandle[nvIndex], &nvDynamicPStatesEx);
    if( NVAPI_OK != NvStatus )
        return 0;

    m_pGPUUtilization->qzGPUUtilization.sprintf("%d%", nvDynamicPStatesEx.utilization[0].percentage);
    m_pGPUUtilization->qzFBUtilization.sprintf("%d%", nvDynamicPStatesEx.utilization[1].percentage);
    m_pGPUUtilization->qzVIDUtilization.sprintf("%d%", nvDynamicPStatesEx.utilization[2].percentage);
    m_pGPUUtilization->qzBUSUtilization.sprintf("%d%", nvDynamicPStatesEx.utilization[3].percentage);

    return m_pGPUUtilization;
}

NvU32 CNvidiaManager::GetPhysicalGPUCount()
{
    return m_nvGpuCount;
}


