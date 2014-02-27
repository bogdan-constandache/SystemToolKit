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

    ui->hddTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hddTree->header()->setStretchLastSection(true);
    ui->hddTree->header()->setDefaultAlignment(Qt::AlignLeft);

    // connects
    connect(ui->hddTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnHddTreeItemClicked(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestHddProperties(QString)),
            m_pController, SLOT(OnRequestATAItemProperties(QString)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetATAHDDItemsInformation(QStandardItemModel*)),
            this, SLOT(OnSetHddItemsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetATAItemPropertiesInformation(QStandardItemModel*)),
            this, SLOT(OnSetPropertiesTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
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

    emit OnShowWidget(this);
}

void CATAWidget::OnSetPropertiesTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->propertiesTree->setModel(pModel);
    ui->propertiesTree->resizeColumnToContents(0);
    ui->propertiesTree->resizeColumnToContents(1);
}

void CATAWidget::OnHddTreeItemClicked(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->hddTree->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);
    QString qzItemText = pItem->text();

    emit OnRequestHddProperties(qzItemText);
}
