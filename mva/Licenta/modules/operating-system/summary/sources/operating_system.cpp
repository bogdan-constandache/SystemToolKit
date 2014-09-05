#include "../headers/operating_system.h"

COperatingSystemInformation::COperatingSystemInformation(): m_pOSInfo(0), m_pDataModel(0)
{
    m_pDataModel = new QStandardItemModel;
    CHECK_ALLOCATION(m_pDataModel);
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    m_pOSInfo = new OSInformation;
    CHECK_ALLOCATION(m_pOSInfo);

    GetOSProperties();

    QList<QStandardItem*> qList;

    qList << new QStandardItem("OS name: ") << new QStandardItem(m_pOSInfo->qszOSName);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("OS language: ") << new QStandardItem(m_pOSInfo->qszOSLanguage);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Kernel type: ") << new QStandardItem(m_pOSInfo->qszKernelType);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("OS version: ") << new QStandardItem(m_pOSInfo->qszOSVersion);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Service pack: ") << new QStandardItem(m_pOSInfo->qszServicePack);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Install date: ") << new QStandardItem(m_pOSInfo->qszInstallDate);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Root path: ") << new QStandardItem(m_pOSInfo->qszRoot);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Registered owner: ") << new QStandardItem(m_pOSInfo->qszRegisteredOwner);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Registered organization: ") << new QStandardItem(m_pOSInfo->qszRegisteredOrganization);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Product ID: ") << new QStandardItem(m_pOSInfo->qszProductID);
    m_pDataModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Product Key: ") << new QStandardItem(m_pOSInfo->qszProductKey);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem() << new QStandardItem();
    m_pDataModel->appendRow(qList); qList.clear();

    QStandardItem *pFeatures = new QStandardItem("Features: ");
    m_pDataModel->appendRow(pFeatures);

    qList << new QStandardItem("DBCS version: ") << new QStandardItem(m_pOSInfo->bDBCS ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("Debug version: ") << new QStandardItem(m_pOSInfo->bDebug ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("Security present: ") << new QStandardItem(m_pOSInfo->bSecure ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("Network present: ") << new QStandardItem(m_pOSInfo->bNetwork ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("Remote session: ") << new QStandardItem(m_pOSInfo->bRemote ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("Terminal services: ") << new QStandardItem(m_pOSInfo->bTerminalServices ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("Slow processor: ") << new QStandardItem(m_pOSInfo->bSlowMachine ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
    qList << new QStandardItem("IMM version: ") << new QStandardItem(m_pOSInfo->bIMM ? "Yes" : "No");
    pFeatures->appendRow(qList); qList.clear();
}

COperatingSystemInformation::~COperatingSystemInformation()
{
    SAFE_DELETE(m_pOSInfo);
    SAFE_DELETE(m_pDataModel);
}

QStandardItemModel *COperatingSystemInformation::GetOSModelInformation()
{
    return m_pDataModel;
}

int COperatingSystemInformation::GetOSProperties()
{
    int nStatus = Uninitialized;
    QString qszVersion = "";
    HKEY hKey = 0;
    DWORD dwType = 0, dwSize = 1024;
    BYTE bData[1024] = {0};
    WCHAR wszLocaleName[MAX_PATH] = {0};

    nStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                           L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                           0, KEY_READ, &hKey);
    CHECK_OPERATION_STATUS_EX(nStatus);

    // os name
    nStatus = RegQueryValueEx(hKey, L"ProductName", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszOSName = QString("Microsoft ") + QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // service pack
    nStatus = RegQueryValueEx(hKey, L"CSDVersion", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszServicePack = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // kernel type
    nStatus = RegQueryValueEx(hKey,  L"CurrentType", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszKernelType = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    //version
    nStatus = RegQueryValueEx(hKey, L"CurrentVersion", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        qszVersion = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    nStatus = RegQueryValueEx(hKey, L"BuildLabEx", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType )
    {
        // to fix the version
        QString qszTemp = QString::fromWCharArray((WCHAR*)bData);
        QStringList qTokens = qszTemp.split(".");
    }
    m_pOSInfo->qszOSVersion = qszVersion;
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // root
    nStatus = RegQueryValueEx(hKey, L"SystemRoot", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszRoot = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // getting locale name
    if( 0 < GetSystemDefaultLocaleName(wszLocaleName, MAX_PATH) )
    {
        m_pOSInfo->qszOSLanguage = WcharArrayToQString(wszLocaleName);
    }

    // registered owner
    nStatus = RegQueryValueEx(hKey, L"RegisteredOwner", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszRegisteredOwner = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // registered organization
    nStatus = RegQueryValueEx(hKey, L"RegisteredOrganization", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszRegisteredOrganization = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // product id
    nStatus = RegQueryValueEx(hKey, L"ProductId", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_SZ == dwType)
        m_pOSInfo->qszProductID = QString::fromWCharArray((WCHAR*)bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // product key
    nStatus = RegQueryValueEx(hKey, L"DigitalProductId", NULL, &dwType, bData, &dwSize);
    CHECK_OPERATION_STATUS(nStatus);

    if( REG_BINARY == dwType)
        m_pOSInfo->qszProductKey = DecodeProductKey(bData);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    int nMetric = 0;

    nMetric = GetSystemMetrics(SM_DBCSENABLED);
    if( nMetric )
        m_pOSInfo->bDBCS = true;
    else
        m_pOSInfo->bDBCS = false;

    nMetric = GetSystemMetrics(SM_DEBUG);
    if( nMetric )
        m_pOSInfo->bDebug = true;
    else
        m_pOSInfo->bDebug = false;

    nMetric = GetSystemMetrics(SM_IMMENABLED);
    if( nMetric )
        m_pOSInfo->bIMM = true;
    else
        m_pOSInfo->bIMM = false;

    nMetric = GetSystemMetrics(SM_NETWORK);
    if( nMetric )
        m_pOSInfo->bNetwork = true;
    else
        m_pOSInfo->bNetwork = false;

    nMetric = GetSystemMetrics(SM_REMOTECONTROL);
    if( nMetric )
        m_pOSInfo->bRemote = true;
    else
        m_pOSInfo->bRemote = false;

    nMetric = GetSystemMetrics(SM_REMOTESESSION);
    if( nMetric )
        m_pOSInfo->bTerminalServices = false;
    else
        m_pOSInfo->bTerminalServices = true;

    nMetric = GetSystemMetrics(SM_SLOWMACHINE);
    if( nMetric )
        m_pOSInfo->bSlowMachine = true;
    else
        m_pOSInfo->bSlowMachine = false;

    nMetric = GetSystemMetrics(SM_SECURE);
    if( nMetric )
        m_pOSInfo->bSecure = true;
    else
        m_pOSInfo->bSecure = false;

    nStatus = Success;
    return nStatus;
}

QString COperatingSystemInformation::DecodeProductKey(BYTE *bData)
{
    BYTE pbProductKey[15] = {0};

    char pchCDKey[256] = {0};

    char *pChars[] = { "B", "C", "D", "F", "G",
                       "H", "J", "K", "M", "P",
                       "Q", "R", "T", "V", "W",
                       "X", "Y", "2", "3", "4",
                       "6", "7", "8", "9", NULL };

    long nValue = 0;

    // Product id starts from 52 by 66
    for(int i = 52; i <= 66; i++)
    {
        pbProductKey[i - 52] = bData[i];
    }

    for(int i = 24; i >= 0; i--)
    {
        nValue = 0;
        for(int j = 14; j >= 0; j--)
        {
            nValue = (nValue * 256) ^ pbProductKey[j];
            pbProductKey[j] = nValue / 24;
            nValue = nValue % 24;
        }

        _strrev(pchCDKey);
        strcat_s(pchCDKey, pChars[nValue]);
        _strrev(pchCDKey);

        if( !(i % 5) && (i))
        {
            _strrev(pchCDKey);
            strcat_s(pchCDKey, "-");
            _strrev(pchCDKey);
        }
    }

    return QString(pchCDKey);
}
