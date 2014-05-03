#include "../headers/factory_board_sensor.h"


FactoryBoardSensor::FactoryBoardSensor():
    m_pDriver( NULL ),
    m_pBoardSensor( NULL )
{
    m_pDriver = new CSTKDriverWrapper;
    CHECK_ALLOCATION(m_pDriver);

    int nStatus = m_pDriver->Initialize();
    CHECK_OPERATION_STATUS(nStatus);
}

FactoryBoardSensor::~FactoryBoardSensor()
{
    int nStatus = Uninitialized;

    if (m_pBoardSensor)
    {
        nStatus = m_pBoardSensor->Destroy();
        CHECK_OPERATION_STATUS(nStatus);
        SAFE_DELETE(m_pBoardSensor);
    }

    nStatus = m_pDriver->Destroy();
    CHECK_OPERATION_STATUS(nStatus);
    SAFE_DELETE(m_pDriver);
}

USHORT FactoryBoardSensor::ReadWordFromPort(BYTE bRegisterPort, BYTE bValuePort, BYTE bRegister)
{
    USHORT usVal = 0;

    m_pDriver->WriteIoPortByte(bRegisterPort, bRegister);
    BYTE bVal1 = 0;
    m_pDriver->ReadIoPortByte(bValuePort, &bVal1);

    m_pDriver->WriteIoPortByte(bRegisterPort, bRegister + 1);
    BYTE bVal2 = 0;
    m_pDriver->ReadIoPortByte(bValuePort, &bVal2);

    usVal = ((bVal1 << 8)| bVal2);

    return usVal;
}

BYTE FactoryBoardSensor::ReadByteFromPort(BYTE bRegisterPort, BYTE bValuePort, BYTE bRegister)
{
    BYTE bVal = 0;

    m_pDriver->WriteIoPortByte(bRegisterPort, bRegister);
    m_pDriver->ReadIoPortByte(bValuePort, &bVal);

    return bVal;
}

bool FactoryBoardSensor::DetectIT87XXSensor()
{
    USHORT usChipID = 0;
    BYTE bChipIDRegister = 0x20;
    Chip eChip = Unknown_Chip;

    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x87);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x01);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x55);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x55);

    usChipID = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, bChipIDRegister);
    switch (usChipID)
    {
    case 0x8705:
        eChip = IT8705F; break;
    case 0x8712:
        eChip = IT8712F; break;
    case 0x8716:
        eChip = IT8716F; break;
    case 0x8718:
        eChip = IT8718F; break;
    case 0x8720:
        eChip = IT8720F; break;
    case 0x8721:
        eChip = IT8721F; break;
    case 0x8726:
        eChip = IT8726F; break;
    case 0x8728:
        eChip = IT8728F; break;
    case 0x8771:
        eChip = IT8771E; break;
    case 0x8772:
        eChip = IT8772E; break;
    default:
        eChip = Unknown_Chip;
    }

    if (Unknown_Chip == eChip)
    {
        if (usChipID != 0 && usChipID != 0xFFFF)
        {
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x02);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, 0x02);
        }
    }
    else
    {
        // select environment
        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
        m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, IT87_ENVIRONMENT_CONTROLLER);

        USHORT usAddress = 0;
        USHORT usVerify = 0;

        usAddress = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, BASE_ADDRESS_REGISTER);

#ifdef STK_WINDOWS
        Sleep(1);
#endif

        usVerify = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, BASE_ADDRESS_REGISTER);

        BYTE bVersion = 0;
        m_pDriver->ReadIoPortByte((IT87_CHIP_VERSION_REGISTER & 0x0F), &bVersion);

        USHORT usGPIOAdress = 0;
        USHORT usGPIOVerify = 0;

        if (IT8705F == eChip)
        {
            BYTE bGPIO_LND = 0x05;
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bGPIO_LND);

            usGPIOAdress = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, BASE_ADDRESS_REGISTER);

#ifdef STK_WINDOWS
            Sleep(1);
#endif

            usGPIOVerify = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, BASE_ADDRESS_REGISTER);
        }
        else
        {
            BYTE bGPIO_LND_2 = 0x07;
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bGPIO_LND_2);

            usGPIOAdress = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, BASE_ADDRESS_REGISTER + 2);

#ifdef STK_WINDOWS
            Sleep(1);
#endif

            usGPIOVerify = ReadWordFromPort(IT87_REGISTER_PORT, IT87_VALUE_PORT, BASE_ADDRESS_REGISTER + 2);
        }

        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x02);
        m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, 0x02);


        if (usAddress != usVerify || usAddress < 0x100 || (usAddress & 0xF007) != 0)
            return false;

        if (usGPIOAdress != usGPIOVerify || usGPIOAdress < 0x100 || (usGPIOAdress & 0xF007) != 0)
            return false;

        m_pBoardSensor = new CIT87(eChip, usAddress);
        return true;
    }
    return false;
}

bool FactoryBoardSensor::DetectW836XXSensor()
{
    int nStatus = Uninitialized;
    BYTE bChipID = 0, bRevision = 0;
    BYTE bChipIDRegister = 0x20;
    BYTE bRevisionRegister = 0x21;
    BYTE bLogicalDeviceNumber = 0x0B;
    Chip eChip = Unknown_Chip;

    nStatus = m_pDriver->WriteIoPortByte(W836_REGISTER_PORT, 0x87);
    CHECK_OPERATION_STATUS(nStatus);
    nStatus = m_pDriver->WriteIoPortByte(W836_REGISTER_PORT, 0x87);
    CHECK_OPERATION_STATUS(nStatus);

    bChipID = ReadByteFromPort(W836_REGISTER_PORT, W836_VALUE_PORT, bChipIDRegister);
    bRevision = ReadByteFromPort(W836_REGISTER_PORT, W836_VALUE_PORT, bRevisionRegister);

    switch (bChipID)
    {
    case 0x52:
        switch (bRevision)
        {
        case 0x17:
        case 0x3A:
        case 0x41:
            eChip = W83627HF;
            break;
        }

        break;
    case 0x82:
        switch (bRevision & 0xF0)
        {
        case 0x80:
            eChip = W83627THF;
            break;
        }

        break;
    case 0x85:
        switch (bRevision)
        {
        case 0x41:
            eChip = W83687THF;
            break;
        }

        break;
    case 0x88:
        switch (bRevision & 0xF0)
        {
        case 0x50:
        case 0x60:
            eChip = W83627EHF;
            break;
        }

        break;
    case 0xA0:
        switch (bRevision & 0xF0)
        {
        case 0x20:
            eChip = W83627DHG;
            break;
        }

        break;
    case 0xA5:
        switch (bRevision & 0xF0)
        {
        case 0x10:
            eChip = W83667HG;
            break;
        }

        break;
    case 0xB0:
        switch (bRevision & 0xF0)
        {
        case 0x70:
            eChip = W83627DHGP;
            break;
        }

        break;
    case 0xB3:
        switch (bRevision & 0xF0)
        {
        case 0x50:
            eChip = W83667HGB;
            break;
        }

        break;
    default:
        eChip = Unknown_Chip;
        break;
    }

    if( Unknown_Chip == eChip )
    {
        if( 0 != bChipID && 0xFF != bChipID )
        {
            m_pDriver->WriteIoPortByte(W836_REGISTER_PORT, 0xAA);
        }
    }
    else
    {
        // select environment
        nStatus = m_pDriver->WriteIoPortByte(W836_REGISTER_PORT, 0x07);
        CHECK_OPERATION_STATUS(nStatus);
        nStatus = m_pDriver->WriteIoPortByte(W836_VALUE_PORT, bLogicalDeviceNumber);
        CHECK_OPERATION_STATUS(nStatus);

        USHORT usAddress = 0;
        USHORT usVerify = 0;

        usAddress = ReadWordFromPort(W836_REGISTER_PORT, W836_VALUE_PORT, BASE_ADDRESS_REGISTER);
#ifdef STK_WINDOWS
        Sleep(1);
#endif
        usVerify = ReadWordFromPort(W836_REGISTER_PORT, W836_VALUE_PORT, BASE_ADDRESS_REGISTER);

        nStatus = m_pDriver->WriteIoPortByte(W836_REGISTER_PORT, 0xAA);
        CHECK_OPERATION_STATUS(nStatus);

        if( usAddress != usVerify )
            return false;

        if( usAddress < 0x100 || ((usAddress & 0xF007) != 0) )
            return false;

        m_pBoardSensor = new CW836XX(eChip, usAddress, bRevision);
        return true;
    }

    return false;
}


ISensor *FactoryBoardSensor::GetBoardSensor()
{
    if (DetectIT87XXSensor())
        return m_pBoardSensor;

    if (DetectW836XXSensor())
        return m_pBoardSensor;

    return NULL;
}

int FactoryBoardSensor::DestroySensor()
{
    int nStatus = m_pBoardSensor->Destroy();
    SAFE_DELETE(m_pBoardSensor);

    return nStatus;
}
