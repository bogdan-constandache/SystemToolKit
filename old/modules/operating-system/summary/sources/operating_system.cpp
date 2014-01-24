#include "../headers/operating_system.h"

OperatingSystem::OperatingSystem(): m_pOSInfo(0)
{
}

OperatingSystem::~OperatingSystem()
{
}

int OperatingSystem::Initialize()
{
    int nStatus = Uninitialized;
    m_pOSInfo = new OSInformation;
    if( 0 == m_pOSInfo )
    {
        nStatus = NotAllocated;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    nStatus = GetOSProperties();

    nStatus = Success;
    return nStatus;
}

int OperatingSystem::Destroy()
{
    int nStatus = Uninitialized;

    nStatus = Success;
    return nStatus;
}


int OperatingSystem::GetOSProperties()
{
    int nStatus = Uninitialized;
    QString qszVersion = "";
    HKEY hKey = 0;
    DWORD dwType = 0;
    DWORD dwSize = 1024;
    BYTE bData[1024] = {0};
    WCHAR wszLocaleName[MAX_PATH] = {0};

    nStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                           L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                           0,
                           KEY_READ,
                           &hKey);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    // os name
    nStatus = RegQueryValueEx(hKey,
                              L"ProductName",
                              NULL,
                              &dwType,
                              bData,
                              &dwSize);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = GetLastError();
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( REG_SZ == dwType)
        m_pOSInfo->qszOSName = QString("Microsoft ") + ByteToQString(bData, dwSize);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // service pack
    nStatus = RegQueryValueEx(hKey,
                              L"CSDVersion",
                              NULL,
                              &dwType,
                              bData,
                              &dwSize);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = GetLastError();
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( REG_SZ == dwType)
        m_pOSInfo->qszServicePack = ByteToQString(bData, dwSize);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // kernel type
    nStatus = RegQueryValueEx(hKey,
                              L"CurrentType",
                              NULL,
                              &dwType,
                              bData,
                              &dwSize);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = GetLastError();
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( REG_SZ == dwType)
        m_pOSInfo->qszKernelType = ByteToQString(bData, dwSize);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    //version
    nStatus = RegQueryValueEx(hKey,
                              L"CurrentVersion",
                              NULL,
                              &dwType,
                              bData,
                              &dwSize);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = GetLastError();
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( REG_SZ == dwType)
        qszVersion =  ByteToQString(bData, dwSize);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    nStatus = RegQueryValueEx(hKey,
                              L"BuildLabEx",
                              NULL,
                              &dwType,
                              bData,
                              &dwSize);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = GetLastError();
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( REG_SZ == dwType )
    {
        // to fix the version
        QString qszTemp = ByteToQString(bData, dwSize);
        QStringList qTokens = qszTemp.split(".");
    }
    m_pOSInfo->qszOSVersion = qszVersion;
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // root
    nStatus = RegQueryValueEx(hKey,
                              L"SystemRoot",
                              NULL,
                              &dwType,
                              bData,
                              &dwSize);
    if( ERROR_SUCCESS != nStatus )
    {
        nStatus = GetLastError();
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    if( REG_SZ == dwType)
        m_pOSInfo->qszRoot = ByteToQString(bData, dwSize);
    ZeroMemory(bData, 1024);
    dwSize = 1024;

    // getting locale name
    if( 0 < GetSystemDefaultLocaleName(wszLocaleName, MAX_PATH) )
    {
        m_pOSInfo->qszOSLanguage = WcharArrayToQString(wszLocaleName);
    }



    nStatus = Success;
    return nStatus;
}
