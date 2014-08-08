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
}

CUserInformationWidget::~CUserInformationWidget()
{
    delete ui;
}


void CUserInformationWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);

    emit OnShowWidget(this);
}
