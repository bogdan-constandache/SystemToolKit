#include "active_connections.h"


CActiveConnections::CActiveConnections()
{
}

CActiveConnections::~CActiveConnections()
{
    foreach(ActiveConnectionsStruct *pStruct, m_Connections)
    {
        delete pStruct;
    }
    m_Connections.clear();
}

int CActiveConnections::GetAllTCPConnections()
{
    PMIB_TCPTABLE_OWNER_MODULE pTcpTable = 0;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    struct in_addr IpAddr;

    ActiveConnectionsStruct *pStruct = 0;

    pTcpTable = (PMIB_TCPTABLE_OWNER_MODULE) malloc (sizeof(MIB_TCPTABLE_OWNER_MODULE));
    if( NULL == pTcpTable )
    {
        return NotAllocated;
    }

    dwSize = sizeof(MIB_TCPTABLE_OWNER_MODULE);

    if( (dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET,
                                        TCP_TABLE_OWNER_MODULE_ALL, 0)) == ERROR_INSUFFICIENT_BUFFER )
    {
        free(pTcpTable);
        pTcpTable = (PMIB_TCPTABLE_OWNER_MODULE) malloc (dwSize);
        if( NULL == pTcpTable )
        {
            return NotAllocated;
        }
    }

    if( (dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0)) == NO_ERROR )
    {
        for(int i = 0; i < (int) pTcpTable->dwNumEntries; i++)
        {
            pStruct = new ActiveConnectionsStruct;

            pStruct->qzConnectionType = "TCP";

            switch (pTcpTable->table[i].dwState) {
            case MIB_TCP_STATE_CLOSED:
                pStruct->qzState = "Closed";
                break;
            case MIB_TCP_STATE_LISTEN:
                pStruct->qzState = "Listen";
                break;
            case MIB_TCP_STATE_SYN_SENT:
                pStruct->qzState = "Syn-Sent";
                break;
            case MIB_TCP_STATE_SYN_RCVD:
                pStruct->qzState = "Syn-Received";
                break;
            case MIB_TCP_STATE_ESTAB:
                pStruct->qzState = "Established";
                break;
            case MIB_TCP_STATE_FIN_WAIT1:
                pStruct->qzState = "Fin-Wait-1";
                break;
            case MIB_TCP_STATE_FIN_WAIT2:
                pStruct->qzState = "Fin-Wait-2";
                break;
            case MIB_TCP_STATE_CLOSE_WAIT:
                pStruct->qzState = "Close-wait";
                break;
            case MIB_TCP_STATE_CLOSING:
                pStruct->qzState = "Closing";
                break;
            case MIB_TCP_STATE_LAST_ACK:
                pStruct->qzState = "Last-Ack";
                break;
            case MIB_TCP_STATE_TIME_WAIT:
                pStruct->qzState = "Time-wait";
                break;
            case MIB_TCP_STATE_DELETE_TCB:
                pStruct->qzState = "Delete";
                break;
            default:
                pStruct->qzState = "Unknown";
                break;
            }

            IpAddr.S_un.S_addr = (ULONG) pTcpTable->table[i].dwLocalAddr;
            char szAddress[128] = {0};
            strcpy_s(szAddress, sizeof(szAddress), inet_ntoa(IpAddr));
            pStruct->qzLocalAddress = szAddress;

            pStruct->qzLocalPort = QString::number(ntohs((USHORT)pTcpTable->table[i].dwLocalPort));

            IpAddr.S_un.S_addr = (ULONG) pTcpTable->table[i].dwRemoteAddr;
            char szRemoteAddress[128] = {0};
            strcpy_s(szRemoteAddress, sizeof(szRemoteAddress), inet_ntoa(IpAddr));
            pStruct->qzRemoteAddress = szRemoteAddress;

            pStruct->qzRemotePort = QString::number(ntohs((USHORT)pTcpTable->table[i].dwRemotePort));

            pStruct->qzProcessPid = QString::number(pTcpTable->table[i].dwOwningPid);

            pStruct->qzProcessName = this->GetProcessNameFromPID(pTcpTable->table[i].dwOwningPid);

            QString qzCreationTime = "";

            SYSTEMTIME pTime;

            BOOL bResult = FileTimeToSystemTime((FILETIME*)&pTcpTable->table[i].liCreateTimestamp, &pTime);

            if (pTime.wYear > 2000 && bResult == TRUE )
            {
                qzCreationTime += QString::number(pTime.wDay);
                qzCreationTime += "/";
                qzCreationTime += QString::number(pTime.wMonth);
                qzCreationTime += "/";
                qzCreationTime += QString::number(pTime.wYear);
                qzCreationTime += " ";
                qzCreationTime += QString::number(pTime.wHour);
                qzCreationTime += ":";
                qzCreationTime += QString::number(pTime.wMinute);
                qzCreationTime += ":";
                qzCreationTime += QString::number(pTime.wSecond);
            }
            else
            {
                qzCreationTime = "N/A";
            }

            pStruct->qzCreationTime = qzCreationTime;

            m_Connections.append(pStruct);
        }
        free(pTcpTable);
    }

    return Success;
}

int CActiveConnections::GetAllUDPConnections()
{
    PMIB_UDPTABLE_OWNER_MODULE pUdpTable = 0;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    struct in_addr IpAddr;

    ActiveConnectionsStruct *pStruct = 0;

    pUdpTable = (PMIB_UDPTABLE_OWNER_MODULE) malloc (sizeof(MIB_UDPTABLE_OWNER_MODULE));
    if( NULL == pUdpTable )
    {
        return NotAllocated;
    }

    dwSize = sizeof(MIB_UDPTABLE_OWNER_MODULE);

    if( (dwRetVal = GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, AF_INET,
                                        UDP_TABLE_OWNER_MODULE, 0)) == ERROR_INSUFFICIENT_BUFFER )
    {
        free(pUdpTable);
        pUdpTable = (PMIB_UDPTABLE_OWNER_MODULE) malloc (dwSize);
        if( NULL == pUdpTable )
        {
            return NotAllocated;
        }
    }

    if( (dwRetVal = GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_MODULE, 0)) == NO_ERROR )
    {
        for(int i = 0; i < (int) pUdpTable->dwNumEntries; i++)
        {
            pStruct = new ActiveConnectionsStruct;

            pStruct->qzConnectionType = "UDP";
            pStruct->qzState = "N/A";

            IpAddr.S_un.S_addr = (ULONG) pUdpTable->table[i].dwLocalAddr;
            char szAddress[128] = {0};
            strcpy_s(szAddress, sizeof(szAddress), inet_ntoa(IpAddr));
            pStruct->qzLocalAddress = szAddress;

            pStruct->qzLocalPort = QString::number(ntohs((USHORT)pUdpTable->table[i].dwLocalPort));

            pStruct->qzRemoteAddress = "N/A";
            pStruct->qzRemotePort = "N/A";

            pStruct->qzProcessPid = QString::number(pUdpTable->table[i].dwOwningPid);

            pStruct->qzProcessName = this->GetProcessNameFromPID(pUdpTable->table[i].dwOwningPid);

            QString qzCreationTime = "";

            SYSTEMTIME pTime;

            BOOL bResult = FileTimeToSystemTime((FILETIME*)&pUdpTable->table[i].liCreateTimestamp, &pTime);

            if (pTime.wYear > 2000 && bResult == TRUE )
            {
                qzCreationTime += QString::number(pTime.wDay);
                qzCreationTime += "/";
                qzCreationTime += QString::number(pTime.wMonth);
                qzCreationTime += "/";
                qzCreationTime += QString::number(pTime.wYear);
                qzCreationTime += " ";
                qzCreationTime += QString::number(pTime.wHour);
                qzCreationTime += ":";
                qzCreationTime += QString::number(pTime.wMinute);
                qzCreationTime += ":";
                qzCreationTime += QString::number(pTime.wSecond);
            }
            else
            {
                qzCreationTime = "N/A";
            }

            pStruct->qzCreationTime = qzCreationTime;

            m_Connections.append(pStruct);
        }
        free(pUdpTable);
    }

    return Success;
}

QString CActiveConnections::GetProcessNameFromPID(int nPID)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(hSnapshot)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if(Process32First(hSnapshot,&pe32))
        {
            do
            {
                if (nPID == pe32.th32ProcessID)
                {
                    CloseHandle(hSnapshot);
                    return QString::fromWCharArray(pe32.szExeFile);
                }
            } while(Process32Next(hSnapshot,&pe32));
        }
        CloseHandle(hSnapshot);
    }
    return "";
}


QStandardItemModel *CActiveConnections::GetActiveConnections()
{
    QStandardItemModel *pModel = new QStandardItemModel;
    QStandardItem *pItem = 0;

    this->GetAllTCPConnections();
    this->GetAllUDPConnections();

    pModel->setColumnCount(9);
    pModel->setRowCount(m_Connections.count());


    pModel->setHorizontalHeaderLabels(QStringList() << "Name" << "PID" << "Type" << "State" << "Local address" << "Local port"
                                      << "Remote address" << "Remote port" << "Creation time");

    for(int i = 0; i < m_Connections.count(); i++)
    {
        ActiveConnectionsStruct *pStruct = m_Connections.at(i);

        pItem = new QStandardItem(pStruct->qzProcessName);
        pModel->setItem(i, 0, pItem);

        pItem = new QStandardItem(pStruct->qzProcessPid);
        pModel->setItem(i, 1, pItem);

        pItem = new QStandardItem(pStruct->qzConnectionType);
        pModel->setItem(i, 2, pItem);

        pItem = new QStandardItem(pStruct->qzState);
        pModel->setItem(i, 3, pItem);

        pItem = new QStandardItem(pStruct->qzLocalAddress);
        pModel->setItem(i, 4, pItem);

        pItem = new QStandardItem(pStruct->qzLocalPort);
        pModel->setItem(i, 5, pItem);

        pItem = new QStandardItem(pStruct->qzRemoteAddress);
        pModel->setItem(i, 6, pItem);

        pItem = new QStandardItem(pStruct->qzRemotePort);
        pModel->setItem(i, 7, pItem);

        pItem = new QStandardItem(pStruct->qzCreationTime);
        pModel->setItem(i, 8, pItem);
    }

    return pModel;
}

