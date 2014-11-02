#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
#include <QDebug>

#include <string>

#include "../controller/enums.h"

class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent = 0);
    virtual ~AbstractController() {}

signals:
    /*** View Adapter Signals ***/
    void OnCreateMainWindowSignal();
    void OnShowMainWindowSignal();
    void OnPopulateMenuTreeSignal(QStandardItemModel*);

    /*** Device Manager Signals ***/
    void OnSetDeviceManagerInformation(QStandardItemModel*);
    void OnDeviceManagerInformationDataChanged();
    void OnSetDevicePropertiesInformation(QStandardItemModel*);
    void OnDevicePropertiesInformationChanged();

    /*** DMI Signals ***/
    void OnSetDMIItemsInformation(QStandardItemModel*);
    void OnDMIItemsInformationDataChanged();
    void OnSetDMIPropertiesInfomation(QStandardItemModel*);
    void OnDMIPropertiesInformationDataChanged();

    /*** Battery Manager Signals ***/
    void OnSetPowerManagementInformation(QStandardItemModel *);
    void OnPowerManagementInformationDataChanged();

    /*** CpuId Manager Signals ***/
    void OnSetCPUIDInformations(QStandardItemModel*);
    void OnCPUIDInformationDataChanged();

    /*** SPD Manager Signals ***/
    void OnSetAvailableDIMMSInformation(QStandardItemModel*);
    void OnAvailableDIMMSInformationDataChanged();
    void OnSetDimmSPDInformation(QStandardItemModel*);
    void OnSPDDimmInformationDataChanged();

    /*** Video card Manager Signals ***/
    void OnSetAvailableVCardsInformation(QStandardItemModel*);
    void OnAvailableVCardsInformationDataChanged();
    void OnSetVCardInfromation(QStandardItemModel*);
    void OnVideoCardInformationDataChanged();

    /*** Operating system Manager Signals ***/
    void OnSetOperatingSystemInformation(QStandardItemModel*);
    void OnOperatingSystemInformationDataChanged();

    /*** System drivers Manager Signals ***/
    void OnSetSystemDriversModelInformation(QStandardItemModel *);
    void OnSystemDriversInformationDataChanged();

    /*** Process Manager Signals ***/
    void OnSetProcessesInformations(QStandardItemModel*);
    void OnSetModulesInformations(QStandardItemModel*);
    void OnProcessInformationDataChanged();
    void OnProcessModuleInformationDataChanged();

    /*** System users ***/
    void OnSetUsersInformations(QStandardItemModel*);
    void OnUserInformationDataChanged();

    /*** Storage ATA ***/
    void OnSetATAHDDItemsInformation(QStandardItemModel*);
    void OnATAHDDInformationDataChanged();
    void OnSetATAItemPropertiesInformation(QStandardItemModel*);
    void OnATAItemPropertiesDataChanged();

    /*** Storage SMART ***/
    void OnSetSMARTHDDItemsInformation(QStandardItemModel*);
    void OnSMARTHDDInformationDataChanged();
    void OnSetSMARTItemPropertiesInformation(QStandardItemModel*);
    void OnSMARTItemPropertiesDataChanged();

    /*** Network Active connections Signals ***/
    void OnSetActiveConnectionsInformation(QStandardItemModel *);
    void OnActiveConnectionsInformationDataChanged();

    /*** Network Devices Signals ***/
    void OnSetNetworkDevicesNames(QStandardItemModel *);
    void OnNetworkAdaptersInformationDataChanged();
    void OnSetNetworkDeviceInformation(QStandardItemModel *);
    void OnNetworkAdapterPropertiesDataChanged();

    /*** Startup manager Signals ***/
    void OnSetStartupApplicationsInformations(QStandardItemModel *);
    void OnStartupApplicationInformationDataChanged();

    void OnSetApplicationManagerInformation(QStandardItemModel *);




    void OnSetSensorsInformations(std::string);




public slots:
    virtual void OnDispatchMenuOptionTagSlot(int) = 0;

    virtual void OnComputerDeviceManagerOptClickedSlot() = 0;
    virtual void OnComputerDMIOptClickedSlot() = 0;
    virtual void OnComputerPowerManagementOptClickedSlot() = 0;
    virtual void OnHddInformationOptClickedSlot() = 0;
    virtual void OnOperatingSystemOptClickedSlot() = 0;
    virtual void OnOperatingSystemProcessesOptClickedSlot() = 0;
    virtual void OnOperatingSystemDriversOptClickedSlot() = 0;
    virtual void OnStorageATAOptClickedSlot() = 0;
    virtual void OnStorageSmartOptClickedSlot() = 0;
    virtual void OnSmbiosOptClickedSlot() = 0;
    virtual void OnSoftwareApplicationManagerOptClickedSlot() = 0;
    virtual void OnSoftwareStartupApplicationsOptClickedSlot() = 0;
    virtual void OnNetworkConnectionsOptClickedSlot() = 0;
    virtual void OnNetworkDevicesOptClickedSlot() = 0;
    virtual void OnCPUOptClickedSlot() = 0;
    virtual void OnMotherBoardCPUOptClickedSlot() = 0;
    virtual void OnComputerSensorsOptClickedSlot() = 0;
    virtual void OnOperatingSystemUserInformationsOptClickedSlot() = 0;
    virtual void OnMotherBoardSPDOptClickedSlot() = 0;
    virtual void OnMotherboardVCardOptClickedSlot() = 0;

    // Device manager slots
    virtual void OnRequestDeviceDetailsSlot(QString) = 0;

    // DMI slots
    virtual void OnRequestDMIItemProperties(DMIModuleType) = 0;

    // SPD slots
    virtual void OnRequestSPDDimmDetailsSlot(int) = 0;

    // Video card slots
    virtual void OnRequestVCardInformationSlot(int) = 0;

    // ATA slots
    virtual void OnRequestATAItemProperties(QString) = 0;

    // SMART slots
    virtual void OnRequestSMARTProperties(QString) = 0;

    // Application manager slots
    virtual void OnUninstallApplicationSlot(QString) = 0;

    // Network devices manager slots
    virtual void OnRequestNetworkDeviceInfomationsSlot(QString) = 0;

    // Processes manager slot
    virtual void OnRequestModulesInformationsSlot(int) = 0;

    // Startup Applications manager slot
    virtual void OnRemoveStartupApplicationSlot(QString) = 0;
    virtual void OnChangeStartupApplicationStateSlot(QString) = 0;
    virtual void OnAddStartupApplicationSlot(QString, QString) = 0;
};

#endif // ABSTRACT_CONTROLLER_H
