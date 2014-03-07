#include "../headers/main_window.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, AbstractController *pController) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pController(NULL),
    m_pItemTreeModel(NULL), m_pPowerManagementWidget(NULL),
    m_pApplicationManagerWidget(NULL), m_pDMIManagerWidget(NULL),
    m_pSMARTManagerWidget(NULL), m_pATAManagerWidget(NULL),
    m_pSystemDriversWidget(NULL), m_pActiveConnectionsWidget(NULL),
    m_pNetworkDevicesWidget(NULL)
{
    ui->setupUi(this);
    this->setMinimumWidth(1000);
    this->setMinimumHeight(600);
    this->setWindowTitle(QString("System Toolkit"));

    m_pController = pController;

    connect(m_pController, SIGNAL(OnPopulateMenuTreeSignal(QStandardItemModel*)),
            this, SLOT(OnPopulateMenuTreeSlot(QStandardItemModel*)), Qt::QueuedConnection);

    connect(this, SIGNAL(OnComputerDMIOptClickedSignal()),
            m_pController, SLOT(OnComputerDMIOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnComputerPowerManagementOptClickedSignal()),
            m_pController, SLOT(OnComputerPowerManagementOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnHddInformationOptClickedSignal()),
            m_pController, SLOT(OnHddInformationOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnOperatingSystemOptClickedSignal()),
            m_pController, SLOT(OnOperatingSystemOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnProcessesOptClickedSignal()),
            m_pController, SLOT(OnProcessesOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnSystemDriversOptClickedSignal()),
            m_pController, SLOT(OnSystemDriversOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnStorageATAOptClickedSignal()),
            m_pController, SLOT(OnStorageATAOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnStorageSmartOptClickedSignal()),
            m_pController, SLOT(OnStorageSmartOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnSmbiosOptClickedSignal()),
            m_pController, SLOT(OnSmbiosOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnApplicationManagerOptClickedSignal()),
            m_pController, SLOT(OnApplicationManagerOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnStartupApplicationsOptClickedSignal()),
            m_pController, SLOT(OnStartupApplicationsOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnActiveConnectionsOptClickedSignal()),
            m_pController, SLOT(OnActiveConnectionsOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnNetworkDevicesOptClickedSignal()),
            m_pController, SLOT(OnNetworkDevicesOptClickedSlot()), Qt::QueuedConnection);

    InitializeStackedWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitializeStackedWidget()
{
    // Create all modules widgets
    m_pDMIManagerWidget = new CDMIWidget(ui->stackedWidget, m_pController);
    connect(m_pDMIManagerWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pPowerManagementWidget = new CBatteryStatusWidget(ui->stackedWidget, m_pController);
    connect(m_pPowerManagementWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pApplicationManagerWidget = new CApplicationManagerWidget(ui->stackedWidget, m_pController);
    connect(m_pApplicationManagerWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pSMARTManagerWidget = new CSmartWidget(ui->stackedWidget, m_pController);
    connect(m_pSMARTManagerWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pATAManagerWidget = new CATAWidget(ui->stackedWidget, m_pController);
    connect(m_pATAManagerWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pSystemDriversWidget = new CSystemDriversWidget(ui->stackedWidget, m_pController);
    connect(m_pSystemDriversWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pActiveConnectionsWidget = new CActiveConnectionsWidget(ui->stackedWidget, m_pController);
    connect(m_pActiveConnectionsWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pNetworkDevicesWidget = new CNetworkDevicesWidget(ui->stackedWidget, m_pController);
    connect(m_pNetworkDevicesWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    // Add widget to the stacked widget
    ui->stackedWidget->addWidget(m_pDMIManagerWidget);
    ui->stackedWidget->addWidget(m_pPowerManagementWidget);
    ui->stackedWidget->addWidget(m_pApplicationManagerWidget);
    ui->stackedWidget->addWidget(m_pSMARTManagerWidget);
    ui->stackedWidget->addWidget(m_pATAManagerWidget);
    ui->stackedWidget->addWidget(m_pSystemDriversWidget);
    ui->stackedWidget->addWidget(m_pActiveConnectionsWidget);
    ui->stackedWidget->addWidget(m_pNetworkDevicesWidget);

    // remove first to pages
    ui->stackedWidget->removeWidget(ui->page_2);

    // set the empty page
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::OnItemsTreeClickedSlot(QModelIndex index)
{
    QStandardItem *pStandardItem = this->m_pItemTreeModel->itemFromIndex(index);
    QString qzItemText = pStandardItem->text();

    if( "DMI" == qzItemText )
    {
        qDebug() << "DMI clicked!";
        emit OnComputerDMIOptClickedSignal();
    }
    if( "Power management" == qzItemText )
    {
        qDebug() << "Power management";
        emit OnComputerPowerManagementOptClickedSignal();
    }
    if( "Operating system" == qzItemText )
    {
        qDebug() << "Operating system clicked";
        emit OnOperatingSystemOptClickedSignal();
    }
    if( "Processes" == qzItemText )
    {
        qDebug() << "Processes clicked";
        emit OnProcessesOptClickedSignal();
    }
    if( "System drivers" == qzItemText )
    {
        qDebug() << "System drivers clicked";
        emit OnSystemDriversOptClickedSignal();
    }
    if( "ATA" == qzItemText)
    {
        qDebug() << "ATA clicked";
        emit OnStorageATAOptClickedSignal();
    }
    if( "SMART" == qzItemText )
    {
        qDebug() << "SMART";
        emit OnStorageSmartOptClickedSignal();
    }
    if( "Startup applications" == qzItemText )
    {
        qDebug() << "Startup applications clicked";
        emit OnStartupApplicationsOptClickedSignal();
    }
    if( "Application manager" == qzItemText )
    {
        qDebug() << "Application manager clicked";
        emit OnApplicationManagerOptClickedSignal();
    }
    if( "Connections" == qzItemText )
    {
        qDebug() << "Connections clicked";
        emit OnActiveConnectionsOptClickedSignal();
    }
    if( "Network devices" == qzItemText)
    {
        qDebug() << "Network devices clicked";
        emit OnNetworkDevicesOptClickedSignal();
    }
}

void MainWindow::OnPopulateMenuTreeSlot(QStandardItemModel *pModel)
{
    ui->menuTreeView->setModel(pModel);
    m_pItemTreeModel = pModel;

    connect(ui->menuTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemsTreeClickedSlot(QModelIndex)), Qt::QueuedConnection);

    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::OnShowWidget(QWidget *pWidget)
{
    if (pWidget)
        ui->stackedWidget->setCurrentWidget(pWidget);
}
