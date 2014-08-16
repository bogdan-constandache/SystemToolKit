#include "../headers/main_window.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFont>
#include <QPicture>

MainWindow::MainWindow(QWidget *parent, AbstractController *pController) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pController(NULL), m_pAboutDialog(NULL),
    m_pItemTreeModel(NULL), m_pPowerManagementWidget(NULL),
    m_pApplicationManagerWidget(NULL), m_pDMIManagerWidget(NULL),
    m_pSMARTManagerWidget(NULL), m_pATAManagerWidget(NULL),
    m_pSystemDriversWidget(NULL), m_pActiveConnectionsWidget(NULL),
    m_pNetworkDevicesWidget(NULL), m_pCPUIDWidget(NULL), m_pSensorsWidget(NULL),
    m_pProcessesWidget(NULL), m_pStartupAppsWidget(NULL), m_pDeviceManagerWidget(NULL)
{
    ui->setupUi(this);
    this->setMinimumWidth(1000);
    this->setMinimumHeight(600);
    this->setWindowTitle(QString("System Monitoring"));
    this->setWindowIcon(QIcon(":/img/logo.png"));

    // LOGO
    ui->labelLogo->setFixedSize(48, 48);
    ui->labelLogo->setPixmap(QPixmap(":/img/logo.png"));
    ui->labelText->setFixedHeight(48);
    ui->labelText->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->labelText->setText("System Monitoring");

    // LINES
    ui->lineLogo->setFixedHeight(1);
    ui->lineMenu->setFixedWidth(1);

    ui->menuTreeView->setEditTriggers(QTreeView::NoEditTriggers);
    ui->menuTreeView->setSelectionMode(QAbstractItemView::SingleSelection);

    m_pController = pController;

    connect(m_pController, SIGNAL(OnPopulateMenuTreeSignal(QStandardItemModel*)),
            this, SLOT(OnPopulateMenuTreeSlot(QStandardItemModel*)), Qt::QueuedConnection);

    connect(this, SIGNAL(OnComputerDeviceManagerOptClickedSignal()),
            m_pController, SLOT(OnComputerDeviceManagerOptClickedSlot()), Qt::QueuedConnection);
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
    connect(this, SIGNAL(OnCPUOptClickedSignal()),
            m_pController, SLOT(OnCPUOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnCPUIDOptClickedSignal()),
            m_pController, SLOT(OnCPUIDOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnSensorsOptClickedSignal()),
            m_pController, SLOT(OnSensorsOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnSystemUsersOptClickedSignal()),
            m_pController, SLOT(OnUserInformationsOptClickedSlot()), Qt::QueuedConnection);

    InitializeStackedWidget();

    QFile qFile(":/style/qss/main_dialog.qss");
    if( !qFile.open(QIODevice::ReadOnly) )
        return;

    QString qzStyleSheet = QLatin1String(qFile.readAll());

    setStyleSheet(qzStyleSheet);
    ensurePolished();

    // Create about dialog
    m_pAboutDialog = new CAboutDialog(this);

    connect(ui->btnAbout, SIGNAL(clicked()),
            m_pAboutDialog, SLOT(show()), Qt::QueuedConnection);
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

    m_pCPUIDWidget = new CCPUIDWidget(ui->stackedWidget, m_pController);
    connect(m_pCPUIDWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pSensorsWidget = new CSensorsWidget(ui->stackedWidget, m_pController);
    connect(m_pSensorsWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pProcessesWidget = new CProcessesWidget(ui->stackedWidget, m_pController);
    connect(m_pProcessesWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pStartupAppsWidget = new CStartupAppsWidget(ui->stackedWidget, m_pController);
    connect(m_pStartupAppsWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pDeviceManagerWidget = new CDeviceManagerWidget(ui->stackedWidget, m_pController);
    connect(m_pDeviceManagerWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    m_pUserInformationWidget = new CUserInformationWidget(ui->stackedWidget, m_pController);
    connect(m_pUserInformationWidget, SIGNAL(OnShowWidget(QWidget*)), this, SLOT(OnShowWidget(QWidget*)), Qt::QueuedConnection);

    // Add widget to the stacked widget
    ui->stackedWidget->addWidget(m_pDMIManagerWidget);
    ui->stackedWidget->addWidget(m_pPowerManagementWidget);
    ui->stackedWidget->addWidget(m_pApplicationManagerWidget);
    ui->stackedWidget->addWidget(m_pSMARTManagerWidget);
    ui->stackedWidget->addWidget(m_pATAManagerWidget);
    ui->stackedWidget->addWidget(m_pSystemDriversWidget);
    ui->stackedWidget->addWidget(m_pActiveConnectionsWidget);
    ui->stackedWidget->addWidget(m_pNetworkDevicesWidget);
    ui->stackedWidget->addWidget(m_pCPUIDWidget);
    ui->stackedWidget->addWidget(m_pSensorsWidget);
    ui->stackedWidget->addWidget(m_pProcessesWidget);
    ui->stackedWidget->addWidget(m_pStartupAppsWidget);
    ui->stackedWidget->addWidget(m_pDeviceManagerWidget);
    ui->stackedWidget->addWidget(m_pUserInformationWidget);

    // remove first to pages
    ui->stackedWidget->removeWidget(ui->page_2);

    // set the empty page
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::OnItemsTreeClickedSlot(QModelIndex index)
{
    QStandardItem *pStandardItem = this->m_pItemTreeModel->itemFromIndex(index);
    QString qzItemText = pStandardItem->text();

    if( "Device manager" == qzItemText )
    {
        qDebug() << "Device manager clicked!";
        emit OnComputerDeviceManagerOptClickedSignal();
        ui->stackedWidget->setCurrentWidget(m_pDeviceManagerWidget);
    }
    if( "DMI" == qzItemText )
    {
        qDebug() << "DMI clicked!";
        emit OnComputerDMIOptClickedSignal();
        ui->stackedWidget->setCurrentWidget(m_pDMIManagerWidget);
    }
    if( "Power management" == qzItemText )
    {
        qDebug() << "Power management";
        emit OnComputerPowerManagementOptClickedSignal();
        ui->stackedWidget->setCurrentWidget(m_pPowerManagementWidget);
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
    if( "System users" == qzItemText )
    {
        qDebug() << "System users clicked";
        emit OnSystemUsersOptClickedSignal();
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
    if( "CPU" == qzItemText )
    {
        qDebug() << "CPU clicked";
        emit OnCPUOptClickedSignal();
    }
    if( "CPUID" == qzItemText )
    {
        qDebug() << "CPUID clicked";
        emit OnCPUIDOptClickedSignal();
    }
    if( "Sensors" == qzItemText )
    {
        qDebug() << "Sensors clicked";
        emit OnSensorsOptClickedSignal();
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
