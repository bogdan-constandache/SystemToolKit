#include "../headers/w836xx.h"

CW836XX::CW836XX(Chip eChip, USHORT usAddress, BYTE bRevision)
{
    m_eChip = eChip;
    this->usAddress = usAddress;
    this->bRevision = bRevision;

    m_bTempReg = new BYTE[3];
    m_bTempReg[0] = 0x50;
    m_bTempReg[1] = 0x50;
    m_bTempReg[0] = 0x27;

    m_bTempBank = new BYTE[3];
    m_bTempBank[0] = 1;
    m_bTempBank[1] = 2;
    m_bTempBank[2] = 0;

    m_bFanTachoReg = new BYTE[5];
    m_bFanTachoReg[0] = 0x28;
    m_bFanTachoReg[1] = 0x29;
    m_bFanTachoReg[2] = 0x2A;
    m_bFanTachoReg[3] = 0x3F;
    m_bFanTachoReg[4] = 0x53;

    m_bFanTachoBank = new BYTE[5];
    m_bFanTachoBank[0] = 0;
    m_bFanTachoBank[1] = 0;
    m_bFanTachoBank[2] = 0;
    m_bFanTachoBank[3] = 0;
    m_bFanTachoBank[4] = 5;

    m_bFanBitReg = new BYTE[5];
    m_bFanBitReg[0] = 0x47;
    m_bFanBitReg[1] = 0x4B;
    m_bFanBitReg[2] = 0x4C;
    m_bFanBitReg[3] = 0x59;
    m_bFanBitReg[4] = 0x5D;

    m_bFanDivBit0 = new BYTE[5];
    m_bFanDivBit0[0] = 36;
    m_bFanDivBit0[1] = 38;
    m_bFanDivBit0[2] = 30;
    m_bFanDivBit0[3] = 8;
    m_bFanDivBit0[4] = 10;

    m_bFanDivBit1 = new BYTE[5];
    m_bFanDivBit1[0] = 37;
    m_bFanDivBit1[1] = 39;
    m_bFanDivBit1[2] = 31;
    m_bFanDivBit1[3] = 9;
    m_bFanDivBit1[4] = 11;

    m_bFanDivBit2 = new BYTE[5];
    m_bFanDivBit2[0] = 5;
    m_bFanDivBit2[1] = 6;
    m_bFanDivBit2[2] = 7;
    m_bFanDivBit2[3] = 23;
    m_bFanDivBit2[4] = 15;

    m_pDriver = new CSTKDriverWrapper;
    CHECK_OPERATION_STATUS(m_pDriver->Initialize());
}

CW836XX::~CW836XX()
{
    Destroy();
}

int CW836XX::ReadByteFromBank(BYTE bBank, BYTE bReg, BYTE *bResult)
{
    int nStatus = Uninitialized;

    nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_ADDRESS_REGISTER_OFFSET,
                                         W836_CHIP_BANK_SELECT_REGISTER);
    if( Success != nStatus )
    {
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_DATA_REGISTER_OFFSET, bBank);
    if( Success != nStatus )
    {
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_ADDRESS_REGISTER_OFFSET, bReg);
    if( Success != nStatus )
    {
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    nStatus = m_pDriver->ReadIoPortByte(usAddress + W836_CHIP_DATA_REGISTER_OFFSET, bResult);

    return nStatus;
}

int CW836XX::Initialize()
{
    int nStatus = Uninitialized;
    BYTE bVal = 0;
    USHORT usVendorID = 0;

    // Check vendor ID;
    nStatus = ReadByteFromBank(0x80, W836_CHIP_VENDOR_ID_REGISTER, &bVal);
    if( Success != nStatus )
        return nStatus;

    usVendorID = bVal;
    usVendorID = usVendorID << 8;

    nStatus = ReadByteFromBank(0, W836_CHIP_VENDOR_ID_REGISTER, &bVal);
    if( Success != nStatus )
        return nStatus;

    usVendorID = usVendorID | bVal;

    if( usVendorID != 0x5CA3 )
        return Unsuccessful;

    switch (m_eChip)
    {
    case W83667HG:
    case W83667HGB:
        nStatus = ReadByteFromBank(0, W836_CHIP_TEMPERATURE_SOURCE_FLAG, &bVal);
        if( Success != nStatus )
            return nStatus;
        m_bPCITemps[0] = (bVal & 0x04) != 0;
        m_bPCITemps[1] = (bVal & 0x40) != 0;
        m_bPCITemps[2] = false;
        break;
    case W83627DHG:
    case W83627DHGP:
        nStatus = ReadByteFromBank(0, W836_CHIP_TEMPERATURE_SOURCE_FLAG, &bVal);
        if( Success != nStatus )
            return nStatus;
        m_bPCITemps[0] = (bVal & 0x07) != 0;
        m_bPCITemps[1] = (bVal & 0x70) != 0;
        m_bPCITemps[2] = false;
        break;
    default:
        m_bPCITemps[0] = false;
        m_bPCITemps[1] = false;
        m_bPCITemps[2] = false;
        break;
    }

    switch (m_eChip) {
    case W83627EHF:
        m_pVoltages = new VoltageReading[10];
        m_bVoltageReg = new BYTE[10];
        m_bVoltageBank = new BYTE[10];
        nVoltages = 10;

        dVoltageGain = 0.008f;

        m_bVoltageReg[0] = 0x20; m_bVoltageReg[1] = 0x21; m_bVoltageReg[2] = 0x22;
        m_bVoltageReg[3] = 0x23; m_bVoltageReg[4] = 0x24; m_bVoltageReg[5] = 0x25;
        m_bVoltageReg[6] = 0x26, m_bVoltageReg[7] = 0x50; m_bVoltageReg[8] = 0x51;
        m_bVoltageReg[9] = 0x52;

        m_bVoltageBank[0] = 0; m_bVoltageBank[1] = 0; m_bVoltageBank[2] = 0;
        m_bVoltageBank[3] = 0; m_bVoltageBank[4] = 0; m_bVoltageBank[5] = 0;
        m_bVoltageBank[6] = 0; m_bVoltageBank[7] = 5; m_bVoltageBank[8] = 5;
        m_bVoltageBank[9] = 5;

//        m_bVoltageReg = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x50, 0x51, 0x52};
//        m_bVoltageBank = {0, 0, 0, 0, 0, 0, 0, 5, 5, 5};

        break;
    case W83627DHG:
    case W83627DHGP:
    case W83667HG:
    case W83667HGB:
        m_pVoltages = new VoltageReading[9];
        m_bVoltageReg = new BYTE[9];
        m_bVoltageBank = new BYTE[9];
        nVoltages = 9;

        dVoltageGain = 0.008f;

        m_bVoltageReg[0] = 0x20; m_bVoltageReg[1] = 0x21; m_bVoltageReg[2] = 0x22;
        m_bVoltageReg[3] = 0x23; m_bVoltageReg[4] = 0x24; m_bVoltageReg[5] = 0x25;
        m_bVoltageReg[6] = 0x26, m_bVoltageReg[7] = 0x50; m_bVoltageReg[8] = 0x51;

        m_bVoltageBank[0] = 0; m_bVoltageBank[1] = 0; m_bVoltageBank[2] = 0;
        m_bVoltageBank[3] = 0; m_bVoltageBank[4] = 0; m_bVoltageBank[5] = 0;
        m_bVoltageBank[6] = 0; m_bVoltageBank[7] = 5; m_bVoltageBank[8] = 5;

//        m_bVoltageReg = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x50, 0x51};
//        m_bVoltageBank = {0, 0, 0, 0, 0, 0, 0, 5, 5};

        break;
    case W83627HF:
    case W83627THF:
    case W83687THF:
        m_pVoltages = new VoltageReading[7];
        m_bVoltageReg = new BYTE[7];
        m_bVoltageBank = new BYTE[7];
        nVoltages = 7;

        dVoltageGain = 0.016f;

        m_bVoltageReg[0] = 0x20; m_bVoltageReg[1] = 0x21; m_bVoltageReg[2] = 0x22;
        m_bVoltageReg[3] = 0x23; m_bVoltageReg[4] = 0x24; m_bVoltageReg[5] = 0x50;
        m_bVoltageReg[6] = 0x51;

        m_bVoltageBank[0] = 0; m_bVoltageBank[1] = 0; m_bVoltageBank[2] = 0;
        m_bVoltageBank[3] = 0; m_bVoltageBank[4] = 0; m_bVoltageBank[5] = 5;
        m_bVoltageBank[6] = 5;

        break;
    }

    return Success;
}

int CW836XX::Destroy()
{
    m_pDriver->Destroy();
    SAFE_DELETE(m_pDriver);

    SAFE_DELETE(m_pVoltages);
    SAFE_DELETE(m_bVoltageBank);
    SAFE_DELETE(m_bVoltageReg);

    SAFE_DELETE(m_bTempBank);
    SAFE_DELETE(m_bTempReg);

    SAFE_DELETE(m_bFanTachoReg);
    SAFE_DELETE(m_bFanTachoBank);
    SAFE_DELETE(m_bFanBitReg);
    SAFE_DELETE(m_bFanDivBit0);
    SAFE_DELETE(m_bFanDivBit1);
    SAFE_DELETE(m_bFanDivBit2);

    return Success;
}

int CW836XX::Update()
{
    int nStatus = Uninitialized;
    BYTE bVal = 0;
    int nValue = 0;
    double dTempVal = 0.0;

    // temperatures
    for( int i = 0; i < 3; i++)
    {
        nStatus = ReadByteFromBank(m_bTempBank[i], m_bTempReg[i], &bVal);
        if( Success != nStatus )
            return nStatus;

        nValue = bVal;
        nValue = nValue << 1;

        if( m_bTempBank[i] > 0 )
        {
            nStatus = ReadByteFromBank(m_bTempBank[i], m_bTempReg[i] + 1, &bVal);
            if( Success != nStatus )
                return nStatus;
            nValue |= bVal >> 7;
        }

        dTempVal = nValue / 2.0f;

        if( dTempVal <= 125 && dTempVal >= -55 && !m_bPCITemps[i])
            m_pTemps[i] = dTempVal;
        else
            m_pTemps[i] = 0;
    }

    // voltages
    double dValue = 0;
    for( int i = 0; i < nVoltages; i++ )
    {
        if( W836_CHIP_VOLTAGE_VBAT_REGISTER != m_bVoltageReg[i] )
        {
            if( W83627HF == m_eChip || W83627THF == m_eChip ||
                   W83687THF == m_eChip  && 0 == i )
            {
                BYTE bVrmConfig = 0;
                nStatus = ReadByteFromBank(0, 0x18, &bVrmConfig);
                if( Success != nStatus )
                    return nStatus;
                nStatus = ReadByteFromBank(m_bVoltageBank[i], m_bVoltageReg[i], &bVal);
                if( Success != nStatus )
                    return nStatus;
                nValue = bVal;

                if( 0 == (bVrmConfig & 0x01) )
                    dValue = 0x016f * nValue;
                else
                    dValue = 0x00488f * nValue + 0.69f;
            }
            else
            {
                nStatus = ReadByteFromBank(m_bVoltageBank[i], m_bVoltageReg[i], &bVal);
                if( Success != nStatus )
                    return nStatus;
                nValue = bVal;

                dValue = dVoltageGain * nValue;
            }

            m_pVoltages[i].qzName.sprintf("#%d", i);
            if( 0 < dValue )
                m_pVoltages[i].qzValue.sprintf("%.3fV", dValue);
            else
                m_pVoltages[i].qzValue = "0V";
        }
        else
        {
            nStatus = ReadByteFromBank(0, 0x5D, &bVal);
            if( Success != nStatus )
                return nStatus;

            m_pVoltages[i].qzName.sprintf("#%d", i);

            if( 0 < bVal )
            {
                nStatus = ReadByteFromBank(5, W836_CHIP_VOLTAGE_VBAT_REGISTER, &bVal);
                if( Success != nStatus )
                    return nStatus;
                m_pVoltages[i].qzValue.sprintf("%.3fV", dVoltageGain * bVal);
            }
            else
                m_pVoltages[i].qzValue = "0V";
        }
    }

    // fan speeds
    ULONGLONG ulBits = 0;
    dValue = 0;
    for( int i = 0; i < 5; i++ )
    {
        nStatus = ReadByteFromBank(0, m_bFanBitReg[i], &bVal);
        if( Success != nStatus )
            return nStatus;

        ulBits = (ulBits << 8) | bVal;

    }
    ULONGLONG ulNewBits = ulBits;
    for( int i = 0; i < 5; i++ )
    {
        int nCount = 0;
        nStatus = ReadByteFromBank(m_bFanTachoBank[i], m_bFanTachoReg[i], &bVal);
        if( Success != nStatus )
            return nStatus;

        nCount = bVal;

        int nDivisiorBits = (((ulBits >> m_bFanDivBit2[i]) & 1) << 2) |
                            (((ulBits >> m_bFanDivBit1[i]) & 1) << 1) |
                            ((ulBits >> m_bFanDivBit0[i]) & 1);

        int nDivisior = 1 << nDivisiorBits;

        dValue = (nCount < 0xff) ? 1.35e6f / (nCount * nDivisior) : 0;
        m_pFans[i] = dValue;

        if( nCount > 192 && nDivisiorBits < 7 )
            nDivisiorBits++;
        if( nCount < 96 && nDivisiorBits > 0 )
            nDivisiorBits--;

        ULONGLONG ulMask = 1 << m_bFanDivBit2[i];
        if( ((nDivisiorBits >> 2) & 1) > 0 )
            ulNewBits = ulNewBits | ulMask;
        else
            ulNewBits = ulNewBits & ~ulMask;

        ulMask = (ULONGLONG)1 << m_bFanDivBit1[i];
        if( ((nDivisiorBits >> 1) & 1) > 0 )
            ulNewBits = ulNewBits | ulMask;
        else
            ulNewBits = ulNewBits & ~ulMask;

        ulMask = (ULONGLONG)1 << m_bFanDivBit0[i];
        if( (nDivisiorBits & 1) > 0 )
            ulNewBits = ulNewBits | ulMask;
        else
            ulNewBits = ulNewBits & ~ulMask;
    }

    // new divisiors
    for( int i = 4; i >= 0; i-- )
    {
        BYTE bOld = ulBits & 0xFF;
        BYTE bNew = ulNewBits & 0xFF;

        ulBits = ulBits >> 8;
        ulNewBits = ulNewBits >> 8;

        if( bOld != bNew )
        {
            nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_ADDRESS_REGISTER_OFFSET,
                                                 W836_CHIP_BANK_SELECT_REGISTER);
            if( Success != nStatus )
                return nStatus;
            nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_DATA_REGISTER_OFFSET, 0);
            if( Success != nStatus )
                return nStatus;
            nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_ADDRESS_REGISTER_OFFSET,
                                                 m_bFanBitReg[i]);
            if( Success != nStatus )
                return nStatus;
            nStatus = m_pDriver->WriteIoPortByte(usAddress + W836_CHIP_DATA_REGISTER_OFFSET, bNew);
            if( Success != nStatus )
                return nStatus;
        }
    }

    return Success;
}

double *CW836XX::GetTemps()
{
    return m_pTemps;
}

double *CW836XX::GetFanSpeeds()
{
    return m_pFans;
}

VoltageReading *CW836XX::GetVoltages()
{
    return m_pVoltages;
}

QString CW836XX::GetChipName()
{
    QString qzChipName = "";

    switch (m_eChip)
    {
    case W83627DHG:
        qzChipName = "W83627DHG";
        break;
    case W83627DHGP:
        qzChipName = "W83627DHGP";
        break;
    case W83627EHF:
        qzChipName = "W83627EHF";
        break;
    case W83627HF:
        qzChipName = "W83627HF";
        break;
    case W83627THF:
        qzChipName = "W83627THF";
        break;
    case W83667HG:
        qzChipName = "W83667HG";
        break;
    case W83667HGB:
        qzChipName = "W83667HGB";
        break;
    case W83687THF:
        qzChipName = "W83687THF";
        break;
    }

    return qzChipName;
}
