#include "../headers/cpuid_widget.h"
#include "ui_cpuid_widget.h"

CCPUIDWidget::CCPUIDWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CCPUIDWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->treeView->setRootIsDecorated(false);

    connect(m_pController, SIGNAL(OnSetCPUIDInformations(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
}

CCPUIDWidget::~CCPUIDWidget()
{
    delete ui;
}

void CCPUIDWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);

    emit OnShowWidget(this);
}
