#ifndef ACTIVE_CONNECTIONS_H
#define ACTIVE_CONNECTIONS_H


#include <Windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <TlHelp32.h>

#include <QList>
#include <QStandardItemModel>

#include "network_structures.h"
#include "../../../utils/headers/utils.h"



class CActiveConnections
{
private:
    QList<ActiveConnectionsStruct*> m_Connections;
    QStandardItemModel *m_pDataModel;

    int GetAllTCPConnections();
    int GetAllUDPConnections();
    QString GetProcessNameFromPID(int nPID);

public:
    CActiveConnections();
    ~CActiveConnections();
    QStandardItemModel *GetActiveConnections();

    void OnRefreshData();
};

#endif // ACTIVE_CONNECTIONS_H
