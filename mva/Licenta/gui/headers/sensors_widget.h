#ifndef SENSORS_WIDGET_H
#define SENSORS_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CSensorsWidget;
}

class CSensorsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CSensorsWidget *ui;
    AbstractController *m_pController;

public:
    explicit CSensorsWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CSensorsWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);

signals:
    void OnShowWidget(QWidget*);
};

#endif // SENSORS_WIDGET_H
