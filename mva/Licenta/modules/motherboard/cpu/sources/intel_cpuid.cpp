#include "../headers/intel_cpuid.h"


CIntelCpuID::CIntelCpuID() : m_data(0), m_pDataModel(NULL)
{
    m_data = new CpuIDInformation();

    int nStatus = Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    m_pDataModel = new QStandardItemModel();
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    QList<QStandardItem*> qList;

    qList << new QStandardItem("Processor type: ") << new QStandardItem(m_data->ProcessorType);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Manufacturer: ") << new QStandardItem(m_data->Manufacturer);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Name: ") << new QStandardItem(m_data->CPUName);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Stepping: ") << new QStandardItem(m_data->Stepping);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Family: ") << new QStandardItem(m_data->Family);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Model: ") << new QStandardItem(m_data->Model);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Extended family: ") << new QStandardItem(m_data->ExtendedFamily);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Extended model: ") << new QStandardItem(m_data->ExtendedModel);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Max clock: ") << new QStandardItem(m_data->MaxClock);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("Multiplier: ") << new QStandardItem(m_data->MinMultiplier);
    m_pDataModel->appendRow(qList); qList.clear();

    qList << new QStandardItem("") << new QStandardItem("");
    m_pDataModel->appendRow(qList); qList.clear();

    foreach(PCacheInformation pInf, m_data->CacheInformation)
    {
        qList << new QStandardItem(pInf->Level + ", " + pInf->Type) << new QStandardItem(pInf->Size + ", " + pInf->Descriptor);
        m_pDataModel->appendRow(qList); qList.clear();
    }

    qList << new QStandardItem("") << new QStandardItem("");
    m_pDataModel->appendRow(qList); qList.clear();

    foreach(CpuInstruction *pInstr, m_data->Instructions)
    {
        qList << new QStandardItem(pInstr->qzName) << new QStandardItem(pInstr->qzDescription);
        m_pDataModel->appendRow(qList); qList.clear();
    }
}

CIntelCpuID::~CIntelCpuID()
{
    if (0 != m_data)
    {
        for(int i = 0; i < m_data->CacheInformation.count(); i++)
        {
            delete m_data->CacheInformation.at(i);
        }
        for(int i = 0; i < m_data->Instructions.count(); i++)
        {
            delete m_data->Instructions.at(i);
        }
        m_data->CacheInformation.clear();
        m_data->Instructions.clear();

        delete m_data;
        m_data = 0;
    }

    SAFE_DELETE(m_pDataModel);
}

QStandardItemModel *CIntelCpuID::GetCPUIDInformations()
{
    return m_pDataModel;
}

int CIntelCpuID::Initialize()
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
    m_data->Stepping = QString().sprintf("0x%.2x", RegisterValue);
    RegisterValue = (CPUInfo[0] >> 4) & 0xF;
    m_data->Model = QString().sprintf("0x%.2x", RegisterValue);
    RegisterValue = (CPUInfo[0] >> 8) & 0xF;
    m_data->Family = QString().sprintf("0x%.2x", RegisterValue);
    RegisterValue = (CPUInfo[0] >> 16) & 0xF;
    m_data->ExtendedFamily = QString().sprintf("0x%.2x", RegisterValue);
    RegisterValue = (CPUInfo[0] >> 20) & 0xFF;
    m_data->ExtendedModel = QString().sprintf("0x%.2x", RegisterValue);

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
        m_data->Instructions.append(new CpuInstruction("SSE3", "SSE3 new instructions"));
    if((CPUInfo[2] >> 1) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PCLMULQDQ", "Carryless Multiplication"));
    if((CPUInfo[2] >> 2) & 0x1)
        m_data->Instructions.append(new CpuInstruction("DTES64", "64-bit DS Area"));
    if((CPUInfo[2] >> 3) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MONITOR", "Monitor/MWAIT"));
    if((CPUInfo[2] >> 4) & 0x1)
        m_data->Instructions.append(new CpuInstruction("DS-CPL", "CPL Qualified Debug Store"));
    if((CPUInfo[2] >> 5) & 0x1)
        m_data->Instructions.append(new CpuInstruction("VMX", "Virtual Machine Extensions"));
    if((CPUInfo[2] >> 6) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SMX", "Safer Mode Extensions"));
    if((CPUInfo[2] >> 7) & 0x1)
        m_data->Instructions.append(new CpuInstruction("EIST", "Enhanced INTEL SpeedStep technology"));
    if((CPUInfo[2] >> 8) & 0x1)
        m_data->Instructions.append(new CpuInstruction("TM2", "Thermal Monitor 2"));
    if((CPUInfo[2] >> 9) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSSE3", "SSSE3 Extensions"));
    if((CPUInfo[2] >> 10) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CNXT-ID", "L1 Context ID"));
    if((CPUInfo[2] >> 12) & 0x1)
        m_data->Instructions.append(new CpuInstruction("FMA", "Fused Multiply Add"));
    if((CPUInfo[2] >> 13) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CMPXCHG16B", "CMPXCHG16B Available"));
    if((CPUInfo[2] >> 14) & 0x1)
        m_data->Instructions.append(new CpuInstruction("xTPR Update Control", "xTPR Update Control"));
    if((CPUInfo[2] >> 15) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PDCM", "Perf/Debug Capability MSR"));
    if((CPUInfo[2] >> 17) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PCID", "Process-context Identifiers"));
    if((CPUInfo[2] >> 18) & 0x1)
        m_data->Instructions.append(new CpuInstruction("DCA", "Direct Cache Access"));
    if((CPUInfo[2] >> 19) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE4.1", "SSE4.1 Extensions"));
    if((CPUInfo[2] >> 20) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE4.2", "SSE4.2 Extensions"));
    if((CPUInfo[2] >> 21) & 0x1)
        m_data->Instructions.append(new CpuInstruction("x2APIC", "x2APIC Instruction"));
    if((CPUInfo[2] >> 22) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MOVBE", "MOVBE Instruction"));
    if((CPUInfo[2] >> 23) & 0x1)
        m_data->Instructions.append(new CpuInstruction("POPCNT", "POPCNT Instruction"));
    if((CPUInfo[2] >> 24) & 0x1)
        m_data->Instructions.append(new CpuInstruction("TSC-Deadline", "TSC-Deadline Instruction"));
    if((CPUInfo[2] >> 25) & 0x1)
        m_data->Instructions.append(new CpuInstruction("AESNI", "AESNI Instruction"));
    if((CPUInfo[2] >> 26) & 0x1)
        m_data->Instructions.append(new CpuInstruction("XSAVE", "XSAVE Instruction"));
    if((CPUInfo[2] >> 27) & 0x1)
        m_data->Instructions.append(new CpuInstruction("OSXSAVE", "OSXSAVE Instruction"));
    if((CPUInfo[2] >> 28) & 0x1)
        m_data->Instructions.append(new CpuInstruction("AVX", "AVX Instruction"));
    if((CPUInfo[2] >> 29) & 0x1)
        m_data->Instructions.append(new CpuInstruction("F16C", "F16C Instruction"));
    if((CPUInfo[2] >> 30) & 0x1)
        m_data->Instructions.append(new CpuInstruction("RDRAND", "RDRAND Instruction"));

    // getting processor features from EDX register
    if((CPUInfo[3]) & 0x1)
        m_data->Instructions.append(new CpuInstruction("FPU", "Floating Point Unit On-Chip"));
    if((CPUInfo[3] >> 1) & 0x1)
        m_data->Instructions.append(new CpuInstruction("VME", "Virtual 8086 Mode Enhancements"));
    if((CPUInfo[3] >> 2) & 0x1)
        m_data->Instructions.append(new CpuInstruction("DE", "Debugging Extensions"));
    if((CPUInfo[3] >> 3) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PSE", "Page Size Extensions"));
    if((CPUInfo[3] >> 4) & 0x1)
        m_data->Instructions.append(new CpuInstruction("TSC", "Time Stamp Counter"));
    if((CPUInfo[3] >> 5) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MSR", "Model Specific Registers RDMSR and WRMSR Instructions"));
    if((CPUInfo[3] >> 6) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PAE", "Physical Address Extension"));
    if((CPUInfo[3] >> 7) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MCE", "Machine Check Exception"));
    if((CPUInfo[3] >> 8) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CX8", "CMPXCHG8B Instruction"));
    if((CPUInfo[3] >> 9) & 0x1)
        m_data->Instructions.append(new CpuInstruction("APIC", "APIC On-Chip"));
    if((CPUInfo[3] >> 11) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SEP", "SYSENTER and SYSEXIT Instructions"));
    if((CPUInfo[3] >> 12) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MTRR", "Memory Type Range Registers"));
    if((CPUInfo[3] >> 13) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PGE", "Page Global Bit"));
    if((CPUInfo[3] >> 14) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MCA", "Machine Check Architecture"));
    if((CPUInfo[3] >> 15) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CMOV", "Conditional Move Intructions"));
    if((CPUInfo[3] >> 16) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PAT", "Page Attribute Table"));
    if((CPUInfo[3] >> 17) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PSE-36", "36-bit Page Size Extension"));
    if((CPUInfo[3] >> 18) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PSN", "Processor Serial Number"));
    if((CPUInfo[3] >> 19) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CLFSH", "CLFLUSH Instruction"));
    if((CPUInfo[3] >> 21) & 0x1)
        m_data->Instructions.append(new CpuInstruction("DS", "Debug Store"));
    if((CPUInfo[3] >> 22) & 0x1)
        m_data->Instructions.append(new CpuInstruction("ACPI", "Thermal Monitor and Software Controlled Clock Facilities"));
    if((CPUInfo[3] >> 23) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MMX", "Intel MMX Technology"));
    if((CPUInfo[3] >> 24) & 0x1)
        m_data->Instructions.append(new CpuInstruction("FXSR", "FXSAVE and FXRSTOR Instructions"));
    if((CPUInfo[3] >> 25) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE", "SSE Extensions"));
    if((CPUInfo[3] >> 26) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE2", "SSE2 Extensions"));
    if((CPUInfo[3] >> 27) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SS", "Self Snoop"));
    if((CPUInfo[3] >> 28) & 0x1)
        m_data->Instructions.append(new CpuInstruction("HTT", "Max APIC IDs reserved field is Valid"));
    if((CPUInfo[3] >> 29) & 0x1)
        m_data->Instructions.append(new CpuInstruction("TM", "Thermal Monitor"));
    if((CPUInfo[3] >> 31) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PBE", "Pending Break Enable"));


    // determine information about cache
    __cpuid(CPUInfo, 2);
    GetCacheInformation(CPUInfo);

    if(m_data->CacheInformation.count())
        return Success;

    GetSecondCacheInformation();

    nStatus = Success;
    return nStatus;
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

int CIntelCpuID::GetSecondCacheInformation()
{
    int nStatus = Uninitialized;
    PCacheInformation pCacheInfo = 0;
    int CPUInfo[4];
    for(int i = 0; i < 4; i++)
    {
        QString qzLvl = "", qzType = "", qzSize = "", qzDesc = "";

        __cpuidex(CPUInfo, 4, i);
        if((CPUInfo[0] & 0x1) == 0)
            continue;
        if((CPUInfo[0] >> 1) & 0x1)
            qzType = "Data cache";
        if((CPUInfo[0] >> 2) & 0x1)
            qzType = "Instruction cache";
        if((CPUInfo[0] >> 3) & 0x1)
            qzType = "Unified cache";

        if( qzType.isEmpty() )
            qzType = "Data/Instruction cache";

        BYTE bLvl = (CPUInfo[0] >> 5) & 0x7;
        qzLvl = "L" + QString::number(bLvl);

        int nWay = 0;
        nWay = CPUInfo[1] & 0xFFF;
        nWay++;
        int nPart = 0;
        nPart = (CPUInfo[1] >> 12) & 0x3FF;
        nPart++;
        int nLineS = 0;
        nLineS = (CPUInfo[1] >> 22) & 0x3FF;
        nLineS++;
        int nSets = 0;
        nSets = CPUInfo[2];
        nSets++;

        long nSize = (nWay * nPart * nLineS * nSets) / 1024;

        qzSize = QString::number(nSize);
        qzSize += " KBytes";

        qzDesc = QString("%1-way associative, %2 byte line size").arg(QString::number(nLineS), QString::number(nWay));

        if( qzLvl.isEmpty() || qzSize.isEmpty() )
            continue;

        pCacheInfo = new CacheInformation;

        pCacheInfo->Type = qzType;
        pCacheInfo->Descriptor = qzDesc;
        pCacheInfo->Level = qzLvl;
        pCacheInfo->Size = qzSize;

        m_data->CacheInformation.append(pCacheInfo);
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
