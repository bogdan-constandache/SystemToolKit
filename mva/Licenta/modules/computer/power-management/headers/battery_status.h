#ifndef BATTERY_STATUS_H
#define BATTERY_STATUS_H

// WinAPI includes
#include <Windows.h>
#include <WinBase.h>
#include <batclass.h>
#include <SetupAPI.h>

// QT includes
#include <QStandardItemModel>

// My includes
#include "../../../../utils/headers/utils.h"

DEFINE_GUID(GUID_DEVCLASS_BATTERY,         0x72631E54, 0x78A4, 0x11D0, 0xBC, 0xF7, 0x00, 0xAA, 0x00, 0xB7, 0xB3, 0x2A);

typedef struct _BATTERY_INFORMATION_
{
    QString ACLineStatus;
    QString BatteryStatus;
    QString LifePercent;
    QString LifeTime;
    QString FullLifeTime;
    QString Technology;
    QString Chemistry;
    QString DesignedCapacity;
    QString FullChargedCapacity;
    QString CurrentCapacity;
    QString LowBatteryAlert1;
    QString LowBatteryAlert2;
    QString CycleCount;
    QString Voltage;
    QString WearLevel;
}BatteryInformationStruct, *PBatteryInformationStruct;

class BatteryStatus
{
private:
    PBatteryInformationStruct m_data;
    bool m_bIsDesktop;
    int Initialize();

public:
    BatteryStatus();
    ~BatteryStatus();
    QStandardItemModel *GetBatteryInformation();
};

#endif
