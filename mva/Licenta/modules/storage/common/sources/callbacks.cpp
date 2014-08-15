#include "../headers/callbacks.h"

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
            StringCchPrintf(tszDrivePath, 32, L"\\\\.\\%c:", _T('A' + chDriveCount));
//            _stprintf_s(tszDrivePath, 32, _T("\\\\.\\%c:"), _T('A' + chDriveCount));

            TCHAR tszDriveType[10];
            memset(tszDriveType, '\0', sizeof(tszDriveType));
            StringCchPrintf(tszDriveType, 10, L"%c:\\", _T('A' + chDriveCount));
//            _stprintf_s(tszDriveType, 10, _T("%c:\\"), _T('A'+chDriveCount));


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

            delete pVolumeDiskExtends;
        }

        // check next drive letter
        ++chDriveCount;
        dwDriveVal *= 2;
    }
    return retValue;
}


int GetDriveLength(HANDLE hDrive)
{
    GET_LENGTH_INFORMATION driveInformation;
    DWORD dwResult = 0;

    if( NULL == hDrive )
    {
        return InvalidHandle;
    }

    bool bResult = DeviceIoControl(hDrive,
                                   IOCTL_DISK_GET_LENGTH_INFO ,
                                   NULL,
                                   0,
                                   &driveInformation,
                                   sizeof(GET_LENGTH_INFORMATION),
                                   &dwResult,
                                   NULL);
    if( !bResult )
    {
        return Unsuccessful;
    }

    QString qz;
    qz.sprintf("%llu bytes", driveInformation.Length);
    //    float value = (float)(driveInformation.Length  (1024 * 1024));

    //    return (int)value;
    return 0;
}


ATADeviceProperties *GetATADeviceProperties(const wchar_t *wszDriveName)
{
    DWORD dwResult = 0;
    SENDCMDINPARAMS pIN;
    HANDLE hDevice = NULL;
    WORD *bData = new WORD[OUT_IDENTIFY_SIZE];
    WORD *pIdentifyData = 0;
    DISK_GEOMETRY diskGeometry;
    WORD wAux = 0;
    QString qzTemp;

    ATADeviceProperties *pItem = 0;
    pItem = new ATADeviceProperties;
    if( 0 == pItem )
        return 0;

    memset(&pIN, 0, sizeof(SENDCMDINPARAMS));
    memset(bData, 0, OUT_IDENTIFY_SIZE);

    if( NULL == wszDriveName )
        return 0;

    hDevice = CreateFile(
                wszDriveName,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_SYSTEM,
                NULL);

    if( INVALID_HANDLE_VALUE == hDevice )
        return 0;

    pIN.cBufferSize = IDENTIFY_BUFFER_SIZE;
    pIN.irDriveRegs.bFeaturesReg = 0;
    pIN.irDriveRegs.bSectorCountReg = 1;
    pIN.irDriveRegs.bSectorNumberReg = 1;
    pIN.irDriveRegs.bCylLowReg = 0;
    pIN.irDriveRegs.bCylHighReg = 0;
    pIN.irDriveRegs.bDriveHeadReg = DRIVE_HEAD_REG;
    pIN.irDriveRegs.bCommandReg = ID_CMD;

    bool bResult = DeviceIoControl(
                hDevice,
                SMART_RCV_DRIVE_DATA,
                &pIN,
                sizeof(SENDCMDINPARAMS),
                bData,
                OUT_BUFFER_SIZE,
                &dwResult,
                NULL);

    if( !bResult )
        goto CleanUp;

    bResult = DeviceIoControl(
                hDevice,
                IOCTL_DISK_GET_DRIVE_GEOMETRY,
                NULL,
                0,
                &diskGeometry,
                sizeof(DISK_GEOMETRY),
                &dwResult,
                NULL);
    if( !bResult )
        goto CleanUp;

    CloseHandle(hDevice);

    pItem->Cylinders.sprintf("%llu", diskGeometry.Cylinders);
    pItem->SectorPerTrack.sprintf("%d", diskGeometry.SectorsPerTrack);
    pItem->BytesPerSector.sprintf("%d", diskGeometry.BytesPerSector);

    pIdentifyData = bData + 8;

    //Rotation Speed
    if( 0 == pIdentifyData[NMRR] )
        pItem->RotationSpeed = "Rotation speed not reported";
    if( 1 == pIdentifyData[NMRR] )
        pItem->RotationSpeed.sprintf("0 RPM(Solid State Device)");
    else
        if( pIdentifyData[NMRR] > 0x401 )
            pItem->RotationSpeed.sprintf("%u RPM", pIdentifyData[NMRR]);
    //end Rotation Speed

    //ATA Standard
    wAux = pIdentifyData[ATA_STANDARD];

    if( 0x0000 == wAux || 0xFFFF == wAux )
        pItem->ATAStandard = "ATA Standard not reported";
    else
    {
        if( wAux >> 8 & 0x01 )
            pItem->ATAStandard = "ATA8-ACS";
        else
            if( wAux >> 7 & 0x01 )
                pItem->ATAStandard = "ATA/ATAPI-7";
            else
                if( wAux >> 6 & 0x01 )
                    pItem->ATAStandard = "ATA/ATAPI-6";
                else
                    if( wAux >> 5 & 0x01 )
                        pItem->ATAStandard = "ATA/ATAPI-5";
                    else
                        if( wAux >> 4 & 0x01 )
                            pItem->ATAStandard = "ATA/ATAPI-4";
    }
    //end ATA Standard

    //Max and Active UDMA Transfer Mode
    wAux = pIdentifyData[ULTRA_DMA];

    if( wAux >> 14 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 6";
    if( wAux >> 13 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 5";
    if( wAux >> 12 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 4";
    if( wAux >> 11 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 3";
    if( wAux >> 10 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 2";
    if( wAux >> 9 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 1";
    if( wAux >> 8 & 0x01 )
        pItem->ActiveUDMATransferMode = "UDMA 0";

    if( wAux >> 6 & 0x01 )
        pItem->UDMATransferMode = "UDMA 6";
    else
        if( wAux >> 5 & 0x01 )
            pItem->UDMATransferMode = "UDMA 5";
        else
            if( wAux >> 4 & 0x01 )
                pItem->UDMATransferMode = "UDMA 4";
            else
                if( wAux >> 3 & 0x01 )
                    pItem->UDMATransferMode = "UDMA 3";
                else
                    if( wAux >> 2 & 0x01 )
                        pItem->UDMATransferMode = "UDMA 2";
                    else
                        if( wAux >> 1 & 0x01 )
                            pItem->UDMATransferMode = "UDMA 1";
                        else
                            if( wAux & 0x01 )
                                pItem->UDMATransferMode = "UDMA 0";
    //end Max and Active UDMA Transfer Mode

    //PIO Transfer Mode
    wAux = pIdentifyData[PIO];
    if( wAux >> 1 & 0x01 )
        pItem->PIOTransferMode = "PIO 4";
    else
        if( wAux & 0x01 )
            pItem->PIOTransferMode = "PIO 3";
    //end PIO TransferMode

    //Max and active MWDMA Transfer Mode
    wAux = pIdentifyData[MWDMA];

    if( wAux >> 10 & 0x01 )
        pItem->ActiveMWDMATransferMode = "MWDMA 2";
    if( wAux >> 9 & 0x01 )
        pItem->ActiveMWDMATransferMode = "MWDMA 1";
    if( wAux >> 8 & 0x01 )
        pItem->ActiveMWDMATransferMode = "MWDMA 0";

    if( wAux >> 2 & 0x01 )
        pItem->MWDMATransferMode = "MWDMA 2";
    else
        if( wAux >> 1 & 0x01 )
            pItem->MWDMATransferMode = "MWDMA 1";
        else
            if( wAux & 0x01 )
                pItem->MWDMATransferMode = "MWDMA 0";
    //end Max and active MWDMA Transfer Mode

    //Model
    qzTemp.clear();

    for(unsigned int i = 0; i < LENGTH_MODEL; i++)
    {
        qzTemp.append((char)(0x00ff & pIdentifyData[i + START_MODEL] >> 8 ));
        qzTemp.append((char)(0x00ff & pIdentifyData[i + START_MODEL] ));
    }
    pItem->Model = qzTemp.trimmed();
    //end Model

    //Revision
    qzTemp.clear();

    for(unsigned int i = 0; i < LENGTH_FIRMWARE; i++)
    {
        qzTemp.append((char)(0x00ff & pIdentifyData[i + START_FIRMWARE] >> 8 ));
        qzTemp.append((char)(0x00ff & pIdentifyData[i + START_FIRMWARE] ));
    }
    pItem->FirmwareRevision = qzTemp.trimmed();
    //end Revision

    //Serial
    qzTemp.clear();

    for(unsigned int i = 0; i < LENGTH_SERIAL; i++)
    {
        qzTemp.append((char)(0x00ff & pIdentifyData[i + START_SERIAL] >> 8 ));
        qzTemp.append((char)(0x00ff & pIdentifyData[i + START_SERIAL] ));
    }
    pItem->SerialNumber = qzTemp.trimmed();
    //end Serial

    //Buffer size
    if (pIdentifyData[20] <= 3 && pIdentifyData[BUFFER_SIZE] && pIdentifyData[BUFFER_SIZE] != 0xffff)
    {
        pItem->BufferSize.sprintf("%u KBytes", pIdentifyData[BUFFER_SIZE] / 2);
    }
    //Buffer size

    //Device type
    wAux = pIdentifyData[TRANSPORT_MAJOR];
    if( wAux >> 12 == 0 )
        pItem->DeviceType = "Parallel";
    else
    {
        if( wAux >> 5 & 0x01 )
            pItem->DeviceType = "SATA-III";
        else
            if( wAux >> 4 & 0x01 ||
                    wAux >> 3 & 0x01 ||
                    wAux >> 2 & 0x01 )
                pItem->DeviceType = "SATA-II";
            else
                if( wAux >> 1 & 0x01 )
                    pItem->DeviceType = "SATA-I";
    }
    //end Device type

    //Logical heads
    wAux = pIdentifyData[LOGICAL_HEADS];
    pItem->Heads.sprintf("%u", wAux);
    //end Logical heads

    return pItem;

CleanUp:
    delete bData;
    return 0;
}


QString GetDiskTotalSize(QString qzDrive)
{
    HANDLE hDisk = INVALID_HANDLE_VALUE;
    BOOL bResult = FALSE;
    DWORD dwJunk = 0;
    DISK_GEOMETRY pDiskGeom = {0};
    ULONGLONG ulDiskSize = 0;

    WCHAR *lpDriveName = CharArrayToWcharArray(qzDrive.toLatin1().data());

    hDisk = CreateFile(lpDriveName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL, OPEN_EXISTING, 0, NULL);

    if( INVALID_HANDLE_VALUE ==  hDisk )
        return "";

    bResult = DeviceIoControl(hDisk,
                              IOCTL_DISK_GET_DRIVE_GEOMETRY,
                              NULL, 0,
                              (LPVOID)&pDiskGeom, sizeof(DISK_GEOMETRY),
                              &dwJunk, (LPOVERLAPPED)NULL);

    CloseHandle(hDisk);
    if( !bResult )
        return "";

    ulDiskSize = pDiskGeom.Cylinders.QuadPart * (ULONG)pDiskGeom.TracksPerCylinder *
                 (ULONG) pDiskGeom.SectorsPerTrack * (ULONG)pDiskGeom.BytesPerSector;

    return QString().sprintf("%.2f GB", (double)ulDiskSize / (1024 * 1024 * 1024));
}
