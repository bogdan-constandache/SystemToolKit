#ifndef FACTORY_CPU_SENSOR_H
#define FACTORY_CPU_SENSOR_H

// WinAPI includes
#include <Windows.h>
#include <intrin.h>
#include <string>

#include "intel_temperature.h"
#include "amd0f_temperature.h"
#include "amd10_temperature.h"

class FactoryCpuSensor
{
    CSTKDriverWrapper *m_pDriver;
    ICPUSensor *m_pCpuSensor;

    bool DetectCpuSensor();

public:
    FactoryCpuSensor();
    ~FactoryCpuSensor();

    ICPUSensor *GetCpuSensor();
    int DestroySensor();
};

#endif // FACTORY_CPU_SENSOR_H
