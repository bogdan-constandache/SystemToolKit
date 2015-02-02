#include "controller.h"

#pragma warning(disable:4996)

int Controller::OnLoadDriverFile()
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    SYSTEM_INFO SysInfo;
    GetNativeSystemInfo(&SysInfo);
    OSVERSIONINFOEX OsVersion;
    OsVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((LPOSVERSIONINFO)&OsVersion);

    BOOL bResult = FALSE;

    // Compute driver's path
    QString qzDriverPath = QCoreApplication::applicationDirPath();
    qzDriverPath += "/config/drivers/";
    if( OsVersion.dwMajorVersion == 6 && (OsVersion.dwMinorVersion == 3 || OsVersion.dwMinorVersion == 2) &&
            OsVersion.wProductType == VER_NT_WORKSTATION )
        qzDriverPath += "win8/";
    if( OsVersion.dwMajorVersion == 6 && OsVersion.dwMinorVersion == 1  &&
            OsVersion.wProductType == VER_NT_WORKSTATION )
        qzDriverPath += "win7/";
    if( OsVersion.dwMajorVersion == 6 && OsVersion.dwMinorVersion == 0 &&
            OsVersion.wProductType == VER_NT_WORKSTATION )
        qzDriverPath += "vista/";

    if( SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
        qzDriverPath += "stk_driver64.sys";
    else if( SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
        qzDriverPath += "stk_driver.sys";

    qzDriverPath.replace("/", "\\");

    WCHAR *lpDriverPath = CharArrayToWcharArray(qzDriverPath.toLatin1().data());
    if( NULL == lpDriverPath )
        CHECK_OPERATION_STATUS_EX(Unsuccessful);

    // Create service
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if( NULL == hSCManager )
        CHECK_OPERATION_STATUS_EX(Unsuccessful);

    hService = CreateService(hSCManager, L"STKSensorService", L"STKSensorService",
                             SERVICE_START, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
                             SERVICE_ERROR_NORMAL, lpDriverPath, NULL, NULL, NULL,
                             NULL, NULL);

    if( NULL == hService )
    {
        if( ERROR_SERVICE_EXISTS != GetLastError() )
        {
            CloseServiceHandle(hSCManager);
            CHECK_OPERATION_STATUS_EX(Unsuccessful);
        }
        else
        {
            hService = OpenService(hSCManager, L"STKSensorService", SERVICE_ALL_ACCESS);
            if( NULL == hService )
            {
                CloseServiceHandle(hSCManager);
                CHECK_OPERATION_STATUS_EX(Unsuccessful);
            }
        }
    }

    bResult = StartService(hService, 0, NULL);
    if( FALSE == bResult  )
    {
        DWORD dwLastError = GetLastError();
        if( ERROR_SERVICE_ALREADY_RUNNING != dwLastError )
        {
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            qDebug() << "LastErr: " << dwLastError;
            CHECK_OPERATION_STATUS_EX(Unsuccessful);
        }
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return Success;
}

int Controller::OnUnloadDriverFile()
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    BOOL bResult = FALSE;
    SERVICE_STATUS ServStat;

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if( NULL == hSCManager )
        return Unsuccessful;

    hService = OpenService(hSCManager, L"STKSensorService", SERVICE_ALL_ACCESS);
    if( NULL == hService )
    {
        CloseServiceHandle(hSCManager);
        return Unsuccessful;
    }

    bResult = ControlService(hService, SERVICE_CONTROL_STOP, &ServStat);
    if( FALSE == bResult )
    {
        if( ERROR_SERVICE_NOT_ACTIVE != GetLastError() )
        {
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            return Unsuccessful;
        }
    }

    bResult = DeleteService(hService);
    if( FALSE == bResult )
    {
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return Unsuccessful;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return Success;
}

int Controller::AssignStandardModelsToUi()
{
    QStandardItemModel *pModel = 0;
    // Device Manager Models
    pModel = m_pDeviceManager->GetAllDeviceDetails();
    if( pModel )
        emit OnSetDeviceManagerInformation(pModel);
    pModel = m_pDeviceManager->GetDeviceProperties();
    if( pModel )
        emit OnSetDevicePropertiesInformation(pModel);

    // DMI Manager Models
    pModel = m_pDMIManager->GetItemsModel();
    if( pModel )
        emit OnSetDMIItemsInformation(pModel);
    pModel = m_pDMIManager->GetItemPropertiesModel();
    if( pModel )
        emit OnSetDMIPropertiesInfomation(pModel);

    // Battery status Models
    pModel = m_pBatteryStatus->GetBatteryInformation();
    if( pModel )
        emit OnSetPowerManagementInformation(pModel);

    // CPUID manager models
    pModel = m_pCPUIDManager->GetCPUIDDataModel();
    if( pModel )
        emit OnSetCPUIDInformations(pModel); // HEAP CORRUPTION

    // SPD models
    pModel = m_pSPDManager->GetDimmsModel();
    if( pModel )
        emit OnSetAvailableDIMMSInformation(pModel);
    pModel = m_pSPDManager->GetDimmsInformationModel();
    if( pModel )
        emit OnSetDimmSPDInformation(pModel);

    // Nvidia models
    pModel = m_pNVidiaManager->GetPhysicalGPUModel();
    if( pModel )
    {
        m_nIsNvidia = 1;
        emit OnSetAvailableVCardsInformation(pModel);
        pModel = m_pNVidiaManager->GetGPUDetailsModel();
        if( pModel )
            emit OnSetVCardInfromation(pModel);
    }
    else
    {
        m_nIsNvidia = 0;
        pModel = m_pRadeonManager->GetPhysicalGPUModel();
        emit OnSetAvailableVCardsInformation(pModel);
        pModel = m_pRadeonManager->GetGPUDetailsModel();
        if( pModel )
            emit OnSetVCardInfromation(pModel);
    }

    // Operating system models
    pModel = m_pOperatingSystemManager->GetOSModelInformation();
    if( pModel )
        emit OnSetOperatingSystemInformation(pModel);

    // System users models
    pModel = m_pUserInformationManager->GetUserInformations();
    if( pModel )
        emit OnSetUsersInformations(pModel);

    // System drivers models
    pModel = m_pSystemDriversManager->GetSystemDriversInformation();
    if( pModel )
        emit OnSetSystemDriversModelInformation(pModel);

    // Process manager models
    pModel = m_pProcessesManager->GetProcessesInformations();
    if( pModel )
        emit OnSetProcessesInformations(pModel);
    pModel = m_pProcessesManager->GetModulesInformationsForProcess();
    if( pModel )
        emit OnSetModulesInformations(pModel);

    // Storage ATA models
    emit OnSetATAHDDItemsInformation(m_pATAHdds);
    emit OnSetATAItemPropertiesInformation(m_pATAProp);

    // Storage SMART models
    emit OnSetSMARTHDDItemsInformation(m_pSmartManager->GetAvailableHDD());
    emit OnSetSMARTItemPropertiesInformation(m_pSmartManager->GetSMARTPropertiesForHDD());

    // Active connections models
    emit OnSetActiveConnectionsInformation(m_pActiveConnectionsManager->GetActiveConnections());

    // Network devices models
    emit OnSetNetworkDevicesNames(m_pNetworkDevicesManager->GetAdapterNames());
    emit OnSetNetworkDeviceInformation(m_pNetworkDevicesManager->GetAdapterInformations());

    // Startup applications models
    emit OnSetStartupApplicationsInformations(m_pStartupAppsManager->GetDataModel());

    return Success;
}

Controller::Controller(): m_pBatteryStatus(NULL), m_pApplicationManager(NULL),
    m_pDMIManager(NULL), m_pSmartManager(NULL), m_pSystemDriversManager(NULL),
    m_pActiveConnectionsManager(NULL), m_pNetworkDevicesManager(NULL), m_pCPUIDManager(NULL),
    m_pSensorsManager(NULL), m_pSensor(NULL), m_pSensorsTimer(NULL), m_pCpuSensor(NULL), m_pNVidiaManager(NULL),
    m_pProcessesManager(NULL), m_pStartupAppsManager(NULL), m_pComputerSummaryModel(NULL), m_pDeviceManager(NULL),
    m_pUserInformationManager(NULL), m_pUninstallerProcess(NULL), m_pSPDManager(NULL), m_pOperatingSystemManager(NULL),
    m_pATAHdds(NULL), m_pATAProp(NULL), m_nIsNvidia(-1)
{
    m_pSensorsTimer = new QTimer(this);
    connect(m_pSensorsTimer, SIGNAL(timeout()), this, SLOT(OnComputerSensorsOptClickedSlot()), Qt::QueuedConnection);
    m_pGPUTimer = new QTimer(this);
    connect(m_pGPUTimer, SIGNAL(timeout()), this, SLOT(OnRefreshVCardInformations()), Qt::QueuedConnection);

    connect(this, SIGNAL(OnCancelSensorsTimerSignal()), this, SLOT(OnCancelSensorsTimerSlot()), Qt::QueuedConnection);

    qDebug() << "Driver Loading status: " << OnLoadDriverFile();

    // Create device manager obj
    m_pDeviceManager = new CDeviceInfo();
    // Create DMI manager obj
    m_pDMIManager = new CSMBiosEntryPoint();
    // Create battery manager obj
    m_pBatteryStatus = new BatteryStatus();

    // Create CPUID manager obj
    m_pCPUIDManager = new CCPUIDManager();
    // Create SPD manager obj
    m_pSPDManager = new CSPDInformation();

    // Create nvidia manager obj
    m_pNVidiaManager = new CNvidiaManager();
    // Create radeon manager obj
    m_pRadeonManager = new CAmdCard();

    // Create operating system manager obj
    m_pOperatingSystemManager = new COperatingSystemInformation();
    // Create system users manager obj
    m_pUserInformationManager = new CSystemUsersInformation();
    // Create system drivers manager obj
    m_pSystemDriversManager = new SystemDrivers();
    // Create process manager obj
    m_pProcessesManager = new Processes();

    // Create ATA objects
    m_pATAHdds = new QStandardItemModel();
    m_pATAProp = new QStandardItemModel();
    // Populate hdd list
    QStringList qHddList = GetPhysicalDrivesList();
    m_pATAHdds->setHorizontalHeaderLabels(QStringList() << "Devices:");
    foreach(QString qsDrive, qHddList)
    {
        ATADeviceProperties *pProperty = GetATADeviceProperties(qsDrive.toStdWString().c_str());
        if( NULL == pProperty )
            continue;
        QStandardItem *pStandardItem = new QStandardItem(pProperty->Model);
        pStandardItem->setData(qsDrive, ATA_HDD_ID_TAG);
        pStandardItem->setIcon(QIcon(":/img/hdd.png"));

        m_pATAHdds->appendRow(pStandardItem);

        delete pProperty;
    }
    // Create SMART manager obj
    m_pSmartManager = new CSmartInfo();

    // Create Active connection manager obj
    m_pActiveConnectionsManager = new CActiveConnections();
    // Create Network devices manager obj
    m_pNetworkDevicesManager = new CNetworkDevices();

    // Create Startup manager obj
    m_pStartupAppsManager = new CStartupManager();
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
//    SAFE_DELETE(m_pCPUIDManager); // HEAP CORRUPTION
    SAFE_DELETE(m_pSensorsManager)
    SAFE_DELETE(m_pProcessesManager);
    SAFE_DELETE(m_pStartupAppsManager);
    SAFE_DELETE(m_pUserInformationManager);
    SAFE_DELETE(m_pSPDManager);
    SAFE_DELETE(m_pNVidiaManager);
    SAFE_DELETE(m_pRadeonManager);
    SAFE_DELETE(m_pOperatingSystemManager);

    SAFE_DELETE(m_pATAHdds);
    SAFE_DELETE(m_pATAProp);

    m_HDDModelToPhysicalDrive.clear();

    OnUnloadDriverFile();

    if( NULL != m_pUninstallerProcess )
    {
        m_pUninstallerProcess->Stop();
        m_pUninstallerProcess->WaitForFinished();

        SAFE_DELETE(m_pUninstallerProcess);
    }

    CoUninitialize();
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

    // CREATE UNINSTALLER PROCESS
    m_pUninstallerProcess = new QProcessWrapper();
    connect(m_pUninstallerProcess, SIGNAL(OnErrorSignal(QString)), this, SLOT(OnUninstallApplicationErrorReportSlot(QString)), Qt::QueuedConnection);

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
    pChildItem->setData(COMPUTER_DEVICE_MANAGER_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("DMI"));
    pChildItem->setData(COMPUTER_DMI_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Power management"));
    pChildItem->setData(COMPUTER_POWER_MANAGEMENT_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Sensors"));
    pChildItem->setData(COMPUTER_SENSORS_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);


    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Motherboard"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("CPU"));
    pChildItem->setData(MOTHERBOARD_CPU_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("SPD"));
    pChildItem->setData(MOTHERBOARD_SPD_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Video card"));
    pChildItem->setData(MOTHERBOARD_VIDEO_CARD_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Operating system"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Operating system"));
    pChildItem->setData(OPERATING_SYSTEM_SUMMARY_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Processes"));
    pChildItem->setData(OPERATING_SYSTEM_PROCESSES_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("System drivers"));
    pChildItem->setData(OPERATING_SYSTEM_SYSTEM_DRIVERS_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("System users"));
    pChildItem->setData(OPERATING_SYSTEM_USERS_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Storage"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("ATA"));
    pChildItem->setData(STORAGE_ATA_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("SMART"));
    pChildItem->setData(STORAGE_SMART_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Network"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Network devices"));
    pChildItem->setData(NETWORK_DEVICES_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Connections"));
    pChildItem->setData(NETWORK_CONNECTIONS_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Software"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Startup applications"));
    pChildItem->setData(SOFTWARE_STARTUP_APPLICATIONS_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Application manager"));
    pChildItem->setData(SOFTWARE_APPLICATION_MANAGER_ID, MENU_OPTION_ID_TAG);
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataRoot->appendRows(qList);
    qList.clear();

    emit OnPopulateMenuTreeSignal(pModel);

    AssignStandardModelsToUi();

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

    if( m_pCpuSensor )
    {
        if( m_pCpuSensor->GetMultiplier() )
            m_pCPUIDManager->GetCPUIDDataModel()->item(9, 1)->setText(QString::number(m_pCpuSensor->GetMultiplier()));
        else
            m_pCPUIDManager->GetCPUIDDataModel()->item(9, 1)->setText("N/A");
    }
}


void Controller::OnDispatchMenuOptionTagSlot(int nOptionTag)
{
    switch(nOptionTag)
    {
    case COMPUTER_DEVICE_MANAGER_ID:
        OnComputerDeviceManagerOptClickedSlot(); break;
    case COMPUTER_DMI_ID:
        OnComputerDMIOptClickedSlot(); break;
    case COMPUTER_POWER_MANAGEMENT_ID:
        OnComputerPowerManagementOptClickedSlot(); break;
    case COMPUTER_SENSORS_ID:
        OnComputerSensorsOptClickedSlot(); break;

    case MOTHERBOARD_CPU_ID:
        OnMotherBoardCPUOptClickedSlot(); break;
    case MOTHERBOARD_SPD_ID:
        OnMotherBoardSPDOptClickedSlot(); break;
    case MOTHERBOARD_VIDEO_CARD_ID:
        OnMotherboardVCardOptClickedSlot(); break;

    case OPERATING_SYSTEM_SUMMARY_ID:
        OnOperatingSystemOptClickedSlot(); break;
    case OPERATING_SYSTEM_PROCESSES_ID:
        OnOperatingSystemProcessesOptClickedSlot(); break;
    case OPERATING_SYSTEM_SYSTEM_DRIVERS_ID:
        OnOperatingSystemDriversOptClickedSlot(); break;
    case OPERATING_SYSTEM_USERS_ID:
        OnOperatingSystemUserInformationsOptClickedSlot(); break;

    case STORAGE_ATA_ID:
        OnStorageATAOptClickedSlot(); break;
    case STORAGE_SMART_ID:
        OnStorageSmartOptClickedSlot(); break;

    case NETWORK_DEVICES_ID:
        OnNetworkDevicesOptClickedSlot(); break;
    case NETWORK_CONNECTIONS_ID:
        OnNetworkConnectionsOptClickedSlot(); break;

    case SOFTWARE_APPLICATION_MANAGER_ID:
        OnSoftwareApplicationManagerOptClickedSlot(); break;
    case SOFTWARE_STARTUP_APPLICATIONS_ID:
        OnSoftwareStartupApplicationsOptClickedSlot(); break;

    default:
        qDebug() << "Invalid Menu Option Clicked";
    }
}


void Controller::OnComputerDeviceManagerOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    m_pDeviceManager->OnRefreshDevices();

    emit OnDeviceManagerInformationDataChanged();
}

void Controller::OnComputerDMIOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnDMIItemsInformationDataChanged();
}

void Controller::OnComputerPowerManagementOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    m_pBatteryStatus->OnRefresh();

    emit OnPowerManagementInformationDataChanged();
}

void Controller::OnComputerSensorsOptClickedSlot()
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
    pItemPair->set_name("Load: ");
    pItemPair->set_value(QString().sprintf("%.2f%", m_pSensorsManager->GetCpuLoad()).toLatin1().data());

RAM:
    pRAMData = pSensorData.mutable_ramdata();
    pRAMData->set_name("Memory");

    pMemoryStatus = m_pSensorsManager->GetMemoryStat();

    // DATA
    pDataType = pRAMData->add_data();
    pDataType->set_dataname("Data: ");

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Total (Physical): ");
    pItemPair->set_value(pMemoryStatus->qzTotalPhys.toLatin1().data());

    pItemPair = pDataType->add_datavalue();
    pItemPair->set_name("Available (Physical): ");
    pItemPair->set_value(pMemoryStatus->qzAvailPhys.toLatin1().data());

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


void Controller::OnMotherBoardCPUOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // Do nothing, model contains static data
    emit OnCPUIDInformationDataChanged();
}

void Controller::OnMotherBoardSPDOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnAvailableDIMMSInformationDataChanged();
}

void Controller::OnMotherboardVCardOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnAvailableVCardsInformationDataChanged();
}


void Controller::OnOperatingSystemOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnOperatingSystemInformationDataChanged();
}

void Controller::OnOperatingSystemProcessesOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    m_pProcessesManager->OnRefreshProcessList();

    emit OnProcessInformationDataChanged();
}

void Controller::OnOperatingSystemDriversOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    m_pSystemDriversManager->OnRefreshData();

    emit OnSystemDriversInformationDataChanged();
}

void Controller::OnOperatingSystemUserInformationsOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnUserInformationDataChanged();
}


void Controller::OnStorageATAOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnATAHDDInformationDataChanged();
}

void Controller::OnStorageSmartOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnSMARTHDDInformationDataChanged();
}


void Controller::OnNetworkConnectionsOptClickedSlot()
{
    // cancel all timers
    emit OnCancelSensorsTimerSignal();

    m_pActiveConnectionsManager->OnRefreshData();

    emit OnActiveConnectionsInformationDataChanged();
}

void Controller::OnNetworkDevicesOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    // Do nothing - Model contains static data
    emit OnNetworkAdaptersInformationDataChanged();
}


void Controller::OnSoftwareApplicationManagerOptClickedSlot()
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

void Controller::OnSoftwareStartupApplicationsOptClickedSlot()
{
    emit OnCancelSensorsTimerSignal();

    m_pStartupAppsManager->OnRefreshData();

    emit OnStartupApplicationInformationDataChanged();
}



void Controller::OnCPUOptClickedSlot()
{

}

void Controller::OnSmbiosOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnHddInformationOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}


void Controller::OnRequestDeviceDetailsSlot(QString qzDeviceID)
{
    m_pDeviceManager->OnRefreshDetails(qzDeviceID);

    emit OnDevicePropertiesInformationChanged();
}

void Controller::OnRequestDMIItemProperties(DMIModuleType ItemType)
{
    m_pDMIManager->OnRefreshData(ItemType);

    emit OnDMIPropertiesInformationDataChanged();
}

void Controller::OnRequestSPDDimmDetailsSlot(int nDimm)
{
    m_pSPDManager->OnRefreshData(nDimm);

    emit OnSPDDimmInformationDataChanged();
}

void Controller::OnRequestVCardInformationSlot(int nIndex)
{
    if( m_nIsNvidia == 1 )
        m_pNVidiaManager->OnRefreshData(nIndex);
    if( m_nIsNvidia == 0 )
        m_pRadeonManager->OnRefreshData(nIndex);

    m_pGPUTimer->setProperty("VCardIndex", nIndex);
    if (!m_pGPUTimer->isActive())
        m_pGPUTimer->start(750);

    emit OnVideoCardInformationDataChanged();
}

void Controller::OnRequestATAItemProperties(QString qzModel)
{
    ATADeviceProperties *pProp = GetATADeviceProperties(qzModel.toStdWString().c_str());
    CHECK_ALLOCATION(pProp);

    m_pATAProp->clear();
    m_pATAProp->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    QList<QStandardItem*> qList;

    qList << new QStandardItem("Model:") << new QStandardItem(pProp->Model);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Firmware revision:") << new QStandardItem(pProp->FirmwareRevision);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Serial number:") << new QStandardItem(pProp->SerialNumber);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Rotation speed:") << new QStandardItem(pProp->RotationSpeed);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Buffer size:") << new QStandardItem(pProp->BufferSize == "" ? "N/A (SSD)" : pProp->BufferSize);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Type:") << new QStandardItem(pProp->DeviceType);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Cylinders:") << new QStandardItem(pProp->Cylinders);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Heads:") << new QStandardItem(pProp->Heads);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Sectors per track:") << new QStandardItem(pProp->SectorPerTrack);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Bytes per sector:") << new QStandardItem(pProp->BytesPerSector);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("ATA standard:") << new QStandardItem(pProp->ATAStandard);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("UDMA transfer mode:") << new QStandardItem(pProp->UDMATransferMode);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("Active UDMA transfer mode:") << new QStandardItem(pProp->ActiveUDMATransferMode);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("PIO transfer mode") << new QStandardItem(pProp->PIOTransferMode);
    m_pATAProp->appendRow(qList); qList.clear();

    qList << new QStandardItem("MWDMA transfer mode:") << new QStandardItem(pProp->MWDMATransferMode);
    m_pATAProp->appendRow(qList); qList.clear();


    emit OnATAItemPropertiesDataChanged();
}

void Controller::OnRequestSMARTProperties(QString qzModel)
{
    m_pSmartManager->OnRefreshData(qzModel);

    emit OnSMARTItemPropertiesDataChanged();
}

void Controller::OnUninstallApplicationSlot(QString qzUninstallString)
{
    if( !m_pUninstallerProcess )
        return;

    m_pUninstallerProcess->StartProcess(qzUninstallString);
}

void Controller::OnRequestNetworkDeviceInfomationsSlot(QString qzAdapterName)
{
    m_pNetworkDevicesManager->OnRefreshAdapterData(qzAdapterName);

    emit OnNetworkAdapterPropertiesDataChanged();
}

void Controller::OnRequestModulesInformationsSlot(int nPid)
{
    m_pProcessesManager->OnRefreshModuleList(nPid);

    emit OnProcessModuleInformationDataChanged();
}

void Controller::OnRemoveStartupApplicationSlot(QString qsApplication)
{
    m_pStartupAppsManager->OnRemoveApplication(qsApplication);

    OnSoftwareStartupApplicationsOptClickedSlot();
}

void Controller::OnChangeStartupApplicationStateSlot(QString qsApplication)
{
    m_pStartupAppsManager->OnChangeApplicationState(qsApplication);

    OnSoftwareStartupApplicationsOptClickedSlot();
}

void Controller::OnAddStartupApplicationSlot(QString qsAppName, QString qsPath)
{
    m_pStartupAppsManager->OnAddApplication(qsAppName, qsPath);

    OnSoftwareStartupApplicationsOptClickedSlot();
}

void Controller::OnCancelSensorsTimerSlot()
{
    m_pSensorsTimer->stop();
    m_pGPUTimer->stop();
}

void Controller::OnRefreshVCardInformations()
{
    if( m_nIsNvidia == 1 )
        m_pNVidiaManager->OnRefreshData(m_pGPUTimer->property("VCardIndex").toInt());
    if( m_nIsNvidia == 0 )
        m_pRadeonManager->OnRefreshData(m_pGPUTimer->property("VCardIndex").toInt());


    emit OnVideoCardInformationDataChanged();
}

void Controller::OnUninstallApplicationErrorReportSlot(QString qzErrorMessage)
{
    qDebug() << qzErrorMessage;
}
