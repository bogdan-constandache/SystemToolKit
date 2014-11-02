#include "../headers/cpuid_manager.h"

CCPUIDManager::CCPUIDManager(): m_bIsAmd(false), m_bIsIntel(false)/*, m_pCpuID(NULL)/*,*/
//    m_pIntel(NULL), m_pAmd(NULL)
{
    OnDetectInstalledCPU();
}

CCPUIDManager::~CCPUIDManager()
{
//    if( m_bIsIntel )
//        SAFE_DELETE(m_pIntel);
//    if( m_bIsAmd )
//        SAFE_DELETE(m_pAmd);
    SAFE_DELETE(m_pCpuID);
}

QStandardItemModel *CCPUIDManager::GetCPUIDDataModel()
{
//    if( m_bIsIntel && m_pIntel )
//        return m_pIntel->GetCPUIDInformations();
//    else if( m_bIsAmd && m_pAmd )
//        return m_pAmd->GetCPUIDInformations();

    if( m_pCpuID )
        return m_pCpuID->GetCPUIDInformations();

    return 0;
}

void CCPUIDManager::OnDetectInstalledCPU()
{
    int CPUInfo[4] = {0};
    std::string RegisterString;

    // getting manufacturer
    __cpuid(CPUInfo, 0);
    RegisterString += std::string((char*)&CPUInfo[1], 4);
    RegisterString += std::string((char*)&CPUInfo[3], 4);
    RegisterString += std::string((char*)&CPUInfo[2], 4);

    QString qsManufacturer = QString::fromStdString(RegisterString);

    if( qsManufacturer == "GenuineIntel" )
    {
        m_bIsIntel = true;
        m_bIsAmd = false;
        m_pCpuID = new CIntelCpuID();
    }
    else if( qsManufacturer == "AuthenticAMD" )
    {
        m_bIsAmd = true;
        m_bIsIntel = false;
        m_pCpuID = new CAMDCpuID();
    }
}
