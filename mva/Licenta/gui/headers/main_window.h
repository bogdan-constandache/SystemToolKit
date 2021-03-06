#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QDebug>
#include <QMap>
#include <QTreeView>
#include <QTableView>
#include <QStandardItemModel>
#include <QLayoutItem>
#include <QLabel>
#include <QAction>

#include "../abstract_controller.h"

#include "about_dialog.h"

#include "battery_status_widget.h"
#include "application_manager_widget.h"
#include "dmi_widget.h"
#include "smart_widget.h"
#include "ata_widget.h"
#include "system_drivers_widget.h"
#include "active_connections_widget.h"
#include "network_devices_widget.h"
#include "cpuid_widget.h"
#include "sensors_widget.h"
#include "processes_widget.h"
#include "startup_apps_widget.h"
#include "device_manager_widget.h"
#include "user_information_widget.h"
#include "spd_widget.h"
#include "video_card_widget.h"
#include "operating_system_widget.h"
#include "waiting_widget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow                  *ui;
    AbstractController              *m_pController;
    QStandardItemModel              *m_pItemTreeModel;

    // About dialog
    CAboutDialog                    *m_pAboutDialog;

    // Modules widgets
    CDMIWidget                      *m_pDMIManagerWidget;
    CBatteryStatusWidget            *m_pPowerManagementWidget;
    CApplicationManagerWidget       *m_pApplicationManagerWidget;
    CSmartWidget                    *m_pSMARTManagerWidget;
    CATAWidget                      *m_pATAManagerWidget;
    CSystemDriversWidget            *m_pSystemDriversWidget;
    CActiveConnectionsWidget        *m_pActiveConnectionsWidget;
    CNetworkDevicesWidget           *m_pNetworkDevicesWidget;
    CCPUIDWidget                    *m_pCPUIDWidget;
    CSensorsWidget                  *m_pSensorsWidget;
    CProcessesWidget                *m_pProcessesWidget;
    CStartupAppsWidget              *m_pStartupAppsWidget;
    CDeviceManagerWidget            *m_pDeviceManagerWidget;
    CUserInformationWidget          *m_pUserInformationWidget;
    CSPDWidget                      *m_pSPDWidget;
    CVideoCardWidget                *m_pVideoCardWidget;
    COperatingSystemWidget          *m_pOperatingSystemWidget;
    QWaitingWidget                  *m_pLoadingWidget;
    QWidget                         *m_pInitialWidget;

    void InitializeStackedWidget();

public:
    explicit MainWindow(QWidget *parent = 0, AbstractController *pController = 0);
    ~MainWindow();

private slots:
    void OnItemsTreeClickedSlot(QModelIndex index);
    void OnPopulateMenuTreeSlot(QStandardItemModel *pModel);
    void OnShowWidget(QWidget *pWidget);

signals:
//    void OnComputerDeviceManagerOptClickedSignal();
//    void OnComputerDMIOptClickedSignal();
//    void OnComputerPowerManagementOptClickedSignal();
//    void OnHddInformationOptClickedSignal();
//    void OnOperatingSystemOptClickedSignal();
//    void OnProcessesOptClickedSignal();
//    void OnSystemDriversOptClickedSignal();
//    void OnStorageATAOptClickedSignal();
//    void OnStorageSmartOptClickedSignal();
//    void OnSmbiosOptClickedSignal();
//    void OnApplicationManagerOptClickedSignal();
//    void OnStartupApplicationsOptClickedSignal();
//    void OnActiveConnectionsOptClickedSignal();
//    void OnNetworkDevicesOptClickedSignal();
//    void OnCPUOptClickedSignal();
//    void OnCPUIDOptClickedSignal();
//    void OnSensorsOptClickedSignal();
//    void OnSystemUsersOptClickedSignal();
//    void OnMotherboardSPDOptClickedSignal();
//    void OnMotherboardVideoCardOptClickedSignal();

    void OnDispatchMenuOptionTagSignal(int);
};

#endif // MAINWINDOW_H
