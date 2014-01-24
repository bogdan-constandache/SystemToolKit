#ifndef MODULE_H
#define MODULE_H

// WinAPI includes

// Qt includes
#include <QWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>

// My includes

typedef enum _MODULE_TYPE_
{
    COMPUTER_DMI,
    COMPUTER_POWER_MANAGEMENT,
    HDD_HDD_INFORMATION,
    OS_OPERATING_SYSTEM,
    OS_PROCESSES,
    OS_SYSTEM_DRIVERS,
    STORAGE_SMART,
    SMBIOS_ENTRY_POINT,
    SOFTWARE_APPLICATION_MANAGER,
    SOFTWARE_STARTUP_APPLICATIONS
}ModuleType;

class IModule : public QObject
{
    Q_OBJECT
public:
    virtual int OnCreateWidget(QWidget **ppWidget) = 0;

public slots:
    virtual void OnStartLoadingModuleDataSlot() = 0;

signals:
    void OnCreateWidgetSignal(ModuleType, IModule*);
    void OnLoadingModuleDataCompleteSignal();
    void OnSendModuleDataToGUISignal(ModuleType, QWidget*, QStandardItemModel*);
};

#endif
