#include "../headers/startup_apps_widget.h"
#include "ui_startup_apps_widget.h"

CStartupAppsWidget::CStartupAppsWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CStartupAppsWidget)
{
    ui->setupUi(this);

    m_pController = pController;

    ui->treeWApps->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWApps->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWApps->setRootIsDecorated(false);
    ui->treeWApps->header()->setStretchLastSection(true);
    ui->treeWApps->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->treeWApps->setFocusPolicy(Qt::NoFocus);

    connect(m_pController, SIGNAL(OnSetStartupApplicationsInformations(QStandardItemModel*)),
            this, SLOT(OnSetTreeModel(QStandardItemModel*)), Qt::QueuedConnection);
    connect(m_pController, SIGNAL(OnStartupApplicationInformationDataChanged()),
            this, SLOT(OnApplicationsDataChangedSlot()), Qt::QueuedConnection);
    connect(ui->btnRefresh, SIGNAL(clicked()),
            m_pController, SLOT(OnSoftwareStartupApplicationsOptClickedSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnRemoveApplicationSignal(QString)),
            m_pController, SLOT(OnRemoveStartupApplicationSlot(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnChangeApplicationStateSignal(QString)),
            m_pController, SLOT(OnChangeStartupApplicationStateSlot(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(OnAddStartupApplicationSignal(QString,QString)),
            m_pController, SLOT(OnAddStartupApplicationSlot(QString,QString)), Qt::QueuedConnection);

    connect(ui->btnEnable, SIGNAL(clicked()),
            this, SLOT(OnEnableApplicationButtonClickedSlot()), Qt::QueuedConnection);
    connect(ui->btnRemove, SIGNAL(clicked()),
            this, SLOT(OnRemoveApplicationButtonClickedSlot()), Qt::QueuedConnection);
    connect(ui->btnAdd, SIGNAL(clicked()),
            this, SLOT(OnAddApplicationClickedSlot()), Qt::QueuedConnection);
    connect(ui->btnCancel, SIGNAL(clicked()),
            this, SLOT(OnCancelModificationsClickedSlot()), Qt::QueuedConnection);
    connect(ui->btnApply, SIGNAL(clicked()),
            this, SLOT(OnApplyModificationsClickedSlot()), Qt::QueuedConnection);
    connect(ui->btnOpenPath, SIGNAL(clicked()),
            this, SLOT(OnApplicationPathClickedSlot()), Qt::QueuedConnection);
    connect(ui->lineEditPath, SIGNAL(textChanged(QString)),
            this, SLOT(OnLineEditTextChangedSlot(QString)), Qt::QueuedConnection);
    connect(ui->lineEditAppName, SIGNAL(textChanged(QString)),
            this, SLOT(OnLineEditTextChangedSlot(QString)), Qt::QueuedConnection);

    QFont qFont1;
    qFont1.setItalic(true);
    ui->lineEditPath->setFont(qFont1);
    ui->lineEditAppName->setFont(qFont1);

    ui->frameAddApplication->hide();
    ui->line_2->hide();

    ui->lineEditPath->setDisabled(true);
    ui->lineEditPath->setPlaceholderText("Application path");
    ui->lineEditPath->setText("");

    ui->lineEditAppName->setPlaceholderText("Application name");
    ui->lineEditAppName->setText("");
}

CStartupAppsWidget::~CStartupAppsWidget()
{
    delete ui;
}

void CStartupAppsWidget::OnSetTreeModel(QStandardItemModel *pModel)
{
    if (pModel)
        ui->treeWApps->setModel(pModel);
}

void CStartupAppsWidget::OnApplicationsDataChangedSlot()
{
    ui->treeWApps->setColumnWidth(0, 200);
    ui->treeWApps->setColumnWidth(2, 320);

    ui->treeWApps->resizeColumnToContents(1);
    ui->treeWApps->resizeColumnToContents(3);
    ui->treeWApps->resizeColumnToContents(4);

    emit OnShowWidget(this);
}

void CStartupAppsWidget::OnRemoveApplicationButtonClickedSlot()
{
    OnCancelModificationsClickedSlot();

    if( !ui->treeWApps->currentIndex().isValid() )
        return;

    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->treeWApps->model());
    QStandardItem *pItem = pModel->item(ui->treeWApps->currentIndex().row(), 0);

    QVariant qvData = pItem->data();

    if( qvData.isValid() )
        emit OnRemoveApplicationSignal(qvData.toString());
}

void CStartupAppsWidget::OnEnableApplicationButtonClickedSlot()
{
    OnCancelModificationsClickedSlot();

    if( !ui->treeWApps->currentIndex().isValid() )
        return;

    QStandardItemModel *pModel = dynamic_cast<QStandardItemModel*>(ui->treeWApps->model());
    QStandardItem *pItem = pModel->item(ui->treeWApps->currentIndex().row(), 0);

    QVariant qvData = pItem->data();

    if( qvData.isValid() )
        emit OnChangeApplicationStateSignal(qvData.toString());
}

void CStartupAppsWidget::OnAddApplicationClickedSlot()
{
    ui->line_2->show();
    ui->frameAddApplication->show();
}

void CStartupAppsWidget::OnApplyModificationsClickedSlot()
{
    QString qsName = ui->lineEditAppName->text();
    QString qsPath = ui->lineEditPath->text();

    if( !qsName.isEmpty() && !qsPath.isEmpty() )
    {
        emit OnAddStartupApplicationSignal(qsName, qsPath);

        ui->line_2->hide();
        ui->frameAddApplication->hide();
    }
}

void CStartupAppsWidget::OnCancelModificationsClickedSlot()
{
    ui->line_2->hide();
    ui->frameAddApplication->hide();
}

void CStartupAppsWidget::OnApplicationPathClickedSlot()
{
    QString qsFileName = QFileDialog::getOpenFileName(this, "Open file", QString(), "Application file (*.exe)", 0, QFileDialog::ReadOnly);
    ui->lineEditPath->setText(qsFileName);
}

void CStartupAppsWidget::OnLineEditTextChangedSlot(QString qsText)
{
    QLineEdit *pLineEdit = (QLineEdit*)sender();
    QFont qFont;

    if( qsText.isEmpty() )
    {
        qFont.setItalic(true);
    }
    else
    {
        qFont.setItalic(false);
    }
    pLineEdit->setFont(qFont);
}
