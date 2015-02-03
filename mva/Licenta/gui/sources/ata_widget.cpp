#include "../headers/ata_widget.h"
#include "ui_ata_widget.h"

CATAWidget::CATAWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CATAWidget),
    m_pController(NULL)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->propertiesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->propertiesTree->header()->setStretchLastSection(true);
    ui->propertiesTree->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->propertiesTree->setRootIsDecorated(false);
    ui->propertiesTree->setFocusPolicy(Qt::NoFocus);

    ui->hddTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hddTree->header()->setStretchLastSection(true);
    ui->hddTree->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->hddTree->setRootIsDecorated(false);
    ui->hddTree->setFocusPolicy(Qt::NoFocus);

    // connects
    connect(ui->hddTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnHddTreeItemClicked(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestHddProperties(QString)),
            m_pController, SLOT(OnRequestATAItemProperties(QString)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetATAHDDItemsInformation(QStandardItemModel*)),
            this, SLOT(OnSetHddItemsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetATAItemPropertiesInformation(QStandardItemModel*)),
            this, SLOT(OnSetPropertiesTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnATAHDDInformationDataChanged()),
            this, SLOT(OnATAHddDataChanged()), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnATAItemPropertiesDataChanged()),
            this, SLOT(OnATAItemPropDataChanged()), Qt::QueuedConnection);

    ui->line->hide();
}

CATAWidget::~CATAWidget()
{
    m_pController = NULL;
    delete ui;
}

void CATAWidget::OnSetHddItemsTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->hddTree->setModel(pModel);
    ui->hddTree->resizeColumnToContents(0);

    int nTableSize = ui->hddTree->model()->rowCount() * 30 + 20;
    if( nTableSize > 100 )
        ui->hddTree->setFixedHeight(100);
    else
        ui->hddTree->setFixedHeight(nTableSize);
}

void CATAWidget::OnSetPropertiesTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->propertiesTree->setModel(pModel);

    ui->propertiesTree->setColumnWidth(0, 150);
}

void CATAWidget::OnHddTreeItemClicked(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->hddTree->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);

    QVariant qvData = pItem->data(ATA_HDD_ID_TAG);
    if( qvData.isValid() )
        emit OnRequestHddProperties(qvData.toString());
}

void CATAWidget::OnATAHddDataChanged()
{
    emit OnShowWidget(this);
}

void CATAWidget::OnATAItemPropDataChanged()
{
    ui->propertiesTree->setColumnWidth(0, 175);
}
