#ifndef SENSOR_DEFINES_H
#define SENSOR_DEFINES_H

#include <QString>

typedef enum _CHIP_
{
    Unknown_Chip = 0,

    ATK0110 = 1,

    F71858 = 2,
    F71862 = 3,
    F71869 = 4,
    F71869A = 5,
    F71882 = 6,
    F71889AD = 7,
    F71889ED = 8,
    F71889F = 9,
    F71808E = 10,

    IT8705F = 11,
    IT8712F = 12,
    IT8716F = 13,
    IT8718F = 14,
    IT8720F = 15,
    IT8721F = 16,
    IT8726F = 17,
    IT8728F = 18,
    IT8771E = 19,
    IT8772E = 20,

    NCT6771F = 21,
    NCT6776F = 22,
    NCT6779D = 23,
    NCT6791D = 24,

    W83627DHG = 25,
    W83627DHGP = 26,
    W83627EHF = 27,
    W83627HF = 28,
    W83627THF = 29,
    W83667HG = 30,
    W83667HGB = 31,
    W83687THF = 32

}Chip;

typedef enum _INTEL_MICROARCHITECTURE_
{
    UNKNOWN_IMA = -1,
    i386_IMA = 0,
    i486_IMA = 1,
    P5_IMA = 2,
    P6_IMA = 3,
    PENTIUM_M_IMA = 4,
    NETBURST_IMA = 5,
    CORE_IMA = 6,
    ATOM_IMA = 7,
    PENRYN_IMA = 8,
    NEHALEM_IMA = 9,
    SANDY_BRIDGE_IMA = 10,
    IVY_BRIDGE_IMA = 11,
    HASWELL_IMA = 12,
    BROADWELL_IMA = 13,
    SKYLAKE_IMA = 14,
    CANNON_LAKE_IMA = 15,
    LARRA_BEE_IMA = 16
}IntelMicroArchitecture;

typedef enum _AMD_MICROARCHITECTURE_
{
    UNKNOWN_AMA = -1,
    AMD_K5 = 0,
    AMD_K6 = 1,
    AMD_K6_2 = 2,
    AMD_K6_3 = 3,
    AMD_K7_ATHLON = 4,
    AMD_K8_HAMMER = 5,
    AMD_K10_BARCELONA = 6,
    AMD_TURION_X2 = 7,
    AMD_LLANO = 8,
    AMD_BOBCAT = 9,
    AMD_INTERLAGOS = 10,
    AMD_ZAMBEZI = 11,
    AMD_VISHERA = 12
}AMDMicroArchitecture;

typedef struct _MEMORY_STATUS_
{
    QString qzTotalPhys;
    QString qzAvailPhys;
    QString qzTotalPageFile;
    QString qzAvailPageFile;
    QString qzTotalVirtual;
    QString qzAvailVirtual;
    QString qzAvailExtVirtual;
    QString qzMemoryLoad;
}MemoryStatus;

typedef struct _VOLTAGE_READING_STRUCT_
{
    QString qzName;
    QString qzValue;
}VoltageReading;

#endif // SENSOR_DEFINES_H
