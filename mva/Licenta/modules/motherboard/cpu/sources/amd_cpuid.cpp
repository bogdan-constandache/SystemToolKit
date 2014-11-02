#include "../headers/amd_cpuid.h"

CAMDCpuID::CAMDCpuID(): m_data(0), m_pDataModel(NULL)
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
        qList << new QStandardItem(pInf->Type == "" ? pInf->Level : pInf->Level + ", " + pInf->Type) << new QStandardItem(pInf->Size + ", " + pInf->Descriptor);
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

CAMDCpuID::~CAMDCpuID()
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

QStandardItemModel *CAMDCpuID::GetCPUIDInformations()
{
    return m_pDataModel;
}

int CAMDCpuID::Initialize()
{
    int CPUInfo[4];
    int Eax = 0x80000000;
    int RegisterValue = 0; int nAux = 0, nTemp = 0;
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

    nAux = (CPUInfo[0] >> 4) & 0xF;
    nTemp = (CPUInfo[0] >> 16) & 0xF;
    RegisterValue = (nTemp << 4) | nAux;
    m_data->Model = QString().setNum(RegisterValue);
    m_data->ExtendedModel = QString().setNum(nTemp);

    nAux = (CPUInfo[0] >> 8) & 0xF;
    nTemp = (CPUInfo[0] >> 20) & 0xFF;
    RegisterValue = nAux + nTemp;
    m_data->Family = QString().setNum(RegisterValue);
    m_data->ExtendedFamily = QString().setNum(nTemp);

    // getting processor type EAX[12:13]
    m_data->ProcessorType = QString("Original OEM Processor");

    // getting instructions from ECX register
    if((CPUInfo[2]) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE3", "SSE3 new instructions"));
    if((CPUInfo[2] >> 1) & 0x1)
        m_data->Instructions.append(new CpuInstruction("PCLMULQDQ", "Carryless Multiplication"));
    if((CPUInfo[2] >> 3) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MONITOR", "Monitor/MWAIT"));
    if((CPUInfo[2] >> 9) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSSE3", "SSSE3 Extensions"));
    if((CPUInfo[2] >> 12) & 0x1)
        m_data->Instructions.append(new CpuInstruction("FMA", "Fused Multiply Add"));
    if((CPUInfo[2] >> 13) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CMPXCHG16B", "CMPXCHG16B Available"));
    if((CPUInfo[2] >> 19) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE4.1", "SSE4.1 Extensions"));
    if((CPUInfo[2] >> 20) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE4.2", "SSE4.2 Extensions"));
    if((CPUInfo[2] >> 23) & 0x1)
        m_data->Instructions.append(new CpuInstruction("POPCNT", "POPCNT Instruction"));
    if((CPUInfo[2] >> 25) & 0x1)
        m_data->Instructions.append(new CpuInstruction("AES", "AES Instruction"));
    if((CPUInfo[2] >> 26) & 0x1)
        m_data->Instructions.append(new CpuInstruction("XSAVE", "XSAVE Instruction"));
    if((CPUInfo[2] >> 27) & 0x1)
        m_data->Instructions.append(new CpuInstruction("OSXSAVE", "OSXSAVE Instruction"));
    if((CPUInfo[2] >> 28) & 0x1)
        m_data->Instructions.append(new CpuInstruction("AVX", "AVX Instruction"));
    if((CPUInfo[2] >> 29) & 0x1)
        m_data->Instructions.append(new CpuInstruction("F16C", "F16C Instruction"));

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
        m_data->Instructions.append(new CpuInstruction("CMPXCHG8B", "CMPXCHG8B Instruction"));
    if((CPUInfo[3] >> 9) & 0x1)
        m_data->Instructions.append(new CpuInstruction("APIC", "APIC On-Chip"));
    if((CPUInfo[3] >> 11) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SysEnterSysExit", "SYSENTER and SYSEXIT Instructions"));
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
    if((CPUInfo[3] >> 19) & 0x1)
        m_data->Instructions.append(new CpuInstruction("CLFSH", "CLFLUSH Instruction"));
    if((CPUInfo[3] >> 23) & 0x1)
        m_data->Instructions.append(new CpuInstruction("MMX", "Intel MMX Technology"));
    if((CPUInfo[3] >> 24) & 0x1)
        m_data->Instructions.append(new CpuInstruction("FXSR", "FXSAVE and FXRSTOR Instructions"));
    if((CPUInfo[3] >> 25) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE", "SSE Extensions"));
    if((CPUInfo[3] >> 26) & 0x1)
        m_data->Instructions.append(new CpuInstruction("SSE2", "SSE2 Extensions"));
    if((CPUInfo[3] >> 28) & 0x1)
        m_data->Instructions.append(new CpuInstruction("HTT", "Max APIC IDs reserved field is Valid"));

    // Cache information
    __cpuid(CPUInfo, 0x80000005);

    // ECX - L1 data cache
    QString qsDescriptor = "";
    CacheInformation *pCache = new CacheInformation;
    pCache->Level = "L1";
    pCache->Type = "Data cache";
    pCache->Size = QString().sprintf("%d, KBytes", ((CPUInfo[2] >> 24) & 0xFF) );
    switch( ((CPUInfo[2] >> 16) & 0xFF) )
    {
    case 0:
        qsDescriptor = "Reserved"; break;
    case 0xFF:
        qsDescriptor = "Fully associative, "; break;
    default:
        qsDescriptor.sprintf("%d-way set associative, ", ((CPUInfo[2] >> 16) & 0xFF));
    }

    qsDescriptor += QString().sprintf("%d, bytes line size", (CPUInfo[2] & 0xFF));
    pCache->Descriptor = qsDescriptor;
    m_data->CacheInformation.append(pCache);

    // EDX - L1 instruction cache
    pCache = new CacheInformation;
    pCache->Level = "L1";
    pCache->Type = "Instruction cache";
    pCache->Size = QString().sprintf("%d, KBytes", ((CPUInfo[3] >> 24) & 0xFF) );
    switch( ((CPUInfo[3] >> 16) & 0xFF) )
    {
    case 0:
        qsDescriptor = "Reserved"; break;
    case 0xFF:
        qsDescriptor = "Fully associative, "; break;
    default:
        qsDescriptor.sprintf("%d-way set associative, ", ((CPUInfo[3] >> 16) & 0xFF));
    }

    qsDescriptor += QString().sprintf("%d, bytes line size", (CPUInfo[3] & 0xFF));
    pCache->Descriptor = qsDescriptor;
    m_data->CacheInformation.append(pCache);


    __cpuid(CPUInfo, 0x80000006);

    // ECX - L2 cache
    pCache = new CacheInformation;
    pCache->Level = "L2";
    pCache->Size = QString().sprintf("%d KBytes", ((CPUInfo[2] >> 16) & 0xFFFF) );
    switch( ((CPUInfo[2] >> 12) & 0xF) )
    {
    case 0:
        qsDescriptor = "Disabled"; break;
    case 1:
    case 2:
    case 4:
    case 8:
        qsDescriptor.sprintf("%d-way set associative, ", ((CPUInfo[2] >> 12) & 0xF)); break;
    default:
        qsDescriptor = "Reserved";
    }

    qsDescriptor += QString().sprintf("%d, bytes line size", (CPUInfo[2] & 0xFF));
    pCache->Descriptor = qsDescriptor;
    m_data->CacheInformation.append(pCache);

    // EDX - L3 cache
    if( 0 == (((CPUInfo[3] >> 18) & 0x3FFF) * 512) )
        goto Exit;
    pCache = new CacheInformation;
    pCache->Level = "L3";
    pCache->Size = QString().sprintf("%d, KBytes", ((CPUInfo[3] >> 18) & 0x3FFF) * 512 );
    switch( ((CPUInfo[3] >> 12) & 0xF) )
    {
    case 0:
        qsDescriptor = "Disabled"; break;
    case 1:
        qsDescriptor = "1-way set associative, "; break;
    case 2:
        qsDescriptor = "2-way set associative, "; break;
    case 4:
        qsDescriptor = "4-way set associative, "; break;
    case 6:
        qsDescriptor = "8-way set associative, "; break;
    case 8:
        qsDescriptor = "16-way set associative, "; break;
    case 0xA:
        qsDescriptor = "32-way set associative, "; break;
    case 0xB:
        qsDescriptor = "48-way set associative, "; break;
    case 0xC:
        qsDescriptor = "64-way set associative, "; break;
    case 0xD:
        qsDescriptor = "96-way set associative, "; break;
    case 0xE:
        qsDescriptor = "128-way set associative, "; break;
    case 0xF:
        qsDescriptor = "Fully associative, "; break;
    default:
        qsDescriptor = "Reserved";
    }

    qsDescriptor += QString().sprintf("%d, bytes line size", (CPUInfo[3] & 0xFF));
    pCache->Descriptor = qsDescriptor;
    m_data->CacheInformation.append(pCache);

Exit:

    return Success;
}
