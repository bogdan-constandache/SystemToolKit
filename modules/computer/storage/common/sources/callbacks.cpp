#include "../../include/smart/callbacks.h"

QStringList GetPhysicalDrivesList()
{
    QStringList retValue;
    DWORD dwDrivesBitMask = 0;
    DWORD dwDriveVal = 2;
    char chDriveCount = 1; // we skip drive A: (floppy) in case GetLogicalDrives() is successful
    char chIsDrive = 0;
    VOLUME_DISK_EXTENTS *pVolumeDiskExtends = 0;
    HANDLE hDevice = NULL;
    TCHAR tszDrivePath[32];
    memset(tszDrivePath, '\0', sizeof(tszDrivePath));

    // get available drive letters
    dwDrivesBitMask = GetLogicalDrives();
    if( 0 == dwDrivesBitMask )
    {
        DEBUG_STATUS(Unsuccessful);
    }

    // check each available drive letter in case is up
    while( chDriveCount < 32 )
    {
        // check if current bit != 0
        chIsDrive = dwDrivesBitMask & dwDriveVal;

        if( 0 != chIsDrive )
        {
            // if so then we have current letter available for process
            // prepare path for CreateFile function
            _stprintf_s(tszDrivePath, 32, _T("\\\\.\\%c:"), _T('A' + chDriveCount));

            TCHAR tszDriveType[10];
            memset(tszDriveType, '\0', sizeof(tszDriveType));
            _stprintf_s(tszDriveType, 10, _T("%c:\\"), _T('A'+chDriveCount));


            // check if current drive that we found is a fixed one
            // otherwise we skip him
            if( DRIVE_FIXED != GetDriveType(tszDriveType) )
            {
                ++chDriveCount;
                dwDriveVal *= 2;
                continue;
            }
            // open handle to the current drive using letter path
            hDevice = CreateFile( tszDrivePath
                                  , GENERIC_READ|GENERIC_WRITE
                                  , FILE_SHARE_WRITE|FILE_SHARE_READ
                                  , NULL
                                  , OPEN_EXISTING
                                  , FILE_ATTRIBUTE_NORMAL
                                  , NULL );

            if( INVALID_HANDLE_VALUE == hDevice )
            {
                DEBUG_STATUS(InvalidHandle);
            }

            pVolumeDiskExtends = new VOLUME_DISK_EXTENTS;
            if( 0 == pVolumeDiskExtends )
            {
                DEBUG_STATUS(NotAllocated);
            }


            // use DeviceIoControl function to find what physical drive, current volume extends
            DWORD dwBytesReturned = 0;
            bool bResult = DeviceIoControl( hDevice
                                            , IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS
                                            , NULL
                                            , 0
                                            , pVolumeDiskExtends
                                            , sizeof(VOLUME_DISK_EXTENTS)
                                            , &dwBytesReturned
                                            , NULL );

            CloseHandle(hDevice);

            if( 0 == bResult )
            {
                DEBUG_STATUS(Unsuccessful);
            }

            // if DeviceIoControl was a success then we map letter found to the coresponding struct
            // containing a list with all drives that this drive is extending

            for( DWORD k = 0; k < pVolumeDiskExtends->NumberOfDiskExtents; k++)
            {
                QString qzDiskName = "";
                qzDiskName.sprintf("\\\\.\\PhysicalDrive%d", pVolumeDiskExtends->Extents[k].DiskNumber);
                if( !retValue.contains(qzDiskName) )
                {
                    retValue.append(qzDiskName);
                }
            }

        }

        // check next drive letter
        ++chDriveCount;
        dwDriveVal *= 2;
    }
    return retValue;
}
