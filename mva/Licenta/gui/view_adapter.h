#ifndef VIEW_ADAPTER_H
#define VIEW_ADAPTER_H

#include <QObject>
#include "abstract_controller.h"
#include "headers/main_window.h"

class ViewAdapter : public QObject
{
    Q_OBJECT

private:
    AbstractController      *m_pController;
    MainWindow              *m_pMainWindow;

public:
    explicit ViewAdapter(QObject *parent = 0);
    void SetController(AbstractController *pController);
    void Initialize();
    void Destroy();

signals:
    void OnInitializeSignal();
    void OnDestroySignal();

public slots:
    void OnInitializeSlot();
    void OnDestroySlot();
    void OnCreateMainWindowSlot();
    void OnShowMainWindowSlot();

};

#endif // VIEW_ADAPTER_H
