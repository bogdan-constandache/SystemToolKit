#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <Windows.h>
#include <devioctl.h>
#include <ntdddisk.h>
#include <commctrl.h>
#include <winioctl.h>
#include <WinBase.h>
#include <atlstr.h>
#include <tchar.h>
#include <stdio.h>

#include <QStringList>

#include "../utils/utils.h"

QStringList GetPhysicalDrivesList();

#endif // CALLBACKS_H
