#include "../headers/sensors_widget.h"
#include "ui_sensors_widget.h"

CSensorsWidget::CSensorsWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSensorsWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    m_bIsFirstTime = true;

    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidget->header()->setStretchLastSection(true);
    ui->treeWidget->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->treeWidget->setColumnCount(2);

    connect(m_pController, SIGNAL(OnSetSensorsInformations(std::string)),
            this, SLOT(OnSetTreeModel(std::string)), Qt::QueuedConnection);
}

CSensorsWidget::~CSensorsWidget()
{
    delete ui;
}

void CSensorsWidget::OnSetTreeModel(std::string buffer)
{
    if (!m_bIsFirstTime)
    {
        OnUpdateTree(&buffer);
        return;
    }

    m_bIsFirstTime = false;

    ui->treeWidget->clear();

    SensorsData pSensorsData;
    MotherboardData pmbData;
    CpuData pCpuData;
    DataType pDataType;
    ItemPair pItemPair;

    QTreeWidgetItem *pRootItem = new QTreeWidgetItem(ui->treeWidget);
    QTreeWidgetItem *pItem = 0;

    if (!pSensorsData.ParseFromString(buffer))
        return;

    if (pSensorsData.has_mbdata())
        pmbData = pSensorsData.mbdata();

    if (pSensorsData.has_cpudata())
        pCpuData = pSensorsData.cpudata();

    pRootItem->setText(0, pmbData.name().c_str());
    ui->treeWidget->addTopLevelItem(pRootItem);

    for(int i = 0; i < pmbData.data_size(); i++)
    {
        pDataType = pmbData.data(i);

        pItem = OnAddChildItem(pRootItem, pDataType.dataname().c_str(), "");

        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            OnAddChildItem(pItem, pItemPair.name().c_str(), pItemPair.value().c_str());
        }
    }

    pRootItem = new QTreeWidgetItem(ui->treeWidget);
    pRootItem->setText(0, pCpuData.name().c_str());
    ui->treeWidget->addTopLevelItem(pRootItem);

    for(int i = 0; i < pCpuData.data_size(); i++)
    {
        pDataType = pCpuData.data(i);

        pItem = OnAddChildItem(pRootItem, pDataType.dataname().c_str(), "");

        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            OnAddChildItem(pItem, pItemPair.name().c_str(), pItemPair.value().c_str());
        }
    }

    emit OnShowWidget(this);
}

void CSensorsWidget::OnUpdateTree(std::string buffer)
{
    Q_UNUSED(buffer);
}

QTreeWidgetItem* CSensorsWidget::OnAddChildItem(QTreeWidgetItem *pParent, QString qzText1, QString qzText2)
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem;
    pItem->setText(0, qzText1);
    pItem->setText(1, qzText2);

    pParent->addChild(pItem);

    return pItem;
}
