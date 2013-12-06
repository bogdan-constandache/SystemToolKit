#ifndef CSMARTINFO_H
#define CSMARTINFO_H

//WinAPI includes
#include <Windows.h>
#include <devioctl.h>
#include <ntdddisk.h>
#include <commctrl.h>
#include <winioctl.h>

//QT includes
#include <QMap>
#include <QList>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

//My includes
#include "smart_defines.h"
#include "smart_structs.h"
#include "../../../../utils/headers/utils.h"
#include "../../../../utils/headers/sqlite3.h"
#include "../../common/headers/callbacks.h"
#include "../../../../interfaces/module.h"

class CSmartInfo : public IModule
{
    Q_OBJECT

private:
    QMap<BYTE, SmartDetails*> m_dbSmartDetails;
    QStringList m_PhysicalDrives;
    DriveInfo *m_data;
    QList<DriveInfo*> m_qInfomations;

    QTableView *m_pAvailableHDDTableView;
    QTableView *m_pHDDDetailsTableView;

    QStandardItemModel *m_pAvailableHDDModel;
    QStandardItemModel *m_pHDDDetailsModel;

    int ReadSMARTDetailsFromDB();
    bool CheckForCMDCommands(const wchar_t *tszDriveName);
    bool CheckForSmartFlag(const wchar_t* tszDriveName);
    DeviceData* GetATADriveData(const wchar_t* tszDriveName);
    int CollectSmartAttributes(const wchar_t* tszDriveName);
    QString ParseFirmware(PBYTE pbData);
    QString ParseModel(PBYTE pbData);
    QString ParseSerial(PBYTE pbData);
    void PopulateModel(DriveInfo *pDriveInfo);

public:
    CSmartInfo();
    ~CSmartInfo();
    int Initialize(int nDriveIndex);
    SmartDetails *GetSMARTDetails(short sAttribIndex);
    DeviceData *GetDriveInfo();
    QList<SmartData *> GetSmartData();
    QStringList GetAvailablePhysicalDrives();

    virtual int OnCreateWidget(QWidget **ppWidget);

public slots:
    virtual void OnStartLoadingModuleDataSlot();

private slots:
    void OnAvailableHDDTableItemClicked(QModelIndex);
};
#endif // CSMARTINFO_H
