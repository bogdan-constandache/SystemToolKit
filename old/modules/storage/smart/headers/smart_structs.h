#ifndef SMARTSTRUCTS_H
#define SMARTSTRUCTS_H

//WinAPI includes
#include <Windows.h>
#include <WinBase.h>
#include <atlstr.h>
#include <tchar.h>

//QT includes
#include <QString>
#include <QStringList>
#include <QList>

typedef struct  _DEVICE_DATA
{
    QString Manufacturer;
    QString FormFactor;
    QString PhysicalDimensions;
    QString Model;
    QString FirmwareRevision;
    QString SerialNumber;
    QString Interface;
    QString TransferMode;
    QString BufferSize;
    QString DeviceType;
    QString Cylinders;
    QString Heads;
    QString SectorPerTrack;
    QString BytesPerSector;
    QString ATAStandard;
    QStringList DeviceFeatures;
}DeviceData, *PDeviceData;

typedef struct  _ATA_DEVICE_PROPERTIES_
{
    QString RotationSpeed;              //done
    QString UDMATransferMode;           //done
    QString ActiveUDMATransferMode;     //done
    QString PIOTransferMode;            //done
    QString MWDMATransferMode;          //done
    QString ActiveMWDMATransferMode;    //done
    QString ECCBytes;
    QString Model;                      //done
    QString FirmwareRevision;           //done
    QString SerialNumber;               //done
    QString BufferSize;                 //done
    QString DeviceType;                 //done
    QString Cylinders;                  //done
    QString Heads;                      //done
    QString SectorPerTrack;             //done
    QString BytesPerSector;             //done
    QString ATAStandard;                //done
    QStringList DeviceFeatures;
}ATADeviceProperties, *PATADeviceProperties;

typedef struct
{
    BYTE  bDriverError;
    BYTE  bIDEStatus;
    BYTE  bReserved[2];
    DWORD dwReserved[2];
} DriverStats;

typedef struct
{
    DWORD      cBufferSize;
    DriverStats DriverStatus;
    BYTE       bBuffer[1];
} ATAOutParam;

typedef struct
{
    BYTE m_ucAttribIndex;
    DWORD m_dwAttribValue;
    BYTE m_ucValue;
    BYTE m_ucWorst;
    DWORD m_dwThreshold;
}SmartData;

typedef struct
{
    GETVERSIONINPARAMS GetVersionParams;
    DeviceData *DeviceInformation;
    QList<SmartData*> SmartEntries;
    short DriveIndex;
}DriveInfo;

typedef struct
{
    BOOL m_bCritical;
    BYTE m_ucAttribId;
    QString m_csAttribName;
    QString m_csAttribDetails;
}SmartDetails;

#endif