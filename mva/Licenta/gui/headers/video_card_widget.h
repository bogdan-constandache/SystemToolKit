#ifndef VIDEO_CARD_WIDGET_H
#define VIDEO_CARD_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CVideoCardWidget;
}

class CVideoCardWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CVideoCardWidget *ui;
    AbstractController *m_pController;

public:
    explicit CVideoCardWidget(QWidget *pParent = 0, AbstractController *pController = 0);
    ~CVideoCardWidget();

private slots:
    void OnSetCardsTreeModel(QStandardItemModel *pModel);
    void OnSetCardInfoTreeModel(QStandardItemModel *pModel);
    void OnItemsTreeClicked(QModelIndex index);
    void OnDataChangedSlot();
    void OnAvailableVCardsDataChangedSlot();

signals:
    void OnRequestCardInformations(int);
    void OnShowWidget(QWidget*);
};

#endif // VIDEO_CARD_WIDGET_H
