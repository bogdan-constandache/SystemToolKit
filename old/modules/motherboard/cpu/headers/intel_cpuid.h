#ifndef INTEL_CPUID_H
#define INTEL_CPUID_H

// WinAPI includes
#include <Windows.h>
#include <intrin.h>
#include <string>

// QT includes
#include <QList>
#include <QStringList>
#include <QDebug>

// My includes
#include "cpuid_defines.h"
#include "../utils/utils.h"

// defines

class CIntelCpuID
{
private:
    PCpuIDInformation m_data;

private:
    int GetCacheInformation(int *pData);
    PCacheInformation GetRegisterCacheInformation(int nByte);
public:
    CIntelCpuID();
    ~CIntelCpuID();
    int GetCpuIDInformation();
    PCpuIDInformation GetData();
};

#endif // INTEL_CPUID_H
