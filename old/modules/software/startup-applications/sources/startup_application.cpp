#include "../headers/startup_application.h"

CStartupApplication::CStartupApplication()
{
}

CStartupApplication::~CStartupApplication()
{
}

bool CStartupApplication::IsX64System()
{
    SYSTEM_INFO sysinfo;
    GetNativeSystemInfo(&sysinfo);
    if( PROCESSOR_ARCHITECTURE_AMD64 == sysinfo.wProcessorArchitecture )
        return true;
    return false;
}

int CStartupApplication::FillApplicationList()
{
    if( IsX64System() )
    {
        FillX64ApplicationList(HKEY_LOCAL_MACHINE);
        FillX64ApplicationList(HKEY_CURRENT_USER);
        FillX86ApplicationList(HKEY_LOCAL_MACHINE);
//        FillX86ApplicationList(HKEY_CURRENT_USER);
        FillAppDataApplicationList();
        FillCurrentUserApplicationList();
    }
    else
    {
        FillX86ApplicationList(HKEY_LOCAL_MACHINE);
        FillX86ApplicationList(HKEY_CURRENT_USER);
        FillAppDataApplicationList();
        FillCurrentUserApplicationList();
    }
    qDebug() << m_data.count();
    return Success;
}

int CStartupApplication::FillX64ApplicationList(HKEY key)
{
    qDebug() << "CStartupApplication::FillX64ApplicationList()\n";
    HKEY hKey = 0;
    DWORD dwIndex = 0, dwValueNameSize = 512, dwValueDataSize = 512, dwType;
    TCHAR tzValueName[512] = {0}, tzValueData[512] = {0};
    StartupApplication *pApplication = 0;

    if( ERROR_SUCCESS != RegOpenKeyEx(key,
                                      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                      NULL,
                                      KEY_READ | KEY_WOW64_64KEY,
                                      &hKey) )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    while( ERROR_NO_MORE_ITEMS != RegEnumValue(hKey,
                                               dwIndex,
                                               tzValueName,
                                               &dwValueNameSize,
                                               NULL,
                                               &dwType,
                                               (LPBYTE)tzValueData,
                                               &dwValueDataSize) )
    {
        if( REG_SZ == dwType || REG_EXPAND_SZ == dwType )
        {
            pApplication = new StartupApplication;
            pApplication->Name = QString::fromWCharArray(tzValueName);
            pApplication->CommandLine = QString::fromWCharArray(tzValueData);
            if( HKEY_CURRENT_USER == key )
                pApplication->Location = "HKCU:Run";
            if( HKEY_LOCAL_MACHINE == key )
                pApplication->Location = "HKLM:Run";
        }

        if( 0 != pApplication )
        {
            m_data.append(pApplication);
        }

        memset(tzValueName, 0, 512);
        dwValueNameSize = 512;
        dwValueDataSize = 512;
        dwIndex++;
        memset(tzValueData, 0, 512);
    }
    RegCloseKey(hKey);
    qDebug() << "CStartupApplication::FillX64ApplicationList() -> List Size = " << m_data.count() << "\n";
    return Success;
}

int CStartupApplication::FillX86ApplicationList(HKEY key)
{
    qDebug() << "CStartupApplication::FillX86ApplicationList()\n";
    HKEY hKey = 0;
    DWORD dwIndex = 0, dwValueNameSize = 512, dwValueDataSize = 512, dwType;
    TCHAR tzValueName[512] = {0}, tzValueData[512] = {0};
    StartupApplication *pApplication = 0;

    if( ERROR_SUCCESS != RegOpenKeyEx(key,
                                      L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                      NULL,
                                      KEY_READ | KEY_WOW64_32KEY,
                                      &hKey) )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    while( ERROR_NO_MORE_ITEMS != RegEnumValue(hKey,
                                               dwIndex,
                                               tzValueName,
                                               &dwValueNameSize,
                                               NULL,
                                               &dwType,
                                               (LPBYTE)tzValueData,
                                               &dwValueDataSize) )
    {
        if( REG_SZ == dwType || REG_EXPAND_SZ == dwType )
        {
            pApplication = new StartupApplication;
            pApplication->Name = QString::fromWCharArray(tzValueName);
            pApplication->CommandLine = QString::fromWCharArray(tzValueData);
            if( HKEY_CURRENT_USER == key )
                pApplication->Location = "HKCU:Run";
            if( HKEY_LOCAL_MACHINE == key )
                pApplication->Location = "HKLM:Run";
        }

        if( 0 != pApplication )
        {
            m_data.append(pApplication);
        }

        memset(tzValueName, 0, 512);
        dwValueNameSize = 512;
        dwValueDataSize = 512;
        dwIndex++;
        memset(tzValueData, 0, 512);
    }
    RegCloseKey(hKey);
    qDebug() << "CStartupApplication::FillX86ApplicationList() -> List Size = " << m_data.count() << "\n";
    return Success;
}



QList<PStartupApplication> CStartupApplication::GetApplicationList()
{
    return m_data;
}

int CStartupApplication::OnCreateWidget(QWidget **ppWidget)
{
    QWidget *pWidget = new QWidget();
    pWidget->setLayout(new QVBoxLayout());
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);

    QTableWidget *pTableWidget = new QTableWidget();
    QTableWidgetItem *pTableItem = 0;
    pTableWidget->setColumnCount(3);
    pTableWidget->setRowCount(this->m_data.count());
    pTableWidget->setHorizontalHeaderLabels(QStringList() << "Application name" << "Location" << "Command");
    pTableWidget->verticalHeader()->setVisible(false);
    pTableWidget->setShowGrid(false);
    pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    pTableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pTableWidget->setColumnWidth(0, 270);
    pTableWidget->setColumnWidth(2, 300);

    for (int i = 0; i < this->m_data.count();i++)
    {
        pTableItem = new QTableWidgetItem(this->m_data.at(i)->Name);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 0, pTableItem);
        pTableItem = new QTableWidgetItem(this->m_data.at(i)->Location);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 1, pTableItem);
        pTableItem = new QTableWidgetItem(this->m_data.at(i)->CommandLine);
        pTableItem->setFlags(pTableItem->flags() ^ Qt::ItemIsEditable);
        pTableWidget->setItem(i, 2, pTableItem);
    }

    pWidget->layout()->addWidget(pTableWidget);

    (*ppWidget) = pWidget;

    return Success;
}

void CStartupApplication::StartLoadingModuleSlot()
{
    FillApplicationList();
    emit LoadingCompleteSignal();
    emit CreateWidgetSignal(this);
}

int CStartupApplication::FillAppDataApplicationList()
{
    qDebug() << "CStartupApplication::FillAppDataApplicationList()";
    QString qzStartUpFolderPath = "";
    QString qzItem = "";
    StartupApplication *pApplication = 0;
    if( QSysInfo::WV_XP == QSysInfo::windowsVersion() || QSysInfo::WV_2003 == QSysInfo::windowsVersion() )
    {
        qzStartUpFolderPath = QDir::rootPath();
        qzStartUpFolderPath += "Documents and Settings/All Users/Start Menu/Programs/Startup/";
    }
    else
    {
        qzStartUpFolderPath = QDir::rootPath();
        qzStartUpFolderPath += "ProgramData/Microsoft/Windows/Start Menu/Programs/Startup/";
    }
    QDir StartupDir;
    StartupDir.cd(qzStartUpFolderPath);
    QStringList DirList = StartupDir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

    qDebug() << DirList.count();

    foreach( qzItem, DirList )
    {
        pApplication = new StartupApplication;
        pApplication->Name = qzItem;
        pApplication->Location = "Startup folder";
        if( qzItem.endsWith(".lnk") )
        {
            QString qzShortcutPath = qzStartUpFolderPath + qzItem;
            qzShortcutPath.replace("/", "\\");
            pApplication->CommandLine = GetTargetOfShortcut(qzShortcutPath);
        }
        else
        {
            if( qzItem.endsWith(".exe") )
            {
                pApplication->CommandLine = qzStartUpFolderPath + qzItem;
            }
        }
        m_data.append(pApplication);
        qDebug() << qzItem;
    }
    qDebug() << "CStartupApplication::FillAppDataApplicationList() -> List Size = " << m_data.count() << "\n";
    return Success;
}

int CStartupApplication::FillCurrentUserApplicationList()
{
    qDebug() << "CStartupApplication::FillCurrentUserApplicationList()";
    QString qzStartUpFolderPath = "";
    QString qzItem = "";
    StartupApplication *pApplication = 0;
    if( QSysInfo::WV_XP == QSysInfo::windowsVersion() || QSysInfo::WV_2003 == QSysInfo::windowsVersion() )
    {
        qzStartUpFolderPath = QDir::homePath();
        qzStartUpFolderPath += "/Start Menu/Programs/Startup/";
    }
    else
    {
        qzStartUpFolderPath = QDir::homePath();
        qzStartUpFolderPath += "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/";
    }
    QDir StartupDir;
    StartupDir.cd(qzStartUpFolderPath);
    QStringList DirList = StartupDir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

    qDebug() << DirList.count();

    foreach( qzItem, DirList )
    {
        pApplication = new StartupApplication;
        pApplication->Name = qzItem;
        pApplication->Location = "Startup folder";
        if( qzItem.endsWith(".lnk") )
        {
            QString qzShortcutPath = qzStartUpFolderPath + qzItem;
            qzShortcutPath.replace("/", "\\");
            pApplication->CommandLine = GetTargetOfShortcut(qzShortcutPath);
        }
        else
        {
            if( qzItem.endsWith(".exe") )
            {
                pApplication->CommandLine = qzStartUpFolderPath + qzItem;
            }
        }
        m_data.append(pApplication);
        qDebug() << qzItem;
    }
    qDebug() << "CStartupApplication::FillCurrentUserApplicationList() -> List Size = " << m_data.count() << "\n";
    return Success;
}

QString CStartupApplication::GetTargetOfShortcut(QString qzShortcutPath)
{
    IShellLink *pShellLink;
    TCHAR pTempString[MAX_PATH];
    WIN32_FIND_DATA win32DF;
    std::wstring path = qzShortcutPath.toStdWString();

    HRESULT hr;

//    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

//    switch( hr )
//    {
//    case E_INVALIDARG:
//        DEBUG_STATUS(Uninitialized);
//        break;
//    case E_OUTOFMEMORY:
//        DEBUG_STATUS(Uninitialized);
//        break;
//    case E_UNEXPECTED:
//        DEBUG_STATUS(Uninitialized);
//        break;
//    case S_OK:
//        DEBUG_STATUS(Uninitialized);
//        break;
//    case S_FALSE:
//        DEBUG_STATUS(Uninitialized);
//        break;
//    case RPC_E_CHANGED_MODE:
//        DEBUG_STATUS(Uninitialized);
//        break;
//    }

//    if(FAILED(hr))
//    {
//        DEBUG_STATUS(Unsuccessful);
//        return "";
//    }

    hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*) &pShellLink);

    if(SUCCEEDED(hr))
    {
        IPersistFile* pPersistFile;
        hr = pShellLink->QueryInterface( IID_IPersistFile, (LPVOID *) &pPersistFile);
        if(SUCCEEDED(hr))
        {
            hr = pPersistFile->Load(path.c_str(), STGM_READ);

            if(SUCCEEDED(hr))
            {
                pShellLink->GetPath(pTempString, MAX_PATH, &win32DF, SLGP_UNCPRIORITY | SLGP_RAWPATH);
                return QString::fromWCharArray(pTempString);
            }
        }
    }
    return "";
}




