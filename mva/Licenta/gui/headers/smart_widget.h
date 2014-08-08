#ifndef SMART_WIDGET_H
#define SMART_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CSmartWidget;
}

class CSmartWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CSmartWidget *ui;
    AbstractController *m_pController;

    QStandardItemModel *m_pPropertiesModel;

public:
    explicit CSmartWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CSmartWidget();

private slots:
    void OnSetHDDItemsTreeModel(QStandardItemModel*);
    void OnSetPropertiesTreeModel(QStandardItemModel *);
    void OnItemsTreeClicked(QModelIndex);

signals:
    void OnShowWidget(QWidget*);
    void OnRequestHDDInformations(QString);
};

#endif // SMART_WIDGET_H
