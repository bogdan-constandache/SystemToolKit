#include "../headers/device_manager_widget.h"
#include "ui_device_manager_widget.h"

CDeviceManagerWidget::CDeviceManagerWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CDeviceManagerWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    // set tree properties
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);

    connect(m_pController, SIGNAL(OnSetDeviceManagerInformation(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
}

CDeviceManagerWidget::~CDeviceManagerWidget()
{
    delete ui;
    m_pController = 0;
}

void CDeviceManagerWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->sortByColumn(0, Qt::AscendingOrder);

    emit OnShowWidget(this);
}
