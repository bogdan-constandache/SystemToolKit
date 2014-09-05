#include "../headers/video_card_widget.h"
#include "ui_video_card_widget.h"

CVideoCardWidget::CVideoCardWidget(QWidget *pParent, AbstractController *pController) :
    QWidget(pParent),
    ui(new Ui::CVideoCardWidget)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->tvDescription->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvDescription->setRootIsDecorated(false);
    ui->tvDescription->header()->setStretchLastSection(true);
    ui->tvDescription->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->tvDescription->setFocusPolicy(Qt::NoFocus);

    ui->tvCards->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvCards->setRootIsDecorated(false);
    ui->tvCards->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->tvCards->setFocusPolicy(Qt::NoFocus);

    ui->tvCards->setFixedHeight(110);

    connect(m_pController, SIGNAL(OnSetAvailableVCardsInformation(QStandardItemModel*)),
            this, SLOT(OnSetCardsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetVCardInfromation(QStandardItemModel*)),
            this, SLOT(OnSetCardInfoTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnVideoCardInformationDataChanged()),
            this, SLOT(OnDataChangedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestCardInformations(int)),
            m_pController, SLOT(OnRequestVCardInformationSlot(int)), Qt::QueuedConnection);
    connect(ui->tvCards, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemsTreeClicked(QModelIndex)), Qt::QueuedConnection);
}

CVideoCardWidget::~CVideoCardWidget()
{
    delete ui;
    m_pController = 0;
}

void CVideoCardWidget::OnSetCardsTreeModel(QStandardItemModel *pModel)
{
    if( pModel )
        ui->tvCards->setModel(pModel);
    ui->tvCards->resizeColumnToContents(0);
}

void CVideoCardWidget::OnSetCardInfoTreeModel(QStandardItemModel *pModel)
{
    if( pModel )
        ui->tvDescription->setModel(pModel);
    ui->tvDescription->resizeColumnToContents(0);
    ui->tvDescription->resizeColumnToContents(1);
}

void CVideoCardWidget::OnItemsTreeClicked(QModelIndex index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->tvCards->model());
    QStandardItem *pItem = pModel->itemFromIndex(index);

    QVariant qVariant = pItem->data();

    if( qVariant.isValid() )
        emit OnRequestCardInformations(qVariant.toInt());
}

void CVideoCardWidget::OnDataChangedSlot()
{
    ui->tvDescription->resizeColumnToContents(0);
    ui->tvDescription->resizeColumnToContents(1);
    ui->tvDescription->expandAll();
}


