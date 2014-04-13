#include "../headers/hardware_usage.h"


CHardwareUsage::CHardwareUsage()
{
    PdhOpenQuery(NULL, NULL, &m_CpuQuery);
    PdhOpenQuery(NULL, NULL, &m_DiskQuery);

    PdhAddCounter(m_CpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &m_CpuCounter);
    PdhAddCounter(m_DiskQuery, L"\\Disk(_Total)\\% Disk Time", NULL, &m_DiskCounter);

    PdhCollectQueryData(m_CpuQuery);
    PdhCollectQueryData(m_DiskQuery);
}

CHardwareUsage::~CHardwareUsage()
{

}

double CHardwareUsage::GetCpuLoad()
{
    PDH_FMT_COUNTERVALUE CounterValue;
    memset(&CounterValue, 0, sizeof(PDH_FMT_COUNTERVALUE));

    PdhCollectQueryData(m_CpuQuery);
    PdhGetFormattedCounterValue(m_CpuCounter, PDH_FMT_DOUBLE, NULL, &CounterValue);

    qDebug() << "CPU USAGE:" <<  CounterValue.doubleValue;

    return CounterValue.doubleValue;
}

QString CHardwareUsage::GetCpuName()
{
    int CPUInfo[4];
    int Eax = 0;
    std::string RegisterString;
    __cpuid(CPUInfo, 0x80000000);
    Eax = CPUInfo[0];
    RegisterString.clear();
    if( Eax >= 0x80000004 )
    {
        for(int i = 0x80000000; i <= Eax; i++)
        {
            if( 0x80000002 == i || 0x80000003 == i || 0x80000004 == i)
            {
                __cpuid(CPUInfo, i);
                RegisterString += std::string((char*)&CPUInfo[0],4);
                RegisterString += std::string((char*)&CPUInfo[1],4);
                RegisterString += std::string((char*)&CPUInfo[2],4);
                RegisterString += std::string((char*)&CPUInfo[3],4);
            }
        }
        return QString().fromStdString(RegisterString);
    }

    return "";
}

double CHardwareUsage::GetDiskLoad()
{
    PDH_FMT_COUNTERVALUE CounterValue;
    memset(&CounterValue, 0, sizeof(PDH_FMT_COUNTERVALUE));

    PdhCollectQueryData(m_DiskQuery);
    PdhGetFormattedCounterValue(m_DiskCounter, PDH_FMT_DOUBLE, NULL, &CounterValue);

    return CounterValue.doubleValue;
}

MemoryStatus *CHardwareUsage::GetMemoryStatus()
{
    MemoryStatus* pMemoryStatus = new MemoryStatus;

    MEMORYSTATUSEX MemStat;
    MemStat.dwLength = sizeof(MemStat);

    if (!GlobalMemoryStatusEx(&MemStat))
    {
        delete pMemoryStatus;
        return 0;
    }

    pMemoryStatus->qzMemoryLoad.sprintf("%d%", MemStat.dwMemoryLoad);
    pMemoryStatus->qzAvailExtVirtual.sprintf("0");
    pMemoryStatus->qzAvailPageFile.sprintf("%d MB", MemStat.ullAvailPageFile / 1048576);
    pMemoryStatus->qzTotalPageFile.sprintf("%d MB", MemStat.ullTotalPageFile / 1048576);
    pMemoryStatus->qzAvailVirtual.sprintf("%d MB", MemStat.ullAvailVirtual / 1048576);
    pMemoryStatus->qzTotalVirtual.sprintf("%d MB", MemStat.ullTotalVirtual / 1048576);
    pMemoryStatus->qzAvailPhys.sprintf("%d MB", MemStat.ullAvailPhys / 1048576);
    pMemoryStatus->qzTotalPhys.sprintf("%d MB", MemStat.ullTotalPhys / 1048576);

    return pMemoryStatus;
}


