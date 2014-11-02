#ifndef VIDEO_STRUCTURES_H
#define VIDEO_STRUCTURES_H

typedef struct _NVIDIA_GPU_INFORMATIONS_
{
    QString qzFullName;
    QString qzCoreCount;
    QString qzVoltage;
    QString qzGraphicsClock;
    QString qzMemoryClock;
} NVidiaGPUInformations;

typedef struct _NVIDIA_UTILIZATION_REPORT_
{
    QString qzGPUUtilization;
    QString qzFBUtilization;
    QString qzVIDUtilization;
    QString qzBUSUtilization;
} NVidiaUtilizationReport;

typedef struct _NVIDIA_THERMAL_REPORT_
{
    QString qzTarget;
    QString qzTemp;
} NVidiaThermalReport;

typedef struct _NVIDIA_REPORT_
{
    QString qsName;
    QString qsCoreCount;
    QString qsGPUClock;
    QString qsMemoryClock;
    QString qsVoltage;
    QString qsDriverVersion;
    QString qsBusType;
    QString qsBiosVersion;
    QString qsVendorName;
    QString qsChipsetName;
}NVidiaReport;

#endif // VIDEO_STRUCTURES_H
