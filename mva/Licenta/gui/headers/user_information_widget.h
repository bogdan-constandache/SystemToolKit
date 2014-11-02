#ifndef USER_INFORMATION_WIDGET_H
#define USER_INFORMATION_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CUserInformationWidget;
}

class CUserInformationWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CUserInformationWidget *ui;
    AbstractController *m_pController;

public:
    explicit CUserInformationWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CUserInformationWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnUserInformationDataChangedSlot();

signals:
    void OnShowWidget(QWidget*);
};

#endif // USER_INFORMATION_WIDGET_H
