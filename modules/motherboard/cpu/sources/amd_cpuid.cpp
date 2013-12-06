#include "../../include/hardware/amd_cpuid.h"

CAMDCpuID::CAMDCpuID(): m_data(0)
{
    m_data = new CpuIDInformation();
}

CAMDCpuID::~CAMDCpuID()
{
    if( 0 != m_data )
    {
        m_data->Instructions.clear();
        delete m_data;
        m_data = 0;
    }
}
