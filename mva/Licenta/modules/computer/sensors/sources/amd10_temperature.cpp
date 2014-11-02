#include "../headers/amd10_temperature.h"

UINT CAmd10CpuSensor::GetControllAddress(int nFunction, USHORT usDeviceID)
{
    UINT unAddress = 0;
    unAddress = (0 & 0xFF) << 8;
    unAddress |= ((AMD_BASE_DEVICE + 0) & 0x1F) << 3;
    unAddress |= (nFunction & 7);

    // check that we have the correct bus
    ULONG unDeviceVendor = 0;
    int nStatus = m_pDriver->ReadPCIConfiguration(unAddress, 0, &unDeviceVendor);
    if( Success != nStatus )
        return AMD_INVALID_ADDRESS;

    if( unDeviceVendor != (usDeviceID << 16 | AMD_VENDOR_ID) )
        return AMD_INVALID_ADDRESS;

    return unAddress;
}

CAmd10CpuSensor::CAmd10CpuSensor(int nFamily, int nModel, int nStepping, int nCoreCount)
{
    m_pDriver = new CSTKDriverWrapper;
    int nStatus = m_pDriver->Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    m_nFamily = nFamily;
    m_nModel = nModel;
    m_nStepping = nStepping;
    m_nCoreCount = nCoreCount;

    m_pTemps = new double[nCoreCount];
    memset(m_pTemps, 0, nCoreCount * sizeof(double));

    m_usControlDeviceID = 0;
}

CAmd10CpuSensor::~CAmd10CpuSensor()
{
    SAFE_DELETE(m_pTemps);

    if( m_pDriver )
    {
        int nStatus = m_pDriver->Destroy();
        CHECK_OPERATION_STATUS(nStatus);
        SAFE_DELETE(m_pDriver);
    }
}

int CAmd10CpuSensor::Initialize()
{
    switch( m_nFamily )
    {
    case 0x10:
        m_usControlDeviceID = FAMILY_10H_CONTROL_DEVICE_ID; break;
    case 0x11:
        m_usControlDeviceID = FAMILY_11H_CONTROL_DEVICE_ID; break;
    case 0x12:
        m_usControlDeviceID = FAMILY_12H_CONTROL_DEVICE_ID; break;
    case 0x14:
        m_usControlDeviceID = FAMILY_14H_CONTROL_DEVICE_ID; break;
    case 0x15:
        switch( m_nModel )
        {
        case 0x00:
            m_usControlDeviceID = FAMILY_15H_MODEL_00_CONTROL_DEVICE_ID; break;
        case 0x10:
            m_usControlDeviceID = FAMILY_15H_MODEL_10_CONTROL_DEVICE_ID; break;
        default:
            m_usControlDeviceID = 0;
        }
        break;
    case 0x16:
        switch( m_nModel & 0xF0 )
        {
        case 0x00:
            m_usControlDeviceID = FAMILY_16H_MODEL_00__CONTROL_DEVICE_ID; break;
        default:
            m_usControlDeviceID = 0;
        }
        break;
    default:
        m_usControlDeviceID = 0;
    }

    return Success;
}

int CAmd10CpuSensor::Destroy()
{
    return Success;
}

int CAmd10CpuSensor::Update()
{
    BOOL bResult = FALSE;
    DWORD_PTR dwProcAffinityMask = 0;
    DWORD_PTR dwSysAffinityMask = 0;
    DWORD_PTR dwCurrentMask = 1;

    ULONG ulValue = 0;
    int nStatus = Uninitialized;

    bResult = GetProcessAffinityMask(GetCurrentProcess(), &dwProcAffinityMask, &dwSysAffinityMask);
    if( FALSE == bResult )
        return Unsuccessful;

    for(int i = 0; i < m_nCoreCount; i++)
    {
        dwCurrentMask = (DWORD_PTR)1 << i;

        SetProcessAffinityMask(GetCurrentProcess(), dwCurrentMask);

        if( m_uiControlAddress == AMD_INVALID_ADDRESS )
            return Unsuccessful;

        nStatus = m_pDriver->ReadPCIConfiguration(m_uiControlAddress,
                                                  REPORTED_TEMPERATURE_CONTROL_REGISTER,
                                                  &ulValue);
        CHECK_OPERATION_STATUS_EX(nStatus);

        if( m_nFamily == 0x15 && (ulValue & 0x30000) == 0x30000 )
        {
            if( (m_nModel & 0xF0) == 0x00 )
            {
                m_pTemps[i] = ((ulValue >> 21) & 0x7FC) / 8.0f - 49; // 49 is temperature offset
                                                                        // specific for AMD
            }
            else
            {
                m_pTemps[i] = ((ulValue >> 21) & 0x7FF) / 8.0f - 49;
            }
        }
        else if( m_nFamily = 0x16 && ((ulValue & 0x30000) == 0x30000 || (ulValue & 0x80000) == 0x80000) )
        {
            m_pTemps[i] = ((ulValue >> 21) & 0x7FF) / 8.0f - 49;
        }
        else
        {
            m_pTemps[i] = ((ulValue >> 21) & 0x7FF) / 8.0f;
        }
    }

    SetProcessAffinityMask(GetCurrentProcess(), dwProcAffinityMask);

    return Success;
}

double *CAmd10CpuSensor::GetTemps()
{
    return m_pTemps;
}

QString CAmd10CpuSensor::GetCpuName()
{
    return "";
}

QString CAmd10CpuSensor::GetMicroArchitecture()
{
    return "Unknown";
}

int CAmd10CpuSensor::GetNumberOfCores()
{
    return m_nCoreCount;
}

int CAmd10CpuSensor::GetMultiplier()
{
    return 0;
}
