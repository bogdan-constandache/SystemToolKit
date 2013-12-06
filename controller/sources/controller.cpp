#include "../headers/controller.h"

CController::CController()
{
}

CController::~CController()
{
}

void CController::SetView(IMainWindow *pView)
{
    this->m_pMainWindow = pView;
}

IModule *CController::CreateModule(ModuleType mType)
{
    switch(mType)
    {
//    case HARDWARE_INTEL_CPUID:
//        return new CIntelCpuID();
//    case HDD_HDD_INFORMATION:
//        return 0;
//    case OS_OPERATING_SYSTEM:
//        return new OperatingSystem();
    case OS_SYSTEM_DRIVERS:
        return new SystemDrivers();
    case OS_PROCESSES:
        return new Processes();
//    case SMART_SMART_INFORMATION:
//        return new CSmartInfo();
    case COMPUTER_DMI:
        return new CSMBiosEntryPoint();
    case COMPUTER_POWER_MANAGEMENT:
        return new BatteryStatus();
//    case SOFTWARE_APPLICATION_MANAGER:
//        return new CApplicationManager();
//    case SOFTWARE_STARTUP_APPLICATIONS:
//        return new CStartupApplication();
    case STORAGE_SMART:
        return new CSmartInfo();
    default:
        return 0;
    };
    return 0;
}

void CController::StartControllerSlot(void)
{
    emit StartDrawingMainWindowSignal();
}

void CController::DrawingMainWindowCompleteSlot()
{

}

void CController::CreateModuleSlot(int mType)
{
    IModule *pModule = CreateModule((ModuleType)mType);
    QThread *pModuleThread;

    if( m_qThreadMap.contains((ModuleType)mType) )
    {
        pModuleThread = m_qThreadMap.value((ModuleType)mType);
    }
    else
    {
        pModuleThread = new QThread();
        m_qThreadMap.insert((ModuleType)mType, pModuleThread);
    }

    if (0 == pModule)
    {
        return;
    }

    pModule->moveToThread(pModuleThread);
    QObject::connect(pModuleThread, SIGNAL(started()),
                     pModule, SLOT(OnStartLoadingModuleDataSlot()),
                     Qt::QueuedConnection);
    QObject::connect(pModule, SIGNAL(OnCreateWidgetSignal(ModuleType,IModule *)),
                     m_pMainWindow, SLOT(CreateWidgetSlot(ModuleType,IModule *)),
                     Qt::QueuedConnection);

    QObject::connect(pModule, SIGNAL(OnSendModuleDataToGUISignal(ModuleType,QWidget*,QStandardItemModel*)),
                     m_pMainWindow, SLOT(SendDataToGUISlot(ModuleType,QWidget*,QStandardItemModel*)),
                     Qt::QueuedConnection);

    pModuleThread->start();
}

void CController::OnMainWindowDialogClosedSlot()
{
    for(int i = 0; i < m_qThreadMap.values().count();i++)
    {
        QThread *pThread = m_qThreadMap.values().at(i);

        pThread->quit();
        pThread->wait();
    }
}

