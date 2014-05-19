#ifndef NETWORK_STRUCTURES_H
#define NETWORK_STRUCTURES_H

#include <QString>

typedef struct _ACTIVE_CONNECTIONS_
{
    QString qzProcessName;
    QString qzProcessPid;
    QString qzState;
    QString qzLocalAddress;
    QString qzLocalPort;
    QString qzRemoteAddress;
    QString qzRemotePort;
    QString qzCreationTime;
    QString qzConnectionType;
}ActiveConnectionsStruct;


typedef struct _NETWORK_ADAPTER_INFO_
{
    QString qzAdapterName;
    QString qzDescription;
    QString qzMACAddress;
    QString qzIndex;
    QString qzAdaptorType;
    QString qzDHCPEnabled;
    QString qzIPAddress;
    QString qzIPMask;
    QString qzGateway;
    QString qzDHCPServer;
    QString qzHaveWINS; // WINDOWS INTERNET NAME SERVICE
    QString qzPrimaryWINSServer;
    QString qzSecondaryWINSServer;
    QString qzDHCPLeaseObtained;
    QString qzDHCPLeaseExpires;
}NetworkAdapterInfo;


typedef struct _NETWORK_SHARED_RESOURCE_
{
    QString qzNetworkName;
    QString qzComments;
    QString qzPath;
}NetworkSharedResource;


#endif // NETWORK_STRUCTURES_H
