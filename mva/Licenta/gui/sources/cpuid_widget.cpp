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
    ui->treeView->setFocusPolicy(Qt::NoFocus);

    connect(m_pController, SIGNAL(OnSetCPUIDInformations(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnCPUIDInformationDataChanged()),
            this, SLOT(OnCPUIDDataChanged()), Qt::QueuedConnection);
}

CCPUIDWidget::~CCPUIDWidget()
{
    delete ui;
}

void CCPUIDWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
}

void CCPUIDWidget::OnCPUIDDataChanged()
{
    ui->treeView->setColumnWidth(0, 200);
    emit OnShowWidget(this);
}
