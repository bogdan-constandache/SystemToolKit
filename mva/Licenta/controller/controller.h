#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <comdef.h>
#include <lmcons.h>

#include "../gui/abstract_controller.h"
#include "../modules/api.h"

#include "../proto-buffers/sensors_data.pb.h"

#include <QDebug>
#include <QTimer>

class Controller : public AbstractController
{
    Q_OBJECT
private: // internal objects
    CDeviceInfo                 *m_pDeviceManager;
    BatteryStatus               *m_pBatteryStatus;
    CApplicationManager         *m_pApplicationManager;
    QProcessWrapper             *m_pUninstallerProcess;
    CSMBiosEntryPoint           *m_pDMIManager;
    CSmartInfo                  *m_pSmartManager;
    SystemDrivers               *m_pSystemDriversManager;
    CNetworkDevices             *m_pNetworkDevicesManager;
    CActiveConnections          *m_pActiveConnectionsManager;
    CIntelCpuID                 *m_pCPUIDManager;
    Processes                   *m_pProcessesManager;
    CStartupApplication         *m_pStartupAppsManager;
    CSystemUsersInformation     *m_pUserInformationManager;
//    CNvidiaManager              *m_pNVidiaManager;

    CSensorModule *m_pSensorsManager;
    ISensor *m_pSensor;
    ICPUSensor *m_pCpuSensor;
    QTimer *m_pSensorsTimer;

    QMap<QString, QString> m_HDDModelToPhysicalDrive;

    QStandardItemModel *m_pComputerSummaryModel;

    void OnCreateComputerSummary();
    int OnLoadDriverFile();
    int OnUnloadDriverFile();

public:
    Controller();
    virtual ~Controller();

public slots:
    void StartController();
    virtual void OnComputerDeviceManagerOptClickedSlot();
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
    virtual void OnCPUOptClickedSlot();
    virtual void OnCPUIDOptClickedSlot();
    virtual void OnSensorsOptClickedSlot();
    virtual void OnUserInformationsOptClickedSlot();

    // Device manager slots()
    virtual void OnRequestDeviceDetailsSlot(QString);

    // DMI slots()
    virtual void OnRequestDMIItemProperties(DMIModuleType);

    // ATA slots()
    virtual void OnRequestATAItemProperties(QString);

    // SMART slots()
    virtual void OnRequestSMARTProperties(QString);

    // Power management slots()

    // Application manager slots()
    virtual void OnUninstallApplicationSlot(QString);

    // Network devices manager slots()
    virtual void OnRequestNetworkDeviceInfomationsSlot(QString);

    // Processes manager slots()
    virtual void OnRequestModulesInformationsSlot(int);

signals:
    void OnCancelSensorsTimerSignal();

private slots:
    void OnCancelSensorsTimerSlot();
    void OnUninstallApplicationErrorReportSlot(QString);
};

#endif // CONTROLLER_H
