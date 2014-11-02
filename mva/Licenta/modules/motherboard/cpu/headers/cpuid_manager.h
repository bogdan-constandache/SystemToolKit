#ifndef CPUID_MANAGER_H
#define CPUID_MANAGER_H

#include "intel_cpuid.h"
#include "amd_cpuid.h"

class CCPUIDManager
{

private:
    bool m_bIsAmd;
    bool m_bIsIntel;

//    CIntelCpuID *m_pIntel;
//    CAMDCpuID   *m_pAmd;
    ICpuId *m_pCpuID;

    void OnDetectInstalledCPU();

public:
    CCPUIDManager();
    ~CCPUIDManager();

    QStandardItemModel *GetCPUIDDataModel();
};

#endif // CPUID_MANAGER_H
