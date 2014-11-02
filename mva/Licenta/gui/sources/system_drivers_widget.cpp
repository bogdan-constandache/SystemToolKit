#include "../headers/system_drivers_widget.h"
#include "ui_system_drivers_widget.h"

CSystemDriversWidget::CSystemDriversWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSystemDriversWidget),
    m_pController(NULL)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);

    connect(m_pController, SIGNAL(OnSetSystemDriversModelInformation(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnSystemDriversInformationDataChanged()),
            this, SLOT(OnDataChanged()), Qt::QueuedConnection);
    connect(ui->refreshButton, SIGNAL(clicked()),
            m_pController, SLOT(OnOperatingSystemDriversOptClickedSlot()), Qt::QueuedConnection);
}

CSystemDriversWidget::~CSystemDriversWidget()
{
    m_pController = NULL;

    delete ui;
}

void CSystemDriversWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeView->setModel(pModel);
    ui->treeView->resizeColumnToContents(0);
}

void CSystemDriversWidget::OnDataChanged()
{
    ui->treeView->resizeColumnToContents(0);

    emit OnShowWidget(this);
}


