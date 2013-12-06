#ifndef CSMARTINFO_H
#define CSMARTINFO_H

//My includes
#include "smart_defines.h"
#include "smart_structs.h"
#include "utils/utils.h"
#include "utils/sqlite3.h"
#include "callbacks.h"

//WinAPI includes
#include <devioctl.h>
#include <ntdddisk.h>
#include <commctrl.h>
#include <winioctl.h>

//QT includes
#include <QMap>
#include <QList>
#include <QDebug>


class CSmartInfo
{
public:
    CSmartInfo();
    ~CSmartInfo();
    int Initialize(int nDriveIndex);
    SmartDetails *GetSMARTDetails(short sAttribIndex);
    DeviceData *GetDriveInfo();
    QList<SmartData *> GetSmartData();
    QStringList GetAvailablePhysicalDrives();

private:
    int ReadSMARTDetailsFromDB();
    static int SQLiteCallback(void *NotUsed, int argc, char **argv, char **azColName);
    bool CheckForCMDCommands(const wchar_t *tszDriveName);
    bool CheckForSmartFlag(const wchar_t* tszDriveName);
    DeviceData* GetATADriveData(const wchar_t* tszDriveName);
    int CollectSmartAttributes(const wchar_t* tszDriveName);
    QString ParseFirmware(PBYTE pbData);
    QString ParseModel(PBYTE pbData);
    QString ParseSerial(PBYTE pbData);

    static QMap<BYTE, SmartDetails*> *m_dbSmartDetails;
    QStringList m_PhysicalDrives;
    DriveInfo *m_data;
};
#endif // CSMARTINFO_H
