#ifndef STARTUP_APPS_WIDGET_H
#define STARTUP_APPS_WIDGET_H

#include <QWidget>
#include <QFileDialog>

#include "../abstract_controller.h"

namespace Ui {
class CStartupAppsWidget;
}

class CStartupAppsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CStartupAppsWidget *ui;
    AbstractController *m_pController;

public:
    explicit CStartupAppsWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CStartupAppsWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnApplicationsDataChangedSlot();
    void OnRemoveApplicationButtonClickedSlot();
    void OnEnableApplicationButtonClickedSlot();
    void OnAddApplicationClickedSlot();
    void OnApplyModificationsClickedSlot();
    void OnCancelModificationsClickedSlot();
    void OnApplicationPathClickedSlot();

    void OnLineEditTextChangedSlot(QString qsText);

signals:
    void OnShowWidget(QWidget*);
    void OnRemoveApplicationSignal(QString);
    void OnChangeApplicationStateSignal(QString);
    void OnAddStartupApplicationSignal(QString, QString);
};

#endif // STARTUP_APPS_WIDGET_H
