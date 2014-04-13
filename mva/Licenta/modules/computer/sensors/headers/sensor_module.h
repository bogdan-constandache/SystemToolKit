#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "factory_board_sensor.h"
#include "factory_cpu_sensor.h"
#include "hardware_usage.h"

class CSensorModule
{
private:
    FactoryBoardSensor *m_pBoardFactory;
    FactoryCpuSensor *m_pCpuFactory;
    CHardwareUsage *m_pHardwareUsage;

public:
    CSensorModule();
    ~CSensorModule();

    ISensor *GetBoardSensor();
    ICPUSensor *GetCpuSensor();

    double GetCpuLoad();
    QString GetCpuName();
    double GetDiskLoad();
    MemoryStatus* GetMemoryStat();
};

#endif // SENSOR_MODULE_H
