#include "../headers/it87.h"


CIT87::CIT87(Chip eChip, USHORT usAddress)
{
    this->usAddress = usAddress;
    this->eChip = eChip;
    this->usAddressReg = usAddress + ADDRESS_REGISTER_OFFSET;
    this->usDataReg = usAddress + DATA_REGISTER_OFFSET;

    m_pDriver = new CSTKDriverWrapper;
    m_pDriver->Initialize();
}

CIT87::~CIT87()
{
    m_pDriver->Destroy();
    delete m_pDriver;
}

int CIT87::Initialize()
{
    int nStatus = Uninitialized;
    BYTE bVendorID = 0;
    BYTE bConfigReg = 0;

    nStatus = m_pDriver->WriteIoPortByte(usAddressReg, VENDOR_ID_REGISTER);
    nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVendorID);
    if (Success != nStatus || bVendorID != VENDOR_ID)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    // Bit 0x10 of the configuration register should always be 1
    nStatus = m_pDriver->WriteIoPortByte(usAddressReg, CONFIGURATION_REGISTER);
    nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bConfigReg);
    if (Success != nStatus || bConfigReg == 0)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    if (eChip == IT8721F || eChip == IT8728F || eChip == IT8771E ||
            eChip == IT8772E)
    {
        dVoltageGain = 0.012f;
    }
    else
    {
        dVoltageGain = 0.016f;
    }

    // older revisions don't have 16-bit fan counter
    if (eChip == IT8705F || eChip == IT8712F)
        bHas16BitFanCounter = false;
    else
        bHas16BitFanCounter = true;

    return Success;
}

int CIT87::Destroy()
{
    return Success;
}

int CIT87::Update()
{
    int nStatus = Uninitialized;
    BYTE bVal = 0;
    int nVal = 0;

    for (int i = 0; i < 9; i++)
    {
        nStatus = m_pDriver->WriteIoPortByte(usAddressReg, (VOLTAGE_BASE_REGISTER + i));
        nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVal);
        if (Success != nStatus)
            continue;

        if (bVal > 0)
            m_pVoltages[i] = bVal * dVoltageGain;
        else
            m_pVoltages[i] = 0;
    }

    for (int i = 0; i < 3; i++)
    {
        nStatus = m_pDriver->WriteIoPortByte(usAddressReg, (TEMP_BASE_REGISTER + i));
        nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVal);
        if (Success != nStatus)
            continue;

        if (bVal > 0 && bVal < 100)
            m_pTemps[i] = bVal;
        else
            m_pTemps[i] = 0;
    }

    if (bHas16BitFanCounter)
        for (int i = 0; i < 3; i++)
        {
            nStatus = m_pDriver->WriteIoPortByte(usAddressReg, (FAN_SPEED_REG[i]));
            nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVal);
            if (Success != nStatus)
                continue;
            nVal = bVal;

            nStatus = m_pDriver->WriteIoPortByte(usAddressReg, (FAN_SPEED_EXT_REG[i]));
            nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVal);
            if (Success != nStatus)
                continue;
            nVal |= bVal;

            if (nVal > 0x3f)
                m_pFans[i] = (nVal < 0xFFFF) ? 1.35e6f / (nVal * 2) : 0;
            else
                m_pFans[i] = 0;
        }
    else
        for (int i = 0; i < 3; i++)
        {
            nStatus = m_pDriver->WriteIoPortByte(usAddressReg, (FAN_SPEED_REG[i]));
            nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVal);
            if (Success != nStatus)
                continue;
            nVal = bVal;

            int nDivisor = 2;
            if (i < 2)
            {
                nStatus = m_pDriver->WriteIoPortByte(usAddressReg, (FAN_SPEED_DIVISOR_REGISTER));
                nStatus = m_pDriver->ReadIoPortByte(usDataReg, &bVal);
                if (Success != nStatus)
                    continue;
                nDivisor = 1 << ((bVal >> (3 * i)) & 0x7);
            }

            if (nVal > 0)
                m_pFans[i] = (nVal < 0xFFFF) ? 1.35e6f / (nVal * nDivisor) : 0;
            else
                m_pFans[i] = 0;
        }

    return Success;
}

double *CIT87::GetTemps()
{
    return m_pTemps;
}

double *CIT87::GetFanSpeeds()
{
    return m_pFans;
}

double *CIT87::GetVoltages()
{
    return m_pVoltages;
}


