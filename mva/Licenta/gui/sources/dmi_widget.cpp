#include "../headers/dmi_widget.h"
#include "ui_dmi_widget.h"

CDMIWidget::CDMIWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CDMIWidget),
    m_pController(NULL)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->propertiesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->propertiesTree->header()->setStretchLastSection(true);
    ui->propertiesTree->header()->setDefaultAlignment(Qt::AlignLeft);

    ui->itemsTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->itemsTree->header()->setDefaultAlignment(Qt::AlignLeft);

    ui->itemsTree->setFixedHeight(110);

    // connects
    connect(ui->itemsTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemsTreeClicked(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestPropertiesModel(DMIModuleType)),
            m_pController, SLOT(OnRequestDMIItemProperties(DMIModuleType)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetDMIItemsInformation(QStandardItemModel*)),
            this, SLOT(OnSetItemsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetDMIPropertiesInfomation(QStandardItemModel*)),
            this, SLOT(OnSetPropertiesTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
}

CDMIWidget::~CDMIWidget()
{
    delete ui;
    m_pController = NULL;
}

void CDMIWidget::OnSetItemsTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->itemsTree->setModel(pModel);
    ui->itemsTree->resizeColumnToContents(0);

    emit OnShowWidget(this);
}

void CDMIWidget::OnSetPropertiesTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->propertiesTree->setModel(pModel);
    ui->propertiesTree->resizeColumnToContents(0);
    ui->propertiesTree->resizeColumnToContents(1);
}

void CDMIWidget::OnItemsTreeClicked(QModelIndex index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->itemsTree->model());
    QStandardItem *pItem = pModel->itemFromIndex(index);
    QString qzItemText = pItem->text();

    if ("BIOS" == qzItemText)
        emit OnRequestPropertiesModel(DMI_BIOS);
    if ("Baseboard" == qzItemText)
        emit OnRequestPropertiesModel(DMI_BASEBOARD);
    if ("CPU" == qzItemText)
        emit OnRequestPropertiesModel(DMI_CPU);
    if ("Memory devices" == qzItemText)
        emit OnRequestPropertiesModel(DMI_MEMORY_DEVICE);
}
