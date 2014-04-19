#ifndef STARTUP_APPLICATION_H
#define STARTUP_APPLICATION_H

#include <Windows.h>
#include <WinReg.h>
#include <tchar.h>
#include <objidl.h>
#include <shlobj.h>

#include <QStringList>
#include <QList>
#include <QDir>
#include <QSysInfo>
#include <QStandardItemModel>

#include "../../../../utils/headers/utils.h"
#include "../../applications-manager/headers/software_structures.h"

class CStartupApplication
{
private:
    QList<PStartupApplication> m_qApplicationList;

public:
    CStartupApplication();
    ~CStartupApplication();
    QStandardItemModel *GetStartupApplicationsInformations();

private:
    bool IsX64System();
    int FillX64ApplicationList(HKEY key);
    int FillX86ApplicationList(HKEY key);
    int FillApplicationList(UserScope eScope);
    QString GetTargetOfShortcut(QString qzShortcutPath);
};

#endif // STARTUP_APPLICATION_H
