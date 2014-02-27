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
}ActiveConnectionsStruct;


#endif // NETWORK_STRUCTURES_H
