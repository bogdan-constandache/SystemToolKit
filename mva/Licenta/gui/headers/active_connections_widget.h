#ifndef ACTIVE_CONNECTIONS_WIDGET_H
#define ACTIVE_CONNECTIONS_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CActiveConnectionsWidget;
}

class CActiveConnectionsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CActiveConnectionsWidget *ui;
    AbstractController *m_pController;

public:
    explicit CActiveConnectionsWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CActiveConnectionsWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnActiveConnectionDataChangedSlot();

signals:
    void OnShowWidget(QWidget*);
};

#endif // ACTIVE_CONNECTIONS_WIDGET_H
