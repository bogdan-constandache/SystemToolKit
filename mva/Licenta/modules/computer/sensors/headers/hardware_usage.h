#ifndef HARDWARE_USAGE_H
#define HARDWARE_USAGE_H

#include <Windows.h>
#include <Pdh.h>

#include "sensor_defines.h"

#include <QDebug>

class CHardwareUsage
{
private:
    PDH_HQUERY m_CpuQuery;
    PDH_HQUERY m_DiskQuery;
    PDH_HCOUNTER m_CpuCounter;
    PDH_HCOUNTER m_DiskCounter;

public:
    CHardwareUsage();
    ~CHardwareUsage();

    double GetCpuLoad();
    QString GetCpuName();
    double GetDiskLoad();
    MemoryStatus* GetMemoryStatus();
};

#endif // HARDWARE_USAGE_H
