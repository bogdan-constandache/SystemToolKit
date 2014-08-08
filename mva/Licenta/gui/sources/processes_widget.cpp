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

    connect(m_pController, SIGNAL(OnSetProcessesInformations(QStandardItemModel*)),
            this, SLOT(OnSetProcessTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRequestModuleInformationSignal(int)),
            m_pController, SLOT(OnRequestModulesInformationsSlot(int)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSetModulesInformations(QStandardItemModel*)),
            this, SLOT(OnSetModuleTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(ui->treeWProcesses, SIGNAL(clicked(QModelIndex)),
            this, SLOT(OnProcessNamesItemClickedSlot(QModelIndex)), Qt::QueuedConnection);
}

CProcessesWidget::~CProcessesWidget()
{
    delete ui;
}

void CProcessesWidget::OnProcessNamesItemClickedSlot(QModelIndex Index)
{
    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->treeWProcesses->model());
    QStandardItem *pItem = pModel->itemFromIndex(Index);
    QString qzItemText = pItem->statusTip();

    emit OnRequestModuleInformationSignal(qzItemText.toInt());
}

void CProcessesWidget::OnSetProcessTreeModel(QStandardItemModel *pModel)
{
    ui->treeWProcesses->setModel(pModel);

    emit OnShowWidget(this);
}

void CProcessesWidget::OnSetModuleTreeModel(QStandardItemModel *pModel)
{
    ui->treeWDlls->setModel(pModel);
}
