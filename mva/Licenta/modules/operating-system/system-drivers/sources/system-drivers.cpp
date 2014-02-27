#include "../headers/system_drivers.h"

SystemDrivers::SystemDrivers():
    m_qszWinDir("")
{
}

SystemDrivers::~SystemDrivers()
{
    Destroy();
}

QStandardItemModel *SystemDrivers::GetSystemDriversInformation()
{
    int nStatus = Initialize();
    if( Success != nStatus )
    {
        return 0;
    }

    QStandardItemModel *pModel = new QStandardItemModel;
    QStandardItem *pItem = 0;
    SystemDriverInfo *pDriver = 0;

    pModel->setColumnCount(6);
    pModel->setRowCount(this->m_qlSystemDrivers.count());
    pModel->setHorizontalHeaderLabels(QStringList() << "Driver name" <<
                                             "Description" << "File name" <<
                                             "Version" << "Type" << "Current state");

    for(int i = 0; i < m_qlSystemDrivers.count(); i++)
    {
        pDriver = m_qlSystemDrivers.at(i);
        pItem = new QStandardItem(pDriver->qszDriverName == "" ? "N/A" : pDriver->qszDriverName);
        pModel->setItem(i, 0, pItem);

        pItem = new QStandardItem(pDriver->qszDescription == "" ? "N/A" : pDriver->qszDescription);
        pModel->setItem(i, 1, pItem);

        pItem = new QStandardItem(pDriver->qszFileName == "" ? "N/A" : pDriver->qszFileName);
        pModel->setItem(i, 2, pItem);

        pItem = new QStandardItem(pDriver->qszVersion == "" ? "N/A" : pDriver->qszVersion);
        pModel->setItem(i, 3, pItem);

        pItem = new QStandardItem(pDriver->qszType == "" ? "N/A" : pDriver->qszType);
        pModel->setItem(i, 4, pItem);

        pItem = new QStandardItem(pDriver->qszState == "" ? "N/A" : pDriver->qszState);
        pModel->setItem(i, 5, pItem);
    }

    return pModel;
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

