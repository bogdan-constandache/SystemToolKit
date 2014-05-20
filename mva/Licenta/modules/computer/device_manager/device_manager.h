#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_

#include <Windows.h>
#include <SetupAPI.h>
#include <devguid.h>

#include <QMap>
#include <QStandardItemModel>

typedef struct _DEVICE_DETAILS_
{
    QString qzParent;
    QString qzDisplayName;
    QMap<QString, QString> qDetails;
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

public:
    CDeviceInfo();
    ~CDeviceInfo();
    BOOL EnumerateDeviceInformation();
    BOOL GetDeviceRegistryProperty(DWORD, LPBYTE);
    BOOL GetClassDescription(LPWSTR);

    QStandardItemModel *GetAllDeviceDetails();
};

#endif
