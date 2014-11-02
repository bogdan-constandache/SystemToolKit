#include "../headers/intel_temperature.h"

CIntelCpuSensor::CIntelCpuSensor(int nFamily, int nModel, int nStepping, int nCoreCount):
    m_pTemps( NULL ),
    m_nFamily( 0 ), m_nModel( 0 ), m_nStepping( 0 ), m_nCoreCount( 0 ),
    m_nMultiplier( 0 )
{
    m_nFamily = nFamily;
    m_nModel = nModel;
    m_nStepping = nStepping;
    m_nCoreCount = nCoreCount;

    m_pDriver = new CSTKDriverWrapper;
    int nStatus = m_pDriver->Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    m_pTemps = new double[m_nCoreCount];
}

CIntelCpuSensor::~CIntelCpuSensor()
{
    int nStatus = Uninitialized;

    SAFE_DELETE(m_pTemps);

    SAFE_DELETE(m_pTjMax);

    if (m_pDriver)
    {
        nStatus = m_pDriver->Destroy();
        CHECK_OPERATION_STATUS(nStatus);
        SAFE_DELETE(m_pDriver);
    }
}

void CIntelCpuSensor::InitializeTjMaxTable(int nValue)
{
    m_pTjMax = new double[m_nCoreCount];
    CHECK_ALLOCATION(m_pTjMax);

    for(int i = 0; i < m_nCoreCount; i++)
        m_pTjMax[i] = (double)nValue;
}

void CIntelCpuSensor::InitializeTjMaxTableFromMSR()
{
    m_pTjMax = new double[m_nCoreCount];
    CHECK_ALLOCATION(m_pTjMax);

    int nStatus = Uninitialized;
    ULONG ulEax = 0, ulEdx = 0;

    for(int i = 0; i < m_nCoreCount; i++)
    {
        nStatus = m_pDriver->ReadMsr(IA32_TEMPERATURE_TARGET, &ulEax, &ulEdx);
        if (Success == nStatus)
            m_pTjMax[i] = (ulEax >> 16) & 0xFF;
        else
            m_pTjMax[i] = 100;
    }
}

int CIntelCpuSensor::Initialize()
{
    int nStatus = Uninitialized;

    // compute microarchitecture and initialize tjmax array
    switch (m_nFamily)
    {
    case 0x06:
        switch (m_nModel)
        {
        case 0x0F:
            m_eMicroArch = CORE_IMA; // Intel Core 2
            switch (m_nStepping)
            {
            case 0x06:
                switch (m_nCoreCount)
                {
                case 2:
                    InitializeTjMaxTable(90); break;
                case 4:
                    InitializeTjMaxTable(100); break;
                default:
                    InitializeTjMaxTable(95); break;
                }

                break;
            case 0x0B:
                InitializeTjMaxTable(100); break;
            case 0x0D:
                InitializeTjMaxTable(95); break;
            default:
                InitializeTjMaxTable(95); break;
            }

            break;
        case 0x17:
            m_eMicroArch = CORE_IMA;
            InitializeTjMaxTable(100);
            break;
        case 0x1C:
            m_eMicroArch = ATOM_IMA;
            switch (m_nStepping)
            {
            case 0x02:
                InitializeTjMaxTable(90); break;
            case 0x0A:
                InitializeTjMaxTable(100); break;
            default:
                InitializeTjMaxTable(90); break;
            }

            break;
        case 0x1A:
        case 0x1E:
        case 0x1F:
        case 0x25:
        case 0x2C:
        case 0x2E:
        case 0x2F:
             m_eMicroArch = NEHALEM_IMA;
             InitializeTjMaxTableFromMSR();
             break;
        case 0x2A:
        case 0x2D:
            m_eMicroArch = SANDY_BRIDGE_IMA;
            InitializeTjMaxTableFromMSR();
            break;
        case 0x3A:
        case 0x3E:
            m_eMicroArch = IVY_BRIDGE_IMA;
            InitializeTjMaxTableFromMSR();
            break;
        case 0x3C:
        case 0x45:
        case 0x46:
            m_eMicroArch = HASWELL_IMA;
            InitializeTjMaxTableFromMSR();
            break;
        default:
            m_eMicroArch = UNKNOWN_IMA;
            InitializeTjMaxTable(100);
            break;
        }

        break;
    case 0x0F:
        switch (m_nModel)
        {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x06:
            m_eMicroArch = NETBURST_IMA;
            InitializeTjMaxTable(100);
            break;
        default:
            m_eMicroArch = UNKNOWN_IMA;
            InitializeTjMaxTable(100);
            break;
        }

        break;
    default:
        m_eMicroArch = UNKNOWN_IMA;
        InitializeTjMaxTable(100);
        break;
    }

    ULONG uiEax = 0, uiEdx = 0;

    // read multiplier
    switch (m_eMicroArch)
    {
    case NETBURST_IMA:
    case ATOM_IMA:
    case CORE_IMA:
        nStatus = m_pDriver->ReadMsr(IA32_PERF_STATUS_MSR, &uiEax, &uiEdx);
        if (Success != nStatus)
            return nStatus;
        m_nMultiplier = ((uiEdx >> 8) & 0x1F) + 0.5 * ((uiEdx >> 14) & 1);
        break;
    case NEHALEM_IMA:
    case SANDY_BRIDGE_IMA:
    case IVY_BRIDGE_IMA:
    case HASWELL_IMA:
        nStatus = m_pDriver->ReadMsr(MSR_PLATFORM_INFO, &uiEax, &uiEdx);
        if (Success != nStatus)
            return nStatus;
        m_nMultiplier = (uiEax >> 8) & 0xFF;
        break;
    default:
        m_nMultiplier = 0;
        break;
   }

    return Success;
}

int CIntelCpuSensor::Destroy()
{
    return Success;
}

int CIntelCpuSensor::Update()
{
    int nStatus = Uninitialized;
    BOOL bResult = FALSE;
    DWORD_PTR dwProcAffinityMask = 0;
    DWORD_PTR dwSysAffinityMask = 0;
    DWORD_PTR dwCurrentMask = 1;
    ULONG uiEax = 0, uiEdx = 0;

    bResult = GetProcessAffinityMask(GetCurrentProcess(), &dwProcAffinityMask, &dwSysAffinityMask);
    if (FALSE == bResult)
        return Unsuccessful;

    for(int i = 0; i < m_nCoreCount; i++)
    {
        dwCurrentMask = (DWORD_PTR)1 << i;

        SetProcessAffinityMask(GetCurrentProcess(), dwCurrentMask);

        nStatus = m_pDriver->ReadMsr(IA32_THERM_STATUS_MSR, &uiEax, &uiEdx);
        CHECK_OPERATION_STATUS(nStatus);

        double dDeltaT = ((uiEax & 0x007F0000) >> 16);
        double dTjMax = m_pTjMax[i];
        double dTSlope = 1;
        m_pTemps[i] = dTjMax - dTSlope * dDeltaT;
    }

    SetProcessAffinityMask(GetCurrentProcess(), dwProcAffinityMask);

    return Success;
}

double *CIntelCpuSensor::GetTemps()
{
    return m_pTemps;
}

QString CIntelCpuSensor::GetCpuName()
{
    return m_qzCpuName;
}

QString CIntelCpuSensor::GetMicroArchitecture()
{
    switch (m_eMicroArch)
    {
    case UNKNOWN_IMA:
        return "Unknown";
    case i386_IMA:
        return "i386";
    case i486_IMA:
        return "i486";
    case P5_IMA:
        return "Original Pentium microprocessor";
    case P6_IMA:
        return "Pentium Pro";
    case PENTIUM_M_IMA:
        return "Pentium III";
    case NETBURST_IMA:
        return "Pentium 4";
    case CORE_IMA:
        return "Intel Core 2";
    case ATOM_IMA:
        return "Intel Atom";
    case PENRYN_IMA:
        return "Intel Core 2 (45nm)";
    case NEHALEM_IMA:
        return "Nehalem";
    case SANDY_BRIDGE_IMA:
        return "Sandy Bridge";
    case IVY_BRIDGE_IMA:
        return "Ivy Bridge";
    case HASWELL_IMA:
        return "Haswell";
    case BROADWELL_IMA:
        return "Broadwell";
    case SKYLAKE_IMA:
        return "Skylake";
    case CANNON_LAKE_IMA:
        return "Cannonlake";
    case LARRA_BEE_IMA:
        return "Larrabee";
    }

    return "";
}

int CIntelCpuSensor::GetNumberOfCores()
{
    return m_nCoreCount;
}

int CIntelCpuSensor::GetMultiplier()
{
    return m_nMultiplier;
}
