#include "../headers/application_manager.h"

CApplicationManager::CApplicationManager() : m_qApps()
{
}

CApplicationManager::~CApplicationManager()
{
    ClearInternalMap();
}

void CApplicationManager::ClearInternalMap()
{
    for(auto it = m_qApps.begin(); it != m_qApps.end(); it++)
    {
        Application *pApp = *it;
        if( pApp )
        {
            delete pApp;
            *it = nullptr;
        }
    }
}

bool CApplicationManager::IsX64System()
{
    SYSTEM_INFO sysinfo;
    GetNativeSystemInfo(&sysinfo);
    if( PROCESSOR_ARCHITECTURE_AMD64 == sysinfo.wProcessorArchitecture )
        return true;
    return false;
}

void CApplicationManager::GetInstalledApplication(HKEY hKey, QString qsPath, bool bIsX64Hive)
{
    HKEY hRegKey = 0, hItemKey = 0;
    DWORD dwIndex = 0, dwKeyNameSize = 0, dwValueSize = 512;
    WCHAR wsKeyName[512] = {0}, wsValue[512] = {0};

    Application *pApp = 0;

    if( ERROR_SUCCESS != RegOpenKeyEx(hKey, qsPath.toStdWString().c_str(), NULL,
                                      KEY_READ | (bIsX64Hive ? KEY_WOW64_64KEY : KEY_WOW64_32KEY), &hRegKey) )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    while( ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hRegKey, dwIndex, wsKeyName, &dwKeyNameSize,
                                               NULL, NULL, NULL, NULL) )
    {
        QString qzKeyName = QString::fromWCharArray(wsKeyName);
        if( qzKeyName.contains("}.KB") )
        {
            memset(wsKeyName, 0, sizeof(wsKeyName));
            dwKeyNameSize = 512;
            dwIndex++;
            continue;
        }

        if( ERROR_SUCCESS != RegOpenKeyEx(hRegKey, wsKeyName,
                                          NULL, KEY_READ, &hItemKey) )
        {
            memset(wsKeyName, 0, sizeof(wsKeyName));
            dwKeyNameSize = 512;
            dwIndex++;
            continue;
        }

        pApp = new Application();

        dwValueSize = 512;
        memset(wsValue, 0, sizeof(wsValue));
        if( ERROR_SUCCESS == RegQueryValueEx(hItemKey, L"DisplayName", NULL, NULL,
                                            (LPBYTE)wsValue, &dwValueSize) )
        {
            pApp->Name = QString::fromWCharArray(wsValue);
        }

        dwValueSize = 512;
        memset(wsValue, 0, sizeof(wsValue));
        if( ERROR_SUCCESS == RegQueryValueEx(hItemKey, L"DisplayVersion", NULL, NULL,
                                            (LPBYTE)wsValue, &dwValueSize) )
        {
            pApp->Version = QString::fromWCharArray(wsValue);
        }

        dwValueSize = 512;
        memset(wsValue, 0, sizeof(wsValue));
        if( ERROR_SUCCESS == RegQueryValueEx(hItemKey, L"Publisher", NULL, NULL,
                                            (LPBYTE)wsValue, &dwValueSize) )
        {
            pApp->Publisher = QString::fromWCharArray(wsValue);
        }

        dwValueSize = 512;
        memset(wsValue, 0, sizeof(wsValue));
        if( ERROR_SUCCESS == RegQueryValueEx(hItemKey, L"InstallDate", NULL, NULL,
                                            (LPBYTE)wsValue, &dwValueSize) )
        {
            pApp->InstallDate = QString::fromWCharArray(wsValue);
            pApp->InstallDate.insert(4,"/");
            pApp->InstallDate.insert(7,"/");
        }

        dwValueSize = 512;
        memset(wsValue, 0, sizeof(wsValue));
        if( ERROR_SUCCESS == RegQueryValueEx(hItemKey, L"UninstallString", NULL, NULL,
                                            (LPBYTE)wsValue, &dwValueSize) )
        {
            pApp->UninstallString = QString::fromWCharArray(wsValue);
        }

        if( !pApp->Name.isEmpty() && !pApp->UninstallString.isEmpty() )
        {
            m_qApps.insert(pApp->Name, pApp);
        }
        else
        {
            delete pApp;
        }

        memset(wsKeyName, 0, sizeof(wsKeyName));
        dwKeyNameSize = 512;
        dwIndex++;
        RegCloseKey(hItemKey);
    }
    RegCloseKey(hRegKey);
}

QStandardItemModel* CApplicationManager::GetApplicationList()
{
    ClearInternalMap();

    if( IsX64System() )
    {
        GetInstalledApplication(HKEY_CURRENT_USER, UNINSTALL_REGISTRY_LOCATION, false);
        GetInstalledApplication(HKEY_CURRENT_USER, UNINSTALL_REGISTRY_LOCATION, true);

        GetInstalledApplication(HKEY_LOCAL_MACHINE, UNINSTALL_REGISTRY_LOCATION, false);
        GetInstalledApplication(HKEY_LOCAL_MACHINE, UNINSTALL_REGISTRY_LOCATION, true);
    }
    else
    {
        GetInstalledApplication(HKEY_CURRENT_USER, UNINSTALL_REGISTRY_LOCATION, false);

        GetInstalledApplication(HKEY_LOCAL_MACHINE, UNINSTALL_REGISTRY_LOCATION, false);
    }

    QStandardItemModel *pModel = new QStandardItemModel();

    QStandardItem *pItem = NULL;

    QList<QStandardItem*> qRow;

    pModel->setHorizontalHeaderLabels(QStringList() << "Application name" << "Publisher" << "Install date" << "Version");


    for( auto it = m_qApps.begin(); it != m_qApps.end(); it++ )
    {
        Application *pApp = *it;

        pItem = new QStandardItem(pApp->Name);
        pItem->setData(pApp->UninstallString);
        qRow << pItem;

        pItem = new QStandardItem(pApp->Publisher);
        pItem->setData(pApp->UninstallString);
        qRow << pItem;

        pItem = new QStandardItem(pApp->InstallDate);
        pItem->setData(pApp->UninstallString);
        qRow << pItem;

        pItem = new QStandardItem(pApp->Version);
        pItem->setData(pApp->UninstallString);
        qRow << pItem;

        pModel->appendRow(qRow);

        qRow.clear();
    }

    return pModel;
}
