#include "../headers/network_devices.h"
#include <LM.h>


CNetworkDevices::CNetworkDevices()
{
    m_pAdaptersModel = new QStandardItemModel();
    m_pPropsModel = new QStandardItemModel();

    this->GetAllAdaptersInformations();

    m_pAdaptersModel->setHorizontalHeaderLabels(QStringList() <<"Select an adapter to get more informations:");

    for(int i = 0; i < m_qAdapterNames.count(); i++)
    {
        QStandardItem *pStandardItem = new QStandardItem(m_qAdapterNames.at(i));
        pStandardItem->setData(m_qAdapterNames.at(i));
        m_pAdaptersModel->appendRow(pStandardItem);
    }
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

    SAFE_DELETE(m_pPropsModel);
    SAFE_DELETE(m_pAdaptersModel);
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
    return m_pAdaptersModel;
}

QStandardItemModel *CNetworkDevices::GetAdapterInformations()
{
    return m_pPropsModel;
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

void CNetworkDevices::OnRefreshAdapterData(QString qsAdapter)
{
    if (!m_qAdapterNames.contains(qsAdapter))
        return;

    m_pPropsModel->clear();
    m_pPropsModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    NetworkAdapterInfo *pDev = 0;

    for(int i = 0; i < m_qNetworkAdapters.count(); i++)
    {
        if (qsAdapter == m_qNetworkAdapters.at(i)->qzDescription)
        {
            pDev = m_qNetworkAdapters.at(i);
            break;
        }
    }

    QList<QStandardItem*> qList;

    qList << new QStandardItem("Adapter name: ") << new QStandardItem(pDev->qzAdapterName);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Adapter description: ") << new QStandardItem(pDev->qzDescription);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Adapter MAC address: ") << new QStandardItem(pDev->qzMACAddress);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Index: ") << new QStandardItem(pDev->qzIndex);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Type: ") << new QStandardItem(pDev->qzAdaptorType);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("IP address: ") << new QStandardItem(pDev->qzIPAddress);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("IP mask: ") << new QStandardItem(pDev->qzIPMask);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Gateway: ") << new QStandardItem(pDev->qzGateway);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("DHCP enabled: ") << new QStandardItem(pDev->qzDHCPEnabled);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("DHCP server: ") << new QStandardItem(pDev->qzDHCPServer);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("DHCP lease obtained: ") << new QStandardItem(pDev->qzDHCPLeaseObtained);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("DHCP lease expires: ") << new QStandardItem(pDev->qzDHCPLeaseExpires);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Have WINS (Windows Internet Name Service): ") << new QStandardItem(pDev->qzHaveWINS);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Primary WINS server: ") << new QStandardItem(pDev->qzPrimaryWINSServer);
    m_pPropsModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Secondary WINS server: ") << new QStandardItem(pDev->qzSecondaryWINSServer);
    m_pPropsModel->appendRow(qList); qList.clear();
}
