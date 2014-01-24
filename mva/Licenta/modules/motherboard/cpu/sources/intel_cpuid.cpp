#include "../../include/hardware/intel_cpuid.h"


CIntelCpuID::CIntelCpuID() : m_data(0)
{
    m_data = new CpuIDInformation();
}

CIntelCpuID::~CIntelCpuID()
{
    if( 0 != m_data )
    {
        m_data->Instructions.clear();
        delete m_data;
        m_data = 0;
    }
}

int CIntelCpuID::GetCpuIDInformation()
{
    int nStatus = Uninitialized;
    int CPUInfo[4];
    int Eax = 0x80000000;
    int RegisterValue = 0;
    std::string RegisterString;

    // getting manufacturer
    __cpuid(CPUInfo, 0);
    RegisterString += std::string((char*)&CPUInfo[1],4);
    RegisterString += std::string((char*)&CPUInfo[3],4);
    RegisterString += std::string((char*)&CPUInfo[2],4);
    m_data->Manufacturer = QString().fromStdString(RegisterString);

    // getting cpuname
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
        m_data->CPUName = QString().fromStdString(RegisterString);
    }

    // determine max clock

    if( m_data->CPUName.contains("MHz") || m_data->CPUName.contains("GHz") || m_data->CPUName.contains("THz") )
    {
        m_data->MaxClock = m_data->CPUName.split(" ").last();
    }

    // determine stepping, model, family, extended family, extended model and revision
    __cpuid(CPUInfo, 1);
    RegisterValue = CPUInfo[0] & 0xF;
    m_data->Stepping = QString().setNum(RegisterValue);
    RegisterValue = (CPUInfo[0] >> 4) & 0xF;
    m_data->Model = QString().setNum(RegisterValue);
    RegisterValue = (CPUInfo[0] >> 8) & 0xF;
    m_data->Family = QString().setNum(RegisterValue);
    RegisterValue = (CPUInfo[0] >> 16) & 0xF;
    m_data->ExtendedFamily = QString().setNum(RegisterValue);
    RegisterValue = (CPUInfo[0] >> 20) & 0xFF;
    m_data->ExtendedModel = QString().setNum(RegisterValue);

    // getting processor type EAX[12:13]
    switch((CPUInfo[0] >> 12) & 0x03)
    {
    case 0:
        m_data->ProcessorType = QString("Original OEM Processor");
        break;
    case 1:
        m_data->ProcessorType = QString("Intel Overdrive Processor");
        break;
    case 2:
        m_data->ProcessorType = QString("Dual Processor");
        break;
    case 3:
        m_data->ProcessorType = QString("Intel reserved");
        break;
    }

    // getting instructions from ECX register
    if((CPUInfo[2]) & 0x1)
        m_data->Instructions.append("SSE3");
    if((CPUInfo[2] >> 1) & 0x1)
        m_data->Instructions.append("PCLMULQDQ");
    if((CPUInfo[2] >> 2) & 0x1)
        m_data->Instructions.append("DTES64");
    if((CPUInfo[2] >> 3) & 0x1)
        m_data->Instructions.append("MONITOR");
    if((CPUInfo[2] >> 4) & 0x1)
        m_data->Instructions.append("DS-CPL");
    if((CPUInfo[2] >> 5) & 0x1)
        m_data->Instructions.append("VMX");
    if((CPUInfo[2] >> 6) & 0x1)
        m_data->Instructions.append("SMX");
    if((CPUInfo[2] >> 7) & 0x1)
        m_data->Instructions.append("EIST");
    if((CPUInfo[2] >> 8) & 0x1)
        m_data->Instructions.append("TM2");
    if((CPUInfo[2] >> 9) & 0x1)
        m_data->Instructions.append("SSSE3");
    if((CPUInfo[2] >> 10) & 0x1)
        m_data->Instructions.append("CNXT-ID");
    if((CPUInfo[2] >> 12) & 0x1)
        m_data->Instructions.append("FMA");
    if((CPUInfo[2] >> 13) & 0x1)
        m_data->Instructions.append("CMPXCHG16B");
    if((CPUInfo[2] >> 14) & 0x1)
        m_data->Instructions.append("xTPR Update Control");
    if((CPUInfo[2] >> 15) & 0x1)
        m_data->Instructions.append("PDCM");
    if((CPUInfo[2] >> 17) & 0x1)
        m_data->Instructions.append("PCID");
    if((CPUInfo[2] >> 18) & 0x1)
        m_data->Instructions.append("DCA");
    if((CPUInfo[2] >> 19) & 0x1)
        m_data->Instructions.append("SSE4.1");
    if((CPUInfo[2] >> 20) & 0x1)
        m_data->Instructions.append("SSE4.2");
    if((CPUInfo[2] >> 21) & 0x1)
        m_data->Instructions.append("x2APIC");
    if((CPUInfo[2] >> 22) & 0x1)
        m_data->Instructions.append("MOVBE");
    if((CPUInfo[2] >> 23) & 0x1)
        m_data->Instructions.append("POPCNT");
    if((CPUInfo[2] >> 24) & 0x1)
        m_data->Instructions.append("TSC-Deadline");
    if((CPUInfo[2] >> 25) & 0x1)
        m_data->Instructions.append("AESNI");
    if((CPUInfo[2] >> 26) & 0x1)
        m_data->Instructions.append("XSAVE");
    if((CPUInfo[2] >> 27) & 0x1)
        m_data->Instructions.append("OSXSAVE");
    if((CPUInfo[2] >> 28) & 0x1)
        m_data->Instructions.append("AVX");
    if((CPUInfo[2] >> 29) & 0x1)
        m_data->Instructions.append("F16C");
    if((CPUInfo[2] >> 30) & 0x1)
        m_data->Instructions.append("RDRAND");

    // getting processor features from EDX register
    if((CPUInfo[3]) & 0x1)
        m_data->Instructions.append("FPU");
    if((CPUInfo[3] >> 1) & 0x1)
        m_data->Instructions.append("VME");
    if((CPUInfo[3] >> 2) & 0x1)
        m_data->Instructions.append("DE");
    if((CPUInfo[3] >> 3) & 0x1)
        m_data->Instructions.append("PSE");
    if((CPUInfo[3] >> 4) & 0x1)
        m_data->Instructions.append("TSC");
    if((CPUInfo[3] >> 5) & 0x1)
        m_data->Instructions.append("MSR");
    if((CPUInfo[3] >> 6) & 0x1)
        m_data->Instructions.append("PAE");
    if((CPUInfo[3] >> 7) & 0x1)
        m_data->Instructions.append("MCE");
    if((CPUInfo[3] >> 8) & 0x1)
        m_data->Instructions.append("CX8");
    if((CPUInfo[3] >> 9) & 0x1)
        m_data->Instructions.append("APIC");
    if((CPUInfo[3] >> 11) & 0x1)
        m_data->Instructions.append("SEP");
    if((CPUInfo[3] >> 12) & 0x1)
        m_data->Instructions.append("MTRR");
    if((CPUInfo[3] >> 13) & 0x1)
        m_data->Instructions.append("PGE");
    if((CPUInfo[3] >> 14) & 0x1)
        m_data->Instructions.append("MCA");
    if((CPUInfo[3] >> 15) & 0x1)
        m_data->Instructions.append("CMOV");
    if((CPUInfo[3] >> 16) & 0x1)
        m_data->Instructions.append("PAT");
    if((CPUInfo[3] >> 17) & 0x1)
        m_data->Instructions.append("PSE-36");
    if((CPUInfo[3] >> 18) & 0x1)
        m_data->Instructions.append("PSN");
    if((CPUInfo[3] >> 19) & 0x1)
        m_data->Instructions.append("CLFSH");
    if((CPUInfo[3] >> 21) & 0x1)
        m_data->Instructions.append("DS");
    if((CPUInfo[3] >> 22) & 0x1)
        m_data->Instructions.append("ACPI");
    if((CPUInfo[3] >> 23) & 0x1)
        m_data->Instructions.append("MMX");
    if((CPUInfo[3] >> 24) & 0x1)
        m_data->Instructions.append("FXSR");
    if((CPUInfo[3] >> 25) & 0x1)
        m_data->Instructions.append("SSE");
    if((CPUInfo[3] >> 26) & 0x1)
        m_data->Instructions.append("SSE2");
    if((CPUInfo[3] >> 27) & 0x1)
        m_data->Instructions.append("SS");
    if((CPUInfo[3] >> 28) & 0x1)
        m_data->Instructions.append("HTT");
    if((CPUInfo[3] >> 29) & 0x1)
        m_data->Instructions.append("TM");
    if((CPUInfo[3] >> 31) & 0x1)
        m_data->Instructions.append("PBE");


    // determine information about cache
    __cpuid(CPUInfo, 2);
    GetCacheInformation(CPUInfo);


    nStatus = Success;
    return nStatus;
}

PCpuIDInformation CIntelCpuID::GetData()
{
    return m_data;
}

int CIntelCpuID::GetCacheInformation(int* pData)
{
    int nStatus = Uninitialized;
    PCacheInformation pCacheInfo = 0;
    int nFirstByte = 0;
    int nSecondByte = 0;
    int nThirdByte = 0;
    int nFourthByte = 0;
    for(int i = 0; i < 4; i++)
    {
        nFirstByte = pData[i] & 0xFF;
        pCacheInfo = GetRegisterCacheInformation(nFirstByte);
        if( 0 != pCacheInfo )
        {
            m_data->CacheInformation.append(pCacheInfo);
        }
        nSecondByte = (pData[i] >> 8) & 0xFF;
        pCacheInfo = GetRegisterCacheInformation(nSecondByte);
        if( 0 != pCacheInfo )
        {
            m_data->CacheInformation.append(pCacheInfo);
        }
        nThirdByte = (pData[i] >> 16) & 0xFF;
        pCacheInfo = GetRegisterCacheInformation(nThirdByte);
        if( 0 != pCacheInfo )
        {
            m_data->CacheInformation.append(pCacheInfo);
        }
        nFourthByte = (pData[i] >> 24) & 0xFF;
        pCacheInfo = GetRegisterCacheInformation(nFourthByte);
        if( 0 != pCacheInfo )
        {
            m_data->CacheInformation.append(pCacheInfo);
        }
    }

    nStatus = Success;
    return nStatus;
}

PCacheInformation CIntelCpuID::GetRegisterCacheInformation(int nByte)
{
    PCacheInformation pCacheInfo = 0;
    switch( nByte )
    {
    case 0x06:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Instruction cache";
        pCacheInfo->Size = "8 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x08:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Instruction cache";
        pCacheInfo->Size = "16 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x09:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Instruction cache";
        pCacheInfo->Size = "32 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x0A:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "8 KBytes";
        pCacheInfo->Descriptor = "2-way set associative, 32 byte line size";
        break;
    case 0x0C:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "16 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x0D:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "16 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x0E:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Instruction cache";
        pCacheInfo->Size = "24 KBytes";
        pCacheInfo->Descriptor = "6-way set associative, 64 byte line size";
        break;
    case 0x21:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "256 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x22:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x23:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x24:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0x25:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x29:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "4 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x2C:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "32 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x30:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "32 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x41:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "128 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x42:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "256 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x43:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x44:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x45:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "4-way set associative, 32 byte line size";
        break;
    case 0x46:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "4 MBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x47:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "8 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x48:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "3 MBytes";
        pCacheInfo->Descriptor = "12-way set associative, 64 byte line size";
        break;
    case 0x49:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "4 MBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0x4A:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "6 MBytes";
        pCacheInfo->Descriptor = "12-way set associative, 64 byte line size";
        break;
    case 0x4B:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "8 MBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0x4C:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "12 MBytes";
        pCacheInfo->Descriptor = "12-way set associative, 64 byte line size";
        break;
    case 0x4D:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "16 KBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0x4E:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "6 MBytes";
        pCacheInfo->Descriptor = "24-way set associative, 64 byte line size";
        break;
    case 0x60:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "16 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x66:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "8 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x67:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "16 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x68:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L1";
        pCacheInfo->Type = "Data cache";
        pCacheInfo->Size = "32 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x70:
        break;
    case 0x71:
        break;
    case 0x72:
        break;
    case 0x78:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x79:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "128 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x7A:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "256 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x7B:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x7C:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size, 2 lines per sector";
        break;
    case 0x7D:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x7F:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "2-way set associative, 64 byte line size";
        break;
    case 0x80:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0x82:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "256 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 32 byte line size";
        break;
    case 0x83:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "8-way set associative, 32 byte line size";
        break;
    case 0x84:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 32 byte line size";
        break;
    case 0x85:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 32 byte line size";
        break;
    case 0x86:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0x87:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L2";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0xD0:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "512 KBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0xD1:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0xD2:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "4-way set associative, 64 byte line size";
        break;
    case 0xD6:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "1 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0xD7:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0xD8:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "4 MBytes";
        pCacheInfo->Descriptor = "8-way set associative, 64 byte line size";
        break;
    case 0xDC:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "1.5 MBytes";
        pCacheInfo->Descriptor = "12-way set associative, 64 byte line size";
        break;
    case 0xDD:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "3 MBytes";
        pCacheInfo->Descriptor = "12-way set associative, 64 byte line size";
        break;
    case 0xDE:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "6 MBytes";
        pCacheInfo->Descriptor = "12-way set associative, 64 byte line size";
        break;
    case 0xE2:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "2 MBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0xE3:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "4 MBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0xE4:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "8 MBytes";
        pCacheInfo->Descriptor = "16-way set associative, 64 byte line size";
        break;
    case 0xEA:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "12 MBytes";
        pCacheInfo->Descriptor = "24-way set associative, 64 byte line size";
        break;
    case 0xEB:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "18 MBytes";
        pCacheInfo->Descriptor = "24-way set associative, 64 byte line size";
        break;
    case 0xEC:
        pCacheInfo = new CacheInformation;
        pCacheInfo->Level = "L3";
        pCacheInfo->Size = "24 MBytes";
        pCacheInfo->Descriptor = "24-way set associative, 64 byte line size";
        break;
    }

    return pCacheInfo;
}
