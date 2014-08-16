#include "../headers/battery_status.h"


BatteryStatus::BatteryStatus():
    m_data(NULL), m_bIsDesktop(false), m_pModel(NULL)
{
    m_pModel = new QStandardItemModel();
}

BatteryStatus::~BatteryStatus()
{
    SAFE_DELETE(m_pModel);
    SAFE_DELETE(m_data);
}

int BatteryStatus::Initialize()
{
    SYSTEM_POWER_STATUS pPowerStatus;
    bool bResult = 0;
    DWORD dwBytesRequired = 0, dwBytesReturned = 0, dwWait = 0;
    SP_DEVICE_INTERFACE_DATA dData = {0};
    dData.cbSize = sizeof(dData);
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDataDetails = 0;

    HANDLE hBattery = 0;
    BATTERY_QUERY_INFORMATION batteryQueryInformation = {0};
    BATTERY_INFORMATION batteryInformation = {0};
    BATTERY_WAIT_STATUS batteryWaitInformation = {0};
    BATTERY_STATUS batteryStatus = {0};
    int count = 0;

    SAFE_DELETE(m_data);
    m_data = new BatteryInformationStruct;
    if( 0 == m_data )
    {
        DEBUG_STATUS(NotAllocated);
        return NotAllocated;
    }

    GetSystemPowerStatus(&pPowerStatus);

    switch(pPowerStatus.ACLineStatus)
    {
    case 0:
        m_data->ACLineStatus = QString("Offline");
        break;
    case 1:
        m_data->ACLineStatus = QString("Online");
        break;
    case 255:
        m_data->ACLineStatus = QString("Unknown status");
        break;
    }

    if( 255 == pPowerStatus.BatteryLifePercent )
        m_data->LifePercent = QString("Unknown");
    else
        m_data->LifePercent = QString().setNum(pPowerStatus.BatteryLifePercent).append("%");

    if( -1 == pPowerStatus.BatteryLifeTime )
        m_data->LifeTime = QString("Unknown");
    else
    {
        int h = 0, m = 0;
        h = (int)(pPowerStatus.BatteryLifeTime / 3600);
        m = (int)((pPowerStatus.BatteryLifeTime % 3600) / 60);
        m_data->LifeTime = QString().setNum(h);
        if( 1 == h )
            m_data->LifeTime.append(" hour, ");
        else
            m_data->LifeTime.append(" hours, ");
        m_data->LifeTime.append(QString().setNum(m));
        if( 1 == m )
            m_data->LifeTime.append(" minute");
        else
            m_data->LifeTime.append(" minutes");
    }

    if( -1 == pPowerStatus.BatteryFullLifeTime )
        m_data->FullLifeTime = QString("Unknown");
    else
    {
        int h = 0, m = 0;
        h = (int)(pPowerStatus.BatteryFullLifeTime / 3600);
        m = (int)((pPowerStatus.BatteryFullLifeTime % 3600) / 60);
        m_data->FullLifeTime = QString().setNum(h);
        if( 1 == h )
            m_data->FullLifeTime.append(" hour, ");
        else
            m_data->FullLifeTime.append(" hours, ");
        m_data->FullLifeTime.append(QString().setNum(m));
        if( 1 == m )
            m_data->FullLifeTime.append(" minute");
        else
            m_data->FullLifeTime.append(" minutes");
    }

    HDEVINFO hDev = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY,
                                        0,
                                        0,
                                        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if( INVALID_HANDLE_VALUE == hDev )
    {
        DEBUG_STATUS(InvalidHandle);
        return InvalidHandle;
    }

    if( false == SetupDiEnumDeviceInterfaces(hDev,
                                             0,
                                             &GUID_DEVCLASS_BATTERY,
                                             count,
                                             &dData))
    {
        DWORD dwLastError = GetLastError();
        if( ERROR_NO_MORE_ITEMS == dwLastError )
            m_bIsDesktop = true;
        DEBUG_STATUS(Unsuccessful);
        goto Cleanup;
    }

    while( ERROR_NO_MORE_ITEMS != GetLastError() )
    {
        dwBytesRequired = 0;

        SetupDiGetDeviceInterfaceDetail(hDev,
                                        &dData,
                                        0,
                                        0,
                                        &dwBytesRequired,
                                        0);

        if( ERROR_INSUFFICIENT_BUFFER == GetLastError() )
        {
            deviceDataDetails = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, dwBytesRequired);

            if( 0 == deviceDataDetails )
            {
                DEBUG_STATUS(NotAllocated);
                goto Cleanup;
            }

            deviceDataDetails->cbSize = sizeof(*deviceDataDetails);
            if( false == SetupDiGetDeviceInterfaceDetail(hDev,
                                                         &dData,
                                                         deviceDataDetails,
                                                         dwBytesRequired,
                                                         &dwBytesRequired,
                                                         0) )
            {
                DEBUG_STATUS(Unsuccessful);
                goto Cleanup;
            }

            hBattery = CreateFile(deviceDataDetails->DevicePath,
                                  GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                                  NULL,
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);

            if( INVALID_HANDLE_VALUE == hBattery )
            {
                DEBUG_STATUS(InvalidHandle);
                goto Cleanup;
            }

            bResult = DeviceIoControl(hBattery,
                                      IOCTL_BATTERY_QUERY_TAG,
                                      &dwWait,
                                      sizeof(dwWait),
                                      &batteryQueryInformation.BatteryTag,
                                      sizeof(batteryQueryInformation.BatteryTag),
                                      &dwBytesReturned,
                                      NULL);

            if( false == bResult )
            {
                DEBUG_STATUS(Unsuccessful);
                CloseHandle(hBattery);
                goto Cleanup;
            }

            bResult = DeviceIoControl(hBattery,
                                      IOCTL_BATTERY_QUERY_INFORMATION,
                                      &batteryQueryInformation,
                                      sizeof(batteryQueryInformation),
                                      &batteryInformation,
                                      sizeof(batteryInformation),
                                      &dwBytesReturned,
                                      NULL);

            if( false == bResult )
            {
                DEBUG_STATUS(Unsuccessful);
                CloseHandle(hBattery);
                goto Cleanup;
            }

            if( batteryInformation.Capabilities & BATTERY_SYSTEM_BATTERY )
            {
                //technology
                switch( batteryInformation.Technology )
                {
                case 0:
                    m_data->Technology = "Nonrechargeable battery.";
                    break;
                case 1:
                    m_data->Technology = "Rechargeable battery.";
                    break;
                }

                char pzTemp[5] = {0};
                memcpy(pzTemp, batteryInformation.Chemistry, 4);
                pzTemp[4] = '\0';

                if( QString(pzTemp).toLower() == "pbac" )
                    m_data->Chemistry = QString("Lead Acid");
                if( QString(pzTemp).toLower() == "lion" )
                    m_data->Chemistry = QString("Lithium Ion");
                if( QString(pzTemp).toLower() == "li-i" )
                    m_data->Chemistry = QString("Lithium Ion");
                if( QString(pzTemp).toLower() == "nicd" )
                    m_data->Chemistry = QString("Nickel Cadmium");
                if( QString(pzTemp).toLower() == "nimh" )
                    m_data->Chemistry = QString("Nickel Metal Hydride");
                if( QString(pzTemp).toLower() == "nizn" )
                    m_data->Chemistry = QString("Nickel Zinc");
                if( QString(pzTemp).toLower() == "ram" )
                    m_data->Chemistry = QString("Rechargeable Alkaline-Manganese");

                if( batteryInformation.DesignedCapacity )
                {
                    m_data->DesignedCapacity = QString().setNum(batteryInformation.DesignedCapacity).append(" mWh");
                }
                else
                    m_data->DesignedCapacity = QString("Unknown");

                if( batteryInformation.FullChargedCapacity )
                {
                    m_data->FullChargedCapacity = QString().setNum(batteryInformation.FullChargedCapacity).append(" mWh");
                }
                else
                    m_data->FullChargedCapacity = QString("Unknown");

                if( batteryInformation.DefaultAlert1 )
                {
                    m_data->LowBatteryAlert1 = QString().setNum(batteryInformation.DefaultAlert1).append(" mWh");
                }
                else
                    m_data->LowBatteryAlert1 = QString("Unknown");

                if( batteryInformation.DefaultAlert2 )
                {
                    m_data->LowBatteryAlert2 = QString().setNum(batteryInformation.DefaultAlert2).append(" mWh");
                }
                else
                    m_data->LowBatteryAlert2 = QString("Unknown");

                m_data->CycleCount = QString().setNum(batteryInformation.CycleCount);
                m_data->WearLevel = QString().setNum((batteryInformation.FullChargedCapacity*100)/batteryInformation.DesignedCapacity).append("%");
            }

            batteryWaitInformation.BatteryTag = batteryQueryInformation.BatteryTag;

            bResult = DeviceIoControl(hBattery,
                                      IOCTL_BATTERY_QUERY_STATUS,
                                      &batteryWaitInformation,
                                      sizeof(batteryWaitInformation),
                                      &batteryStatus,
                                      sizeof(batteryStatus),
                                      &dwBytesReturned,
                                      NULL);

            if( false == bResult )
            {
                DEBUG_STATUS(Unsuccessful);
                CloseHandle(hBattery);
                goto Cleanup;
            }

            switch(batteryStatus.PowerState)
            {
            case BATTERY_CHARGING:
                m_data->BatteryStatus = "Charging";
                break;
            case BATTERY_DISCHARGING:
                m_data->BatteryStatus = "Discharging";
                break;
            case BATTERY_CRITICAL:
                m_data->BatteryStatus = "Critical state";
                break;
            case BATTERY_POWER_ON_LINE:
                m_data->BatteryStatus = "AC-line in use";
                break;
            case BATTERY_CHARGING | BATTERY_POWER_ON_LINE:
                m_data->BatteryStatus = "AC-line in use, battery is charging";
            }

            if( batteryStatus.Capacity )
            {
                m_data->CurrentCapacity = QString().setNum(batteryStatus.Capacity).append(" mWh");
            }
            else
                m_data->CurrentCapacity = QString("Unknown");

            if( batteryStatus.Voltage )
            {
                m_data->Voltage = QString().setNum(batteryStatus.Voltage).append(" mV");
            }
            else
                m_data->Voltage = QString("Unknown");

            CloseHandle(hBattery);
            LocalFree(deviceDataDetails);
            deviceDataDetails = 0;
            dData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
            count++;

            if( false == SetupDiEnumDeviceInterfaces(hDev,
                                                     0,
                                                     &GUID_DEVCLASS_BATTERY,
                                                     count,
                                                     &dData))
            {
                DEBUG_STATUS(Unsuccessful);
                goto Cleanup;
            }
        }
    }

Cleanup:
    if( 0 != deviceDataDetails )
        LocalFree(deviceDataDetails);
    SetupDiDestroyDeviceInfoList(hDev);

    return Success;
}

QStandardItemModel* BatteryStatus::GetBatteryInformation()
{
    return m_pModel;
}

void BatteryStatus::OnRefresh()
{
    int nRet = Initialize();
    if( Success != nRet )
        return;

   m_pModel->clear();
   m_pModel->setHorizontalHeaderLabels(QStringList() << "Property name" << "Property value");

   QList<QStandardItem*> qList;

    if( m_bIsDesktop )
    {
        qList << new QStandardItem("AC-Line status:") << new QStandardItem("Online");
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Battery status:") << new QStandardItem("No battery");
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Life time:") << new QStandardItem("Unknown");
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Full life-time:") << new QStandardItem("Unknown");
        m_pModel->appendRow(qList);
        qList.clear();
    }
    else
    {
        qList << new QStandardItem("AC-Line status:") << new QStandardItem(this->m_data->ACLineStatus);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Battery status:") << new QStandardItem(this->m_data->BatteryStatus);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Life percent:") << new QStandardItem(this->m_data->LifePercent);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Life time:") << new QStandardItem(this->m_data->LifeTime);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Full life-time:") << new QStandardItem(this->m_data->FullLifeTime);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Technology:") << new QStandardItem(this->m_data->Technology);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Chemistry:") << new QStandardItem(this->m_data->Chemistry);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Designed capacity:") << new QStandardItem(this->m_data->DesignedCapacity);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Full charged capacity:") << new QStandardItem(this->m_data->FullChargedCapacity);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Current capacity:") << new QStandardItem(this->m_data->CurrentCapacity);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Low Battery Alert 1:") << new QStandardItem(this->m_data->LowBatteryAlert1);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Low Battery Alert 2:") << new QStandardItem(this->m_data->LowBatteryAlert2);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Cycle count:") << new QStandardItem(this->m_data->CycleCount);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Voltage:") << new QStandardItem(this->m_data->Voltage);
        m_pModel->appendRow(qList);
        qList.clear();

        qList << new QStandardItem("Wear level:") << new QStandardItem(this->m_data->WearLevel);
        m_pModel->appendRow(qList);
        qList.clear();
    }
}
