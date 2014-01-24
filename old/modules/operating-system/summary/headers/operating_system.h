#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H

// WinAPI includes
#include <Windows.h>
#include <winreg.h>
#include <tchar.h>

// Qt includes
#include <QString>
#include <QStringList>

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
}OSInformation;

class OperatingSystem
{
private:
    OSInformation *m_pOSInfo;
public:
    OperatingSystem();
    ~OperatingSystem();

    int Initialize();
    int Destroy();

private:
    int GetOSProperties();
};

#endif
