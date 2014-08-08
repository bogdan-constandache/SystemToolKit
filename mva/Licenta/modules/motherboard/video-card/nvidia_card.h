#ifndef _NVIDIA_CARD_HEADER_
#define _NVIDIA_CARD_HEADER_

#include "../../../utils/headers/utils.h"
#include "video_structures.h"

#include <Windows.h>
#include "api/nvidia-api/nvapi.h"

class CNvidiaManager
{
private:
    NVidiaGPUInformations *m_pGPUInformations;
    NVidiaUtilizationReport *m_pGPUUtilization;
    NVidiaThermalReport *m_pReport;

    NvU32 m_nvGpuCount;
    QString GetThermalTargetName(NV_THERMAL_TARGET nvThermTarget);

public:
    CNvidiaManager();
    ~CNvidiaManager();
    NvU32 GetPhysicalGPUCount();
    NVidiaThermalReport *GetThermalReport(NvU32 nvIndex);
    NvU32 GetFanSpeedReport(NvU32 nvIndex);
    NVidiaGPUInformations *GetGPUInformations(NvU32 nvIndex);
    NVidiaUtilizationReport *GetGPUUtilizationReport(NvU32 nvIndex);
};

#endif
