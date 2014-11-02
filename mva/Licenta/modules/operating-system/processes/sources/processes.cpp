#include "../headers/processes.h"

Processes::Processes() : m_pModuleModel(NULL), m_pProcessModel(NULL)
{
    m_pProcessModel = new QStandardItemModel();
    CHECK_ALLOCATION(m_pProcessModel);

    m_pModuleModel = new QStandardItemModel();
    CHECK_ALLOCATION(m_pModuleModel);
}

Processes::~Processes()
{
    SAFE_DELETE(m_pModuleModel);
    SAFE_DELETE(m_pProcessModel);
}

int Processes::GetProcessList()
{
    int nStatus = Uninitialized;

    // clear current list
    ClearProcessList();

    LPFN_ISWOW64PROCESS IsWOW64Process = NULL;
    IsWOW64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
                GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    CHECK_ALLOCATION_STATUS(IsWOW64Process);

    LPFN_GETPROCESSMEMORYINFO GetProcessMemInfo = NULL;
    HMODULE hDll = LoadLibrary(TEXT("psapi.dll"));
    CHECK_ALLOCATION_STATUS(hDll);
    GetProcessMemInfo = (LPFN_GETPROCESSMEMORYINFO) GetProcAddress(hDll, "GetProcessMemoryInfo");
    if( NULL == GetProcessMemInfo )
    {
        CloseHandle(hDll);
        hDll = LoadLibrary(TEXT("kernel32.dll"));
        CHECK_ALLOCATION_STATUS(hDll);
        GetProcessMemInfo = (LPFN_GETPROCESSMEMORYINFO) GetProcAddress(hDll, "GetProcessMemoryInfo");
        CHECK_ALLOCATION_STATUS(hDll);
    };

    WCHAR wszProcessFileName[MAX_PATH];
    DWORD dwProcessFileNameSize = MAX_PATH;

    HANDLE hProcess = 0;
    HANDLE hProcessSnap = 0;
    PROCESSENTRY32 peProcessEntry;
    PROCESS_MEMORY_COUNTERS pmProcessMemoryCounters;
    Process *pProcess = 0;
    BOOL bIsTrue = 0;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    CHECK_ALLOCATION_STATUS(hProcessSnap);

    peProcessEntry.dwSize = sizeof(PROCESSENTRY32);

    if( FALSE == Process32First(hProcessSnap, &peProcessEntry) )
    {
        CloseHandle(hProcessSnap);
        CHECK_OPERATION_STATUS_EX(Unsuccessful);
    }

    do
    {
        hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , FALSE, peProcessEntry.th32ProcessID );
        if( 0 == hProcess )
        {
            nStatus = InvalidHandle;
            continue;
        }

        pProcess = new Process;
        CHECK_ALLOCATION_STATUS(pProcess);

        dwProcessFileNameSize = MAX_PATH;
        if( 0 != QueryFullProcessImageName(hProcess, 0, wszProcessFileName, &dwProcessFileNameSize))
            pProcess->qszProcessFileName = QString::fromWCharArray(wszProcessFileName);
        else
            pProcess->qszProcessFileName = "N/A";

        pProcess->qszProcessName = QString::fromWCharArray(peProcessEntry.szExeFile);

        pProcess->qnPID = (qint32) peProcessEntry.th32ProcessID;

        pProcess->qnNumberOfThreads = (qint32) peProcessEntry.cntThreads;

        if( 0 == IsWOW64Process(hProcess, &bIsTrue))
        {
            nStatus = Unsuccessful;
            continue;
        }
        pProcess->bType = bIsTrue;

        pProcess->qnMemoryUsed = 0;

        if( 0 != GetProcessMemInfo(hProcess, &pmProcessMemoryCounters, sizeof(pmProcessMemoryCounters)))
        {
            SIZE_T workingSetSize = pmProcessMemoryCounters.WorkingSetSize / (1024 * 1024);
            SIZE_T pageFileUsage = pmProcessMemoryCounters.PagefileUsage / (1024 * 1024);
            pProcess->qnMemoryUsed = (qint32) workingSetSize;
            pProcess->qnPageFileUsage = (qint32) pageFileUsage;
        }

        pProcess->qszProcessCommandLine = "";
        CloseHandle(hProcess);

        m_qlProcesses.append(pProcess);

    }while( Process32Next(hProcessSnap, &peProcessEntry) );


    CloseHandle(hDll);

    nStatus = Success;

    return nStatus;
}

int Processes::GetModuleList(DWORD dwPid)
{
    HANDLE hModuleSnap = 0;
    MODULEENTRY32 meModuleEntry;
    Module *pModule = 0;

    ClearModuleList();

    hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPid );
    if( INVALID_HANDLE_VALUE == hModuleSnap )
    {
        CHECK_OPERATION_STATUS_EX(InvalidHandle);
    }

    meModuleEntry.dwSize = sizeof(MODULEENTRY32);

    if( 0 == Module32First( hModuleSnap, &meModuleEntry ) )
    {
        CloseHandle(hModuleSnap);
        CHECK_OPERATION_STATUS_EX(Unsuccessful);
    }

    do
    {
        pModule = new Module;
        CHECK_ALLOCATION_STATUS(pModule);

        pModule->qszModuleName = QString::fromWCharArray(meModuleEntry.szModule);
        pModule->qszModuleExePath = QString::fromWCharArray(meModuleEntry.szExePath);
        pModule->qnPID = (qint32) meModuleEntry.th32ProcessID;
        pModule->qnBaseSize = (qint32) meModuleEntry.modBaseSize / (1024);

        m_qlModules.append(pModule);

    }while( Module32Next(hModuleSnap, &meModuleEntry));

    CloseHandle(hModuleSnap);

    return Success;
}

void Processes::ClearProcessList()
{
    foreach(Process *pProcess, m_qlProcesses)
        SAFE_DELETE(pProcess);

    m_qlProcesses.clear();
}

void Processes::ClearModuleList()
{
    foreach(Module *pModule, m_qlModules)
        SAFE_DELETE(pModule);

    m_qlModules.clear();
}


QStandardItemModel *Processes::GetModulesInformationsForProcess()
{
    return m_pModuleModel;
}

QStandardItemModel *Processes::GetProcessesInformations()
{
    return m_pProcessModel;
}

void Processes::OnRefreshProcessList()
{
    int nStatus = GetProcessList();
    CHECK_OPERATION_STATUS(nStatus);

    m_pProcessModel->clear();
    m_pProcessModel->setHorizontalHeaderLabels(QStringList() << "Name" << "File name" << "PID" << "Threads" << "Memory" << "Pagefile");

    QList<QStandardItem*> qList;
    QStandardItem *pItem = 0;

    foreach(Process *pProcess, m_qlProcesses)
    {
        pItem = new QStandardItem(pProcess->qszProcessName == "" ? "N/A" : pProcess->qszProcessName);
        QIcon qIcon = GetIconFromHICON(pProcess->qszProcessFileName);
        pItem->setIcon(qIcon);
        pItem->setData(pProcess->qnPID); qList.append(pItem);

        pItem = new QStandardItem(pProcess->qszProcessFileName == "" ? "N/A" : pProcess->qszProcessFileName);
        pItem->setData(pProcess->qnPID); qList.append(pItem);

//        pItem = new QStandardItem(pProcess->qszProcessCommandLine == "" ? "N/A" : pProcess->qszProcessCommandLine);
//        pItem->setData(pProcess->qnPID); qList.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnPID));
        pItem->setData(pProcess->qnPID); qList.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnNumberOfThreads));
        pItem->setData(pProcess->qnPID); qList.append(pItem);

//        pItem = new QStandardItem(pProcess->bType ? "Yes" : "No");
//        pItem->setData(pProcess->qnPID); qList.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnMemoryUsed) + " MB");
        pItem->setData(pProcess->qnPID); qList.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnPageFileUsage) + " MB");
        pItem->setData(pProcess->qnPID); qList.append(pItem);

        m_pProcessModel->appendRow(qList);
        qList.clear();
    }
}

void Processes::OnRefreshModuleList(int nPid)
{
    int nStatus = GetModuleList(nPid);
    CHECK_OPERATION_STATUS(nStatus);

    m_pModuleModel->clear();
    m_pModuleModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Path" << "Size");

    QList<QStandardItem *> qList;
    QStandardItem *pItem = 0;

    WCHAR wszSystemDir[MAX_PATH] = {0};
    GetSystemDirectory(wszSystemDir, MAX_PATH);

    QString qsDefaultIcon = QString::fromWCharArray(wszSystemDir);
    qsDefaultIcon += "\\wmi.dll";

    foreach(Module *pModule, m_qlModules)
    {
        pItem = new QStandardItem(pModule->qszModuleName == "" ? "N/A" : pModule->qszModuleName);
        if( pModule->qszModuleExePath.endsWith(".exe") )
            pItem->setIcon(GetIconFromHICON(pModule->qszModuleExePath));
        else
            pItem->setIcon(GetIconFromHICON(qsDefaultIcon));
        qList << pItem;

        qList << new QStandardItem(pModule->qszModuleExePath == "" ? "N/A" : pModule->qszModuleExePath);

        qList << new QStandardItem(QString().setNum(pModule->qnBaseSize) + " KB");

        m_pModuleModel->appendRow(qList);
        qList.clear();
    }
}
