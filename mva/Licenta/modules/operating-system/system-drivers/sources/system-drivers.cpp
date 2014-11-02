#include "../headers/system_drivers.h"

SystemDrivers::SystemDrivers():
    m_qszWinDir(""), m_pDataModel(NULL)
{
    m_pDataModel = new QStandardItemModel();
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Driver name" <<
                                            "Description" << "File name" <<
                                            "Version" << "Type" << "Current state");
}

SystemDrivers::~SystemDrivers()
{
    for(int i = 0; i < m_qlSystemDrivers.count(); i++)
    {
        delete m_qlSystemDrivers.at(i);
    }
    m_qlSystemDrivers.clear();

    SAFE_DELETE(m_pDataModel);
}

QStandardItemModel *SystemDrivers::GetSystemDriversInformation()
{
    return m_pDataModel;
}

void SystemDrivers::OnRefreshData()
{
    for(int i = 0; i < m_qlSystemDrivers.count(); i++)
    {
        delete m_qlSystemDrivers.at(i);
    }
    m_qlSystemDrivers.clear();

    int nStatus = Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    m_pDataModel->clear();
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Driver name" <<
                                            "Description" << "File name" <<
                                            "Version" << "Type" << "Current state");

    QList<QStandardItem*> qList;
    QStandardItem *pItem = 0;

    foreach(SystemDriverInfo *pDriver, m_qlSystemDrivers)
    {
        pItem = new QStandardItem(pDriver->qszDriverName == "" ? "N/A" : pDriver->qszDriverName);
        pItem->setIcon(QIcon(":/img/windows_service.png"));
        qList << pItem;
        qList << new QStandardItem(pDriver->qszDescription == "" ? "N/A" : pDriver->qszDescription);
        qList << new QStandardItem(pDriver->qszFileName == "" ? "N/A" : pDriver->qszFileName);
        qList << new QStandardItem(pDriver->qszVersion == "" ? "N/A" : pDriver->qszVersion);
        qList << new QStandardItem(pDriver->qszType == "" ? "N/A" : pDriver->qszType);
        qList << new QStandardItem(pDriver->qszState == "" ? "N/A" : pDriver->qszState);

        m_pDataModel->appendRow(qList); qList.clear();
    }
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
        goto CleanUp;
    }
    if( ERROR_MORE_DATA != GetLastError() )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        goto CleanUp;
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
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        goto CleanUp;
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
            delete item;
            CloseServiceHandle(hService);
            continue;
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

CleanUp:

    CloseServiceHandle(hHandle);

    return nStatus;
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

