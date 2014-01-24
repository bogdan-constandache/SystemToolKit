#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMap>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLayoutItem>

#include "../../interfaces/module.h"
#include "../../controller/headers/controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public IMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    CController *m_pController;

    QTreeView *m_pItemsTree;
    QStandardItemModel *m_pItemTreeModel;

    QMap<ModuleType, QWidget*> m_qLoadedModules;

    void PopulateItemTreeModel();
    void ShowModuleWidget(ModuleType);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetController(CController *pController);
    void closeEvent(QCloseEvent *);

public slots:
    void CreateWidgetSlot(ModuleType mType, IModule *pModule);
    void SendDataToGUISlot(ModuleType, QWidget* pWidget, QStandardItemModel *pModel);

private slots:
    void OnItemsTreeClickedSlot(QModelIndex index);

signals:
    void CreateModuleSignal(int mType);
    void OnMainWindowDialogClosedSignal();
};

#endif // MAINWINDOW_H
