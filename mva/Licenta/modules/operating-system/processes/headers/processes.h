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

class Processes
{
private:
    QList<Process*> m_qlProcesses;
    QList<Module*> m_qlModules;

private:
    int GetProcessList();
    int GetModuleList(DWORD dwPid);

    void ClearProcessList();
    void ClearModuleList();

public:
    Processes();
    ~Processes();
    QStandardItemModel *GetModulesInformationsForProcess(DWORD dwPid);
    QStandardItemModel *GetProcessesInformations();
};

#endif
