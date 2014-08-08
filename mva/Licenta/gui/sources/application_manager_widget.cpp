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
    ui->applicationTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->applicationTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->applicationTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->applicationTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->applicationTreeView->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->applicationTreeView->setRootIsDecorated(false);
    ui->applicationTreeView->setSortingEnabled(true);

    // connects
    connect(m_pController, SIGNAL(OnSetApplicationManagerInformation(QStandardItemModel*)),
            this, SLOT(OnSetTableModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->refreshButton, SIGNAL(clicked()),
            m_pController, SLOT(OnApplicationManagerOptClickedSlot()), Qt::QueuedConnection);
    connect(ui->uninstallButton, SIGNAL(clicked()),
            this, SLOT(OnUninstallApplicationSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnUninstallApplicationSignal(QString)),
            m_pController, SLOT(OnUninstallApplicationSlot(QString)), Qt::QueuedConnection);
}

CApplicationManagerWidget::~CApplicationManagerWidget()
{
    delete ui;
    m_pController = 0;
}

void CApplicationManagerWidget::OnSetTableModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->applicationTreeView->setModel(pModel);
    ui->applicationTreeView->resizeColumnToContents(0);

    emit OnShowWidget(this);
}

void CApplicationManagerWidget::OnUninstallApplicationSlot()
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->applicationTreeView->model());
    if( NULL == pModel )
        return;

    QStandardItem *pItem = pModel->itemFromIndex(ui->applicationTreeView->currentIndex());

    emit OnUninstallApplicationSignal(pItem->data().toString());
}
