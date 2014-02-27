#ifndef SMARTSTRUCTS_H
#define SMARTSTRUCTS_H

//WinAPI includes
#include <Windows.h>
//#include <WinBase.h>
//#include <atlstr.h>
//#include <tchar.h>

//QT includes
#include <QString>
#include <QStringList>
#include <QList>

typedef struct  _ATA_DEVICE_PROPERTIES_
{
    QString RotationSpeed;              //done 0
    QString UDMATransferMode;           //done
    QString ActiveUDMATransferMode;     //done
    QString PIOTransferMode;            //done
    QString MWDMATransferMode;          //done
    QString ActiveMWDMATransferMode;    //done
    QString ECCBytes;
    QString Model;                      //done 0
    QString FirmwareRevision;           //done 0
    QString SerialNumber;               //done 0
    QString BufferSize;                 //done 0
    QString DeviceType;                 //done 0
    QString Cylinders;                  //done 0
    QString Heads;                      //done 0
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
