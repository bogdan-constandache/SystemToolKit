#include "../headers/smart_widget.h"
#include "ui_smart_widget.h"

CSmartWidget::CSmartWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSmartWidget),
    m_pController(NULL)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->propertiesTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->propertiesTreeView->setRootIsDecorated(false);
    ui->propertiesTreeView->header()->setStretchLastSection(true);
    ui->propertiesTreeView->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->propertiesTreeView->setFocusPolicy(Qt::NoFocus);

    ui->hddTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hddTreeView->setRootIsDecorated(false);
    ui->hddTreeView->header()->setStretchLastSection(true);
    ui->hddTreeView->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->hddTreeView->setFocusPolicy(Qt::NoFocus);

    // connects
    connect(ui->hddTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemsTreeClicked(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestHDDInformations(QString)),
            m_pController, SLOT(OnRequestSMARTProperties(QString)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetSMARTHDDItemsInformation(QStandardItemModel*)),
            this, SLOT(OnSetHDDItemsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetSMARTItemPropertiesInformation(QStandardItemModel*)),
            this, SLOT(OnSetPropertiesTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSMARTHDDInformationDataChanged()),
            this, SLOT(OnSmartHDDDataChangedSlot()), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSMARTItemPropertiesDataChanged()),
            this, SLOT(OnSmartItemDataChangedSlot()), Qt::QueuedConnection);
}

CSmartWidget::~CSmartWidget()
{
    m_pController = NULL;
    delete ui;
}

void CSmartWidget::OnSetHDDItemsTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->hddTreeView->setModel(pModel);
    ui->hddTreeView->resizeColumnToContents(0);

    int nTableSize = ui->hddTreeView->model()->rowCount() * 30 + 20;
    if( nTableSize > 100 )
        ui->hddTreeView->setFixedHeight(100);
    else
        ui->hddTreeView->setFixedHeight(nTableSize);
}

void CSmartWidget::OnSetPropertiesTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
    {
        ui->propertiesTreeView->setModel(pModel);
    }
    ui->propertiesTreeView->setColumnWidth(0, 150);
}

void CSmartWidget::OnItemsTreeClicked(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->hddTreeView->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);

    QVariant qvData = pItem->data();

    if( qvData.isValid() )
        emit OnRequestHDDInformations(qvData.toString());
}

void CSmartWidget::OnSmartHDDDataChangedSlot()
{
    emit OnShowWidget(this);
}

void CSmartWidget::OnSmartItemDataChangedSlot()
{
    ui->propertiesTreeView->resizeColumnToContents(0);
    ui->propertiesTreeView->setColumnWidth(1, 275);
}


