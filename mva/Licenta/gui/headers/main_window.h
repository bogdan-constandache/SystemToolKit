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
#include "battery_status_widget.h"
#include "application_manager_widget.h"
#include "dmi_widget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    AbstractController *m_pController;
    QStandardItemModel *m_pItemTreeModel;

    // Modules widgets
    CDMIWidget *m_pDMIManagerWidget;
    CBatteryStatusWidget *m_pPowerManagementWidget;
    CApplicationManagerWidget *m_pApplicationManagerWidget;

    void InitializeStackedWidget();

public:
    explicit MainWindow(QWidget *parent = 0, AbstractController *pController = 0);
    ~MainWindow();

private slots:
    void OnItemsTreeClickedSlot(QModelIndex index);
    void OnPopulateMenuTreeSlot(QStandardItemModel *pModel);
    void OnShowWidget(QWidget *pWidget);

signals:
    void OnComputerDMIOptClickedSignal();
    void OnComputerPowerManagementOptClickedSignal();
    void OnHddInformationOptClickedSignal();
    void OnOperatingSystemOptClickedSignal();
    void OnProcessesOptClickedSignal();
    void OnSystemDriversOptClickedSignal();
    void OnStorageSmartOptClickedSignal();
    void OnSmbiosOptClickedSignal();
    void OnApplicationManagerOptClickedSignal();
    void OnStartupApplicationsOptClickedSignal();
};

#endif // MAINWINDOW_H
