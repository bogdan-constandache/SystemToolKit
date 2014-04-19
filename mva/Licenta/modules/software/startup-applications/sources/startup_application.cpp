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

QStandardItemModel *CStartupApplication::GetStartupApplicationsInformations()
{
    m_qApplicationList.clear();

    if( IsX64System() )
    {
        FillX64ApplicationList(HKEY_LOCAL_MACHINE);
        FillX64ApplicationList(HKEY_CURRENT_USER);
        FillX86ApplicationList(HKEY_LOCAL_MACHINE);
    }
    else
    {
        FillX86ApplicationList(HKEY_LOCAL_MACHINE);
        FillX86ApplicationList(HKEY_CURRENT_USER);
    }
    FillApplicationList(ALL_USERS);
    FillApplicationList(CURRENT_USER);

    StartupApplication *pApp = 0;
    QStandardItemModel *pModel = new QStandardItemModel;
    QList<QStandardItem*> qList;

    pModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Location" << "Path");

    foreach(pApp, m_qApplicationList)
    {
        qList.append(new QStandardItem(pApp->Name));
        qList.append(new QStandardItem(pApp->Location));
        qList.append(new QStandardItem(pApp->CommandLine));

        pModel->invisibleRootItem()->appendRow(qList);
        qList.clear();
    }

    return pModel;
}

int CStartupApplication::FillX64ApplicationList(HKEY key)
{
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
            m_qApplicationList.append(pApplication);

        memset(tzValueName, 0, 512);
        dwValueNameSize = 512;
        dwValueDataSize = 512;
        dwIndex++;
        memset(tzValueData, 0, 512);
    }
    RegCloseKey(hKey);
    return Success;
}

int CStartupApplication::FillX86ApplicationList(HKEY key)
{
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
            m_qApplicationList.append(pApplication);

        memset(tzValueName, 0, 512);
        dwValueNameSize = 512;
        dwValueDataSize = 512;
        dwIndex++;
        memset(tzValueData, 0, 512);
    }
    RegCloseKey(hKey);
    return Success;
}

int CStartupApplication::FillApplicationList(UserScope eScope)
{
    QString qzStartUpFolderPath = "";
    QString qzItem = "";
    StartupApplication *pApplication = 0;
    if( QSysInfo::WV_XP == QSysInfo::windowsVersion() || QSysInfo::WV_2003 == QSysInfo::windowsVersion() )
    {
        if( ALL_USERS == eScope )
        {
            qzStartUpFolderPath = QDir::rootPath();
            qzStartUpFolderPath += "Documents and Settings/All Users/Start Menu/Programs/Startup/";
        }
        else if( CURRENT_USER == eScope )
        {
            qzStartUpFolderPath = QDir::homePath();
            qzStartUpFolderPath += "/Start Menu/Programs/Startup/";
        }
    }
    else
    {
        if( ALL_USERS == eScope )
        {
            qzStartUpFolderPath = QDir::rootPath();
            qzStartUpFolderPath += "ProgramData/Microsoft/Windows/Start Menu/Programs/Startup/";
        }
        else if( CURRENT_USER == eScope )
        {
            qzStartUpFolderPath = QDir::homePath();
            qzStartUpFolderPath += "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/";
        }
    }
    QDir StartupDir;
    StartupDir.cd(qzStartUpFolderPath);
    QStringList DirList = StartupDir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Name);

    foreach(qzItem, DirList)
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
        else if( qzItem.endsWith(".exe") )
        {
            pApplication->CommandLine = qzStartUpFolderPath + qzItem;
        }

        m_qApplicationList.append(pApplication);
    }
    return Success;
}

QString CStartupApplication::GetTargetOfShortcut(QString qzShortcutPath)
{
    IShellLink *pShellLink;
    TCHAR pTempString[MAX_PATH];
    WIN32_FIND_DATA win32DF;
    std::wstring path = qzShortcutPath.toStdWString();

    HRESULT hr;

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




