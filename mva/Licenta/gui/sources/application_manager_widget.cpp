#include "../headers/application_manager_widget.h"
#include "ui_application_manager_widget.h"

CApplicationManagerWidget::CApplicationManagerWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CApplicationManagerWidget),
    m_pController(NULL)
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
    connect(m_pController, SIGNAL(OnSetApplicationManagerInformation(QStandardItemModel*)),
            this, SLOT(OnSetTableModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->refreshButton, SIGNAL(clicked()),
            m_pController, SLOT(OnApplicationManagerOptClickedSlot()), Qt::QueuedConnection);
    connect(ui->uninstallButton, SIGNAL(clicked()),
            m_pController, SLOT(OnUninstallApplicationSlot()), Qt::QueuedConnection);
}

CApplicationManagerWidget::~CApplicationManagerWidget()
{
    delete ui;
    m_pController = 0;
}

void CApplicationManagerWidget::OnSetTableModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->tableView->setModel(pModel);
    ui->tableView->resizeColumnsToContents();

    emit OnShowWidget(this);
}
