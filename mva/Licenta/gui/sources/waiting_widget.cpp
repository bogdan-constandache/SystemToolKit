#include "../headers/waiting_widget.h"
#include "ui_waiting_widget.h"

QWaitingWidget::QWaitingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWaitingWidget)
{
    ui->setupUi(this);
    ui->labelText->setStyleSheet("color: #DDD");
}

QWaitingWidget::~QWaitingWidget()
{
    delete ui;
}
