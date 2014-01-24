#ifndef AMD_CPUID_H
#define AMD_CPUID_H

// WinAPI includes
#include <Windows.h>
#include <intrin.h>
#include <string>

// QT includes
#include <QStringList>
#include <QDebug>

// My includes
#include "cpuid_defines.h"
#include "../utils/utils.h"

// Defines

class CAMDCpuID
{
private:
    PCpuIDInformation m_data;
public:
    CAMDCpuID();
    ~CAMDCpuID();
};

#endif
