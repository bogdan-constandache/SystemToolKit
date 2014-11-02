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
#include <QStandardItemModel>

// My includes
#include "cpuid_defines.h"
#include "../../../../utils/headers/utils.h"

// defines

class CIntelCpuID : public ICpuId
{
private:
    PCpuIDInformation m_data;
    QStandardItemModel *m_pDataModel;

private:
    int GetCacheInformation(int *pData);
    int GetSecondCacheInformation();
    PCacheInformation GetRegisterCacheInformation(int nByte);
    int Initialize();

public:
    CIntelCpuID();
    ~CIntelCpuID();
    virtual QStandardItemModel* GetCPUIDInformations();
};

#endif // INTEL_CPUID_H
