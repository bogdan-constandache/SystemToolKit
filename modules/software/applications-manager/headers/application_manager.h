#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

// WinAPI includes
#include <Windows.h>
#include <WinReg.h>
#include <tchar.h>

// QT includes
#include <QString>
#include <QList>
#include <QSysInfo>

// My includes
#include "../../../../interfaces/module.h"
#include "../../../../utils/headers/utils.h"
#include "software_structures.h"

class CApplicationManager : public IModule
{
    Q_OBJECT
private:
    QList<PApplication> m_qApplicationList;

public:
    CApplicationManager();
    ~CApplicationManager();
    int FillApplicationList();
    QList<PApplication> GetApplicationList();
    virtual int OnCreateWidget(QWidget **ppWidget);

public slots:
    virtual void StartLoadingModuleSlot();

private:
    bool IsX64System();
    int FillX64Applications();
    int FillX86Applications();
};

#endif // APPLICATION_MANAGER_H
