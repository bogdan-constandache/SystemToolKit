#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_

// used so i can define my own classes
#define INITGUID

#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>
#include <cfgmgr32.h>
#include <RegStr.h>

#include <QMap>
#include <QStandardItemModel>

// {c166523c-fe0c-4a94-a586-f1a80cfbbf3e}
DEFINE_GUID(GUID_AUDIO, 0xC166523C, 0xFE0C, 0x4A94, 0xA5, 0x86, 0xF1, 0xA8, 0x0C, 0xFB, 0xBF, 0x3E);
// {4d36e96c-e325-11ce-bfc1-08002be10318}
DEFINE_GUID(GUID_SOUND, 0x4D36E96C, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);

typedef struct _DEVICE_DETAILS_
{
    QString qzParent;
    QString qzDisplayName;
    QString qzID;
    QMap<QString, QStringList> qDetails;
}DeviceDetails;

typedef unsigned short USHORT;

class CDeviceInfo
{
private:
    HDEVINFO                m_hDeviceInfo;
    SP_DEVINFO_DATA         m_pDeviceInfoData;
    USHORT                  m_usDeviceIndex;
    QList<DeviceDetails*>   m_qDeviceDetails;
    QStringList             m_qTopLevelItems;

    BOOL ResetInternalCounters();
    BOOL RetrieveDeviceDetails(DeviceDetails **);
    BOOL EnumerateDeviceInformation();
    BOOL GetDeviceRegistryProperty(DWORD, LPBYTE);
    BOOL GetClassDescription(LPWSTR);

public:
    CDeviceInfo();
    ~CDeviceInfo();

    QStandardItemModel *GetAllDeviceDetails();
    QStandardItemModel *GetDeviceProperties(QString qzDeviceID);
};

#endif
