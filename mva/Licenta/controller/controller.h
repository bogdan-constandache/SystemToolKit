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
    CCPUIDManager               *m_pCPUIDManager;
    Processes                   *m_pProcessesManager;
    CStartupManager             *m_pStartupAppsManager;
    CSystemUsersInformation     *m_pUserInformationManager;
    CSPDInformation             *m_pSPDManager;
    CNvidiaManager              *m_pNVidiaManager;
    COperatingSystemInformation *m_pOperatingSystemManager;

    CSensorModule               *m_pSensorsManager;
    ISensor                     *m_pSensor;
    ICPUSensor                  *m_pCpuSensor;
    QTimer                      *m_pSensorsTimer;
    QTimer                      *m_pGPUTimer;

    QMap<QString, QString>      m_HDDModelToPhysicalDrive;

    QStandardItemModel          *m_pComputerSummaryModel;

    // Internal Models
    QStandardItemModel          *m_pATAHdds;
    QStandardItemModel          *m_pATAProp;

    int OnLoadDriverFile();
    int OnUnloadDriverFile();

    int AssignStandardModelsToUi();

public:
    Controller();
    virtual ~Controller();

public slots:
    void StartController();

    virtual void OnDispatchMenuOptionTagSlot(int);

    virtual void OnComputerDeviceManagerOptClickedSlot();
    virtual void OnComputerDMIOptClickedSlot();
    virtual void OnComputerPowerManagementOptClickedSlot();
    virtual void OnComputerSensorsOptClickedSlot();

    virtual void OnMotherBoardCPUOptClickedSlot();
    virtual void OnMotherBoardSPDOptClickedSlot();
    virtual void OnMotherboardVCardOptClickedSlot();

    virtual void OnOperatingSystemOptClickedSlot();
    virtual void OnOperatingSystemProcessesOptClickedSlot();
    virtual void OnOperatingSystemDriversOptClickedSlot();
    virtual void OnOperatingSystemUserInformationsOptClickedSlot();

    virtual void OnStorageATAOptClickedSlot();
    virtual void OnStorageSmartOptClickedSlot();

    virtual void OnNetworkConnectionsOptClickedSlot();
    virtual void OnNetworkDevicesOptClickedSlot();

    virtual void OnSoftwareStartupApplicationsOptClickedSlot();

    virtual void OnHddInformationOptClickedSlot();
    virtual void OnSmbiosOptClickedSlot();
    virtual void OnSoftwareApplicationManagerOptClickedSlot();
    virtual void OnCPUOptClickedSlot();

    // Device manager slots()
    virtual void OnRequestDeviceDetailsSlot(QString);

    // DMI slots()
    virtual void OnRequestDMIItemProperties(DMIModuleType);

    // SPD slots()
    virtual void OnRequestSPDDimmDetailsSlot(int);

    // Video card slots()
    virtual void OnRequestVCardInformationSlot(int);

    // ATA slots()
    virtual void OnRequestATAItemProperties(QString);

    // SMART slots()
    virtual void OnRequestSMARTProperties(QString);

    // Application manager slots()
    virtual void OnUninstallApplicationSlot(QString);

    // Network devices manager slots()
    virtual void OnRequestNetworkDeviceInfomationsSlot(QString);

    // Processes manager slots()
    virtual void OnRequestModulesInformationsSlot(int);

    // Startup application manager slots()
    virtual void OnRemoveStartupApplicationSlot(QString);
    virtual void OnChangeStartupApplicationStateSlot(QString);
    virtual void OnAddStartupApplicationSlot(QString, QString);

signals:
    void OnCancelSensorsTimerSignal();

private slots:
    void OnCancelSensorsTimerSlot();
    void OnRefreshVCardInformations();
    void OnUninstallApplicationErrorReportSlot(QString);
};

#endif // CONTROLLER_H
