#ifndef CONTROLLER_H
#define CONTROLLER_H

// WinAPI includes

// Qt includes
#include <QObject>
#include <QThread>

// My includes
#include "../../modules/api.h"
#include "../../interfaces/imain_window.h"

class CController : public QObject
{
    Q_OBJECT
private:
    IMainWindow *m_pMainWindow;
    QMap<ModuleType, QThread*> m_qThreadMap;

public:
    CController();
    virtual ~CController();
    void SetView(IMainWindow *pView);

private:
    IModule *CreateModule(ModuleType mType);

public slots:
    void StartControllerSlot(void);
    void DrawingMainWindowCompleteSlot(void);
    void CreateModuleSlot(int mType);
    void OnMainWindowDialogClosedSlot();

signals:
    void StartDrawingMainWindowSignal(void);
    void LoadingModulesCompleteSignal(void);
};

#endif // CONTROLLER_H
