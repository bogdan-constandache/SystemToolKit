#include "../headers/network_devices.h"
#include <LM.h>


CNetworkDevices::CNetworkDevices()
{
}

CNetworkDevices::~CNetworkDevices()
{
    foreach(NetworkAdapterInfo *pDev, m_qNetworkAdapters)
        SAFE_DELETE(pDev);
    m_qNetworkAdapters.clear();

    m_qAdapterNames.clear();

    foreach(NetworkSharedResource *pRes, m_qSharedResources)
        SAFE_DELETE(pRes);
    m_qSharedResources.clear();
}

int CNetworkDevices::GetAllAdaptersInformations()
{
    NetworkAdapterInfo *pDeviceStruct = 0;
    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    PIP_ADAPTER_INFO pAdapter = NULL;
    ULONG ulOutBufLen = 0;

    struct tm sTime;
    char pszBuffer[32] = {0};
    int nError = 0;

    ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO*) malloc (ulOutBufLen);

    if (NO_ERROR != GetAdaptersInfo(NULL, &ulOutBufLen))
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*) malloc (ulOutBufLen);
    }

    if (NO_ERROR != GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
    {
        free(pAdapterInfo);
        return Unsuccessful;
    }

    pAdapter = pAdapterInfo;

    while(pAdapter)
    {
        pDeviceStruct = new NetworkAdapterInfo;

        m_qAdapterNames.append(pAdapter->Description);

        pDeviceStruct->qzAdapterName = pAdapter->AdapterName;
        pDeviceStruct->qzDescription = pAdapter->Description;

        pDeviceStruct->qzIPAddress = pAdapter->IpAddressList.IpAddress.String;
        pDeviceStruct->qzIPMask = pAdapter->IpAddressList.IpMask.String;
        pDeviceStruct->qzGateway = pAdapter->GatewayList.IpAddress.String;

        if (pAdapter->DhcpEnabled)
        {
            pDeviceStruct->qzDHCPEnabled = "Yes";
            pDeviceStruct->qzDHCPServer = pAdapterInfo->DhcpServer.IpAddress.String;

            nError = _localtime32_s(&sTime, (__time32_t*) &pAdapter->LeaseObtained);
            if (nError)
                pDeviceStruct->qzDHCPLeaseObtained = "N/A";
            else
            {
                nError = asctime_s(pszBuffer, 32, &sTime);
                if (nError)
                    pDeviceStruct->qzDHCPLeaseObtained = "N/A";
                else
                {
                    pDeviceStruct->qzDHCPLeaseObtained = pszBuffer;
                    pDeviceStruct->qzDHCPLeaseObtained.chop(1);
                }
            }

            nError = _localtime32_s(&sTime, (__time32_t*) &pAdapter->LeaseExpires);
            if (nError)
                pDeviceStruct->qzDHCPLeaseExpires = "N/A";
            else
            {
                nError = asctime_s(pszBuffer, 32, &sTime);
                if (nError)
                    pDeviceStruct->qzDHCPLeaseExpires = "N/A";
                else
                {
                    pDeviceStruct->qzDHCPLeaseExpires = pszBuffer;
                    pDeviceStruct->qzDHCPLeaseExpires.chop(1);
                }
            }
        }
        else
        {
            pDeviceStruct->qzDHCPEnabled = "No";
            pDeviceStruct->qzDHCPServer = "N/A";
            pDeviceStruct->qzDHCPLeaseExpires = "N/A";
            pDeviceStruct->qzDHCPLeaseObtained = "N/A";
        }

        if (pAdapter->HaveWins)
        {
            pDeviceStruct->qzHaveWINS = "Yes";
            pDeviceStruct->qzPrimaryWINSServer = pAdapter->PrimaryWinsServer.IpAddress.String;
            pDeviceStruct->qzSecondaryWINSServer = pAdapter->SecondaryWinsServer.IpAddress.String;
        }
        else
        {
            pDeviceStruct->qzHaveWINS = "No";
            pDeviceStruct->qzPrimaryWINSServer = "N/A";
            pDeviceStruct->qzSecondaryWINSServer = "N/A";
        }

        pDeviceStruct->qzIndex = QString::number(pAdapter->Index);

        for (int i = 0; i < (int) pAdapter->AddressLength; i++)
        {
            pDeviceStruct->qzMACAddress.append(QString().sprintf("%.2X:", pAdapter->Address[i]));
        }
        pDeviceStruct->qzMACAddress.chop(1);

        switch(pAdapter->Type)
        {
        case MIB_IF_TYPE_OTHER:
            pDeviceStruct->qzAdaptorType = "Some other type of network interface";
            break;
        case MIB_IF_TYPE_ETHERNET:
            pDeviceStruct->qzAdaptorType = "Ethernet network interface";
            break;
        case IF_TYPE_ISO88025_TOKENRING:
            break;
        case MIB_IF_TYPE_PPP:
            pDeviceStruct->qzAdaptorType = "PPP network interface";
            break;
        case MIB_IF_TYPE_LOOPBACK:
            pDeviceStruct->qzAdaptorType = "Loopback network interface";
            break;
        case MIB_IF_TYPE_SLIP:
            pDeviceStruct->qzAdaptorType = "ATM network interface";
            break;
        case IF_TYPE_IEEE80211:
            pDeviceStruct->qzAdaptorType = "IEEE 802.11 wireless network interface";
            break;
        }

        m_qNetworkAdapters.append(pDeviceStruct);

        pAdapter = pAdapter->Next;
    }

    if (pAdapterInfo)
        free(pAdapterInfo);

    return Success;
}

int CNetworkDevices::GetSharingInformations()
{
    PSHARE_INFO_502 pBufferPtr = 0, pPtr = 0;
    NET_API_STATUS nStatus;
    DWORD dwEntriesRead = 0, dwTotalEntries = 0, dwResumeHandle = 0;

    NetworkSharedResource *pResource = 0;

    do
    {
        nStatus = NetShareEnum(NULL, 502, (LPBYTE*)&pBufferPtr,
                               MAX_PREFERRED_LENGTH, &dwEntriesRead,
                               &dwTotalEntries, &dwResumeHandle);

        if( ERROR_SUCCESS != nStatus && ERROR_MORE_DATA != nStatus )
            CHECK_OPERATION_STATUS(Unsuccessful);

        pPtr = pBufferPtr;

        for(int i = 0; i < (int)dwEntriesRead; i++)
        {
            pResource = new NetworkSharedResource;

            pResource->qzNetworkName = QString::fromWCharArray(pPtr->shi502_netname);
            pResource->qzComments = QString::fromWCharArray(pPtr->shi502_remark);
            pResource->qzPath = QString::fromWCharArray(pPtr->shi502_path);

            pPtr++;
            m_qSharedResources.append(pResource);
        }

        NetApiBufferFree(pBufferPtr);
    } while( ERROR_MORE_DATA == nStatus );

    return Success;
}

QStandardItemModel *CNetworkDevices::GetAdapterNames()
{
    foreach(NetworkAdapterInfo *pDev, m_qNetworkAdapters)
        SAFE_DELETE(pDev);
    m_qNetworkAdapters.clear();

    m_qAdapterNames.clear();

    this->GetAllAdaptersInformations();

    QStandardItemModel *pModel = new QStandardItemModel;

    pModel->setHorizontalHeaderLabels(QStringList() <<"Select a device to get more informations:");

    pModel->setColumnCount(1);
    pModel->setRowCount(m_qAdapterNames.count());

    for(int i = 0; i < m_qAdapterNames.count(); i++)
    {
        pModel->setItem(i, 0, new QStandardItem(m_qAdapterNames.at(i)));
    }

    return pModel;
}

QStandardItemModel *CNetworkDevices::GetAdapterInformations(QString qzAdapterName)
{
    QStandardItemModel *pModel = 0;

    if (!m_qAdapterNames.contains(qzAdapterName))
        return 0;

    NetworkAdapterInfo *pDev = 0;

    for(int i = 0; i < m_qNetworkAdapters.count(); i++)
    {
        if (qzAdapterName == m_qNetworkAdapters.at(i)->qzDescription)
        {
            pDev = m_qNetworkAdapters.at(i);
            break;
        }
    }

    pModel = new QStandardItemModel;

    pModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    pModel->setColumnCount(2);
    pModel->setRowCount(15);

    pModel->setItem(0, 0, new QStandardItem("Adapter name: "));
    pModel->setItem(0, 1, new QStandardItem(pDev->qzAdapterName));

    pModel->setItem(1, 0, new QStandardItem("Adapter description: "));
    pModel->setItem(1, 1, new QStandardItem(pDev->qzDescription));

    pModel->setItem(2, 0, new QStandardItem("Adapter MAC address: "));
    pModel->setItem(2, 1, new QStandardItem(pDev->qzMACAddress));

    pModel->setItem(3, 0, new QStandardItem("Index: "));
    pModel->setItem(3, 1, new QStandardItem(pDev->qzIndex));

    pModel->setItem(4, 0, new QStandardItem("Type: "));
    pModel->setItem(4, 1, new QStandardItem(pDev->qzAdaptorType));

    pModel->setItem(5, 0, new QStandardItem("IP address: "));
    pModel->setItem(5, 1, new QStandardItem(pDev->qzIPAddress));

    pModel->setItem(6, 0, new QStandardItem("IP mask: "));
    pModel->setItem(6, 1, new QStandardItem(pDev->qzIPMask));

    pModel->setItem(7, 0, new QStandardItem("Gateway: "));
    pModel->setItem(7, 1, new QStandardItem(pDev->qzGateway));

    pModel->setItem(8, 0, new QStandardItem("DHCP enabled: "));
    pModel->setItem(8, 1, new QStandardItem(pDev->qzDHCPEnabled));

    pModel->setItem(9, 0, new QStandardItem("DHCP server: "));
    pModel->setItem(9, 1, new QStandardItem(pDev->qzDHCPServer));

    pModel->setItem(10, 0, new QStandardItem("DHCP lease obtained: "));
    pModel->setItem(10, 1, new QStandardItem(pDev->qzDHCPLeaseObtained));

    pModel->setItem(11, 0, new QStandardItem("DHCP lease expires: "));
    pModel->setItem(11, 1, new QStandardItem(pDev->qzDHCPLeaseExpires));

    pModel->setItem(12, 0, new QStandardItem("Have WINS (Windows Internet Name Service): "));
    pModel->setItem(12, 1, new QStandardItem(pDev->qzHaveWINS));

    pModel->setItem(13, 0, new QStandardItem("Primary WINS server: "));
    pModel->setItem(13, 1, new QStandardItem(pDev->qzPrimaryWINSServer));

    pModel->setItem(14, 0, new QStandardItem("Secondary WINS server: "));
    pModel->setItem(14, 1, new QStandardItem(pDev->qzSecondaryWINSServer));

    return pModel;
}

QStandardItemModel *CNetworkDevices::GetSharedResourcesInformations()
{
    foreach(NetworkSharedResource *pRes, m_qSharedResources)
        SAFE_DELETE(pRes);
    m_qSharedResources.clear();

    int nStatus = GetSharingInformations();
    if( Success != nStatus )
    {
        CHECK_OPERATION_STATUS(nStatus);
        return 0;
    }

    QStandardItemModel *pModel = new QStandardItemModel;
    QStandardItem *pItem = 0, *pTempItem = 0;

    QStandardItem *pRootItem = pModel->invisibleRootItem();

    foreach(NetworkSharedResource *pResource, m_qSharedResources)
    {
        pItem = new QStandardItem(pResource->qzNetworkName);
        pTempItem = new QStandardItem(pResource->qzPath);
        pItem->appendColumn(QList<QStandardItem*>() << pTempItem);

        if( !pResource->qzComments.isEmpty() )
        {
            pTempItem = new QStandardItem(pResource->qzComments);
            pItem->appendColumn(QList<QStandardItem*>() << pTempItem);
        }

        pRootItem->appendRow(pItem);
    }

    return pModel;
}
