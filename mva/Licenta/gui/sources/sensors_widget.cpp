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
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);

    ui->treeWidget->setHeaderLabels(QStringList() << "Sensor" << "Value");

    connect(m_pController, SIGNAL(OnSetSensorsInformations(std::string)),
            this, SLOT(OnSetTreeModel(std::string)), Qt::QueuedConnection);
}

CSensorsWidget::~CSensorsWidget()
{
    delete ui;
}

void CSensorsWidget::ExpandTreeAndResizeColumns()
{
    ui->treeWidget->expandAll();
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
}

void CSensorsWidget::OnSetTreeModel(std::string buffer)
{
    if (!m_bIsFirstTime)
    {
        OnUpdateTree(buffer);
        return;
    }

    m_bIsFirstTime = false;

    ui->treeWidget->clear();

    SensorsData pSensorsData;
    MotherboardData pmbData;
    CpuData pCpuData;
    RAMData pRamData;
    GPUData pGpuData;
    DataType pDataType;
    ItemPair pItemPair;

    QTreeWidgetItem *pRootItem = 0;
    QTreeWidgetItem *pItem = 0;

    if (!pSensorsData.ParseFromString(buffer))
        return;

    if (pSensorsData.has_mbdata())
        pmbData = pSensorsData.mbdata();
    else
        goto CPU_LABEL;

    pRootItem = new QTreeWidgetItem(ui->treeWidget);
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

CPU_LABEL:

    if (pSensorsData.has_cpudata())
        pCpuData = pSensorsData.cpudata();
    else
        goto RAM_LABEL;

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

RAM_LABEL:
    if (pSensorsData.has_ramdata())
        pRamData = pSensorsData.ramdata();
    else
        goto GPU_LABEL;

    pRootItem = new QTreeWidgetItem(ui->treeWidget);
    pRootItem->setText(0, pRamData.name().c_str());
    ui->treeWidget->addTopLevelItem(pRootItem);

    for(int i = 0; i < pRamData.data_size(); i++)
    {
        pDataType = pRamData.data(i);

        pItem = OnAddChildItem(pRootItem, pDataType.dataname().c_str(), "");
        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            OnAddChildItem(pItem, pItemPair.name().c_str(), pItemPair.value().c_str());
        }
    }

GPU_LABEL:
    if (pSensorsData.has_gpudata())
        pGpuData = pSensorsData.gpudata();
    else
        goto EXIT;

    pRootItem = new QTreeWidgetItem(ui->treeWidget);
    pRootItem->setText(0, pGpuData.name().c_str());
    ui->treeWidget->addTopLevelItem(pRootItem);

    for(int i = 0; i < pGpuData.data_size(); i++)
    {
        pDataType = pGpuData.data(i);

        pItem = OnAddChildItem(pRootItem, pDataType.dataname().c_str(), "");
        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            OnAddChildItem(pItem, pItemPair.name().c_str(), pItemPair.value().c_str());
        }
    }

EXIT:
    ExpandTreeAndResizeColumns();
    emit OnShowWidget(this);
}

void CSensorsWidget::OnUpdateTree(std::string buffer)
{
    SensorsData pSensorsData;
    MotherboardData pmbData;
    CpuData pCpuData;
    RAMData pRamData;
    GPUData pGpuData;
    DataType pDataType;
    ItemPair pItemPair;

    int nCount = 0;

    QTreeWidgetItem *pRootItem = ui->treeWidget->topLevelItem(nCount);
    QTreeWidgetItem *pItem = 0, *pItem2 = 0;

    if (!pSensorsData.ParseFromString(buffer))
        return;

    if (pSensorsData.has_mbdata())
        pmbData = pSensorsData.mbdata();
    else
        goto CPU_UPDATE;

    for(int i = 0; i < pmbData.data_size(); i++)
    {
        pDataType = pmbData.data(i);

        pItem = pRootItem->child(i);

        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            if (pItem->text(0) == QString(pDataType.dataname().c_str()))
            {
                pItem2 = pItem->child(j);

                if (pItem2->text(0) == QString(pItemPair.name().c_str()))
                {
                    pItem2->setText(1, pItemPair.value().c_str());
                }
            }
        }
    }
    nCount++;

CPU_UPDATE:
    pRootItem = ui->treeWidget->topLevelItem(nCount);

    if (pSensorsData.has_cpudata())
        pCpuData = pSensorsData.cpudata();
    else
        goto RAM_UPDATE;

    for(int i = 0; i < pCpuData.data_size(); i++)
    {
        pDataType = pCpuData.data(i);

        pItem = pRootItem->child(i);

        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            if (pItem->text(0) == QString(pDataType.dataname().c_str()))
            {
                pItem2 = pItem->child(j);

                if (pItem2->text(0) == QString(pItemPair.name().c_str()))
                {
                    pItem2->setText(1, pItemPair.value().c_str());
                }
            }
        }
    }
    nCount++;

RAM_UPDATE:
    pRootItem = ui->treeWidget->topLevelItem(nCount);

    if (pSensorsData.has_ramdata())
        pRamData = pSensorsData.ramdata();
    else
        goto GPU_UPDATE;

    for(int i = 0; i < pRamData.data_size(); i++)
    {
        pDataType = pRamData.data(i);

        pItem = pRootItem->child(i);

        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            if( pItem->text(0) == QString(pDataType.dataname().c_str()))
            {
                pItem2 = pItem->child(j);

                if( pItem2->text(0) == QString(pItemPair.name().c_str()))
                    pItem2->setText(1, pItemPair.value().c_str());
            }
        }
    }
    nCount++;

GPU_UPDATE:
    pRootItem = ui->treeWidget->topLevelItem(nCount);

    if (pSensorsData.has_gpudata())
        pGpuData = pSensorsData.gpudata();
    else
        goto EXIT;

    for(int i = 0; i < pGpuData.data_size(); i++)
    {
        pDataType = pGpuData.data(i);

        pItem = pRootItem->child(i);

        for(int j = 0; j < pDataType.datavalue_size(); j++)
        {
            pItemPair = pDataType.datavalue(j);

            if( pItem->text(0) == QString(pDataType.dataname().c_str()))
            {
                pItem2 = pItem->child(j);

                if( pItem2->text(0) == QString(pItemPair.name().c_str()))
                    pItem2->setText(1, pItemPair.value().c_str());
            }
        }
    }
    nCount++;

EXIT:;
//    ExpandTreeAndResizeColumns();
    emit OnShowWidget(this);
}

QTreeWidgetItem* CSensorsWidget::OnAddChildItem(QTreeWidgetItem *pParent, QString qzText1, QString qzText2)
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem;

    if( qzText1.contains("Temperature #") || qzText1.contains("Core #") )
    {
        pItem->setIcon(0, QIcon(":/img/thermo.png"));
        pParent->setIcon(0, QIcon(":/img/thermo.png"));
    }

    if( qzText2.endsWith("V") )
    {
        pItem->setIcon(0, QIcon(":/img/voltage.png"));
        pParent->setIcon(0, QIcon(":/img/voltage.png"));
    }

    if( qzText2. endsWith(" MB") )
    {
        pItem->setIcon(0, QIcon(":/img/memory.png"));
        pParent->setIcon(0, QIcon(":/img/memory.png"));
    }

    pItem->setText(0, qzText1);
    pItem->setText(1, qzText2);

    pParent->addChild(pItem);

    return pItem;
}
