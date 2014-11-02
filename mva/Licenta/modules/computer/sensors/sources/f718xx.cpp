#include "../headers/f718xx.h"


CF718::CF718(Chip eChip, USHORT usAddress)
{
    this->eChip = eChip;
    this->usAddress = usAddress;

    m_pDriver = new CSTKDriverWrapper;
    CHECK_OPERATION_STATUS(m_pDriver->Initialize());

    m_pFanSpeedReg = new BYTE[4];
    m_pFanSpeedReg[0] = 0xA0;
    m_pFanSpeedReg[1] = 0xB0;
    m_pFanSpeedReg[2] = 0xC0;
    m_pFanSpeedReg[3] = 0xD0;
}

CF718::~CF718()
{
    CHECK_OPERATION_STATUS(m_pDriver->Destroy());
    SAFE_DELETE(m_pDriver);

    delete[] m_pFanSpeedReg;
}

int CF718::ReadF718Byte(BYTE bRegister, BYTE *bOut)
{
    int nStatus = Uninitialized;
    nStatus = m_pDriver->WriteIoPortByte((usAddress + F718_CHIP_ADDRESS_REGISTER_OFFSET), bRegister);
    CHECK_OPERATION_STATUS_EX(nStatus);

    nStatus = m_pDriver->ReadIoPortByte((usAddress + F718_CHIP_DATA_REGISTER_OFFSET), bOut);
    return nStatus;
}

int CF718::Initialize()
{
    for( int i = 0; i < 9; i++ )
    {
        m_pVoltages[i].qzName = "";
        m_pVoltages[i].qzValue = "";
    }
    for( int i = 0; i < 3; i++ )
    {
        m_pTemps[i] = 0;
    }
    for( int i = 0; i < 5; i++ )
    {
        m_pFans[i] = 0;
    }

    return Success;
}

int CF718::Destroy()
{
    return Success;
}

int CF718::Update()
{
    for( int i = 0; i < m_nVoltageCount; i++ )
    {
        if( eChip == F71808E && i == 6 )
            continue;
        else
        {
            BYTE bVal = 0;
            CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)(F718_CHIP_VOLTAGE_BASE_REGISTER + i), &bVal));
            m_pVoltages[i].qzName.sprintf("#%d", i);
            m_pVoltages[i].qzValue.sprintf("%.3fV", 0.008f * bVal);
        }
    }

    for( int i = 0; i < m_nTempsCount; i++ )
    {
        BYTE bVal = 0;

        switch( eChip )
        {
        case F71858:
        {
            CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)F718_CHIP_TEMPERATURE_CONFIG_REGISTER, &bVal));
            int nTableMode = 0x3 & bVal;

            CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)(F718_CHIP_TEMPERATURE_BASE_REGISTER + 2 * i), &bVal));
            int nHigh = bVal;

            CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)(F718_CHIP_TEMPERATURE_BASE_REGISTER + 2 * i + 1), &bVal));
            int nLow = bVal;

            if( nHigh != 0xBB && nHigh != 0xCC )
            {
                int nBits = 0;
                switch( nTableMode )
                {
                case 0:
                    nBits = 0; break;
                case 1:
                    nBits = 0; break;
                case 2:
                    nBits = (nHigh & 0x80) << 8; break;
                case 3:
                    nBits = (nLow & 0x01) << 15; break;
                }
                nBits |= nHigh << 7;
                nBits |= (nLow & 0xE0) >> 1;

                USHORT usVal = nBits & 0xFFF0;
                m_pTemps[i] = usVal / 128.0f;
            }
            else
                m_pTemps[i] = 0;
        }
            break;
        default:
        {
            BYTE bVal = 0;

            CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)(F718_CHIP_TEMPERATURE_BASE_REGISTER + 2 * (i + 1)), &bVal));

            if( bVal > 0 )
                m_pTemps[i] = bVal;
            else
                m_pTemps[i] = 0;
        }
            break;
        }
    }

    for( int i = 0; i < m_nFansCount; i++ )
    {
        BYTE bVal = 0;
        CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)m_pFanSpeedReg[i], &bVal));

        int nVal = bVal << 8;

        CHECK_OPERATION_STATUS(ReadF718Byte((BYTE)(m_pFanSpeedReg[i] + 1), &bVal));

        nVal |= bVal;

        if( nVal > 0 )
            m_pFans[i] = (nVal < 0x0FFF) ? 1.5e6f / nVal : 0;
        else
            m_pFans[i] = 0;
    }

    return Success;
}

double *CF718::GetTemps()
{
    return m_pTemps;
}

double *CF718::GetFanSpeeds()
{
    return m_pFans;
}

VoltageReading *CF718::GetVoltages()
{
    return m_pVoltages;
}

QString CF718::GetChipName()
{
    switch( eChip )
    {
    case F71858:
        return "F71858"; break;
    case F71862:
        return "F71862"; break;
    case F71869:
        return "F71869"; break;
    case F71869A:
        return "F71869A"; break;
    case F71882:
        return "F71882"; break;
    case F71889AD:
        return "F71889AD"; break;
    case F71889ED:
        return "F71889ED"; break;
    case F71889F:
        return "F71889F"; break;
    case F71808E:
        return "F71808E"; break;
    default:
        return "Unknown";
    }
}
