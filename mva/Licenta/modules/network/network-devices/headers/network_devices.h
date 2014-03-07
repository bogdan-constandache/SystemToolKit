#ifndef _NETWORK_DEVICES_H
#define _NETWORK_DEVICES_H


#include <Windows.h>
#include <iphlpapi.h>
#include <stdlib.h>

#include <QList>
#include <QStringList>
#include <QStandardItemModel>

#include "../../active-connections/network_structures.h"
#include "../../../../utils/headers/utils.h"


//#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
//#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


class CNetworkDevices
{
private:
    QList<NetworkAdapterInfo*> m_qNetworkAdapters;
    QStringList m_qAdapterNames;

    int GetAllAdaptersInformations();

public:
    CNetworkDevices();
    ~CNetworkDevices();

    QStandardItemModel *GetAdapterNames();
    QStandardItemModel *GetAdapterInformations(QString qzAdapterName);
};

#endif
