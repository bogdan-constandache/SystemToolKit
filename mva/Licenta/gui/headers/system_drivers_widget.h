#ifndef SYSTEM_DRIVERS_WIDGET_H
#define SYSTEM_DRIVERS_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CSystemDriversWidget;
}

class CSystemDriversWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CSystemDriversWidget *ui;
    AbstractController *m_pController;

public:
    explicit CSystemDriversWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CSystemDriversWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnDataChanged();

signals:
    void OnShowWidget(QWidget*);
};

#endif // SYSTEM_DRIVERS_WIDGET_H
