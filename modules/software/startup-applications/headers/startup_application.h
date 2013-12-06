#ifndef STARTUP_APPLICATION_H
#define STARTUP_APPLICATION_H

#include <Windows.h>
#include <WinReg.h>
#include <tchar.h>
#include <objidl.h>
#include <shlobj.h>

#include <QStringList>
#include <QList>
#include <QDebug>
#include <QDir>
#include <QSysInfo>

#include "../../../../interfaces/module.h"
#include "../../../../utils/headers/utils.h"
#include "../../applications-manager/headers/software_structures.h"

class CStartupApplication : public IModule
{
    Q_OBJECT
private:
    QList<PStartupApplication> m_data;
public:
    CStartupApplication();
    ~CStartupApplication();
    int FillApplicationList();
    QList<PStartupApplication> GetApplicationList();
    virtual int OnCreateWidget(QWidget **ppWidget);

public slots:
    virtual void StartLoadingModuleSlot();

private:
    bool IsX64System();
    int FillX64ApplicationList(HKEY key);
    int FillX86ApplicationList(HKEY key);
    int FillAppDataApplicationList();
    int FillCurrentUserApplicationList();
    QString GetTargetOfShortcut(QString qzShortcutPath);
};

#endif // STARTUP_APPLICATION_H
