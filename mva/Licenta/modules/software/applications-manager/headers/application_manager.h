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
#include <QStandardItemModel>

// My includes
#include "../../../../utils/headers/utils.h"
#include "software_structures.h"

class CApplicationManager
{
private:
    QList<PApplication> m_qApplicationList;

public:
    CApplicationManager();
    ~CApplicationManager();
    QStandardItemModel* GetApplicationList();

private:
    bool IsX64System();
    int FillX64Applications();
    int FillX86Applications();
};

#endif // APPLICATION_MANAGER_H
