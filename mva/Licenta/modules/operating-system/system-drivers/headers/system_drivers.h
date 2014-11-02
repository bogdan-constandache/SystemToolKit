#ifndef SYSTEM_DRIVERS_H
#define SYSTEM_DRIVERS_H

// WinAPI includes
#include <Windows.h>
#include <winsvc.h>
#include <winver.h>

// QT includes
#include <QList>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>

// My includes
#include "../../../../utils/headers/utils.h"

typedef struct _SYSTEM_DRIVER_INFO
{
    QString qszDriverName;
    QString qszDescription;
    QString qszFileName;
    QString qszVersion;
    QString qszType;
    QString qszState;
}SystemDriverInfo;

class SystemDrivers
{
private:
    QList<SystemDriverInfo*>    m_qlSystemDrivers;
    QString                     m_qszWinDir;
    QStandardItemModel          *m_pDataModel;

public:
    SystemDrivers();
    ~SystemDrivers();
    QStandardItemModel *GetSystemDriversInformation();
    void OnRefreshData();

private:
    int Initialize();
    QString GetDriverVersion(WCHAR* wszFileName);
};

#endif
