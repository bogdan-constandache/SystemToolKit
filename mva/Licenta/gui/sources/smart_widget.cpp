#include "../headers/smart_widget.h"
#include "ui_smart_widget.h"

CSmartWidget::CSmartWidget(QWidget *parent, AbstractController *pController) :
    QWidget(parent),
    ui(new Ui::CSmartWidget),
    m_pController(NULL)
{
    ui->setupUi(this);
    m_pController = pController;
}

CSmartWidget::~CSmartWidget()
{
    m_pController = NULL;
    delete ui;
}


