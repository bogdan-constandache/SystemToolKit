#ifndef BATTERY_STATUS_WIDGET_H
#define BATTERY_STATUS_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CBatteryStatusWidget;
}

class CBatteryStatusWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CBatteryStatusWidget *ui;
    AbstractController *m_pController;

public:
    explicit CBatteryStatusWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CBatteryStatusWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnDataChangedSlot();

signals:
    void OnShowWidget(QWidget*);
};

#endif // BATTERY_STATUS_WIDGET_H
