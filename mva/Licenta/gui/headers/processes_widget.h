#ifndef PROCESSES_WIDGET_H
#define PROCESSES_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CProcessesWidget;
}

class CProcessesWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CProcessesWidget *ui;
    AbstractController *m_pController;

public:
    explicit CProcessesWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CProcessesWidget();

private slots:
    void OnProcessNamesItemClickedSlot(QModelIndex Index);
    void OnSetProcessTreeModel(QStandardItemModel *pModel);
    void OnSetModuleTreeModel(QStandardItemModel *pModel);

signals:
    void OnShowWidget(QWidget*);
    void OnRequestModuleInformationSignal(int);
};

#endif // PROCESSES_WIDGET_H
