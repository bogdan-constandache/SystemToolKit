#ifndef DEVICE_MANAGER_WIDGET_H
#define DEVICE_MANAGER_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CDeviceManagerWidget;
}

class CDeviceManagerWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CDeviceManagerWidget *ui;
    AbstractController *m_pController;

public:
    explicit CDeviceManagerWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CDeviceManagerWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);

signals:
    void OnShowWidget(QWidget*);
};

#endif // DEVICE_MANAGER_WIDGET_H
