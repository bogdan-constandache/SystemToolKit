#ifndef PROCESSES_H
#define PROCESSES_H

//WinAPI includes
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

//QT includes
#include <QFile>

//My includes
#include "../../../../interfaces/module.h"
#include "../../../../utils/headers/utils.h"
#include "processes_structs.h"

//Typedefs
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

class Processes : public IModule
{
public:
    QList<Process*> m_qlProcesses;

    QTableView *m_pTableView;

    QStandardItemModel *m_pTableModel;

public:
    Processes();
    ~Processes();
    int Initialize();
    QList<Process*> GetProcesses();
    virtual int OnCreateWidget(QWidget **ppWidget);

public slots:
    virtual void OnStartLoadingModuleDataSlot();

private:
    int Destroy();
    int GetProcessList();
    int GetModuleList(Process **ppParam);
    void PopulateModel();
};

#endif
