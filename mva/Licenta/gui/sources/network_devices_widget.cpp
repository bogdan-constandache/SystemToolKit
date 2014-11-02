#include "../headers/network_devices_widget.h"
#include "ui_network_devices_widget.h"

CNetworkDevicesWidget::CNetworkDevicesWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CNetworkDevicesWidget),
    m_pController(NULL)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->networkDevInfos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->networkDevInfos->setRootIsDecorated(false);
    ui->networkDevInfos->header()->setStretchLastSection(true);
    ui->networkDevInfos->header()->setDefaultAlignment(Qt::AlignLeft);

    ui->networkDevNames->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->networkDevNames->setRootIsDecorated(false);
    ui->networkDevNames->header()->setStretchLastSection(true);
    ui->networkDevNames->header()->setDefaultAlignment(Qt::AlignLeft);

    // connects
    connect(ui->networkDevNames, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnNetworkNamesItemClickedSlot(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestNetworkDeviceInformationSignal(QString)),
            m_pController, SLOT(OnRequestNetworkDeviceInfomationsSlot(QString)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetNetworkDevicesNames(QStandardItemModel*)),
            this, SLOT(OnSetNetworkNamesTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetNetworkDeviceInformation(QStandardItemModel*)),
            this, SLOT(OnSetNetworkDeviceInformationsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnNetworkAdaptersInformationDataChanged()),
            this, SLOT(OnAdaptersNamesDataChangedSlot()), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnNetworkAdapterPropertiesDataChanged()),
            this, SLOT(OnAdaptersPropertiesDataChangedSlot()), Qt::QueuedConnection);
}

CNetworkDevicesWidget::~CNetworkDevicesWidget()
{
    m_pController = NULL;
    delete ui;
}

void CNetworkDevicesWidget::OnNetworkNamesItemClickedSlot(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->networkDevNames->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);
    QString qzItemText = pItem->text();

    emit OnRequestNetworkDeviceInformationSignal(qzItemText);
}

void CNetworkDevicesWidget::OnSetNetworkNamesTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
    {
        ui->networkDevNames->setModel(pModel);
        ui->networkDevNames->resizeColumnToContents(0);
    }
}

void CNetworkDevicesWidget::OnSetNetworkDeviceInformationsTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
    {
        ui->networkDevInfos->setModel(pModel);
        ui->networkDevInfos->resizeColumnToContents(0);
        ui->networkDevInfos->resizeColumnToContents(1);
    }
}

void CNetworkDevicesWidget::OnAdaptersNamesDataChangedSlot()
{
    ui->networkDevNames->resizeColumnToContents(0);

    emit OnShowWidget(this);
}

void CNetworkDevicesWidget::OnAdaptersPropertiesDataChangedSlot()
{
    ui->networkDevInfos->setColumnWidth(0, 200);
}
