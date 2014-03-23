#include "../headers/sensors_widget.h"
#include "ui_sensors_widget.h"

CSensorsWidget::CSensorsWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSensorsWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);

    connect(m_pController, SIGNAL(OnSetSensorsInformations(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
}

CSensorsWidget::~CSensorsWidget()
{
    delete ui;
}

void CSensorsWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);

    emit OnShowWidget(this);
}
