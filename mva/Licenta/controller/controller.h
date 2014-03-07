#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../gui/abstract_controller.h"
#include "../modules/computer/power-management/headers/battery_status.h"
#include "../modules/software/applications-manager/headers/application_manager.h"
#include "../modules/computer/dmi/headers/smbios_entry_point.h"
#include "../modules/storage/smart/headers/csmartinfo.h"
#include "../modules/operating-system/system-drivers/headers/system_drivers.h"
#include "../modules/network/active-connections/active_connections.h"
#include "../modules/network/network-devices/headers/network_devices.h"

#include <QDebug>

class Controller : public AbstractController
{
    Q_OBJECT
private: // internal objects
    BatteryStatus *m_pBatteryStatus;
    CApplicationManager *m_pApplicationManager;
    CSMBiosEntryPoint *m_pDMIManager;
    CSmartInfo *m_pSmartManager;
    SystemDrivers *m_pSystemDriversManager;
    CNetworkDevices *m_pNetworkDevicesManager;
    CActiveConnections *m_pActiveConnectionsManager;
    QMap<QString, QString> m_HDDModelToPhysicalDrive;

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
    virtual void OnStorageATAOptClickedSlot();
    virtual void OnStorageSmartOptClickedSlot();
    virtual void OnSmbiosOptClickedSlot();
    virtual void OnApplicationManagerOptClickedSlot();
    virtual void OnStartupApplicationsOptClickedSlot();
    virtual void OnActiveConnectionsOptClickedSlot();
    virtual void OnNetworkDevicesOptClickedSlot();

    // DMI slots()
    virtual void OnRequestDMIItemProperties(DMIModuleType);

    // ATA slots()
    virtual void OnRequestATAItemProperties(QString);

    // SMART slots()
    virtual void OnRequestSMARTProperties(QString);

    // Power management slots()

    // Application manager slots()
    virtual void OnUninstallApplicationSlot();

    // Network devices manager slots()
    virtual void OnRequestNetworkDeviceInfomationsSlot(QString);
};

#endif // CONTROLLER_H
