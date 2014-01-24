#include "../headers/system_drivers.h"

SystemDrivers::SystemDrivers():
    m_qszWinDir(""),
    m_pTableModel(NULL),
    m_pTableView(NULL)
{
    m_pTableModel = new QStandardItemModel();
}

int SystemDrivers::Initialize()
{
    int nStatus = Uninitialized;

    SC_HANDLE hHandle = 0;
    SC_HANDLE hService = 0;

    BOOL bReturnedValue = 0;

    DWORD dwBytesNeeded = 0;
    DWORD dwResumeHandle = 0;
    DWORD dwServicesReturned = 0;
    DWORD dwBytes = 0;

    TCHAR wszWindir[MAX_PATH];
    GetWindowsDirectory(wszWindir, MAX_PATH);
    m_qszWinDir = WcharArrayToQString(wszWindir);

    LPBYTE pBuffer = 0;
    SystemDriverInfo *item = 0;

    hHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if( NULL == hHandle )
    {
        nStatus = InvalidHandle;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    bReturnedValue = EnumServicesStatusEx(hHandle,
                                          SC_ENUM_PROCESS_INFO,
                                          SERVICE_DRIVER,
                                          SERVICE_STATE_ALL,
                                          NULL,
                                          0,
                                          &dwBytesNeeded,
                                          &dwServicesReturned,
                                          &dwResumeHandle,
                                          NULL);
    if( 0 != bReturnedValue )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( ERROR_MORE_DATA != GetLastError() )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    dwBytes = sizeof(ENUM_SERVICE_STATUS_PROCESS) + dwBytesNeeded;
    ENUM_SERVICE_STATUS_PROCESS* pServices = NULL;
    pServices = new ENUM_SERVICE_STATUS_PROCESS [dwBytes];

    bReturnedValue = EnumServicesStatusEx(hHandle,
                                          SC_ENUM_PROCESS_INFO,
                                          SERVICE_DRIVER,
                                          SERVICE_STATE_ALL,
                                          (LPBYTE)pServices,
                                          dwBytes,
                                          &dwBytesNeeded,
                                          &dwServicesReturned,
                                          &dwResumeHandle,
                                          NULL);
    if( 0 == bReturnedValue )
    {
        // error
    }
    for(unsigned int i = 0; i < dwServicesReturned; i++)
    {
        item = new SystemDriverInfo;
        if( 0 == item )
        {
            nStatus = NotAllocated;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }
        // driver's name
        item->qszDriverName = QString().fromWCharArray(pServices[i].lpServiceName);
        // driver's type
        switch(pServices[i].ServiceStatusProcess.dwServiceType)
        {
        case SERVICE_KERNEL_DRIVER:
            item->qszType = QString("Kernel driver");
            break;
        case SERVICE_FILE_SYSTEM_DRIVER:
            item->qszType = QString("File system driver");
            break;
        default:
            item->qszType = QString("N/A");
            break;
        }

        // driver's currentState
        switch(pServices[i].ServiceStatusProcess.dwCurrentState)
        {
        case SERVICE_CONTINUE_PENDING:
            item->qszState = QString("The service is about to continue");
            break;
        case SERVICE_PAUSE_PENDING:
            item->qszState = QString("The service is pausing");
            break;
        case SERVICE_PAUSED:
            item->qszState = QString("The service is paused");
            break;
        case SERVICE_RUNNING:
            item->qszState = QString("The service is running");
            break;
        case SERVICE_START_PENDING:
            item->qszState = QString("The service is starting");
            break;
        case SERVICE_STOP_PENDING:
            item->qszState = QString("The service is stopping");
            break;
        case SERVICE_STOPPED:
            item->qszState = QString("The service has stopped");
            break;
        }

        hService = OpenService(hHandle, pServices[i].lpServiceName, SERVICE_QUERY_CONFIG);
        if( 0 == hService )
        {
            nStatus = InvalidHandle;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }

        bReturnedValue = QueryServiceConfig(hService, NULL, 0, &dwBytesNeeded);
        if( 0 != bReturnedValue )
        {
            nStatus = Unsuccessful;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }
        if( ERROR_INSUFFICIENT_BUFFER != GetLastError() )
        {
            nStatus = Unsuccessful;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }

        dwBytes = dwBytesNeeded;
        pBuffer = new BYTE[dwBytesNeeded];
        if( 0 == pBuffer )
        {
            nStatus = NotAllocated;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }
        bReturnedValue = QueryServiceConfig(hService,(QUERY_SERVICE_CONFIG*)pBuffer, dwBytes, &dwBytesNeeded);
        if( 0 == bReturnedValue )
        {
            nStatus = Unsuccessful;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }
        QUERY_SERVICE_CONFIG *pBufferQ = (QUERY_SERVICE_CONFIG*)pBuffer;
        // driver's filename
        item->qszFileName = (QString().fromWCharArray(pBufferQ->lpBinaryPathName).split("\\")).back();
        // driver's description
        item->qszDescription = QString().fromWCharArray(pBufferQ->lpDisplayName);
        // driver's version
        item->qszVersion = GetDriverVersion(pBufferQ->lpBinaryPathName);

        m_qlSystemDrivers.append(item);
    }
    nStatus = Success;
    return nStatus;
}

int SystemDrivers::Destroy()
{
    for(int i = 0; i < m_qlSystemDrivers.count(); i++)
    {
        delete m_qlSystemDrivers.at(i);
    }
    m_qlSystemDrivers.clear();

    return Success;
}

void SystemDrivers::OnStartLoadingModuleDataSlot()
{
    this->Initialize();
    emit OnLoadingModuleDataCompleteSignal();
    emit OnCreateWidgetSignal(OS_SYSTEM_DRIVERS, this);
}

int SystemDrivers::OnCreateWidget(QWidget **ppWidget)
{
    QWidget *pWidget = new QWidget();
    pWidget->setLayout(new QVBoxLayout());
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);

    m_pTableView = new QTableView();
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_pTableView->verticalHeader()->hide();
    m_pTableView->setShowGrid(false);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableView->setFocusPolicy(Qt::NoFocus);

    pWidget->layout()->addWidget(m_pTableView);

    this->PopulateModel();

    (*ppWidget) = pWidget;

    return Success;
}

QString SystemDrivers::GetDriverVersion(WCHAR *wszFileName)
{
    DWORD dwVersionHandle = NULL;
    QString qszFileName = "";
    QString returnValue = "N/A";
    UINT unSize = 0;
    LPBYTE lpBuffer = 0;
    DWORD dwVersionSize = 0;
    WCHAR lpFilePath[MAX_PATH] = {0};

    qszFileName = (QString().fromWCharArray(wszFileName)).replace("SystemRoot", m_qszWinDir);
    qszFileName = qszFileName.right(qszFileName.count()-1);
    qszFileName.toWCharArray(lpFilePath);

    dwVersionSize = GetFileVersionInfoSize(lpFilePath,&dwVersionHandle);

    if( 0 != dwVersionSize )
    {
        LPSTR lpVersionData = new char[dwVersionSize];

        if( GetFileVersionInfo( lpFilePath, dwVersionHandle, dwVersionSize, (LPVOID)lpVersionData ) )
        {
            if (VerQueryValue(lpVersionData, L"\\",(VOID FAR* FAR*)&lpBuffer, &unSize))
            {
                if ( 0 != unSize )
                {
                    VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        int nMajor = HIWORD(verInfo->dwFileVersionMS);
                        int nMinor = LOWORD(verInfo->dwFileVersionMS);
                        int nRev = HIWORD(verInfo->dwFileVersionLS);
                        int nBuild = LOWORD(verInfo->dwFileVersionLS);
                        returnValue = QString().setNum(nMajor) + QString(".") + QString().setNum(nMinor) + QString(".");
                        returnValue += QString().setNum(nRev) + QString(".") + QString().setNum(nBuild);
                    }
                }
            }
        }
        delete[] lpVersionData;
    }
    return returnValue;
}

void SystemDrivers::PopulateModel()
{
    QStandardItem *pItem = 0;
    SystemDriverInfo *pDriver = 0;
    m_pTableModel->clear();

    m_pTableModel->setColumnCount(6);
    m_pTableModel->setHorizontalHeaderLabels(QStringList() << "Driver name" <<
                                             "Description" << "File name" <<
                                             "Version" << "Type" << "Current state");
    m_pTableModel->setRowCount(this->m_qlSystemDrivers.count());

    for(int i = 0; i < m_qlSystemDrivers.count(); i++)
    {
        pDriver = m_qlSystemDrivers.at(i);
        pItem = new QStandardItem(pDriver->qszDriverName == "" ? "N/A" : pDriver->qszDriverName);
        m_pTableModel->setItem(i, 0, pItem);

        pItem = new QStandardItem(pDriver->qszDescription == "" ? "N/A" : pDriver->qszDescription);
        m_pTableModel->setItem(i, 1, pItem);

        pItem = new QStandardItem(pDriver->qszFileName == "" ? "N/A" : pDriver->qszFileName);
        m_pTableModel->setItem(i, 2, pItem);

        pItem = new QStandardItem(pDriver->qszVersion == "" ? "N/A" : pDriver->qszVersion);
        m_pTableModel->setItem(i, 3, pItem);

        pItem = new QStandardItem(pDriver->qszType == "" ? "N/A" : pDriver->qszType);
        m_pTableModel->setItem(i, 4, pItem);

        pItem = new QStandardItem(pDriver->qszState == "" ? "N/A" : pDriver->qszState);
        m_pTableModel->setItem(i, 5, pItem);
    }

    emit OnSendModuleDataToGUISignal(OS_SYSTEM_DRIVERS, m_pTableView, m_pTableModel);
}

