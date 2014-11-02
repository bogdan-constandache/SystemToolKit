#ifndef PROCESSES_H
#define PROCESSES_H

//WinAPI includes
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

//QT includes
#include <QFile>
#include <QStandardItemModel>

//My includes
#include "../../../../utils/headers/utils.h"
#include "processes_structs.h"

//Typedefs
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
typedef BOOL (WINAPI *LPFN_GETPROCESSMEMORYINFO) (HANDLE, PROCESS_MEMORY_COUNTERS*, DWORD);

class Processes
{
private:
    QList<Process*> m_qlProcesses;
    QList<Module*> m_qlModules;

    QStandardItemModel *m_pProcessModel;
    QStandardItemModel *m_pModuleModel;

private:
    int GetProcessList();
    int GetModuleList(DWORD dwPid);

    void ClearProcessList();
    void ClearModuleList();

public:
    Processes();
    ~Processes();
    QStandardItemModel *GetModulesInformationsForProcess();
    QStandardItemModel *GetProcessesInformations();

    void OnRefreshProcessList();
    void OnRefreshModuleList(int nPid);
};

#endif
