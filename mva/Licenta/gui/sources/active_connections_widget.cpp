#include "../headers/active_connections_widget.h"
#include "ui_active_connections_widget.h"

CActiveConnectionsWidget::CActiveConnectionsWidget(QWidget *parent,  AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CActiveConnectionsWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    // set properties
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setShowGrid(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    // connects
    connect(m_pController, SIGNAL(OnSetActiveConnectionsInformation(QStandardItemModel*)),
            this, SLOT(OnSetTableModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->refreshButton, SIGNAL(clicked()),
            m_pController, SLOT(OnActiveConnectionsOptClickedSlot()), Qt::QueuedConnection);
}

CActiveConnectionsWidget::~CActiveConnectionsWidget()
{
    delete ui;
    m_pController = 0;
}

void CActiveConnectionsWidget::OnSetTableModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->tableView->setModel(pModel);
    ui->tableView->resizeColumnsToContents();

    emit OnShowWidget(this);
}
