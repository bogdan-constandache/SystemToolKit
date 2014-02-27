#include "../headers/system_drivers_widget.h"
#include "ui_system_drivers_widget.h"

CSystemDriversWidget::CSystemDriversWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSystemDriversWidget),
    m_pController(NULL)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setShowGrid(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    connect(m_pController, SIGNAL(OnSetSystemDriversModelInformation(QStandardItemModel*)),
            this, SLOT(OnSetTableModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->refreshButton, SIGNAL(clicked()),
            m_pController, SLOT(OnSystemDriversOptClickedSlot()), Qt::QueuedConnection);
}

CSystemDriversWidget::~CSystemDriversWidget()
{
    m_pController = NULL;

    delete ui;
}

void CSystemDriversWidget::OnSetTableModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->tableView->setModel(pModel);
    ui->tableView->resizeColumnsToContents();

    emit OnShowWidget(this);
}


