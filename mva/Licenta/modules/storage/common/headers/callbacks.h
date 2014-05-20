#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <Windows.h>
#include <devioctl.h>
#include <ntdddisk.h>
#include <commctrl.h>
#include <winioctl.h>
#include <WinBase.h>
#include <tchar.h>
#include <strsafe.h>

#include <QStringList>

#include "../../../../utils/headers/utils.h"
#include "../../smart/headers/smart_structs.h"
#include "../../smart/headers/smart_defines.h"

#define OUT_IDENTIFY_SIZE 264

QStringList GetPhysicalDrivesList();
QString GetDiskTotalSize(QString qzDrive);

// return the size of hDrive (partition, disk, etc..) in MB
int GetDriveLength(HANDLE hDrive);

// return ATA device properties
ATADeviceProperties *GetATADeviceProperties(const wchar_t *wszDriveName);

#endif // CALLBACKS_H
