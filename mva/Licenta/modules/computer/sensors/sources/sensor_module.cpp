#include "../headers/sensor_module.h"


CSensorModule::CSensorModule():
    m_pSensor(NULL)
{
    m_pDriver = new CSTKDriverWrapper;
    m_pDriver->Initialize();

    m_Chip = Unknown_Chip;
}

CSensorModule::~CSensorModule()
{
    delete  m_pSensor;

    m_pDriver->Destroy();
    delete m_pDriver;
}

ISensor *CSensorModule::DetectSensor()
{
    USHORT usChipID = 0;
    BYTE bChipIDRegister = 0x20;
    // try IT87 chip
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x87);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x01);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x55);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x55);

    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bChipIDRegister << 8);
    BYTE bVal1 = 0;
    m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bChipIDRegister + 1));
    BYTE bVal2 = 0;
    m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

    usChipID = bVal1 | bVal2;
    switch (usChipID)
    {
    case 0x8705:
        m_Chip = IT8705F;
        break;
    case 0x8712:
        m_Chip = IT8712F;
        break;
    case 0x8716:
        m_Chip = IT8716F;
        break;
    case 0x8718:
        m_Chip = IT8718F;
        break;
    case 0x8720:
        m_Chip = IT8720F;
        break;
    case 0x8721:
        m_Chip = IT8721F;
        break;
    case 0x8726:
        m_Chip = IT8726F;
        break;
    case 0x8728:
        m_Chip = IT8728F;
        break;
    case 0x8771:
        m_Chip = IT8771E;
        break;
    case 0x8772:
        m_Chip = IT8772E;
        break;
    default:
        m_Chip = Unknown_Chip;
    }

    if (Unknown_Chip == m_Chip)
    {
        if (usChipID != 0 && usChipID != 0xFFFF)
        {
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x02);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, 0x02);
        }
    }
    else
    {
        BYTE bIT87Controller = 0x04;
        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
        m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bIT87Controller);

        bVal1 = 0; bVal2 = 0;
        BYTE bBaseAddressRegister = 0x60;
        USHORT usAddress = 0;
        USHORT usVerify = 0;

        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bBaseAddressRegister << 8);
        m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 1));
        m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

        usAddress = bVal1 | bVal2;

#ifdef STK_WINDOWS
        Sleep(1);
#endif

        bVal1 = 0; bVal2 = 0;
        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bBaseAddressRegister << 8);
        m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 1));
        m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

        usVerify = bVal1 | bVal2;

        BYTE bVersion = 0;
        m_pDriver->ReadIoPortByte(IT87_CHIP_VERSION_REGISTER, &bVersion);


        USHORT usGPIOAdress = 0;
        USHORT usGPIOVerify = 0;

        if (IT8705F == m_Chip)
        {
            BYTE bGPIO_LND = 0x05;
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bGPIO_LND);

            bVal1 = 0; bVal2 = 0;

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bBaseAddressRegister << 8);
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 1));
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

            usGPIOAdress = bVal1 | bVal2;

#ifdef STK_WINDOWS
            Sleep(1);
#endif

            bVal1 = 0; bVal2 = 0;

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, bBaseAddressRegister << 8);
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 1));
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

            usGPIOVerify = bVal1 | bVal2;
        }
        else
        {
            BYTE bGPIO_LND_2 = 0x07;
            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x07);
            m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, bGPIO_LND_2);

            bVal1 = 0; bVal2 = 0;

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 2) << 8);
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 3));
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

            usGPIOAdress = bVal1 | bVal2;

#ifdef STK_WINDOWS
            Sleep(1);
#endif

            bVal1 = 0; bVal2 = 0;

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 2) << 8);
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal1);

            m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, (bBaseAddressRegister + 3));
            m_pDriver->ReadIoPortByte(IT87_VALUE_PORT, &bVal2);

            usGPIOVerify = bVal1 | bVal2;
        }

        m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x02);
        m_pDriver->WriteIoPortByte(IT87_VALUE_PORT, 0x02);


        if (usAddress != usVerify || usAddress < 0x100 || (usAddress & 0xF007) != 0)
        {
            // goto next sensor
        }

        if (usGPIOAdress != usGPIOVerify || usGPIOAdress < 0x100 || (usGPIOAdress & 0xF007) != 0)
        {
            // goto next sensor
        }

        m_pSensor = new CIT87(m_Chip, usAddress);
        return m_pSensor;
    }
    return 0;
}

int CSensorModule::DestroySensor()
{
    return m_pSensor->Destroy();
}
