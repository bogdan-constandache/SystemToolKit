#include "../headers/active_connections_widget.h"
#include "ui_active_connections_widget.h"

CActiveConnectionsWidget::CActiveConnectionsWidget(QWidget *parent,  AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CActiveConnectionsWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    // set properties
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->treeView->header()->resizeSections(QHeaderView::Interactive);
    ui->treeView->setRootIsDecorated(false);

    // connects
    connect(m_pController, SIGNAL(OnSetActiveConnectionsInformation(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnActiveConnectionsInformationDataChanged()),
            this, SLOT(OnActiveConnectionDataChangedSlot()), Qt::QueuedConnection);
    connect(ui->refreshButton, SIGNAL(clicked()),
            m_pController, SLOT(OnNetworkConnectionsOptClickedSlot()), Qt::QueuedConnection);
}

CActiveConnectionsWidget::~CActiveConnectionsWidget()
{
    delete ui;
    m_pController = 0;
}

void CActiveConnectionsWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->resizeColumnToContents(3);
}

void CActiveConnectionsWidget::OnActiveConnectionDataChangedSlot()
{
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->resizeColumnToContents(3);

    emit OnShowWidget(this);
}
