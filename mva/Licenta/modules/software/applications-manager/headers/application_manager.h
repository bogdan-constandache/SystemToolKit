#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

// WinAPI includes
#include <Windows.h>
#include <WinReg.h>
#include <tchar.h>

// QT includes
#include <QString>
#include <QList>
#include <QMap>
#include <QSysInfo>
#include <QStandardItemModel>

// My includes
#include "../../../../utils/headers/utils.h"
#include "software_structures.h"

#define UNINSTALL_REGISTRY_LOCATION      "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall"

class CApplicationManager
{
private:
    QMap<QString, PApplication> m_qApps;

    void ClearInternalMap();
    bool IsX64System();

    void GetInstalledApplication(HKEY hKey, QString qsPath, bool bIsX64Hive);

public:
    CApplicationManager();
    ~CApplicationManager();
    QStandardItemModel* GetApplicationList();
};

#endif // APPLICATION_MANAGER_H
