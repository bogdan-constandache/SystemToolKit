#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include <QObject>
#include <QStandardItemModel>

#include <string>

#include "../controller/enums.h"

class AbstractController : public QObject
{
    Q_OBJECT
public:
    explicit AbstractController(QObject *parent = 0);
    virtual ~AbstractController() {}

signals:
    void OnCreateMainWindowSignal();
    void OnShowMainWindowSignal();
    void OnPopulateMenuTreeSignal(QStandardItemModel*);

    void OnSetDeviceManagerInformation(QStandardItemModel*);
    void OnSetDMIItemsInformation(QStandardItemModel*);
    void OnSetDMIPropertiesInfomation(QStandardItemModel*);
    void OnSetATAHDDItemsInformation(QStandardItemModel*);
    void OnSetATAItemPropertiesInformation(QStandardItemModel*);
    void OnSetSMARTHDDItemsInformation(QStandardItemModel*);
    void OnSetSMARTItemPropertiesInformation(QStandardItemModel*);
    void OnSetPowerManagementInformation(QStandardItemModel *);
    void OnSetApplicationManagerInformation(QStandardItemModel *);

    void OnSetSystemDriversModelInformation(QStandardItemModel *);

    void OnSetActiveConnectionsInformation(QStandardItemModel *);

    void OnSetNetworkDevicesNames(QStandardItemModel *);
    void OnSetNetworkDeviceInformation(QStandardItemModel *);

    void OnSetCPUIDInformations(QStandardItemModel*);

    void OnSetSensorsInformations(std::string);

    void OnSetProcessesInformations(QStandardItemModel*);
    void OnSetModulesInformations(QStandardItemModel*);

    void OnSetStartupApplicationsInformations(QStandardItemModel *);

public slots:
    virtual void OnComputerDeviceManagerOptClickedSlot() = 0;
    virtual void OnComputerDMIOptClickedSlot() = 0;
    virtual void OnComputerPowerManagementOptClickedSlot() = 0;
    virtual void OnHddInformationOptClickedSlot() = 0;
    virtual void OnOperatingSystemOptClickedSlot() = 0;
    virtual void OnProcessesOptClickedSlot() = 0;
    virtual void OnSystemDriversOptClickedSlot() = 0;
    virtual void OnStorageATAOptClickedSlot() = 0;
    virtual void OnStorageSmartOptClickedSlot() = 0;
    virtual void OnSmbiosOptClickedSlot() = 0;
    virtual void OnApplicationManagerOptClickedSlot() = 0;
    virtual void OnStartupApplicationsOptClickedSlot() = 0;
    virtual void OnActiveConnectionsOptClickedSlot() = 0;
    virtual void OnNetworkDevicesOptClickedSlot() = 0;
    virtual void OnCPUOptClickedSlot() = 0;
    virtual void OnCPUIDOptClickedSlot() = 0;
    virtual void OnSensorsOptClickedSlot() = 0;

    // DMI slots
    virtual void OnRequestDMIItemProperties(DMIModuleType) = 0;

    // ATA slots
    virtual void OnRequestATAItemProperties(QString) = 0;

    // SMART slots
    virtual void OnRequestSMARTProperties(QString) = 0;

    // Power management slots

    // Application manager slots
    virtual void OnUninstallApplicationSlot() = 0;

    // Network devices manager slots
    virtual void OnRequestNetworkDeviceInfomationsSlot(QString) = 0;

    // Processes manager slot
    virtual void OnRequestModulesInformationsSlot(int) = 0;
};

#endif // ABSTRACT_CONTROLLER_H
