#ifndef CPUID_DEFINES_H
#define CPUID_DEFINES_H

typedef struct _CACHE_INFORMATION
{
    QString Level;
    QString Type;
    QString Size;
    QString Descriptor;
}CacheInformation, *PCacheInformation;

typedef struct _CPU_INSTRUCTIONS
{
    QString qzName;
    QString qzDescription;
    _CPU_INSTRUCTIONS(QString qzN, QString qzD)
    {
        qzName = qzN;
        qzDescription = qzD;
    }
}CpuInstruction;

typedef struct _CPUID_INFORMATION
{
    QString Manufacturer;
    QString CPUName;
    QString MaxClock;
    QString Stepping;
    QString MaxMultiplier;
    QString MinMultiplier;
    QList<CpuInstruction*> Instructions;
    QString Family;
    QString Model;
    QString ExtendedFamily;
    QString ExtendedModel;
    QString Revision;
    QString ProcessorType;
    QList<PCacheInformation> CacheInformation;
}CpuIDInformation, *PCpuIDInformation;

#endif // CPUID_DEFINES_H
