#ifndef AMD_CPUID_H
#define AMD_CPUID_H

// WinAPI includes
#include <Windows.h>
#include <intrin.h>
#include <string>

// QT includes
#include <QStringList>
#include <QDebug>
#include <QStandardItemModel>

// My includes
#include "cpuid_defines.h"
#include "../../../../utils/headers/utils.h"

// Defines

class CAMDCpuID : public ICpuId
{
private:
    PCpuIDInformation m_data;
    QStandardItemModel *m_pDataModel;

private:
    int Initialize();

public:
    CAMDCpuID();
    ~CAMDCpuID();
    virtual QStandardItemModel* GetCPUIDInformations();
};

#endif
