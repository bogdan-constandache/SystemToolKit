#ifndef STARTUP_APPS_WIDGET_H
#define STARTUP_APPS_WIDGET_H

#include <QWidget>

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

signals:
    void OnShowWidget(QWidget*);
};

#endif // STARTUP_APPS_WIDGET_H
