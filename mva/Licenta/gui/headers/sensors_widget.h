#ifndef SENSORS_WIDGET_H
#define SENSORS_WIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "../abstract_controller.h"
#include "../../proto-buffers/sensors_data.pb.h"

namespace Ui {
class CSensorsWidget;
}

class CSensorsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CSensorsWidget *ui;
    AbstractController *m_pController;
    bool m_bIsFirstTime;

public:
    explicit CSensorsWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CSensorsWidget();

private:
    void ExpandTreeAndResizeColumns();

public slots:
    void OnSetTreeModel(std::string buffer);
    void OnUpdateTree(std::string buffer);
    QTreeWidgetItem* OnAddChildItem(QTreeWidgetItem *, QString, QString);

signals:
    void OnShowWidget(QWidget*);
};

#endif // SENSORS_WIDGET_H
