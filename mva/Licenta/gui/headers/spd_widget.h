#ifndef SPD_WIDGET_H
#define SPD_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CSPDWidget;
}

class CSPDWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CSPDWidget *ui;
    AbstractController *m_pController;

public:
    explicit CSPDWidget(QWidget *pParent = 0, AbstractController *pController = 0);
    ~CSPDWidget();

private slots:
    void OnSetItemsTreeModel(QStandardItemModel *pModel);
    void OnSetDimmInfoTreeModel(QStandardItemModel *pModel);
    void OnItemsTreeClicked(QModelIndex index);
    void OnDataChangedSlot();
    void OnAvailableDimmsDataChangedSlot();

signals:
    void OnRequestDimmInformations(int);
    void OnShowWidget(QWidget*);
};

#endif // SPD_WIDGET_H
