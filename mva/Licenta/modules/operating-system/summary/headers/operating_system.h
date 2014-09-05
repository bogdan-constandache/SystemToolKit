#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H

// WinAPI includes
#include <Windows.h>
#include <winreg.h>
#include <tchar.h>

// Qt includes
#include <QString>
#include <QStringList>
#include <QStandardItemModel>

// My includes
#include "../../../../utils/headers/utils.h"

typedef struct _OS_INFORMATION
{
    QString qszOSName;
    QString qszOSLanguage;
    QString qszKernelType;
    QString qszOSVersion;
    QString qszServicePack;
    QString qszInstallDate;
    QString qszRoot;
    QString qszRegisteredOwner;
    QString qszRegisteredOrganization;
    QString qszProductID;
    QString qszProductKey;
    QString qszSystemBoot;
    bool    bDBCS;
    bool    bDebug;
    bool    bIMM;
    bool    bNetwork;
    bool    bRemote;
    bool    bTerminalServices;
    bool    bSlowMachine;
    bool    bSecure;
}OSInformation;

class COperatingSystemInformation
{
private:
    OSInformation *m_pOSInfo;

    QStandardItemModel *m_pDataModel;

    int GetOSProperties();
    QString DecodeProductKey(BYTE *bData);

public:
    COperatingSystemInformation();
    ~COperatingSystemInformation();

    QStandardItemModel* GetOSModelInformation();
};

#endif
