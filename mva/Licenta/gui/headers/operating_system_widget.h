#ifndef OPERATING_SYSTEM_WIDGET_H
#define OPERATING_SYSTEM_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class COperatingSystemWidget;
}

class COperatingSystemWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::COperatingSystemWidget *ui;
    AbstractController *m_pController;

public:
    explicit COperatingSystemWidget(QWidget *pParent = 0, AbstractController *pController = 0);
    ~COperatingSystemWidget();

private slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnDataChangedSlot();

signals:
    void OnShowWidget(QWidget*);
};

#endif // OPERATING_SYSTEM_WIDGET_H
