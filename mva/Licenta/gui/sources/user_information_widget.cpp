#include "../headers/user_information_widget.h"
#include "ui_user_information_widget.h"

CUserInformationWidget::CUserInformationWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CUserInformationWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeView->setSelectionMode(QTreeView::SingleSelection);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setSelectionBehavior(QTreeView::SelectRows);
    ui->treeView->setRootIsDecorated(false);

    connect(m_pController, SIGNAL(OnSetUsersInformations(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnUserInformationDataChanged()),
            this, SLOT(OnUserInformationDataChangedSlot()), Qt::QueuedConnection);
}

CUserInformationWidget::~CUserInformationWidget()
{
    delete ui;
}

void CUserInformationWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->setColumnWidth(0, 175);
}

void CUserInformationWidget::OnUserInformationDataChangedSlot()
{
    ui->treeView->setColumnWidth(0, 175);

    emit OnShowWidget(this);
}
