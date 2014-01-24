#include "../headers/application_manager.h"

CApplicationManager::CApplicationManager()
{
}

CApplicationManager::~CApplicationManager()
{
}

int CApplicationManager::FillApplicationList()
{
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

    emit CreateWidgetSignal(this);
    return Success;
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
//            qDebug() << pApplication->UninstallString;
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
//            qDebug() << pApplication->UninstallString;
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

QList<PApplication> CApplicationManager::GetApplicationList()
{
    return m_qApplicationList;
}

int CApplicationManager::OnCreateWidget(QWidget **ppWidget)
{
    QWidget *pWidget = new QWidget();
    pWidget->setLayout(new QVBoxLayout());
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);

    QTableWidget *pTableWidget = new QTableWidget();
    QTableWidgetItem *pTableItem = 0;
    pTableWidget->setColumnCount(4);
    pTableWidget->setRowCount(this->m_qApplicationList.count());
    pTableWidget->setHorizontalHeaderLabels(QStringList() << "Application name" << "Publisher" << "Install date" << "Version");
    pTableWidget->verticalHeader()->setVisible(false);
    pTableWidget->setShowGrid(false);
    pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    pTableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pTableWidget->setColumnWidth(0, 270);
    pTableWidget->setColumnWidth(1, 150);

    for (int i = 0; i < this->m_qApplicationList.count();i++)
    {
        pTableItem = new QTableWidgetItem(this->m_qApplicationList.at(i)->Name);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 0, pTableItem);
        pTableItem = new QTableWidgetItem(this->m_qApplicationList.at(i)->Publisher);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 1, pTableItem);
        pTableItem = new QTableWidgetItem(this->m_qApplicationList.at(i)->InstallDate);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 2, pTableItem);
        pTableItem = new QTableWidgetItem(this->m_qApplicationList.at(i)->Version);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 3, pTableItem);
    }

    pWidget->layout()->addWidget(pTableWidget);

    (*ppWidget) = pWidget;

    return Success;
}

void CApplicationManager::StartLoadingModuleSlot()
{
    this->FillApplicationList();
    emit LoadingCompleteSignal();
}
