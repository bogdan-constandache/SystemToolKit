#ifndef ATA_WIDGET_H
#define ATA_WIDGET_H

#include <QWidget>
#include "../abstract_controller.h"

namespace Ui {
class CATAWidget;
}

class CATAWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CATAWidget *ui;
    AbstractController *m_pController;

    QStandardItemModel *m_pPropertiesModel;

public:
    explicit CATAWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CATAWidget();

private slots:
    void OnSetHddItemsTreeModel(QStandardItemModel*);
    void OnSetPropertiesTreeModel(QStandardItemModel*);
    void OnHddTreeItemClicked(QModelIndex);

signals:
    void OnShowWidget(QWidget*);
    void OnRequestHddProperties(QString);
};

#endif // ATA_WIDGET_H
