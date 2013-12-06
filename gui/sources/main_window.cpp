#include "../headers/main_window.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    //    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pController(0),
    m_pItemsTree(0)
{
    (parent);
    ui->setupUi(this);
    this->setMinimumWidth(1000);
    this->setMinimumHeight(600);
    this->setWindowTitle(QString("System Toolkit"));
    this->ui->ModuleWidget->setLayout(new QVBoxLayout());
    this->ui->ModuleWidget->layout()->setContentsMargins(0, 0, 0, 0);

    this->ui->MainSplitter->setSizes(QList<int>() << 175 << 625);

    this->m_pItemsTree = new QTreeView(this->ui->widget);
    this->m_pItemTreeModel = new QStandardItemModel();
    this->PopulateItemTreeModel();

    this->m_pItemsTree->setModel(this->m_pItemTreeModel);
    this->m_pItemsTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->widget->setLayout(new QVBoxLayout());
    this->ui->widget->layout()->setContentsMargins(0, 0, 0, 0);
    this->ui->widget->layout()->addWidget(this->m_pItemsTree);

    QObject::connect(this->m_pItemsTree, SIGNAL(clicked(QModelIndex)),
                     this, SLOT(OnItemsTreeClickedSlot(QModelIndex)),
                     Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetController(CController *pController)
{
    this->m_pController = pController;
    if (0 != this->m_pController)
    {
        QObject::connect(m_pController, SIGNAL(StartDrawingMainWindowSignal()),
                         this, SLOT(show()),
                         Qt::QueuedConnection);

        QObject::connect(this, SIGNAL(CreateModuleSignal(int)),
                         m_pController, SLOT(CreateModuleSlot(int)),
                         Qt::QueuedConnection);

        QObject::connect(this, SIGNAL(OnMainWindowDialogClosedSignal()),
                         m_pController, SLOT(OnMainWindowDialogClosedSlot()),
                         Qt::QueuedConnection);
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit OnMainWindowDialogClosedSignal();
}

void MainWindow::CreateWidgetSlot(ModuleType mType, IModule *pModule)
{
    QWidget *pWidget = 0;
    pModule->OnCreateWidget(&pWidget);

    QLayoutItem *pItem = 0;
    while((pItem = this->ui->ModuleWidget->layout()->takeAt(0)) != 0)
    {
        pItem->widget()->hide();
        this->ui->ModuleWidget->layout()->removeItem(pItem);
    }

    if (0 != pWidget)
    {
        m_qLoadedModules.insert(mType, pWidget);
        ui->ModuleWidget->layout()->addWidget(pWidget);
    }
}

void MainWindow::SendDataToGUISlot(ModuleType nType, QWidget *pWidget, QStandardItemModel *pModel)
{
    if( COMPUTER_DMI == nType || COMPUTER_POWER_MANAGEMENT == nType )
    {
        QTreeView *pTree = (QTreeView*)pWidget;
        pTree->setModel(pModel);
        pTree->header()->resizeSections(QHeaderView::ResizeToContents);
    }
    else
    {
        QTableView *pTable = (QTableView*)pWidget;
        pTable->setModel(pModel);
        pTable->resizeRowsToContents();
    }
}

void MainWindow::OnItemsTreeClickedSlot(QModelIndex index)
{
    QStandardItem *pStandardItem = this->m_pItemTreeModel->itemFromIndex(index);
    QString qzItemText = pStandardItem->text();

    if( "DMI" == qzItemText )
    {
        qDebug() << "DMI clicked!";
        if( !m_qLoadedModules.contains(COMPUTER_DMI) )
            emit CreateModuleSignal(COMPUTER_DMI);
        else
        {
            this->ShowModuleWidget(COMPUTER_DMI);
        }
    }
    if( "Power management" == qzItemText )
    {
        qDebug() << "Power management";
        if( !m_qLoadedModules.contains(COMPUTER_POWER_MANAGEMENT) )
            emit CreateModuleSignal(COMPUTER_POWER_MANAGEMENT);
        else
        {
            this->ShowModuleWidget(COMPUTER_POWER_MANAGEMENT);
        }
    }
    if( "Operating system" == qzItemText )
    {
        qDebug() << "Operating system clicked";
    }
    if( "Processes" == qzItemText )
    {
        qDebug() << "Processes clicked";
        if( !m_qLoadedModules.contains(OS_PROCESSES) )
            emit CreateModuleSignal(OS_PROCESSES);
        else
        {
            this->ShowModuleWidget(OS_PROCESSES);
        }
    }
    if( "System drivers" == qzItemText )
    {
        qDebug() << "System drivers clicked";
        if( !m_qLoadedModules.contains(OS_SYSTEM_DRIVERS) )
            emit CreateModuleSignal(OS_SYSTEM_DRIVERS);
        else
        {
            this->ShowModuleWidget(OS_SYSTEM_DRIVERS);
        }
    }
    if( "SMART" == qzItemText )
    {
        qDebug() << "SMART";
        if( !m_qLoadedModules.contains(STORAGE_SMART) )
            emit CreateModuleSignal(STORAGE_SMART);
        else
        {
            this->ShowModuleWidget(STORAGE_SMART);
        }
    }
    if( "Startup applications" == qzItemText )
    {
        qDebug() << "tartup applications clicked";
    }
    if( "Application manager" == qzItemText )
    {
        qDebug() << "Application manager clicked";
    }

}

void MainWindow::PopulateItemTreeModel()
{
    QStandardItem *pDataRoot = m_pItemTreeModel->invisibleRootItem();
    QStandardItem *pDataItem = 0;
    QStandardItem *pChildItem = 0;
    QList<QStandardItem*> qList;
    QList<QStandardItem*> qChildList;

    pDataItem = new QStandardItem(QString("Computer"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("DMI"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Power management"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Operating system"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Operating system"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Processes"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("System drivers"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Storage"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("SMART"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Software"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Startup applications"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Application manager"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataRoot->appendRows(qList);
    qList.clear();

    this->m_pItemTreeModel->setHorizontalHeaderLabels(QStringList() << "Menu");
}

void MainWindow::ShowModuleWidget(ModuleType type)
{
    QWidget *pWidget = m_qLoadedModules.value(type);
    QLayoutItem *pItem = 0;
    while((pItem = this->ui->ModuleWidget->layout()->takeAt(0)) != 0)
    {
        pItem->widget()->hide();
        this->ui->ModuleWidget->layout()->removeItem(pItem);
    }

    if (0 != pWidget)
    {
        ui->ModuleWidget->layout()->addWidget(pWidget);
        pWidget->show();
    }
}
