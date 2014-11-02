#include "../headers/amd0f_temperature.h"

CAmd0FCpuSensor::CAmd0FCpuSensor(int nFamily, int nModel, int nStepping, int nCoreCount):
    m_pDriver(NULL)
{
    m_pDriver = new CSTKDriverWrapper;
    int nStatus = m_pDriver->Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    m_dOffset = -49.0f;

    m_nFamily = nFamily;
    m_nModel = nModel;
    m_nStepping = nStepping;
    m_nCoreCount = nCoreCount;

    m_pTemps = new double[nCoreCount];
    memset(m_pTemps, 0, nCoreCount * sizeof(double));
}

CAmd0FCpuSensor::~CAmd0FCpuSensor()
{
    SAFE_DELETE(m_pTemps);

    if( m_pDriver )
    {
        int nStatus = m_pDriver->Destroy();
        CHECK_OPERATION_STATUS(nStatus);
        SAFE_DELETE(m_pDriver);
    }
}

UINT CAmd0FCpuSensor::GetControllAddress(int nFunction)
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

    if( unDeviceVendor != (AMD0F_DEVICE_ID << 16 | AMD_VENDOR_ID) )
        return AMD_INVALID_ADDRESS;

    return unAddress;
}

int CAmd0FCpuSensor::Initialize()
{
    // AM2+ 65nm need +21 offset
    if( m_nModel >= 0x69 && m_nModel != 0xC1 && m_nModel != 0x6C && m_nModel != 0x7C )
        m_dOffset += 21;

    if( m_nModel < 40 )
    {
        m_bThermalCoreSelectionCPU0 = 0x0;
        m_bThermalCoreSelectionCPU1 = 0x4;
    }
    else
    {
        m_bThermalCoreSelectionCPU0 = 0x4;
        m_bThermalCoreSelectionCPU1 = 0x0;
    }

    return Success;
}

int CAmd0FCpuSensor::Destroy()
{
    return Success;
}

int CAmd0FCpuSensor::Update()
{    
    int nStatus = Uninitialized;
    UINT unAddress = AMD_INVALID_ADDRESS;
    ULONG unTempValue = 0;

    for( int i = 0; i < m_nCoreCount; i++ )
    {
        unAddress = GetControllAddress(AMD_CONTROL_FUNCTION); // 3 - control function
        if( unAddress == AMD_INVALID_ADDRESS )
            continue;

        nStatus = m_pDriver->WritePCIConfiguration(unAddress, AMD0F_THERM_STATUS_REGISTER,
                                                   i > 0 ? m_bThermalCoreSelectionCPU1 : m_bThermalCoreSelectionCPU0);
        if( Success != nStatus )
        {
            m_pTemps[i] = 0;
            continue;
        }

        nStatus = m_pDriver->ReadPCIConfiguration(unAddress, AMD0F_THERM_STATUS_REGISTER,
                                                  &unTempValue);
        if( Success != nStatus )
        {
            m_pTemps[i] = 0;
            continue;
        }

        m_pTemps[i] = (unTempValue >> 16) & 0xFF;
        m_pTemps[i] += m_dOffset;
    }
    return Success;
}

double *CAmd0FCpuSensor::GetTemps()
{
    return m_pTemps;
}

QString CAmd0FCpuSensor::GetCpuName()
{
    return "";
}

QString CAmd0FCpuSensor::GetMicroArchitecture()
{
    return "Unknown";
}

int CAmd0FCpuSensor::GetNumberOfCores()
{
    return m_nCoreCount;
}

int CAmd0FCpuSensor::GetMultiplier()
{
    return 0;
}
