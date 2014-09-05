#include "../headers/battery_status_widget.h"
#include "ui_battery_status_widget.h"

CBatteryStatusWidget::CBatteryStatusWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CBatteryStatusWidget),
    m_pController(NULL)
{
    ui->setupUi(this);
    m_pController = pController;

    // set tree properties
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);

    // connects
    connect(m_pController, SIGNAL(OnSetPowerManagementInformation(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnPowerManagementInformationDataChanged()),
            this, SLOT(OnDataChangedSlot()), Qt::QueuedConnection);
}

CBatteryStatusWidget::~CBatteryStatusWidget()
{
    delete ui;
    m_pController = 0;
}

void CBatteryStatusWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
}

void CBatteryStatusWidget::OnDataChangedSlot()
{
    ui->treeView->setColumnWidth(0, 150);
}

