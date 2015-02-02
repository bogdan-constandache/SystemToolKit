#include "../headers/processes_widget.h"
#include "ui_processes_widget.h"

CProcessesWidget::CProcessesWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CProcessesWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeWProcesses->setSelectionMode(QTreeView::SingleSelection);
    ui->treeWProcesses->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWProcesses->setSelectionBehavior(QTreeView::SelectRows);
    ui->treeWProcesses->setRootIsDecorated(false);
    ui->treeWProcesses->setFocusPolicy(Qt::NoFocus);

    ui->treeWDlls->setSelectionMode(QTreeView::SingleSelection);
    ui->treeWDlls->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWDlls->setSelectionBehavior(QTreeView::SelectRows);
    ui->treeWDlls->setRootIsDecorated(false);
    ui->treeWDlls->setFocusPolicy(Qt::NoFocus);

    connect(m_pController, SIGNAL(OnSetProcessesInformations(QStandardItemModel*)),
            this, SLOT(OnSetProcessTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestModuleInformationSignal(int)),
            m_pController, SLOT(OnRequestModulesInformationsSlot(int)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetModulesInformations(QStandardItemModel*)),
            this, SLOT(OnSetModuleTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->treeWProcesses, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnProcessNamesItemClickedSlot(QModelIndex)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnProcessInformationDataChanged()),
            this, SLOT(OnProcessesDataChanged()), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnProcessModuleInformationDataChanged()),
            this, SLOT(OnModulesDataChanged()), Qt::QueuedConnection);
}

CProcessesWidget::~CProcessesWidget()
{
    delete ui;
}

void CProcessesWidget::OnProcessNamesItemClickedSlot(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->treeWProcesses->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);
    QVariant qVar = pItem->data();

    if( qVar.isValid() )
        emit OnRequestModuleInformationSignal(qVar.toInt());
}

void CProcessesWidget::OnSetProcessTreeModel(QStandardItemModel *pModel)
{
    ui->treeWProcesses->setModel(pModel);
}

void CProcessesWidget::OnSetModuleTreeModel(QStandardItemModel *pModel)
{
    ui->treeWDlls->setModel(pModel);
}

void CProcessesWidget::OnProcessesDataChanged()
{
    ui->treeWProcesses->setColumnWidth(0, 150);
    ui->treeWProcesses->setColumnWidth(1, 225);

    emit OnShowWidget(this);
}

void CProcessesWidget::OnModulesDataChanged()
{
    ui->treeWDlls->setColumnWidth(0, 150);
    ui->treeWDlls->setColumnWidth(1, 400);
}
