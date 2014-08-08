#include "../headers/smart_widget.h"
#include "ui_smart_widget.h"

CSmartWidget::CSmartWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSmartWidget),
    m_pController(NULL), m_pPropertiesModel(NULL)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->propertiesTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->propertiesTreeView->setRootIsDecorated(false);
    ui->propertiesTreeView->header()->setStretchLastSection(true);
    ui->propertiesTreeView->header()->setDefaultAlignment(Qt::AlignLeft);

    ui->hddTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hddTreeView->setRootIsDecorated(false);
    ui->hddTreeView->header()->setStretchLastSection(true);
    ui->hddTreeView->header()->setDefaultAlignment(Qt::AlignLeft);

    // connects
    connect(ui->hddTreeView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnItemsTreeClicked(QModelIndex)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestHDDInformations(QString)),
            m_pController, SLOT(OnRequestSMARTProperties(QString)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetSMARTHDDItemsInformation(QStandardItemModel*)),
            this, SLOT(OnSetHDDItemsTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetSMARTItemPropertiesInformation(QStandardItemModel*)),
            this, SLOT(OnSetPropertiesTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
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

    if( NULL != m_pPropertiesModel )
    {
        delete m_pPropertiesModel;
        m_pPropertiesModel = NULL;
    }

    ui->propertiesTreeView->reset();

    emit OnShowWidget(this);
}

void CSmartWidget::OnSetPropertiesTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
    {
        ui->propertiesTreeView->setModel(pModel);
        m_pPropertiesModel = pModel;
    }
    ui->propertiesTreeView->resizeColumnToContents(0);
    ui->propertiesTreeView->resizeColumnToContents(1);
}

void CSmartWidget::OnItemsTreeClicked(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->hddTreeView->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);
    QString qzItemText = pItem->text();

    emit OnRequestHDDInformations(qzItemText);
}


