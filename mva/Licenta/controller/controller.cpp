#include "controller.h"

void Controller::OnCreateComputerSummary()
{
    bool bResult = false; int nStatus;
    m_pComputerSummaryModel = new QStandardItemModel;

    // GET COMPUTER NAME
    WCHAR lpComputerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD dwCompName = MAX_COMPUTERNAME_LENGTH + 1;

    bResult = GetComputerName(lpComputerName, &dwCompName);

    m_pComputerSummaryModel->setItem(0, 0, new QStandardItem("Computer name: "));
    m_pComputerSummaryModel->setItem(0, 1, new QStandardItem(QString::fromWCharArray(lpComputerName)));

    // OPERATING SYSTEM
    m_pComputerSummaryModel->setItem(1, 0, new QStandardItem("Operating system: "));
    HKEY hRegKey = 0;
    WCHAR lpOsName[MAX_PATH + 1];
    DWORD dwRegKeySize = MAX_PATH + 1;
    nStatus = RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", &hRegKey);
    RegQueryValueEx(hRegKey, L"ProductName", NULL, NULL, (LPBYTE)lpOsName, &dwRegKeySize);
    m_pComputerSummaryModel->setItem(1, 1, new QStandardItem(QString::fromWCharArray(lpOsName)));

    //User name
    WCHAR lpUsername[UNLEN + 1];
    DWORD dwUsernameSize = UNLEN + 1;
    bResult = GetUserName(lpUsername, &dwUsernameSize);

    m_pComputerSummaryModel->setItem(2, 0, new QStandardItem("User name: "));
    m_pComputerSummaryModel->setItem(2, 1, new QStandardItem(QString::fromWCharArray(lpUsername)));

    //Domain name
    WCHAR lpDomainName[UNLEN + 1];
    DWORD dwDomainNameSize = UNLEN + 1;
    bResult = GetComputerNameEx(ComputerNameDnsHostname, lpDomainName, &dwDomainNameSize);

    m_pComputerSummaryModel->setItem(2, 0, new QStandardItem("Domain name: "));
    m_pComputerSummaryModel->setItem(2, 1, new QStandardItem(QString::fromWCharArray(lpDomainName)));


    // System memory
    MemoryStatus *pMemoryInfo = m_pSensorsManager->GetMemoryStat();
    m_pComputerSummaryModel->setItem(4, 0, new QStandardItem("System memory: "));
    m_pComputerSummaryModel->setItem(4, 1, new QStandardItem(pMemoryInfo->qzTotalPhys));

    // Hdd size
    QStringList qzPhysicalDisks = GetPhysicalDrivesList();
    for(int i = 0; i < qzPhysicalDisks.count(); i++)
    {
        m_pComputerSummaryModel->setItem(5 + i, 0, new QStandardItem(QString().sprintf("Disk %d", i + 1)));
        m_pComputerSummaryModel->setItem(5 + i, 1, new QStandardItem(GetDiskTotalSize(qzPhysicalDisks.at(i))));
    }
}

Controller::Controller(): m_pBatteryStatus(NULL), m_pApplicationManager(NULL),
    m_pDMIManager(NULL), m_pSmartManager(NULL), m_pSystemDriversManager(NULL),
    m_pActiveConnectionsManager(NULL), m_pNetworkDevicesManager(NULL), m_pCPUIDManager(NULL),
    m_pSensorsManager(NULL), m_pSensor(NULL), m_pSensorsTimer(NULL), m_pCpuSensor(NULL),
    m_pProcessesManager(NULL), m_pStartupAppsManager(NULL), m_pComputerSummaryModel(NULL), m_pDeviceManager(NULL)
{
    m_pSensorsTimer = new QTimer(this);
    connect(m_pSensorsTimer, SIGNAL(timeout()), this, SLOT(OnSensorsOptClickedSlot()), Qt::QueuedConnection);

    connect(this, SIGNAL(OnCancelSensorsTimerSignal()), this, SLOT(OnCancelSensorsTimerSlot()), Qt::QueuedConnection);
}

Controller::~Controller()
{
    SAFE_DELETE(m_pDeviceManager);
    SAFE_DELETE(m_pBatteryStatus);
    SAFE_DELETE(m_pApplicationManager);
    SAFE_DELETE(m_pDMIManager);
    SAFE_DELETE(m_pSmartManager)
    SAFE_DELETE(m_pSystemDriversManager);
    SAFE_DELETE(m_pActiveConnectionsManager);
    SAFE_DELETE(m_pNetworkDevicesManager);
    SAFE_DELETE(m_pCPUIDManager);
    SAFE_DELETE(m_pSensorsManager)
    SAFE_DELETE(m_pProcessesManager);
    SAFE_DELETE(m_pStartupAppsManager);

    m_HDDModelToPhysicalDrive.clear();
}

void Controller::StartController()
{
    HRESULT hResult = CoInitializeEx(0, COINIT_MULTITHREADED);
    if( 0 != hResult )
    {
        _com_error err(hResult);
        QString qzError = QString::fromWCharArray(err.ErrorMessage());
        qDebug() << "Error initializing COM function CoinitializeEx: " << qzError;
        return;
    }

    // Create main window
    emit OnCreateMainWindowSignal();

    // Populate left menu tree
    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pDataRoot = pModel->invisibleRootItem();
    QStandardItem *pDataItem = 0;
    QStandardItem *pChildItem = 0;
    QList<QStandardItem*> qList;
    QList<QStandardItem*> qChildList;

    pModel->setHorizontalHeaderLabels(QStringList() << "Select a category:");

    pDataItem = new QStandardItem(QString("Computer"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Device manager"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("DMI"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Power management"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Sensors"));
    qChildList.append(pChildItem);


    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Motherboard"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("CPU"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("CPUID"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Operating system"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Operating system"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Processes"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("System drivers"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Storage"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("ATA"));
    qChildList.append(pChildItem);

    pChildItem = new QStandardItem(QString("SMART"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Network"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Network devices"));
    qChildList.append(pChildItem);

    pChildItem = new QStandardItem(QString("Connections"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Software"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Startup applications"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Application manager"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataRoot->appendRows(qList);
    qList.clear();

    emit OnPopulateMenuTreeSignal(pModel);

    emit OnShowMainWindowSignal();

    // Create Sensor Object
    m_pSensorsManager = new CSensorModule;
    // get board sensor
    m_pSensor = m_pSensorsManager->GetBoardSensor();
    if (m_pSensor)
        if (Success != m_pSensor->Initialize())
        {
            m_pSensorsManager->DestroyBoardSensor();
            m_pSensor = NULL;
        }

    // get cpu sensor
    m_pCpuSensor = m_pSensorsManager->GetCpuSensor();
    if (m_pCpuSensor)
        if (Success != m_pCpuSensor->Initialize())
        {
            m_pSensorsManager->DestroyCpuSensor();
            m_pCpuSensor = NULL;
        }

    OnCreateComputerSummary();
}

void Controller::OnComputerDeviceManagerOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // delete previous object
    SAFE_DELETE(m_pDeviceManager);

    m_pDeviceManager = new CDeviceInfo;

    CHECK_ALLOCATION(m_pDeviceManager);

    QStandardItemModel *pModel = m_pDeviceManager->GetAllDeviceDetails();

    if (0 != pModel)
        emit OnSetDeviceManagerInformation(pModel);
}

void Controller::OnComputerDMIOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // delete previous object
    SAFE_DELETE(m_pDMIManager);

    m_pDMIManager = new CSMBiosEntryPoint;

    CHECK_ALLOCATION(m_pDMIManager);

    QStandardItemModel *pModel = m_pDMIManager->GetItemsModel();

    if (0 != pModel)
        emit OnSetDMIItemsInformation(pModel);
}

void Controller::OnComputerPowerManagementOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // delete previous object
    SAFE_DELETE(m_pBatteryStatus);

    m_pBatteryStatus = new BatteryStatus;

    CHECK_ALLOCATION(m_pBatteryStatus);

    QStandardItemModel *pModel = m_pBatteryStatus->GetBatteryInformation();

    if (0 != pModel)
        emit OnSetPowerManagementInformation(pModel);
}

void Controller::OnHddInformationOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnOperatingSystemOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnProcessesOptClickedSlot()
{
    SAFE_DELETE(m_pProcessesManager);

    m_pProcessesManager = new Processes;
    CHECK_ALLOCATION(m_pProcessesManager);

    QStandardItemModel *pModel = m_pProcessesManager->GetProcessesInformations();

    if (0 != pModel)
        emit OnSetProcessesInformations(pModel);
}

void Controller::OnSystemDriversOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // delete previous object
    SAFE_DELETE(m_pSystemDriversManager);

    m_pSystemDriversManager = new SystemDrivers;

    CHECK_ALLOCATION(m_pSystemDriversManager);

    QStandardItemModel *pModel = m_pSystemDriversManager->GetSystemDriversInformation();

    if( 0 != pModel )
        emit OnSetSystemDriversModelInformation(pModel);
}

void Controller::OnStorageATAOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    QStandardItemModel *pModel = new QStandardItemModel();
    QStringList List = GetPhysicalDrivesList();

    pModel->setColumnCount(1);
    pModel->setRowCount(List.count());
    pModel->setHorizontalHeaderLabels(QStringList() << "Device description");

    m_HDDModelToPhysicalDrive.clear();
    for(int i = 0; i < List.count(); i++)
    {
        ATADeviceProperties *pProp = GetATADeviceProperties(List.at(i).toStdWString().c_str());
        if (NULL == pProp)
            continue;
        m_HDDModelToPhysicalDrive.insert(pProp->Model, List.at(i));
        pModel->setItem(i, 0, new QStandardItem(pProp->Model));
        delete pProp;
    }

    emit OnSetATAHDDItemsInformation(pModel);
}

void Controller::OnStorageSmartOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // delete previous object
    SAFE_DELETE(m_pSmartManager);

    m_pSmartManager = new CSmartInfo();

    CHECK_ALLOCATION(m_pSmartManager);

    QStandardItemModel *pModel = m_pSmartManager->GetAvailableHDD();

    if (0 != pModel)
        emit OnSetSMARTHDDItemsInformation(pModel);
}

void Controller::OnSmbiosOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnApplicationManagerOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // delete  previous object
    SAFE_DELETE(m_pApplicationManager);

    m_pApplicationManager = new CApplicationManager();

    CHECK_ALLOCATION(m_pApplicationManager);

    QStandardItemModel *pModel = m_pApplicationManager->GetApplicationList();

    if (0 != pModel)
        emit OnSetApplicationManagerInformation(pModel);
}

void Controller::OnStartupApplicationsOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    SAFE_DELETE(m_pStartupAppsManager);

    m_pStartupAppsManager = new CStartupApplication;

    CHECK_ALLOCATION(m_pStartupAppsManager);

    QStandardItemModel *pModel = m_pStartupAppsManager->GetStartupApplicationsInformations();

    if (0 != pModel)
        emit OnSetStartupApplicationsInformations(pModel);
}

void Controller::OnActiveConnectionsOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    SAFE_DELETE(m_pActiveConnectionsManager);

    m_pActiveConnectionsManager = new CActiveConnections;

    CHECK_ALLOCATION(m_pActiveConnectionsManager);

    QStandardItemModel *pModel = m_pActiveConnectionsManager->GetActiveConnections();

    if (0 != pModel)
        emit OnSetActiveConnectionsInformation(pModel);
}

void Controller::OnNetworkDevicesOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    SAFE_DELETE(m_pNetworkDevicesManager);

    m_pNetworkDevicesManager = new CNetworkDevices;

    CHECK_ALLOCATION(m_pNetworkDevicesManager);

    QStandardItemModel *pModel = m_pNetworkDevicesManager->GetAdapterNames();

    if (0 != pModel)
        emit OnSetNetworkDevicesNames(pModel);
}

void Controller::OnCPUOptClickedSlot()
{

}

void Controller::OnCPUIDOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    SAFE_DELETE(m_pCPUIDManager);

    m_pCPUIDManager = new CIntelCpuID;

    CHECK_ALLOCATION(m_pCPUIDManager);

    QStandardItemModel *pModel = m_pCPUIDManager->GetCPUIDInformations();

    if (0 != pModel )
        emit OnSetCPUIDInformations(pModel);
}

void Controller::OnSensorsOptClickedSlot()
{
    SensorsData pSensorData;

    DataType *pDataType = 0;
    ItemPair *pItemPair = 0;
    MotherboardData *pMBData = 0;
    CpuData *pCpuData = 0;
    RAMData *pRAMData = 0;

    double *pResults = 0;
    VoltageReading *pVoltages = 0;
    QString qzTemp1 = "", qzTemp2 = "";
    MemoryStatus* pMemoryStatus = 0;

    if (!m_pSensorsTimer->isActive())
        m_pSensorsTimer->start(750);

    if (!m_pSensor)
        goto CPU;

    pMBData = pSensorData.mutable_mbdata();
    pMBData->set_name(m_pSensor->GetChipName().toLatin1().data());

    m_pSensor->Update();
    pResults = m_pSensor->GetTemps();

    pDataType = pMBData->add_data();
    pDataType->set_dataname("Temperatures: ");

    for(int i = 0; i < 3; i++)
    {
        if (!pResults[i])
            continue;
        qzTemp1.sprintf("Temperature #%d", i + 1);
        qzTemp2.sprintf("%.1fC", pResults[i]);

        pItemPair = pDataType->add_datavalue();
        pItemPair->set_name(qzTemp1.toLatin1().data());
        pItemPair->set_value(qzTemp2.toLatin1().data());
    }

    // voltages
    pDataType = pMBData->add_data();
    pDataType->set_dataname("Voltages: ");

    pVoltages = m_pSensor->GetVoltages();

    for(int i = 0; i < 4; i++)
    {
        pItemPair = pDataType->add_datavalue();
        pItemPair->set_name(pVoltages[i].qzName.toLatin1().data());
        pItemPair->set_value(pVoltages[i].qzValue.toLatin1().data());
    }

    // fan speed
    pDataType = pMBData->add_data();
    pDataType->set_dataname("Fan speed: ");

    pResults = m_pSensor->GetFanSpeeds();

    for(int i = 0; i < 5; i++)
    {
        if (!pResults[i])
            continue;
        qzTemp1.sprintf("Fan #%d", i + 1);
        qzTemp2.sprintf("%.1fRPM", pResults[i]);

        pItemPair = pDataType->add_datavalue();
        pItemPair->set_name(qzTemp1.toLatin1().data());
        pItemPair->set_value(qzTemp2.toLatin1().data());
    }

CPU:
    if (NULL == m_pCpuSensor)
        goto RAM;

    pCpuData = pSensorData.mutable_cpudata();
    pCpuData->set_name(m_pSensorsManager->GetCpuName().toLatin1().data());

    pDataType = pCpuData->add_data();
    pDataType->set_dataname("Temperatures: ");

    m_pCpuSensor->Update();
    pResults = m_pCpuSensor->GetTemps();

    qzTemp1 = "", qzTemp2 = "";
    for(int i = 0; i < m_pCpuSensor->GetNumberOfCores(); i++)
    {
        if (!pResults[i])
            continue;
        qzTemp1.sprintf("Core #%d", i + 1);
        qzTemp2.sprintf("%.1fC", pResults[i]);

        pItemPair = pDataType->add_datavalue();
        pItemPair->set_name(qzTemp1.toLatin1().data());
        pItemPair->set_value(qzTemp2.toLatin1().data());
    }

    pDataType = pCpuData->add_data();
    pDataType->set_dataname("Usage: ");

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("CPU0: ");
    pItemPair->set_value(QString().sprintf("%.2f%", m_pSensorsManager->GetCpuLoad()).toLatin1().data());

RAM:
    pRAMData = pSensorData.mutable_ramdata();
    pRAMData->set_name("Memory");

    pMemoryStatus = m_pSensorsManager->GetMemoryStat();

    // DATA
    pDataType = pRAMData->add_data();
    pDataType->set_dataname("Data: ");

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Total: ");
    pItemPair->set_value(pMemoryStatus->qzTotalPhys.toLatin1().data());

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Available: ");
    pItemPair->set_value(pMemoryStatus->qzAvailPhys.toLatin1().data());

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Total (Virtual): ");
    pItemPair->set_value(pMemoryStatus->qzTotalVirtual.toLatin1().data());

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Available (Virtual): ");
    pItemPair->set_value(pMemoryStatus->qzAvailVirtual.toLatin1().data());

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Pagefile (Total): ");
    pItemPair->set_value(pMemoryStatus->qzTotalPageFile.toLatin1().data());

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Pagefile (Available): ");
    pItemPair->set_value(pMemoryStatus->qzAvailPageFile.toLatin1().data());

    // LOAD
    pDataType = pRAMData->add_data();
    pDataType->set_dataname("Load: ");

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Value");
    pItemPair->set_value(pMemoryStatus->qzMemoryLoad.toLatin1().data());

    emit OnSetSensorsInformations(pSensorData.SerializeAsString());
}

void Controller::OnRequestDeviceDetailsSlot(QString qzDeviceID)
{
    QStandardItemModel *pModel = m_pDeviceManager->GetDeviceProperties(qzDeviceID);

    if (0 != pModel)
        emit OnSetDevicePropertiesInformation(pModel);
}

void Controller::OnRequestDMIItemProperties(DMIModuleType ItemType)
{
    QStandardItemModel *pModel = m_pDMIManager->GetItemPropertiesModel(ItemType);

    if (0 != pModel)
        emit OnSetDMIPropertiesInfomation(pModel);
}

void Controller::OnRequestATAItemProperties(QString qzModel)
{
    QString qzDrive = m_HDDModelToPhysicalDrive.value(qzModel);
    ATADeviceProperties *pProp = GetATADeviceProperties(qzDrive.toStdWString().c_str());
    CHECK_ALLOCATION(pProp);

    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pItem = 0;

    pModel->setColumnCount(2);
    pModel->setRowCount(15);
    pModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    pItem = new QStandardItem("Model:");
    pModel->setItem(0, 0, pItem);
    pItem = new QStandardItem(pProp->Model);
    pModel->setItem(0, 1, pItem);
    pItem = new QStandardItem("Firmware revision:");
    pModel->setItem(1, 0, pItem);
    pItem = new QStandardItem(pProp->FirmwareRevision);
    pModel->setItem(1, 1, pItem);
    pItem = new QStandardItem("Serial number:");
    pModel->setItem(2, 0, pItem);
    pItem = new QStandardItem(pProp->SerialNumber);
    pModel->setItem(2, 1, pItem);
    pItem = new QStandardItem("Rotation speed:");
    pModel->setItem(3, 0, pItem);
    pItem = new QStandardItem(pProp->RotationSpeed);
    pModel->setItem(3, 1, pItem);
    pItem = new QStandardItem("Buffer size:");
    pModel->setItem(4, 0, pItem);
    pItem = new QStandardItem(pProp->BufferSize);
    pModel->setItem(4, 1, pItem);
    pItem = new QStandardItem("Type:");
    pModel->setItem(5, 0, pItem);
    pItem = new QStandardItem(pProp->DeviceType);
    pModel->setItem(5, 1, pItem);
    pItem = new QStandardItem("Cylinders:");
    pModel->setItem(6, 0, pItem);
    pItem = new QStandardItem(pProp->Cylinders);
    pModel->setItem(6, 1, pItem);
    pItem = new QStandardItem("Heads:");
    pModel->setItem(7, 0, pItem);
    pItem = new QStandardItem(pProp->Heads);
    pModel->setItem(7, 1, pItem);
    pItem = new QStandardItem("Sectors per track:");
    pModel->setItem(8, 0, pItem);
    pItem = new QStandardItem(pProp->SectorPerTrack);
    pModel->setItem(8, 1, pItem);
    pItem = new QStandardItem("Bytes per sector:");
    pModel->setItem(9, 0, pItem);
    pItem = new QStandardItem(pProp->BytesPerSector);
    pModel->setItem(9, 1, pItem);
    pItem = new QStandardItem("ATA standard:");
    pModel->setItem(10, 0, pItem);
    pItem = new QStandardItem(pProp->ATAStandard);
    pModel->setItem(10, 1, pItem);
    pItem = new QStandardItem("UDMA transfer mode:");
    pModel->setItem(11, 0, pItem);
    pItem = new QStandardItem(pProp->UDMATransferMode);
    pModel->setItem(11, 1, pItem);
    pItem = new QStandardItem("Active UDMA transfer mode:");
    pModel->setItem(12, 0, pItem);
    pItem = new QStandardItem(pProp->ActiveUDMATransferMode);
    pModel->setItem(12, 1, pItem);
    pItem = new QStandardItem("PIO transfer mode");
    pModel->setItem(13, 0, pItem);
    pItem = new QStandardItem(pProp->PIOTransferMode);
    pModel->setItem(13, 1, pItem);
    pItem = new QStandardItem("MWDMA transfer mode:");
    pModel->setItem(14, 0, pItem);
    pItem = new QStandardItem(pProp->MWDMATransferMode);
    pModel->setItem(14, 1, pItem);

    emit OnSetATAItemPropertiesInformation(pModel);
}

void Controller::OnRequestSMARTProperties(QString qzModel)
{
    QStandardItemModel *pModel = m_pSmartManager->GetSMARTPropertiesForHDD(qzModel);

    if (0 != pModel)
        emit OnSetSMARTItemPropertiesInformation(pModel);
}

void Controller::OnUninstallApplicationSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnRequestNetworkDeviceInfomationsSlot(QString qzAdapterName)
{
    if (m_pNetworkDevicesManager)
    {
       QStandardItemModel *pModel = m_pNetworkDevicesManager->GetAdapterInformations(qzAdapterName);

       if (pModel)
           emit OnSetNetworkDeviceInformation(pModel);
    }
}

void Controller::OnRequestModulesInformationsSlot(int nPid)
{
    if( m_pProcessesManager )
    {
        QStandardItemModel *pModel = m_pProcessesManager->GetModulesInformationsForProcess(nPid);

        if (0 != pModel)
            emit OnSetModulesInformations(pModel);
    }
}

void Controller::OnCancelSensorsTimerSlot()
{
    m_pSensorsTimer->stop();
}
