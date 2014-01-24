#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include <QObject>
#include <QStandardItemModel>
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

    void OnSetDMIItemsInformation(QStandardItemModel*);
    void OnSetDMIPropertiesInfomation(QStandardItemModel*);
    void OnSetSMARTHDDItemsInformation(QStandardItemModel*);
    void OnSetSMARTItemPropertiesInformation(QStandardItemModel*);
    void OnSetPowerManagementInformation(QStandardItemModel *);
    void OnSetApplicationManagerInformation(QStandardItemModel *);

public slots:
    virtual void OnComputerDMIOptClickedSlot() = 0;
    virtual void OnComputerPowerManagementOptClickedSlot() = 0;
    virtual void OnHddInformationOptClickedSlot() = 0;
    virtual void OnOperatingSystemOptClickedSlot() = 0;
    virtual void OnProcessesOptClickedSlot() = 0;
    virtual void OnSystemDriversOptClickedSlot() = 0;
    virtual void OnStorageSmartOptClickedSlot() = 0;
    virtual void OnSmbiosOptClickedSlot() = 0;
    virtual void OnApplicationManagerOptClickedSlot() = 0;
    virtual void OnStartupApplicationsOptClickedSlot() = 0;

    // DMI slots
    virtual void OnRequestDMIItemProperties(DMIModuleType) = 0;

    // SMART slots
    virtual void OnRequestSMARTProperties(QString) = 0;

    // Power management slots

    // Application manager slots
    virtual void OnUninstallApplicationSlot() = 0;
};

#endif // ABSTRACT_CONTROLLER_H
