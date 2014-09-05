#include "nvidia_card.h"

CNvidiaManager::CNvidiaManager()
{
    NvAPI_Status NvStatus = NvAPI_Initialize();

    if( NVAPI_OK != NvStatus )
        return;

    NvStatus = NvAPI_EnumPhysicalGPUs(m_pGpuHandles, &m_nGPUCount);
    if( NVAPI_OK != NvStatus )
        return;

    m_pGpuModel = new QStandardItemModel();
    CHECK_ALLOCATION(m_pGpuModel);
    m_pGpuDataModel = new QStandardItemModel();
    CHECK_ALLOCATION(m_pGpuDataModel);

    m_pGpuModel->appendRow(new QStandardItem("GTX 760"));
    m_pGpuModel->item(0)->setData(0);

    m_pGpuDataModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");
    m_pGpuDataModel->appendRow(new QStandardItem("General informations:"));
    m_pGpuDataModel->appendRow(new QStandardItem("Temperatures:"));
    m_pGpuDataModel->appendRow(new QStandardItem("Fans:"));
    m_pGpuDataModel->appendRow(new QStandardItem("Usage:"));

    QList<QStandardItem*> qList;

    qList << new QStandardItem("Name: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
    qList << new QStandardItem("Core count: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
    qList << new QStandardItem("GPU clock: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
    qList << new QStandardItem("Memory clock: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
    qList << new QStandardItem("Driver version: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
    qList << new QStandardItem("BIOS version: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
    qList << new QStandardItem("BUS type: ") << new QStandardItem();
    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();

    qList << new QStandardItem() << new QStandardItem();
    m_pGpuDataModel->item(1)->appendRow(qList); qList.clear();

    qList << new QStandardItem("Speed:") << new QStandardItem();
    m_pGpuDataModel->item(2)->appendRow(qList); qList.clear();

    qList << new QStandardItem("GPU usage:") << new QStandardItem();
    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
    qList << new QStandardItem("FBU usage:") << new QStandardItem();
    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
    qList << new QStandardItem("VID usage:") << new QStandardItem();
    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
    qList << new QStandardItem("BUS usage:") << new QStandardItem();
    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
}

CNvidiaManager::~CNvidiaManager()
{
    SAFE_DELETE(m_pGpuDataModel);
    SAFE_DELETE(m_pGpuModel);

    NvAPI_Status NvStatus = NvAPI_Unload();

    if( NVAPI_OK != NvStatus )
        return;
}

QStandardItemModel *CNvidiaManager::GetPhysicalGPUModel()
{
    return m_pGpuModel;
}

QStandardItemModel *CNvidiaManager::GetGPUDetailsModel()
{
    return m_pGpuDataModel;
}

void CNvidiaManager::OnRefreshData(int nIndex)
{
    m_pData = new NVidiaReport;

//    m_pGpuDataModel->clear();
//    m_pGpuDataModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    m_nCurrentIndex = nIndex;

    int nStatus = GetGPUInformations(nIndex);
    CHECK_OPERATION_STATUS(nStatus);

//    m_pGpuDataModel->appendRow(new QStandardItem("General informations:"));
//    m_pGpuDataModel->appendRow(new QStandardItem("Temperatures:"));
//    m_pGpuDataModel->appendRow(new QStandardItem("Fans:"));
//    m_pGpuDataModel->appendRow(new QStandardItem("Usage:"));

//    QList<QStandardItem*> qList;

//    qList << new QStandardItem("Name: ") << new QStandardItem(m_pData->qsName);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("Core count: ") << new QStandardItem(m_pData->qsCoreCount);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("GPU clock: ") << new QStandardItem(m_pData->qsGPUClock);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("Memory clock: ") << new QStandardItem(m_pData->qsMemoryClock);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("Driver version: ") << new QStandardItem(m_pData->qsDriverVersion);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("BIOS version: ") << new QStandardItem(m_pData->qsBiosVersion);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("BUS type: ") << new QStandardItem(m_pData->qsBusType);
//    m_pGpuDataModel->item(0)->appendRow(qList); qList.clear();

    m_pGpuDataModel->item(0)->child(0, 1)->setText(m_pData->qsName);
    m_pGpuDataModel->item(0)->child(1, 1)->setText(m_pData->qsCoreCount);
    m_pGpuDataModel->item(0)->child(2, 1)->setText(m_pData->qsGPUClock);
    m_pGpuDataModel->item(0)->child(3, 1)->setText(m_pData->qsMemoryClock);
    m_pGpuDataModel->item(0)->child(4, 1)->setText(m_pData->qsDriverVersion);
    m_pGpuDataModel->item(0)->child(5, 1)->setText(m_pData->qsBiosVersion);
    m_pGpuDataModel->item(0)->child(6, 1)->setText(m_pData->qsBusType);

    SAFE_DELETE(m_pData);

    // start timer
    OnRefreshDynamicData();
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

int CNvidiaManager::GetGPUInformations(int nIndex)
{
    NvAPI_Status nStatus;
    NvU32 nvValue = 0;
    NvAPI_ShortString szName;

    if( nIndex < 0 || nIndex > NVAPI_MAX_PHYSICAL_GPUS )
        return InvalidParameter;

    nStatus = NvAPI_GPU_GetGpuCoreCount(m_pGpuHandles[nIndex], &nvValue);
    if( NVAPI_OK != nStatus )
        return Unsuccessful;
    m_pData->qsCoreCount = QString::number(nvValue);

    nStatus = NvAPI_GPU_GetFullName(m_pGpuHandles[nIndex], szName);
    if( NVAPI_OK != nStatus )
        return Unsuccessful;
    m_pData->qsName = szName;

    NV_GPU_CLOCK_FREQUENCIES nvClocks = {0};
    nvClocks.version = NV_GPU_CLOCK_FREQUENCIES_VER_2;

    nStatus = NvAPI_GPU_GetAllClockFrequencies(m_pGpuHandles[nIndex], &nvClocks);
    if( NVAPI_OK != nStatus )
        return Unsuccessful;
    m_pData->qsGPUClock.sprintf("%dMHz", nvClocks.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000);
    m_pData->qsMemoryClock.sprintf("%dMHz", nvClocks.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 2000);

    return Success;
}

int CNvidiaManager::OnRefreshDynamicData()
{
    // Clear node 1, 2 and 3 information
//    while( m_pGpuDataModel->item(1)->rowCount() > 0 )
//        m_pGpuDataModel->item(1)->removeRow(0);
//    while( m_pGpuDataModel->item(2)->rowCount() > 0 )
//        m_pGpuDataModel->item(2)->removeRow(0);
//    while( m_pGpuDataModel->item(3)->rowCount() > 0 )
//        m_pGpuDataModel->item(3)->removeRow(0);


    NV_GPU_THERMAL_SETTINGS *pNvThermalSettings = (NV_GPU_THERMAL_SETTINGS*)malloc(sizeof(NV_GPU_THERMAL_SETTINGS) * NVAPI_MAX_THERMAL_SENSORS_PER_GPU);
    for(int i = 0; i < NVAPI_MAX_THERMAL_SENSORS_PER_GPU;i++)
        pNvThermalSettings[i].version = NV_GPU_THERMAL_SETTINGS_VER_2;

    NvAPI_Status nStatus = NvAPI_GPU_GetThermalSettings(m_pGpuHandles[m_nCurrentIndex], NVAPI_THERMAL_TARGET_ALL, pNvThermalSettings);
    if( NVAPI_OK != nStatus )
        return Unsuccessful;

    QList<QStandardItem*> qList;

//    qList << new QStandardItem(GetThermalTargetName(pNvThermalSettings[0].sensor[0].target));
//    qList << new QStandardItem(QString().sprintf("%dC", pNvThermalSettings[0].sensor[0].currentTemp));
//    m_pGpuDataModel->item(1)->appendRow(qList); qList.clear();

    m_pGpuDataModel->item(1)->child(0, 0)->setText(GetThermalTargetName(pNvThermalSettings[0].sensor[0].target));
    m_pGpuDataModel->item(1)->child(0, 1)->setText(QString().sprintf("%dC", pNvThermalSettings[0].sensor[0].currentTemp));

    free(pNvThermalSettings);

    NvU32 nValue = 0;
    nStatus = NvAPI_GPU_GetTachReading(m_pGpuHandles[m_nCurrentIndex], &nValue);
    if( NVAPI_OK != nStatus )
        return Unsuccessful;

//    qList << new QStandardItem("Speed");
//    qList << new QStandardItem(QString().sprintf("%d RPM", nValue));
//    m_pGpuDataModel->item(2)->appendRow(qList); qList.clear();

    m_pGpuDataModel->item(2)->child(0, 1)->setText(QString().sprintf("%d RPM", nValue));

    NV_GPU_DYNAMIC_PSTATES_INFO_EX nvDynamicPStatesEx = {0};
    nvDynamicPStatesEx.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;

    nStatus = NvAPI_GPU_GetDynamicPstatesInfoEx(m_pGpuHandles[m_nCurrentIndex], &nvDynamicPStatesEx);
    if( NVAPI_OK != nStatus )
        return 0;

//    qList << new QStandardItem("GPU usage: ") << new QStandardItem(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[0].percentage));
//    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("FBU usage: ") << new QStandardItem(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[1].percentage));
//    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("VID usage: ") << new QStandardItem(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[2].percentage));
//    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();
//    qList << new QStandardItem("BUS usage: ") << new QStandardItem(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[3].percentage));
//    m_pGpuDataModel->item(3)->appendRow(qList); qList.clear();

    m_pGpuDataModel->item(3)->child(0, 1)->setText(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[0].percentage));
    m_pGpuDataModel->item(3)->child(1, 1)->setText(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[1].percentage));
    m_pGpuDataModel->item(3)->child(2, 1)->setText(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[2].percentage));
    m_pGpuDataModel->item(3)->child(3, 1)->setText(QString().sprintf("%d%", nvDynamicPStatesEx.utilization[3].percentage));

    return Success;
}
