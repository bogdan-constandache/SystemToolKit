#include "../headers/sensor_module.h"

CSensorModule::CSensorModule():
    m_pBoardFactory( NULL ),
    m_pCpuFactory( NULL ),
    m_pHardwareUsage( NULL )
{
    m_pHardwareUsage = new CHardwareUsage;
    CHECK_ALLOCATION(m_pHardwareUsage);

    m_pBoardFactory = new FactoryBoardSensor;
    CHECK_ALLOCATION(m_pHardwareUsage);

    m_pCpuFactory = new FactoryCpuSensor;
    CHECK_ALLOCATION(m_pCpuFactory);
}

CSensorModule::~CSensorModule()
{
    SAFE_DELETE(m_pHardwareUsage);

    SAFE_DELETE(m_pBoardFactory);

    SAFE_DELETE(m_pCpuFactory);
}

ICPUSensor *CSensorModule::GetCpuSensor()
{
    return m_pCpuFactory->GetCpuSensor();
}

int CSensorModule::DestroyBoardSensor()
{
    return m_pBoardFactory->DestroySensor();
}

int CSensorModule::DestroyCpuSensor()
{
    return m_pCpuFactory->DestroySensor();
}

ISensor *CSensorModule::GetBoardSensor()
{
    return m_pBoardFactory->GetBoardSensor();
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
