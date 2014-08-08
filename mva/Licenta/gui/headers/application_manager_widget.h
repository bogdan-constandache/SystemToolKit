#ifndef APPLICATION_MANAGER_WIDGET_H
#define APPLICATION_MANAGER_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CApplicationManagerWidget;
}

class CApplicationManagerWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CApplicationManagerWidget *ui;
    AbstractController *m_pController;

public:
    explicit CApplicationManagerWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CApplicationManagerWidget();

public slots:
    void OnSetTableModel(QStandardItemModel *pModel);
    void OnUninstallApplicationSlot();

signals:
    void OnShowWidget(QWidget*);
    void OnUninstallApplicationSignal(QString);
};

#endif // APPLICATION_MANAGER_WIDGET_H
