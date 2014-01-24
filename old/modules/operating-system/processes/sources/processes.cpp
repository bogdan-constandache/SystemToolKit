#include "../headers/processes.h"

Processes::Processes():
    m_pTableModel(NULL),
    m_pTableView(NULL)
{
    m_pTableModel = new QStandardItemModel();
}

Processes::~Processes()
{
}

int Processes::Initialize()
{
    int nStatus = Uninitialized;

    m_qlProcesses.clear();

    nStatus = GetProcessList();
    return nStatus;
}

int Processes::GetProcessList()
{
    int nStatus = Uninitialized;
    LPFN_ISWOW64PROCESS IsWOW64Process = NULL;
    IsWOW64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
                GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    WCHAR wszProcessFileName[MAX_PATH];
    DWORD dwProcessFileNameSize = MAX_PATH;

    HANDLE hProcess = 0;
    HANDLE hProcessSnap = 0;
    PROCESSENTRY32 peProcessEntry;
    PROCESS_MEMORY_COUNTERS pmProcessMemoryCounters;
    Process *pProcess = 0;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if( 0 == hProcessSnap )
    {
        nStatus = InvalidHandle;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    peProcessEntry.dwSize = sizeof(PROCESSENTRY32);

    if( FALSE == Process32First(hProcessSnap, &peProcessEntry) )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        CloseHandle(hProcessSnap);
        return nStatus;
    }

    do
    {
        hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , FALSE, peProcessEntry.th32ProcessID );
        if( 0 == hProcess )
        {
            nStatus = GetLastError();
            nStatus = InvalidHandle;
            DEBUG_STATUS(nStatus);
            continue;
        }

        pProcess = new Process;
        if( 0 == pProcess )
        {
            nStatus = NotAllocated;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }

        dwProcessFileNameSize = MAX_PATH;
        if( 0 != QueryFullProcessImageName(hProcess, 0, wszProcessFileName, &dwProcessFileNameSize))
        {
            pProcess->qszProcessFileName = WcharArrayToQString(wszProcessFileName);
        }
        else
        {
            pProcess->qszProcessFileName = "N/A";
        }
        pProcess->qszProcessName = WcharArrayToQString(peProcessEntry.szExeFile);
        pProcess->qnPID = (qint32) peProcessEntry.th32ProcessID;
        pProcess->qnNumberOfThreads = (qint32) peProcessEntry.cntThreads;
        if( 0 != IsWOW64Process )
        {
            BOOL isTrue;
            if( 0 == IsWOW64Process(hProcess, &isTrue))
            {
                nStatus = Unsuccessful;
                DEBUG_STATUS(nStatus);
                return nStatus;
            }
            pProcess->bType = isTrue;
        }
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
        nStatus = GetModuleList(&pProcess);
        if( Success != nStatus )
        {
            DEBUG_STATUS(nStatus);
        }
        m_qlProcesses.append(pProcess);
    }while( Process32Next(hProcessSnap, &peProcessEntry) );

    nStatus = Success;
    return nStatus;
}

int Processes::GetModuleList(Process **ppParam)
{
    int nStatus = Uninitialized;

    HANDLE hModuleSnap = 0;
    MODULEENTRY32 meModuleEntry;
    Module *pModule = 0;

    hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, (DWORD)(*ppParam)->qnPID );
    if( INVALID_HANDLE_VALUE == hModuleSnap )
    {
        nStatus = GetLastError();
        nStatus = InvalidHandle;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    meModuleEntry.dwSize = sizeof(MODULEENTRY32);

    if( 0 == Module32First( hModuleSnap, &meModuleEntry ) )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    Module32Next(hModuleSnap, &meModuleEntry);

    do
    {
        pModule = new Module;
        if( 0 == pModule )
        {
            nStatus = NotAllocated;
            DEBUG_STATUS(nStatus);
            return nStatus;
        }

        pModule->qszModuleName = WcharArrayToQString(meModuleEntry.szModule);
        pModule->qszModuleExePath = WcharArrayToQString(meModuleEntry.szExePath);
        pModule->qnPID = (qint32) meModuleEntry.th32ProcessID;
        pModule->qnBaseSize = (qint32) meModuleEntry.modBaseSize;

        (*ppParam)->qlModules.append(pModule);

    }while( Module32Next(hModuleSnap, &meModuleEntry));
    CloseHandle(hModuleSnap);

    nStatus = Success;
    return nStatus;
}

void Processes::PopulateModel()
{
    QStandardItem *pItem = 0;
    Process *pProcess = 0;
    m_pTableModel->clear();

    m_pTableModel->setColumnCount(8);
    m_pTableModel->setRowCount(this->m_qlProcesses.count());

    for(int i = 0; i < m_qlProcesses.count(); i++)
    {
        pProcess = m_qlProcesses.at(i);
        pItem = new QStandardItem(pProcess->qszProcessName == "" ? "N/A" : pProcess->qszProcessName);
        m_pTableModel->setItem(i, 0, pItem);

        pItem = new QStandardItem(pProcess->qszProcessFileName == "" ? "N/A" : pProcess->qszProcessFileName);
        m_pTableModel->setItem(i, 1, pItem);

        pItem = new QStandardItem(pProcess->qszProcessCommandLine == "" ? "N/A" : pProcess->qszProcessCommandLine);
        m_pTableModel->setItem(i, 2, pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnPID));
        m_pTableModel->setItem(i, 3, pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnNumberOfThreads));
        m_pTableModel->setItem(i, 4, pItem);

        pItem = new QStandardItem(pProcess->bType ? "Yes" : "No");
        m_pTableModel->setItem(i, 5, pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnMemoryUsed));
        m_pTableModel->setItem(i, 6, pItem);

        pItem = new QStandardItem(QString().setNum(pProcess->qnPageFileUsage));
        m_pTableModel->setItem(i, 7, pItem);
    }

    emit OnSendModuleDataToGUISignal(OS_PROCESSES, m_pTableView, m_pTableModel);
}

QList<Process *> Processes::GetProcesses()
{
    return m_qlProcesses;
}

int Processes::OnCreateWidget(QWidget **ppWidget)
{
    QWidget *pWidget = new QWidget();
    pWidget->setLayout(new QVBoxLayout());
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);

    m_pTableView = new QTableView();
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_pTableView->verticalHeader()->hide();
    m_pTableView->setShowGrid(false);
    m_pTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    pWidget->layout()->addWidget(m_pTableView);

    this->PopulateModel();

    (*ppWidget) = pWidget;

    return Success;
}

void Processes::OnStartLoadingModuleDataSlot()
{
    this->Initialize();

    emit OnLoadingModuleDataCompleteSignal();
    emit OnCreateWidgetSignal(OS_PROCESSES, this);
}
