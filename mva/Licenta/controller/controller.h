#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../gui/abstract_controller.h"
#include "../modules/computer/power-management/headers/battery_status.h"
#include "../modules/software/applications-manager/headers/application_manager.h"
#include "../modules/computer/dmi/headers/smbios_entry_point.h"

#include <QDebug>

class Controller : public AbstractController
{
    Q_OBJECT
private: // internal objects
    BatteryStatus *m_pBatteryStatus;
    CApplicationManager *m_pApplicationManager;
    CSMBiosEntryPoint *m_pDMIManager;

public:
    Controller();
    virtual ~Controller();

public slots:
    void StartController();
    virtual void OnComputerDMIOptClickedSlot();
    virtual void OnComputerPowerManagementOptClickedSlot();
    virtual void OnHddInformationOptClickedSlot();
    virtual void OnOperatingSystemOptClickedSlot();
    virtual void OnProcessesOptClickedSlot();
    virtual void OnSystemDriversOptClickedSlot();
    virtual void OnStorageSmartOptClickedSlot();
    virtual void OnSmbiosOptClickedSlot();
    virtual void OnApplicationManagerOptClickedSlot();
    virtual void OnStartupApplicationsOptClickedSlot();

    // DMI slots()
    virtual void OnRequestDMIItemProperties(DMIModuleType);

    // Power management slots()

    // Application manager slots()
    virtual void OnUninstallApplicationSlot();
};

#endif // CONTROLLER_H
