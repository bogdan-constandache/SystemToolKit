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
public:
    QList<SystemDriverInfo*> m_qlSystemDrivers;

private:
    QString m_qszWinDir;

public:
    SystemDrivers();
    ~SystemDrivers();
    QStandardItemModel *GetSystemDriversInformation();

private:
    int Initialize();
    int Destroy();
    QString GetDriverVersion(WCHAR* wszFileName);
    void PopulateModel();
};

#endif
