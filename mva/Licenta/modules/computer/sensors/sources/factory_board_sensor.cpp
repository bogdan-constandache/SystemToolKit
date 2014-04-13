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

USHORT FactoryBoardSensor::ReadWordFromPort(BYTE bRegister)
{
    USHORT usVal = 0;

    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bRegister);
    BYTE bVal1 = 0;
    m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bRegister + 1);
    BYTE bVal2 = 0;
    m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

    usVal = ((bVal1 << 8)| bVal2);

    return usVal;
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

    usChipID = ReadWordFromPort(bChipIDRegister);
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

        usAddress = ReadWordFromPort(BASE_ADDRESS_REGISTER);

#ifdef STK_WINDOWS
        Sleep(1);
#endif

        usVerify = ReadWordFromPort(BASE_ADDRESS_REGISTER);

        BYTE bVersion = 0;
        m_pDriver->ReadIoPortByte((IT87_CHIP_VERSION_REGISTER & 0x0F), &bVersion);

        USHORT usGPIOAdress = 0;
        USHORT usGPIOVerify = 0;

        if (IT8705F == eChip)
        {
            BYTE bGPIO_LND = 0x05;
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bGPIO_LND);

            usGPIOAdress = ReadWordFromPort(BASE_ADDRESS_REGISTER);

#ifdef STK_WINDOWS
            Sleep(1);
#endif

            usGPIOVerify = ReadWordFromPort(BASE_ADDRESS_REGISTER);
        }
        else
        {
            BYTE bGPIO_LND_2 = 0x07;
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bGPIO_LND_2);

            usGPIOAdress = ReadWordFromPort(BASE_ADDRESS_REGISTER + 2);

#ifdef STK_WINDOWS
            Sleep(1);
#endif

            usGPIOVerify = ReadWordFromPort(BASE_ADDRESS_REGISTER + 2);
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


ISensor *FactoryBoardSensor::GetBoardSensor()
{
    if (DetectIT87XXSensor())
        return m_pBoardSensor;

    return NULL;
}
