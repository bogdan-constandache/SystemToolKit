#ifndef CPUID_WIDGET_H
#define CPUID_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CCPUIDWidget;
}

class CCPUIDWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CCPUIDWidget *ui;
    AbstractController *m_pController;

public:
    explicit CCPUIDWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CCPUIDWidget();

public slots:
    void OnSetTreeModel(QStandardItemModel *pModel);
    void OnCPUIDDataChanged();

signals:
    void OnShowWidget(QWidget*);
};

#endif // CPUID_WIDGET_H
