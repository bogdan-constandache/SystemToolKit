#include "../headers/factory_cpu_sensor.h"


FactoryCpuSensor::FactoryCpuSensor():
    m_pDriver( NULL ),
    m_pCpuSensor( NULL )
{
    m_pDriver = new CSTKDriverWrapper;
    CHECK_ALLOCATION(m_pDriver);

    int nStatus = m_pDriver->Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    this->DetectCpuSensor();
}

FactoryCpuSensor::~FactoryCpuSensor()
{
    int nStatus = Uninitialized;

    if (m_pCpuSensor)
    {
        nStatus = m_pCpuSensor->Destroy();
        CHECK_OPERATION_STATUS(nStatus);
        SAFE_DELETE(m_pCpuSensor);
    }

    nStatus = m_pDriver->Destroy();
    CHECK_OPERATION_STATUS(nStatus);
    SAFE_DELETE(m_pDriver);
}

bool FactoryCpuSensor::DetectCpuSensor()
{
    bool bResult = false;

    int CPUIDINF[4];
    std::string RegisterString;
    SYSTEM_INFO SystemInfo;

    int nStepping = 0, nModel = 0, nFamily = 0, nModelExt = 0, nFamilyExt = 0;
    int nDispModel = 0, nDispFamily = 0;

    __cpuid(CPUIDINF, 0);
    RegisterString += std::string((char*)&CPUIDINF[1], 4);
    RegisterString += std::string((char*)&CPUIDINF[3], 4);
    RegisterString += std::string((char*)&CPUIDINF[2], 4);

    QString qzManufacturer = QString().fromStdString(RegisterString);
    if ("GenuineIntel" != qzManufacturer)
        if ("AuthenticAMD" != qzManufacturer)
        {
            bResult = false;
            return bResult;
        }

    __cpuid(CPUIDINF, 1);
    nStepping = CPUIDINF[0] & 0xF;
    nModel = (CPUIDINF[0] >> 4) & 0xF;
    nFamily = (CPUIDINF[0] >> 8) & 0xF;
    nModelExt = (CPUIDINF[0] >> 16) & 0xF;
    nFamilyExt = (CPUIDINF[0] >> 20) & 0xFF;

    GetNativeSystemInfo(&SystemInfo);

    if (0xF != nFamily)
        nDispFamily = nFamily;
    else
        nDispFamily = nFamilyExt + nFamily;

    if (0x06 == nFamily || 0xF == nFamily)
        nDispModel = (nModelExt << 4) + nModel;
    else
        nDispModel = nModel;

//    ***INTEL***
//    IF Family_ID ≠ 0FH
//    THEN DisplayFamily = Family_ID;
//    ELSE DisplayFamily = Extended_Family_ID + Family_ID;
//    (* Right justify and zero-extend 4-bit field. *)
//    FI;

//    IF (Family_ID = 06H or Family_ID = 0FH)
//    THEN DisplayModel = (Extended_Model_ID « 4) + Model_ID;
//    (* Right justify and zero-extend 4-bit field; display Model_ID as HEX field.*)
//    ELSE DisplayModel = Model_ID;
//    FI;

    if ("GenuineIntel" == qzManufacturer )
        m_pCpuSensor = new CIntelCpuSensor(nDispFamily, nDispModel, nStepping, SystemInfo.dwNumberOfProcessors);

    if ("AuthenticAMD" == qzManufacturer )
    {
        switch (nFamily) // or nDispFamily
        {
        case 0x0F:
            m_pCpuSensor = new CAmd0FCpuSensor(nDispFamily, nDispModel, nStepping, SystemInfo.dwNumberOfProcessors);
            break;
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x14:
        case 0x15:
        case 0x16:
            m_pCpuSensor = new CAmd10CpuSensor();
            break;
        default:
            return false;
        }
    }

    return true;
}

ICPUSensor *FactoryCpuSensor::GetCpuSensor()
{
    return m_pCpuSensor;
}

int FactoryCpuSensor::DestroySensor()
{
    int nStatus = m_pCpuSensor->Destroy();
    SAFE_DELETE(m_pCpuSensor);

    return nStatus;
}
