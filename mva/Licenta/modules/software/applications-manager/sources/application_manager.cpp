#include "../headers/application_manager.h"

CApplicationManager::CApplicationManager()
{
}

CApplicationManager::~CApplicationManager()
{
}

bool CApplicationManager::IsX64System()
{
    SYSTEM_INFO sysinfo;
    GetNativeSystemInfo(&sysinfo);
    if( PROCESSOR_ARCHITECTURE_AMD64 == sysinfo.wProcessorArchitecture )
        return true;
    return false;
}

int CApplicationManager::FillX64Applications()
{
    HKEY hKey = 0, hItem = 0;
    DWORD dwIndex = 0, dwKeyNameSize = 512, dwValueSize = 512, dwReturnCode = 0;
    TCHAR tzKeyName[512] = {0}, tzValue[512] = {0};
    Application *pApplication = 0;

    if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                                      NULL,
                                      KEY_READ | KEY_WOW64_64KEY,
                                      &hKey) )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    while( ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey,
                                               dwIndex,
                                               tzKeyName,
                                               &dwKeyNameSize,
                                               NULL,
                                               NULL,
                                               NULL,
                                               NULL) )
    {
        QString qzKeyName = QString::fromWCharArray(tzKeyName);
        if( qzKeyName.contains("KB[1-9]*") )
        {
            memset(tzKeyName, 0, 512);
            dwKeyNameSize = 512;
            dwIndex++;
            continue;
        }

        if( ERROR_SUCCESS != RegOpenKeyEx(hKey,
                                          tzKeyName,
                                          NULL,
                                          KEY_READ,
                                          &hItem) )
        {
            memset(tzKeyName, 0, 512);
            dwKeyNameSize = 512;
            dwIndex++;
            continue;
        }

        pApplication = new Application;

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"DisplayName",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);

        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->Name = QString::fromWCharArray(tzValue);
//            qDebug() << pApplication->Name;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"DisplayVersion",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);

        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->Version = QString::fromWCharArray(tzValue);
//            qDebug() << pApplication->Version;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"Publisher",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);
        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->Publisher = QString::fromWCharArray(tzValue);
//            qDebug() << pApplication->Publisher;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"InstallDate",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);
        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->InstallDate = QString::fromWCharArray(tzValue);
            pApplication->InstallDate.insert(4,"/");
            pApplication->InstallDate.insert(7,"/");
//            qDebug() << pApplication->InstallDate;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"UninstallString",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);
        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->UninstallString = QString::fromWCharArray(tzValue);
        }

        if( !pApplication->Name.isEmpty() )
            m_qApplicationList.append(pApplication);
        else
            delete pApplication;

        memset(tzKeyName, 0, 512);
        dwKeyNameSize = 512;
        dwIndex++;
        RegCloseKey(hItem);
    }
    RegCloseKey(hKey);
    return Success;
}

int CApplicationManager::FillX86Applications()
{
    HKEY hKey = 0, hItem = 0;
    DWORD dwIndex = 0, dwKeyNameSize = 512, dwValueSize = 512, dwReturnCode = 0;
    TCHAR tzKeyName[512] = {0}, tzValue[512] = {0};
    Application *pApplication = 0;

    if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                                      NULL,
                                      KEY_READ | KEY_WOW64_32KEY,
                                      &hKey) )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    while( ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey,
                                               dwIndex,
                                               tzKeyName,
                                               &dwKeyNameSize,
                                               NULL,
                                               NULL,
                                               NULL,
                                               NULL) )
    {
        QString qzKeyName = QString::fromWCharArray(tzKeyName);
        if( qzKeyName.contains("}.KB") )
        {
            memset(tzKeyName, 0, 512);
            dwKeyNameSize = 512;
            dwIndex++;
            continue;
        }
        if( ERROR_SUCCESS != RegOpenKeyEx(hKey,
                                          tzKeyName,
                                          NULL,
                                          KEY_READ,
                                          &hItem) )
        {
            memset(tzKeyName, 0, 512);
            dwKeyNameSize = 512;
            dwIndex++;
            continue;
        }

        pApplication = new Application;

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"DisplayName",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);

        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->Name = QString::fromWCharArray(tzValue);
//            qDebug() << pApplication->Name;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"DisplayVersion",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);

        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->Version = QString::fromWCharArray(tzValue);
//            qDebug() << pApplication->Version;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"Publisher",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);
        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->Publisher = QString::fromWCharArray(tzValue);
//            qDebug() << pApplication->Publisher;
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"InstallDate",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);
        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->InstallDate = QString::fromWCharArray(tzValue);
            pApplication->InstallDate.insert(4,"/");
            pApplication->InstallDate.insert(7,"/");
        }

        dwValueSize = 512;
        memset(tzValue, 0, 512);
        dwReturnCode = RegQueryValueEx(hItem,
                                       L"UninstallString",
                                       NULL,
                                       NULL,
                                       (LPBYTE)tzValue,
                                       &dwValueSize);
        if( ERROR_SUCCESS != dwReturnCode )
        {
            DEBUG_STATUS(Unsuccessful);
        }
        else
        {
            pApplication->UninstallString = QString::fromWCharArray(tzValue);
        }

        if( !pApplication->Name.isEmpty() )
            m_qApplicationList.append(pApplication);
        else
            delete pApplication;

        memset(tzKeyName, 0, 512);
        dwKeyNameSize = 512;
        dwIndex++;
        RegCloseKey(hItem);
    }
    RegCloseKey(hKey);
    return Success;
}

QStandardItemModel* CApplicationManager::GetApplicationList()
{
    m_qApplicationList.clear();

    bool bArchitecture = IsX64System();
    if( bArchitecture )
    {
        FillX64Applications();
        FillX86Applications();
    }
    else
    {
        FillX86Applications();
    }

    QStandardItemModel *pModel = new QStandardItemModel();

    QStandardItem *pItem = NULL;

    pModel->setColumnCount(4);
    pModel->setRowCount(m_qApplicationList.count());
    pModel->setHorizontalHeaderLabels(QStringList() << "Application name" << "Publisher" << "Install date" << "Version");

    for (int i = 0; i < this->m_qApplicationList.count();i++)
    {
        pItem = new QStandardItem(this->m_qApplicationList.at(i)->Name);
        pItem->setData(this->m_qApplicationList.at(i)->UninstallString);
        pModel->setItem(i, 0, pItem);

        pItem = new QStandardItem(this->m_qApplicationList.at(i)->Publisher);
        pItem->setData(this->m_qApplicationList.at(i)->UninstallString);
        pModel->setItem(i, 1, pItem);

        pItem = new QStandardItem(this->m_qApplicationList.at(i)->InstallDate);
        pItem->setData(this->m_qApplicationList.at(i)->UninstallString);
        pModel->setItem(i, 2, pItem);

        pItem = new QStandardItem(this->m_qApplicationList.at(i)->Version);
        pItem->setData(this->m_qApplicationList.at(i)->UninstallString);
        pModel->setItem(i, 3, pItem);
    }

    return pModel;
}
