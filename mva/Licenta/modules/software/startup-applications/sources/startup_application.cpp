#include "../headers/startup_application.h"

CStartupManager::CStartupManager(QObject *pParent) :
    QObject(pParent), m_pDataModel( NULL )
{
    m_pDataModel = new QStandardItemModel(this);
}

CStartupManager::~CStartupManager()
{
    OnClearInternalMap();
}

QStandardItemModel *CStartupManager::GetDataModel()
{
    return m_pDataModel;
}

void CStartupManager::OnChangeApplicationState(QString qsApplication)
{
    StartupEntry *pEntry = m_qStartupEntries.value(qsApplication, NULL);
    if( NULL == pEntry )
        return;

    int nStatus = -1;
    HKEY    hKey = 0;
    nStatus = RegOpenKeyEx((pEntry->qsStartupHive == "HKCU" ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE),
                           pEntry->qsStartupApprovedKeyPath.toStdWString().c_str(),
                           0,
                           KEY_ALL_ACCESS | (pEntry->bX64 ? KEY_WOW64_64KEY : KEY_WOW64_32KEY),
                           &hKey);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    BYTE bData[12] = {0};

    if( pEntry->qsState == "Enabled" )
        bData[0] = 0x03;
    else if( pEntry->qsState == "Disabled" )
        bData[0] = 0x02;

    if( bData[0] == 0 )
    {
        RegCloseKey(hKey);
        return;
    }

    DWORD dwDataSize = sizeof(bData);

    nStatus = RegSetValueEx(hKey, pEntry->qsName.toStdWString().c_str(), 0, REG_BINARY, bData, dwDataSize);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
    }

    RegCloseKey(hKey);
}

void CStartupManager::OnRemoveApplication(QString qsApplication)
{
    StartupEntry *pEntry = m_qStartupEntries.value(qsApplication, NULL);
    if( NULL == pEntry )
        return;

    int nStatus = -1;
    HKEY    hKey = 0;

    // Check if has registry key assigned
    if( pEntry->qsRunHive.isEmpty() )
    {
        QFile qFile(pEntry->qsRunKeyPath);
        qFile.remove();
    }
    else
    {
        nStatus = RegOpenKeyEx((pEntry->qsRunHive == "HKCU" ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE),
                               pEntry->qsRunKeyPath.toStdWString().c_str(),
                               0,
                               KEY_ALL_ACCESS | (pEntry->bX64 ? KEY_WOW64_64KEY : KEY_WOW64_32KEY),
                               &hKey);
        if( ERROR_SUCCESS != nStatus )
        {
            CHECK_OPERATION_STATUS(Unsuccessful);
            return;
        }

        nStatus = RegDeleteValue(hKey, pEntry->qsName.toStdWString().c_str());

        if( ERROR_SUCCESS != nStatus )
        {
            CHECK_OPERATION_STATUS(Unsuccessful);
        }

        RegCloseKey(hKey);
    }

    // Delete startup approved key
    nStatus = RegOpenKeyEx((pEntry->qsStartupHive == "HKCU" ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE),
                           pEntry->qsStartupApprovedKeyPath.toStdWString().c_str(),
                           0,
                           KEY_ALL_ACCESS | (pEntry->bX64 ? KEY_WOW64_64KEY : KEY_WOW64_32KEY),
                           &hKey);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    nStatus = RegDeleteValue(hKey, pEntry->qsName.toStdWString().c_str());
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
    }

    RegCloseKey(hKey);
}

void CStartupManager::OnAddApplication(QString qsAppName, QString qsPath)
{
    int nStatus = -1;
    HKEY hKey = 0;

    QString qsLocation = STARTUP_LOCATION_1;

    qsPath.replace("/", "\\");

    nStatus = RegOpenKeyEx(HKEY_CURRENT_USER, qsLocation.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    WCHAR *wsChar = CharArrayToWcharArray(qsPath.toLatin1().data());
    DWORD dwSize = sizeof(WCHAR) * wcslen(wsChar);

    nStatus = RegSetValueEx(hKey, qsAppName.toStdWString().c_str(), 0, REG_SZ, (LPBYTE)wsChar, dwSize );
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
    }

    RegCloseKey(hKey);

    // Make this application enabled
    qsLocation = QString(STARTUP_PATH_1) + QString("\\Run");
    nStatus = RegOpenKeyEx(HKEY_CURRENT_USER, qsLocation.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    BYTE bData[12] = {0};
    bData[0] = 0x02;

    nStatus = RegSetValueEx(hKey, qsAppName.toStdWString().c_str(), 0, REG_BINARY, bData, sizeof(bData));
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
    }

    RegCloseKey(hKey);
}

void CStartupManager::OnRefreshData()
{
    OnClearInternalMap();

    FillGenericApplications(HKEY_CURRENT_USER, STARTUP_LOCATION_1, false);
    FillGenericApplications(HKEY_CURRENT_USER, STARTUP_LOCATION_1, true);
    FillGenericApplications(HKEY_CURRENT_USER, STARTUP_LOCATION_2, false);
    FillGenericApplications(HKEY_CURRENT_USER, STARTUP_LOCATION_2, true);

    FillGenericApplications(HKEY_LOCAL_MACHINE, STARTUP_LOCATION_1, false);
    FillGenericApplications(HKEY_LOCAL_MACHINE, STARTUP_LOCATION_1, true);
    FillGenericApplications(HKEY_LOCAL_MACHINE, STARTUP_LOCATION_2, false);
    FillGenericApplications(HKEY_LOCAL_MACHINE, STARTUP_LOCATION_2, true);

    FillApplicationsFromFolder(ALL_USERS);
    FillApplicationsFromFolder(CURRENT_USER);

    CompleteGenericApplications(HKEY_CURRENT_USER, STARTUP_PATH_1, false);
    CompleteGenericApplications(HKEY_CURRENT_USER, STARTUP_PATH_1, true);
    CompleteGenericApplications(HKEY_LOCAL_MACHINE, STARTUP_PATH_1, false);
    CompleteGenericApplications(HKEY_LOCAL_MACHINE, STARTUP_PATH_1, true);

    OnPopulateData();
}

void CStartupManager::FillGenericApplications(HKEY hKey, QString qsKeyPath, bool bX64Hive)
{
    int     nStatus = 0;
    HKEY    hCurrentKey = 0;
    DWORD   dwIndex = 0, dwValueNameSize = 512, dwValueDataSize = 512, dwType;
    TCHAR   tzValueName[512] = {0}, tzValueData[512] = {0};

    StartupEntry *pEntry = 0;

    nStatus = RegOpenKeyEx(hKey, qsKeyPath.toStdWString().c_str(), 0, KEY_READ | (bX64Hive ? KEY_WOW64_64KEY : KEY_WOW64_32KEY), &hCurrentKey);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    while( ERROR_NO_MORE_ITEMS != RegEnumValue(hCurrentKey,
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
            pEntry = new StartupEntry;
            pEntry->qsName = QString::fromWCharArray(tzValueName);
            pEntry->qsCommandLine = QString::fromWCharArray(tzValueData);
            if( HKEY_CURRENT_USER == hKey )
            {
                pEntry->qsLocation = "HKCU:" + qsKeyPath.split("\\").last();
                pEntry->qsRunHive = "HKCU";
            }
            if( HKEY_LOCAL_MACHINE == hKey )
            {
                pEntry->qsLocation = "HKLM:" + qsKeyPath.split("\\").last();
                pEntry->qsRunHive = "HKLM";
            }
            pEntry->qsRunKeyPath = qsKeyPath;

            pEntry->qsPublisher = GetCompanyNameOfExe(pEntry->qsCommandLine);

            pEntry->bX64 = bX64Hive;
        }

        if( 0 != pEntry )
            m_qStartupEntries.insert(pEntry->qsName, pEntry);

        memset(tzValueName, 0, sizeof(tzValueName));
        memset(tzValueData, 0, sizeof(tzValueData));

        dwValueNameSize = sizeof(tzValueName);
        dwValueDataSize = sizeof(tzValueData);

        dwIndex++;
    }
    RegCloseKey(hCurrentKey);
}

void CStartupManager::FillApplicationsFromFolder(UserScope eScope)
{
    QString qzStartUpFolderPath = "";
    QString qzItem = "";
    StartupEntry *pApplication = 0;

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
        pApplication = new StartupEntry;
        pApplication->qsName = qzItem;
        pApplication->qsLocation = "Startup folder";
        if( qzItem.endsWith(".lnk") )
        {
            QString qzShortcutPath = qzStartUpFolderPath + qzItem;
            pApplication->qsRunKeyPath = qzShortcutPath;
            qzShortcutPath.replace("/", "\\");
            pApplication->qsCommandLine = GetExePathOfShortcut(qzShortcutPath);
        }
        else /*if( qzItem.endsWith(".exe") )*/
        {
            pApplication->qsCommandLine = qzStartUpFolderPath + qzItem;
            pApplication->qsRunKeyPath = pApplication->qsCommandLine;
        }
        pApplication->qsCommandLine = pApplication->qsCommandLine.replace("/", "\\");
        pApplication->qsPublisher = GetCompanyNameOfExe(pApplication->qsCommandLine);

        m_qStartupEntries.insert(pApplication->qsName, pApplication);
    }
}

void CStartupManager::CompleteGenericApplications(HKEY hKey, QString qsKeyPath, bool bX64Hive)
{
    int     nStatus = 0;
    HKEY    hCurrentKey = 0;
    HKEY    hEnumKey = 0;

    StartupEntry *pEntry = 0;

    nStatus = RegOpenKeyEx(hKey, qsKeyPath.toStdWString().c_str(), 0, KEY_READ | (bX64Hive ? KEY_WOW64_64KEY : KEY_WOW64_32KEY), &hCurrentKey);
    if( ERROR_SUCCESS != nStatus )
    {
        CHECK_OPERATION_STATUS(Unsuccessful);
        return;
    }

    TCHAR   tzKeyName[512] = {0}, tzEntryName[512] = {0};
    DWORD   dwKeyIndex = 0, dwValIndex = 0, dwKeyNameSize = 512, dwEntryNameSize = 512, dwValueDataSize = 512, dwType;
    BYTE    pbValueData[512] = {0};

    while( ERROR_NO_MORE_ITEMS != RegEnumKey(hCurrentKey,
                                             dwKeyIndex,
                                             tzKeyName,
                                             dwKeyNameSize) )
    {
        nStatus = RegOpenKeyEx(hCurrentKey, tzKeyName, 0, KEY_READ, &hEnumKey);
        if( ERROR_SUCCESS != nStatus )
        {
            memset(tzKeyName, 0, sizeof(tzKeyName));
            dwKeyNameSize = sizeof(tzKeyName);
            dwKeyIndex++;

            continue;
        }

        dwValIndex = 0;
        while( ERROR_NO_MORE_ITEMS != RegEnumValue(hEnumKey,
                                                   dwValIndex,
                                                   tzEntryName,
                                                   &dwEntryNameSize,
                                                   0,
                                                   &dwType,
                                                   pbValueData,
                                                   &dwValueDataSize) )
        {
            QString qsEntryName = QString::fromWCharArray(tzEntryName);
            pEntry = m_qStartupEntries.value(qsEntryName, NULL);
            if( NULL == pEntry )
            {
                memset(tzEntryName, 0, sizeof(tzEntryName));
                memset(pbValueData, 0, sizeof(pbValueData));

                dwEntryNameSize = sizeof(tzEntryName);
                dwValueDataSize = sizeof(pbValueData);

                dwValIndex++;
                continue;
            }

            if( pbValueData[0] == 0x02 )
                pEntry->qsState = "Enabled";
            else
                pEntry->qsState = "Disabled";

            if( HKEY_CURRENT_USER == hKey )
                pEntry->qsStartupHive = "HKCU";
            if( HKEY_LOCAL_MACHINE == hKey )
                pEntry->qsStartupHive = "HKLM";

            pEntry->qsStartupApprovedKeyPath = qsKeyPath + "\\" + QString::fromWCharArray(tzKeyName);

            DWORD dwLowPart = 0, dwHighPart = 0;
            dwLowPart |= (pbValueData[4] << 24) & 0xFF000000;
            dwLowPart |= (pbValueData[5] << 16) & 0xFF0000;
            dwLowPart |= (pbValueData[6] << 8) & 0xFF00;
            dwLowPart |= pbValueData[7] & 0xFF;

            dwHighPart |= pbValueData[8] << 24;
            dwHighPart |= pbValueData[9] << 16;
            dwHighPart |= pbValueData[10] << 8;
            dwHighPart |= pbValueData[11];

            FILETIME ftTime;
            ftTime.dwLowDateTime = dwLowPart;
            ftTime.dwHighDateTime = dwLowPart;

            SYSTEMTIME stTime;
            FileTimeToSystemTime(&ftTime, &stTime);

            memset(tzEntryName, 0, sizeof(tzEntryName));
            memset(pbValueData, 0, sizeof(pbValueData));

            dwEntryNameSize = sizeof(tzEntryName);
            dwValueDataSize = sizeof(pbValueData);

            dwValIndex++;
        }

        RegCloseKey(hEnumKey);

        memset(tzKeyName, 0, sizeof(tzKeyName));
        dwKeyNameSize = sizeof(tzKeyName);
        dwKeyIndex++;
    }

    RegCloseKey(hCurrentKey);
}

QString CStartupManager::GetCompanyNameOfExe(QString qsExePath)
{
    BOOL bResult = FALSE;

    LPVOID lpOutBuffer = 0;
    DWORD dwBufferSize = 0, dwUseless = 0;

    UINT uiVar1 = 0, uiVar2 = 0;

    LPBYTE lpBuffer;

    QString qsRetVal;

    QString qsCorrectPath;

    if( qsExePath.contains("\"") )
    {
        qsCorrectPath = qsExePath.right(qsExePath.length() - 1);
        qsCorrectPath = qsCorrectPath.left(qsCorrectPath.indexOf("\""));
    }
    else
        qsCorrectPath = qsExePath;

    if( qsCorrectPath.contains("/") )
        qsCorrectPath = qsCorrectPath.left(qsCorrectPath.indexOf("/"));

    if( qsCorrectPath.contains("rundll") )
    {
        qsCorrectPath = qsCorrectPath.right(qsCorrectPath.length() - qsCorrectPath.indexOf(".exe") - 5);
        qsCorrectPath = qsCorrectPath.left(qsCorrectPath.indexOf(".dll") + 4);
    }

    dwBufferSize = GetFileVersionInfoSizeEx(FILE_VER_GET_LOCALISED, qsCorrectPath.toStdWString().c_str(), &dwUseless);
    lpOutBuffer = malloc(dwBufferSize * sizeof(BYTE));

    bResult = GetFileVersionInfoEx(FILE_VER_GET_LOCALISED, qsCorrectPath.toStdWString().c_str(), NULL, dwBufferSize, lpOutBuffer);

    struct LANGANDCODEPAGE *lpLang;
    bResult = VerQueryValue(lpOutBuffer, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpLang, &uiVar1);

    for(int i = 0; i < (uiVar1 / sizeof(LANGANDCODEPAGE)); i++)
    {
        QString qsBlock;
        qsBlock.sprintf("\\StringFileInfo\\%04x%04x\\CompanyName", lpLang[i].wLanguage, lpLang[i].wCodePage);

        bResult = VerQueryValue(lpOutBuffer, qsBlock.toStdWString().c_str(), (LPVOID*)&lpBuffer, &uiVar2);

        qsRetVal = QString::fromWCharArray((WCHAR*)lpBuffer);

        if( qsRetVal.isEmpty() )
        {
            qsBlock.sprintf("\\StringFileInfo\\%04x%04x\\ProductName", lpLang[i].wLanguage, lpLang[i].wCodePage);

            bResult = VerQueryValue(lpOutBuffer, qsBlock.toStdWString().c_str(), (LPVOID*)&lpBuffer, &uiVar2);

            qsRetVal = QString::fromWCharArray((WCHAR*)lpBuffer);
        }
    }

    free(lpOutBuffer);

    return qsRetVal;
}

QString CStartupManager::GetExePathOfShortcut(QString qsShortcutPath)
{
    IShellLink *pShellLink = 0;
    TCHAR pTempString[MAX_PATH] = {0};
    WIN32_FIND_DATA win32DF;
    std::wstring path = qsShortcutPath.toStdWString();

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

void CStartupManager::OnPopulateData()
{
    m_pDataModel->clear();
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Location" << "Command line" << "Publisher" << "State");

    QMap<QString, StartupEntry*>::iterator it;
    QList<QStandardItem *> qRow;

    for( it = m_qStartupEntries.begin(); it != m_qStartupEntries.end(); it++ )
    {
        QStandardItem *pItem = new QStandardItem(it.value()->qsName);
        pItem->setToolTip(it.value()->qsName);
        pItem->setData(it.value()->qsName);
        qRow << pItem;

        pItem = new QStandardItem(it.value()->qsLocation);
        pItem->setToolTip(it.value()->qsLocation);
        qRow << pItem;

        pItem = new QStandardItem(it.value()->qsCommandLine);
        pItem->setToolTip(it.value()->qsCommandLine);
        qRow << pItem;

        pItem = new QStandardItem(it.value()->qsPublisher == "" ? "N/A" : it.value()->qsPublisher);
        pItem->setToolTip(it.value()->qsPublisher);
        qRow << pItem;

        pItem = new QStandardItem(it.value()->qsState);
        pItem->setToolTip(it.value()->qsState);
        qRow << pItem;

        m_pDataModel->appendRow(qRow);
        qRow.clear();
    }
}

void CStartupManager::OnClearInternalMap()
{
    foreach( StartupEntry *pEntry, m_qStartupEntries.values() )
    {
        SAFE_DELETE(pEntry);
    }
    m_qStartupEntries.clear();
}
