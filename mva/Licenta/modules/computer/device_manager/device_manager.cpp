#include "device_manager.h"
#include <QDebug>

#define SAFE_DELETE(X) if(X) { delete X; X = 0; }
#define CHECK_OPERATION(X) if(!X) { qDebug() << "Operation failed - line: " << __LINE__; }

CDeviceInfo::CDeviceInfo()
{
    m_usDeviceIndex = -1;
    m_hDeviceInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if( INVALID_HANDLE_VALUE == m_hDeviceInfo )
        qDebug() << "SetupDiGetClassDevs - Failed";
}

CDeviceInfo::~CDeviceInfo()
{
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
    GetDeviceRegistryProperty(SPDRP_ADDRESS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Address", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_BUSNUMBER, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Bus number", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_BUSTYPEGUID, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Bus type GUID", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_CLASS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Class", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_CLASSGUID, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Class GUID", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_COMPATIBLEIDS, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Compatible IDs", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_DEVICEDESC, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Description", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_DRIVER, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Software key", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_ENUMERATOR_NAME, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Enumerator name", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    GetDeviceRegistryProperty(SPDRP_FRIENDLYNAME, (LPBYTE)szBuffer);
    (*ppDevInfo)->qDetails.insert("Friendly name", QString::fromWCharArray(szBuffer));
    ZeroMemory(szBuffer, 1024);

    return TRUE;
}

QStandardItemModel *CDeviceInfo::GetAllDeviceDetails()
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

        if( InlineIsEqualGUID(Guid, GUID_DEVCLASS_MEDIA) )
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

        if( !m_qTopLevelItems.contains(pDevDetails->qzParent) )
            m_qTopLevelItems.append(pDevDetails->qzParent);

        RetrieveDeviceDetails(&pDevDetails);

        if( !m_qTopLevelItems.contains(pDevDetails->qzParent, Qt::CaseInsensitive) )
            m_qTopLevelItems.append(pDevDetails->qzParent);

        m_qDeviceDetails.append(pDevDetails);
    }

    // form tree
    QStandardItemModel *pModel = new QStandardItemModel;
    QStandardItem *pRoot = pModel->invisibleRootItem();
    QStandardItem *pItem = 0;

    pModel->setHorizontalHeaderLabels(QStringList() << "Devices:");

    for(int i = 0; i < m_qTopLevelItems.count(); i++)
    {
        pItem = new QStandardItem(m_qTopLevelItems.at(i));
        pRoot->appendRow(pItem);

        for(int j = 0; j < m_qDeviceDetails.count(); j++)
        {
            pDevDetails = m_qDeviceDetails.at(j);

            if( pDevDetails->qzParent == pItem->text() )
                pItem->appendRow(new QStandardItem(pDevDetails->qzDisplayName));
        }
    }

    return pModel;
}


