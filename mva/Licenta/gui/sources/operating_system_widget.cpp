#include "../headers/operating_system_widget.h"
#include "ui_operating_system_widget.h"

COperatingSystemWidget::COperatingSystemWidget(QWidget *pParent, AbstractController *pController) :
    QWidget(pParent),
    ui(new Ui::COperatingSystemWidget)
{
    ui->setupUi(this);
    m_pController = pController;

    // set properties
    ui->tvOperatingSystem->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvOperatingSystem->setRootIsDecorated(false);
    ui->tvOperatingSystem->header()->setStretchLastSection(true);
    ui->tvOperatingSystem->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->tvOperatingSystem->setFocusPolicy(Qt::NoFocus);

    connect(m_pController, SIGNAL(OnSetOperatingSystemInformation(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnOperatingSystemInformationDataChanged()),
            this, SLOT(OnDataChangedSlot()), Qt::QueuedConnection);
}

COperatingSystemWidget::~COperatingSystemWidget()
{
    delete ui;
}

void COperatingSystemWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if( pModel )
        ui->tvOperatingSystem->setModel(pModel);
    ui->tvOperatingSystem->setColumnWidth(0, 150);
    ui->tvOperatingSystem->expandAll();
}

void COperatingSystemWidget::OnDataChangedSlot()
{
    ui->tvOperatingSystem->setColumnWidth(0, 150);
    ui->tvOperatingSystem->expandAll();

    emit OnShowWidget(this);
}
