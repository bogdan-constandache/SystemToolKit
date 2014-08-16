#include "../headers/device_manager_widget.h"
#include "ui_device_manager_widget.h"

CDeviceManagerWidget::CDeviceManagerWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CDeviceManagerWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    // set tree properties
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);

    ui->treeViewProperties->setBaseSize(this->width(), 160);
    ui->treeViewProperties->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeViewProperties->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeViewProperties->setRootIsDecorated(false);
    ui->treeViewProperties->header()->setStretchLastSection(true);
    ui->treeViewProperties->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->treeViewProperties->hide();


    connect(m_pController, SIGNAL(OnSetDeviceManagerInformation(QStandardItemModel*)),
            this, SLOT(OnSetTreeModelSlot(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetDevicePropertiesInformation(QStandardItemModel*)),
            this, SLOT(OnSetDevicePropertiesSlot(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemTreeClickedSlot(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestDeviceDetailsSignal(QString)),
            m_pController, SLOT(OnRequestDeviceDetailsSlot(QString)), Qt::QueuedConnection);
}

CDeviceManagerWidget::~CDeviceManagerWidget()
{
    delete ui;
    m_pController = 0;
}

void CDeviceManagerWidget::OnSetTreeModelSlot(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);

//    emit OnShowWidget(this);
}

void CDeviceManagerWidget::OnSetDevicePropertiesSlot(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeViewProperties->setModel(pModel);
    ui->treeViewProperties->resizeColumnToContents(0);
    ui->treeViewProperties->resizeColumnToContents(1);
    ui->treeViewProperties->sortByColumn(0, Qt::AscendingOrder);
}

void CDeviceManagerWidget::OnItemTreeClickedSlot(QModelIndex qIndex)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->treeView->model());
    QStandardItem *pItem = pModel->itemFromIndex(qIndex);
    QString qzItemText = pItem->data().toString();

    ui->treeViewProperties->reset();
    if( "" == qzItemText )
    {
        ui->treeViewProperties->hide();
    }
    else
    {
        ui->treeViewProperties->show();
        emit OnRequestDeviceDetailsSignal( qzItemText );
    }

}
