#ifndef _NVIDIA_CARD_HEADER_
#define _NVIDIA_CARD_HEADER_

#include "../../../utils/headers/utils.h"
#include "video_structures.h"

#include <Windows.h>
#include "api/nvidia-api/nvapi.h"

#include <QStandardItemModel>
#include <QTimer>

class CNvidiaManager
{
private:
    QStandardItemModel  *m_pGpuModel;
    QStandardItemModel  *m_pGpuDataModel;
    int                 m_nCurrentIndex;

    NVidiaReport        *m_pData;

    NvU32               m_nGPUCount;
    NvPhysicalGpuHandle m_pGpuHandles[NVAPI_MAX_PHYSICAL_GPUS];

    QString GetThermalTargetName(NV_THERMAL_TARGET nvThermTarget);
    int GetGPUInformations(int nIndex);

private slots:
    int OnRefreshDynamicData();

public:
    CNvidiaManager();
    ~CNvidiaManager();

    QStandardItemModel *GetPhysicalGPUModel();
    QStandardItemModel *GetGPUDetailsModel();

    void OnRefreshData(int);
};

#endif
