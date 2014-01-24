#ifndef CPUID_DEFINES_H
#define CPUID_DEFINES_H

typedef struct _CACHE_INFORMATION
{
    QString Level;
    QString Type;
    QString Size;
    QString Descriptor;
}CacheInformation, *PCacheInformation;

typedef struct _CPUID_INFORMATION
{
    QString Manufacturer;
    QString CPUName;
    QString MaxClock;
    QString Stepping;
    QString MaxMultiplier;
    QString MinMultiplier;
    QStringList Instructions;
    QString Family;
    QString Model;
    QString ExtendedFamily;
    QString ExtendedModel;
    QString Revision;
    QString ProcessorType;
    QList<PCacheInformation> CacheInformation;
}CpuIDInformation, *PCpuIDInformation;

#endif // CPUID_DEFINES_H
