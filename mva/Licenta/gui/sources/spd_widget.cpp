#include "../headers/spd_widget.h"
#include "ui_spd_widget.h"


CSPDWidget::CSPDWidget(QWidget *pParent, AbstractController *pController):
    QWidget(pParent),
    ui(new Ui::CSPDWidget)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->tvDimmInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvDimmInfo->setRootIsDecorated(false);
    ui->tvDimmInfo->header()->setStretchLastSection(true);
    ui->tvDimmInfo->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->tvDimmInfo->setFocusPolicy(Qt::NoFocus);

    ui->tvDimms->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvDimms->setRootIsDecorated(false);
    ui->tvDimms->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->tvDimms->setFocusPolicy(Qt::NoFocus);

    ui->tvDimms->setFixedHeight(110);

    // connects
    connect(ui->tvDimms, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemsTreeClicked(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestDimmInformations(int)),
            m_pController, SLOT(OnRequestSPDDimmDetailsSlot(int)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetAvailableDIMMSInformation(QStandardItemModel*)),
            this, SLOT(OnSetItemsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetDimmSPDInformation(QStandardItemModel*)),
            this, SLOT(OnSetDimmInfoTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSPDDimmInformationDataChanged()),
            this, SLOT(OnDataChangedSlot()), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnAvailableDIMMSInformationDataChanged()),
            this, SLOT(OnAvailableDimmsDataChangedSlot()), Qt::QueuedConnection);
}

CSPDWidget::~CSPDWidget()
{
    delete ui;
    m_pController = 0;
}

void CSPDWidget::OnSetItemsTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->tvDimms->setModel(pModel);
    ui->tvDimms->resizeColumnToContents(0);
}

void CSPDWidget::OnSetDimmInfoTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->tvDimmInfo->setModel(pModel);
    ui->tvDimmInfo->resizeColumnToContents(0);
    ui->tvDimmInfo->resizeColumnToContents(1);
}

void CSPDWidget::OnItemsTreeClicked(QModelIndex index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->tvDimms->model());
    QStandardItem *pItem = pModel->itemFromIndex(index);

    QVariant qVariant = pItem->data();

    if( qVariant.isValid() )
        emit OnRequestDimmInformations(qVariant.toInt());
}

void CSPDWidget::OnDataChangedSlot()
{
    ui->tvDimmInfo->setColumnWidth(0, 130);
}

void CSPDWidget::OnAvailableDimmsDataChangedSlot()
{
    emit OnShowWidget(this);
}
