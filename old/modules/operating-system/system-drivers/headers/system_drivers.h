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

// My includes
#include "../../../../interfaces/module.h"
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

class SystemDrivers : public IModule
{
    Q_OBJECT

public:
    QList<SystemDriverInfo*> m_qlSystemDrivers;

    QTableView *m_pTableView;

    QStandardItemModel *m_pTableModel;

private:
    QString m_qszWinDir;

public:
    SystemDrivers();
    int Initialize();
    int Destroy();
    virtual int OnCreateWidget(QWidget **ppWidget);

public slots:
    virtual void OnStartLoadingModuleDataSlot();

private:
    QString GetDriverVersion(WCHAR* wszFileName);
    void PopulateModel();
};

#endif
