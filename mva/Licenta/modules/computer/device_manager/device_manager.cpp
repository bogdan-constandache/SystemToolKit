#include "device_manager.h"
#include <QDebug>

#define SAFE_DELETE(X) if(X) { delete X; X = 0; }
#define CHECK_OPERATION(X) if(!X) { qDebug() << "DeviceManager: Operation failed - line: " << __LINE__; }

CDeviceInfo::CDeviceInfo()
{
    m_usDeviceIndex = -1;
    m_hDeviceInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if( INVALID_HANDLE_VALUE == m_hDeviceInfo )
        qDebug() << "SetupDiGetClassDevs - Failed";

    m_pDeviceModel = new QStandardItemModel();
    m_pDetailsModel = new QStandardItemModel();
}

CDeviceInfo::~CDeviceInfo()
{
    SAFE_DELETE(m_pDetailsModel);
    SAFE_DELETE(m_pDeviceModel);

    if( m_hDeviceInfo )
    {
        SetupDiDestroyDeviceInfoList(m_hDeviceInfo);
        m_hDeviceInfo = 0L;
    }
}

BOOL CDeviceInfo::EnumerateDeviceInformation()
{
    m_usDeviceIndex++;
    m_pDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    return SetupDiEnumDeviceInfo(m_hDeviceInfo, m_usDeviceIndex, &m_pDeviceInfoData);
}

BOOL CDeviceInfo::GetDeviceRegistryProperty(DWORD dwProperty, LPBYTE lpPropertyBuffer)
{
    return SetupDiGetDeviceRegistryProperty(m_hDeviceInfo, &m_pDeviceInfoData, dwProperty, NULL, lpPropertyBuffer, 2048, 0);
}

BOOL CDeviceInfo::GetClassDescription(LPWSTR lpClassBuffer)
{
    return SetupDiGetClassDescription(&m_pDeviceInfoData.ClassGuid, lpClassBuffer, MAX_PATH, NULL);
}

BOOL CDeviceInfo::InternalRefresh()
{
    BOOL bResult = ResetInternalCounters();
    CHECK_OPERATION(bResult);

    DeviceDetails *pDevDetails = 0;

    while(EnumerateDeviceInformation())
    {
        GUID    Guid;
        WCHAR   wBuff[1024] = {0};

        bResult = GetDeviceRegistryProperty(SPDRP_CLASSGUID, (LPBYTE)wBuff);
        CHECK_OPERATION(bResult);

        CLSIDFromString(wBuff, &Guid);

        pDevDetails = new DeviceDetails;

        WCHAR szBuff[MAX_PATH] = {0};

        if( InlineIsEqualGUID(Guid, GUID_SOUND) )
        {
            pDevDetails->qzParent = "Sound, video and game controllers";
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_AUDIO) )
        {
            pDevDetails->qzParent = ("Audio inputs and outputs");
            GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_COMPUTER) )
        {
            pDevDetails->qzParent = "Computer";
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_DISKDRIVE) )
        {
            pDevDetails->qzParent = ("Disk drives");
            GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_DISPLAY) )
        {
            pDevDetails->qzParent = ("Display adapters");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_HIDCLASS) )
        {
            pDevDetails->qzParent = ("Human Interface Devices (HID)");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_HDC) )
        {
            pDevDetails->qzParent = ("IDE ATA/ATAPI Controllers");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_KEYBOARD) )
        {
            pDevDetails->qzParent = ("Keyboards");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_MOUSE) )
        {
            pDevDetails->qzParent = ("Pointing devices");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_MONITOR) )
        {
            pDevDetails->qzParent = ("Monitors");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_NET) )
        {
            pDevDetails->qzParent = ("Network adapters");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_WPD) )
        {
            pDevDetails->qzParent = ("Portable devices");
            GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_PORTS) )
        {
            pDevDetails->qzParent = ("Ports (COM & LPT)");
            GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_PRINTER) )
        {
            pDevDetails->qzParent = ("Print queues");
            GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_PROCESSOR) )
        {
            pDevDetails->qzParent = ("Processors");
            GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_SENSOR) )
        {
            pDevDetails->qzParent = ("Sensors");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_USB) )
        {
            pDevDetails->qzParent = ("Universal Serial Bus controllers");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_SYSTEM) )
        {
            pDevDetails->qzParent = ("System devices");
            GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (PBYTE)szBuff);
            pDevDetails->qzDisplayName = QString::fromWCharArray(szBuff);
        }

        if( pDevDetails->qzDisplayName == "" )
        {
            SAFE_DELETE(pDevDetails);
            continue;
        }

        DWORD dwJunk;
        SetupDiGetDeviceInstanceId(m_hDeviceInfo, &m_pDeviceInfoData, szBuff, 2048, &dwJunk);
        pDevDetails->qzID = QString::fromWCharArray(szBuff);

        if( !m_qTopLevelItems.contains(pDevDetails->qzParent, Qt::CaseInsensitive) )
            m_qTopLevelItems.append(pDevDetails->qzParent);

        RetrieveDeviceDetails(&pDevDetails);

        m_qDeviceDetails.append(pDevDetails);
    }

    return TRUE;
}

BOOL CDeviceInfo::ResetInternalCounters()
{
    m_usDeviceIndex = -1;

    if( m_hDeviceInfo )
    {
        SetupDiDestroyDeviceInfoList(m_hDeviceInfo);
        m_hDeviceInfo = 0L;
    }

    m_hDeviceInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if( INVALID_HANDLE_VALUE == m_hDeviceInfo )
        return FALSE;

    foreach(DeviceDetails *pDev, m_qDeviceDetails)
        SAFE_DELETE(pDev);
    m_qDeviceDetails.clear();

    m_qTopLevelItems.clear();

    return TRUE;
}

BOOL CDeviceInfo::RetrieveDeviceDetails(DeviceDetails **ppDevInfo)
{
    WCHAR szBuffer[1024] = {0};
    DWORD dwBuffer = 0;
    QStringList qBufferList;

    GetDeviceRegistryProperty(SPDRP_ADDRESS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Address", QStringList() << QString().sprintf("0x%x", QString::fromWCharArray(szBuffer)));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_BUSNUMBER, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Bus number", QStringList() << QString().sprintf("0x%x", QString::fromWCharArray(szBuffer)));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_BUSTYPEGUID, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Bus type GUID", QStringList() << QString().sprintf("0x%x", QString::fromWCharArray(szBuffer)));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_CAPABILITIES, (LPBYTE)&dwBuffer);
    qBufferList.append(QString().sprintf("0x%x", dwBuffer));
    if(dwBuffer & CM_DEVCAP_LOCKSUPPORTED )
        qBufferList.append("CM_DEVCAP_LOCK_SUPPORTED");
    if(dwBuffer & CM_DEVCAP_EJECTSUPPORTED )
        qBufferList.append("CM_DEVCAP_EJECTSUPPORTED");
    if(dwBuffer & CM_DEVCAP_REMOVABLE )
        qBufferList.append("CM_DEVCAP_REMOVABLE");
    if(dwBuffer & CM_DEVCAP_DOCKDEVICE )
        qBufferList.append("CM_DEVCAP_DOCKDEVICE");
    if(dwBuffer & CM_DEVCAP_UNIQUEID )
        qBufferList.append("CM_DEVCAP_UNIQUEID");
    if(dwBuffer & CM_DEVCAP_SILENTINSTALL )
        qBufferList.append("CM_DEVCAP_SILENTINSTALL");
    if(dwBuffer & CM_DEVCAP_RAWDEVICEOK )
        qBufferList.append("CM_DEVCAP_RAWDEVICEOK");
    if(dwBuffer & CM_DEVCAP_SURPRISEREMOVALOK )
        qBufferList.append("CM_DEVCAP_SURPRISEREMOVALOK");
    if(dwBuffer & CM_DEVCAP_HARDWAREDISABLED )
        qBufferList.append("CM_DEVCAP_HARDWAREDISABLED");
    if(dwBuffer & CM_DEVCAP_NONDYNAMIC )
        qBufferList.append("CM_DEVCAP_NONDYNAMIC");
    (*ppDevInfo)->qDetails.insert("Capabilities", QStringList(qBufferList));
    qBufferList.clear();
    dwBuffer = 0;

    GetDeviceRegistryProperty(SPDRP_CLASS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Class", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_CLASSGUID, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Class GUID", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_COMPATIBLEIDS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Compatible IDs", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_CONFIGFLAGS, (LPBYTE)&dwBuffer);
    qBufferList.append(QString().sprintf("0x%x", dwBuffer));
    if( dwBuffer & CONFIGFLAG_DISABLED )
        qBufferList.append("CONFIGFLAG_DISABLED");
    if( dwBuffer & CONFIGFLAG_REMOVED )
        qBufferList.append("CONFIGFLAG_REMOVED");
    if( dwBuffer & CONFIGFLAG_MANUAL_INSTALL )
        qBufferList.append("CONFIGFLAG_MANUAL_INSTALL");
    if( dwBuffer & CONFIGFLAG_IGNORE_BOOT_LC )
        qBufferList.append("CONFIGFLAG_IGNORE_BOOT_LC");
    if( dwBuffer & CONFIGFLAG_NET_BOOT )
        qBufferList.append("CONFIGFLAG_NET_BOOT");
    if( dwBuffer & CONFIGFLAG_REINSTALL )
        qBufferList.append("CONFIGFLAG_REINSTALL");
    if( dwBuffer & CONFIGFLAG_FAILEDINSTALL )
        qBufferList.append("CONFIGFLAG_FAILEDINSTALL");
    if( dwBuffer & CONFIGFLAG_CANTSTOPACHILD )
        qBufferList.append("CONFIGFLAG_CANTSTOPACHILD");
    if( dwBuffer & CONFIGFLAG_OKREMOVEROM )
        qBufferList.append("CONFIGFLAG_OKREMOVEROM");
    if( dwBuffer & CONFIGFLAG_NOREMOVEEXIT )
        qBufferList.append("CONFIGFLAG_NOREMOVEEXIT");
    if( dwBuffer & CONFIGFLAG_FINISH_INSTALL )
        qBufferList.append("CONFIGFLAG_FINISH_INSTALL");
    if( dwBuffer & CONFIGFLAG_NEEDS_FORCED_CONFIG )
        qBufferList.append("CONFIGFLAG_NEEDS_FORCED_CONFIG");
    if( dwBuffer & CONFIGFLAG_PARTIAL_LOG_CONF )
        qBufferList.append("CONFIGFLAG_PARTIAL_LOG_CONF");
    if( dwBuffer & CONFIGFLAG_SUPPRESS_SURPRISE )
        qBufferList.append("CONFIGFLAG_SUPPRESS_SURPRISE");
    if( dwBuffer & CONFIGFLAG_VERIFY_HARDWARE )
        qBufferList.append("CONFIGFLAG_VERIFY_HARDWARE");
    if( dwBuffer & CONFIGFLAG_FINISHINSTALL_UI )
        qBufferList.append("CONFIGFLAG_FINISHINSTALL_UI");
    if( dwBuffer & CONFIGFLAG_FINISHINSTALL_ACTION )
        qBufferList.append("CONFIGFLAG_FINISHINSTALL_ACTION");
    if( dwBuffer & CONFIGFLAG_BOOT_DEVICE )
        qBufferList.append("CONFIGFLAG_BOOT_DEVICE");
    (*ppDevInfo)->qDetails.insert("Config flags", QStringList(qBufferList));
    qBufferList.clear();
    dwBuffer = 0;

    GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Description", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_DRIVER, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Software key", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_ENUMERATOR_NAME, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Enumerator name", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Friendly name", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_HARDWAREID, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Hardware Id", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_LOCATION_PATHS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Location paths", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_LOCATION_INFORMATION, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Location information", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_LOWERFILTERS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Lower filters", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_MFG, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Manufacturer", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Physical device name", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_SERVICE, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Service name", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_UI_NUMBER_DESC_FORMAT, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("UINumber", QStringList() << QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    return TRUE;
}

QStandardItemModel *CDeviceInfo::GetAllDeviceDetails()
{
    return m_pDeviceModel;
}

QStandardItemModel *CDeviceInfo::GetDeviceProperties()
{
    return m_pDetailsModel;
}

void CDeviceInfo::OnRefreshDevices()
{
    BOOL bResult = InternalRefresh();
    CHECK_OPERATION(bResult);

    DeviceDetails *pDevDetails = 0;
    QStandardItem *pItem = 0, *pItem2 = 0;

    m_pDeviceModel->clear();
    m_pDeviceModel->setHorizontalHeaderLabels(QStringList() << "Devices:");

    for(int i = 0; i < m_qTopLevelItems.count(); i++)
    {
        pItem = new QStandardItem(m_qTopLevelItems.at(i));
        m_pDeviceModel->appendRow(pItem);

        for(int j = 0; j < m_qDeviceDetails.count(); j++)
        {
            pDevDetails = m_qDeviceDetails.at(j);

            if( pDevDetails->qzParent == pItem->text() )
            {
                pItem2 = new QStandardItem(pDevDetails->qzDisplayName);
                pItem2->setData(pDevDetails->qzID);
                pItem->appendRow(pItem2);
            }
        }
    }
}

void CDeviceInfo::OnRefreshDetails(QString qzDeviceID)
{
    m_pDetailsModel->clear();
    m_pDetailsModel->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");

    DeviceDetails *pDevDetails = 0;
    QString qzTemp = "";

    QList<QStandardItem*> qList;

    QMap<QString, QStringList>::iterator it;

    for( int i = 0; i < m_qDeviceDetails.count(); i++ )
    {
        pDevDetails = m_qDeviceDetails.at(i);

        if( pDevDetails->qzID != qzDeviceID )
            continue;

        for(it = pDevDetails->qDetails.begin();
            it != pDevDetails->qDetails.end(); it++)
        {
            qList << new QStandardItem(it.key());
            qzTemp = "";
            for(int j = 0; j < it.value().count(); j++)
                qzTemp += it.value().at(j) + QString(", ");
            qzTemp.chop(2);
            qList << new QStandardItem(qzTemp == "" ? "N/A" : qzTemp);

            m_pDetailsModel->appendRow(qList);

            qList.clear();
        }

        break;
    }
}


