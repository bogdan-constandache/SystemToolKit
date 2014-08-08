#include "../headers/startup_apps_widget.h"
#include "ui_startup_apps_widget.h"

CStartupAppsWidget::CStartupAppsWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CStartupAppsWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeWApps->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWApps->setRootIsDecorated(false);
    ui->treeWApps->header()->setStretchLastSection(true);
    ui->treeWApps->header()->setDefaultAlignment(Qt::AlignLeft);

    connect(m_pController, SIGNAL(OnSetStartupApplicationsInformations(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
}

CStartupAppsWidget::~CStartupAppsWidget()
{
    delete ui;
}

void CStartupAppsWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeWApps->setModel(pModel);
    ui->treeWApps->resizeColumnToContents(0);
    ui->treeWApps->resizeColumnToContents(1);
    ui->treeWApps->resizeColumnToContents(2);

    emit OnShowWidget(this);
}
