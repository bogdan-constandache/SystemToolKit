#ifndef DMI_WIDGET_H
#define DMI_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CDMIWidget;
}

class CDMIWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CDMIWidget *ui;
    AbstractController *m_pController;

public:
    explicit CDMIWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CDMIWidget();

private slots:
    void OnSetItemsTreeModel(QStandardItemModel *pModel);
    void OnSetPropertiesTreeModel(QStandardItemModel *pModel);
    void OnItemsTreeClicked(QModelIndex index);
    void OnDataChangedSlot();

signals:
    void OnRequestPropertiesModel(DMIModuleType);
    void OnShowWidget(QWidget*);
};

#endif // SMART_WIDGET_H
