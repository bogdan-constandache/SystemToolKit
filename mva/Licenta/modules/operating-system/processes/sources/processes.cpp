#include "../headers/processes.h"

Processes::Processes()
{
}

Processes::~Processes()
{
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
        nStatus = Unsuccessful;
        CloseHandle(hProcessSnap);
        return nStatus;
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
            pProcess->qszProcessFileName = WcharArrayToQString(wszProcessFileName);
        else
            pProcess->qszProcessFileName = "N/A";

        pProcess->qszProcessName = WcharArrayToQString(peProcessEntry.szExeFile);
        pProcess->qnPID = (qint32) peProcessEntry.th32ProcessID;
        pProcess->qnNumberOfThreads = (qint32) peProcessEntry.cntThreads;

        if( 0 == IsWOW64Process(hProcess, &bIsTrue))
        {
            nStatus = Unsuccessful;
            continue;
        }
        pProcess->bType = bIsTrue;

        pProcess->qnMemoryUsed = 0;

        if( 0 != GetProcessMemoryInfo(hProcess, &pmProcessMemoryCounters, sizeof(pmProcessMemoryCounters)))
        {
            SIZE_T workingSetSize = pmProcessMemoryCounters.WorkingSetSize / 1024;
            SIZE_T pageFileUsage = pmProcessMemoryCounters.PagefileUsage / 1024;
            pProcess->qnMemoryUsed = (qint32) workingSetSize;
            pProcess->qnPageFileUsage = (qint32) pageFileUsage;
        }

        pProcess->qszProcessCommandLine = "";
        CloseHandle(hProcess);

        m_qlProcesses.append(pProcess);
    }while( Process32Next(hProcessSnap, &peProcessEntry) );

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
        return InvalidHandle;
    }

    meModuleEntry.dwSize = sizeof(MODULEENTRY32);

    if( 0 == Module32First( hModuleSnap, &meModuleEntry ) )
    {
        return Unsuccessful;
    }

    do
    {
        pModule = new Module;
        CHECK_ALLOCATION_STATUS(pModule);

        pModule->qszModuleName = WcharArrayToQString(meModuleEntry.szModule);
        pModule->qszModuleExePath = WcharArrayToQString(meModuleEntry.szExePath);
        pModule->qnPID = (qint32) meModuleEntry.th32ProcessID;
        pModule->qnBaseSize = (qint32) meModuleEntry.modBaseSize;

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


QStandardItemModel *Processes::GetModulesInformationsForProcess(DWORD dwPid)
{
    QStandardItemModel *pModel = new QStandardItemModel;
    Module *pModule;

    QStandardItem *pRoot = pModel->invisibleRootItem();
    QStandardItem *pItem = 0;

    QList<QStandardItem*> qRow;

    GetModuleList(dwPid);

    for(int i = 0; i < m_qlModules.count(); i++)
    {
        pModule = m_qlModules.at(i);

//        pItem = new QStandardItem(QString().setNum(pModule->qnPID));
//        qRow.append(pItem);

        pItem = new QStandardItem(pModule->qszModuleName == "" ? "N/A" : pModule->qszModuleName);
        qRow.append(pItem);

        pItem = new QStandardItem(pModule->qszModuleExePath == "" ? "N/A" : pModule->qszModuleExePath);
        qRow.append(pItem);

        pItem = new QStandardItem(QString().setNum(pModule->qnBaseSize));
        qRow.append(pItem);

        pRoot->appendRow(qRow);
        qRow.clear();
    }

    return pModel;
}

QStandardItemModel *Processes::GetProcessesInformations()
{
    QStandardItemModel *pModel = new QStandardItemModel;
    Process *pProcess;

    QStandardItem *pRoot = pModel->invisibleRootItem();
    QStandardItem *pItem = 0;

    QList<QStandardItem*> qRow;

    GetProcessList();

    for(int i = 0; i < m_qlProcesses.count(); i++)
    {
        pProcess = m_qlProcesses.at(i);
        pItem = new QStandardItem(pProcess->qszProcessName == "" ? "N/A" : pProcess->qszProcessName);
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(pProcess->qszProcessFileName == "" ? "N/A" : pProcess->qszProcessFileName);
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(pProcess->qszProcessCommandLine == "" ? "N/A" : pProcess->qszProcessCommandLine);
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnPID));
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnNumberOfThreads));
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(pProcess->bType ? "Yes" : "No");
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnMemoryUsed));
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnPageFileUsage));
        pItem->setStatusTip(QString().setNum(pProcess->qnPID));
        qRow.append(pItem);

        pRoot->appendRow(qRow);
        qRow.clear();
    }

    return pModel;
}
