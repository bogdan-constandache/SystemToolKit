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
#include <QStandardItemModel>

//My includes
#include "smart_defines.h"
#include "smart_structs.h"
#include "../../../../utils/headers/utils.h"
#include "../../common/headers/callbacks.h"

class CSmartInfo
{
private:
    QMap<BYTE, SmartDetails*> m_dbSmartDetails;
    QMap<QString, QString> m_PhysicalDrivesToModel;
    QStringList m_PhysicalDrives;
    DriveInfo *m_data;

    bool m_bErrorFlag;

    int ReadSMARTDetailsFromDB();
    bool CheckForCMDCommands(const wchar_t *tszDriveName);
    bool CheckForSmartFlag(const wchar_t* tszDriveName);
    int CollectSmartAttributes(const wchar_t* tszDriveName);
    void PopulateModel(DriveInfo *pDriveInfo);
    int Initialize(int nDriveIndex);
    SmartDetails *GetSMARTDetailsFromDB(short sAttribIndex);
    QList<SmartData*> GetSmartData();

public:
    CSmartInfo();
    ~CSmartInfo();
    QStandardItemModel* GetAvailableHDD();
    QStandardItemModel* GetSMARTPropertiesForHDD(QString);

};
#endif // CSMARTINFO_H
