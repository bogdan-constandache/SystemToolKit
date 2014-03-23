#include "../headers/sensor_module.h"


CSensorModule::CSensorModule():
    m_pSensor(NULL)
{
    m_pDriver = new CSTKDriverWrapper;
    m_pDriver->Initialize();

    m_Chip = Unknown_Chip;

    m_pHardwareUsage = new CHardwareUsage;
}

CSensorModule::~CSensorModule()
{
    if (m_pHardwareUsage)
        delete m_pHardwareUsage;

    if (m_pSensor)
        delete  m_pSensor;

    m_pDriver->Destroy();
    delete m_pDriver;
}

bool CSensorModule::DetectIT87XXSensor()
{
    USHORT usChipID = 0;
    BYTE bChipIDRegister = 0x20;

    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x87);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x01);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x55);
    m_pDriver->WriteIoPortByte(IT87_REGISTER_PORT, 0x55);

    usChipID = ReadWordFromPort(bChipIDRegister);
    switch (usChipID)
    {
    case 0x8705:
        m_Chip = IT8705F; break;
    case 0x8712:
        m_Chip = IT8712F; break;
    case 0x8716:
        m_Chip = IT8716F; break;
    case 0x8718:
        m_Chip = IT8718F; break;
    case 0x8720:
        m_Chip = IT8720F; break;
    case 0x8721:
        m_Chip = IT8721F; break;
    case 0x8726:
        m_Chip = IT8726F; break;
    case 0x8728:
        m_Chip = IT8728F; break;
    case 0x8771:
        m_Chip = IT8771E; break;
    case 0x8772:
        m_Chip = IT8772E; break;
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

        if (IT8705F == m_Chip)
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

        m_pSensor = new CIT87(m_Chip, usAddress);
        return true;
    }
    return false;
}

USHORT CSensorModule::ReadWordFromPort(BYTE bRegister)
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

ISensor *CSensorModule::DetectSensor()
{
    if (DetectIT87XXSensor())
        return m_pSensor;
    return 0;
}

int CSensorModule::DestroySensor()
{
    return m_pSensor == 0 ? 0 : m_pSensor->Destroy();
}

double CSensorModule::GetCpuLoad()
{
    return m_pHardwareUsage->GetCpuLoad();
}

QString CSensorModule::GetCpuName()
{
    return m_pHardwareUsage->GetCpuName();
}

double CSensorModule::GetDiskLoad()
{
    return m_pHardwareUsage->GetDiskLoad();
}

MemoryStatus *CSensorModule::GetMemoryStat()
{
    return m_pHardwareUsage->GetMemoryStatus();
}
