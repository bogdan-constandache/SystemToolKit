#include "../headers/battery_status.h"


BatteryStatus::BatteryStatus():
    m_data(NULL), m_bIsDesktop(false)
{
}

BatteryStatus::~BatteryStatus()
{
    SAFE_DELETE(m_data);
}

int BatteryStatus::Initialize()
{
    SYSTEM_POWER_STATUS pPowerStatus;
    bool bResult = 0;
    DWORD dwBytesRequired = 0, dwBytesReturned = 0, dwWait = 0;
    SP_DEVICE_INTERFACE_DATA dData = {0};
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDataDetails = 0;
    dData.cbSize = sizeof(dData);
    HANDLE hBattery = 0;
    BATTERY_QUERY_INFORMATION batteryQueryInformation = {0};
    BATTERY_INFORMATION batteryInformation = {0};
    BATTERY_WAIT_STATUS batteryWaitInformation = {0};
    BATTERY_STATUS batteryStatus = {0};
    int count = 0;

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
    int nRet = Initialize();
    if (Success != nRet)
        return 0;

    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pItem = 0;

    if( m_bIsDesktop )
    {
        pModel->setColumnCount(2);
        pModel->setRowCount(4);

        pModel->setHorizontalHeaderLabels(QStringList() << "Property name" << "Property value");

        pItem = new QStandardItem("AC-Line status:");
        pModel->setItem(0, 0, pItem);

        pItem = new QStandardItem("Online");
        pModel->setItem(0, 1, pItem);

        pItem = new QStandardItem("Battery status:");
        pModel->setItem(1, 0, pItem);

        pItem = new QStandardItem("No battery");
        pModel->setItem(1, 1, pItem);

        pItem = new QStandardItem("Life time:");
        pModel->setItem(2, 0, pItem);

        pItem = new QStandardItem("Unknown");
        pModel->setItem(2, 1, pItem);

        pItem = new QStandardItem("Full life-time:");
        pModel->setItem(3, 0, pItem);

        pItem = new QStandardItem("Unknown");
        pModel->setItem(3, 1, pItem);

        return pModel;
    }

    pModel->setColumnCount(2);
    pModel->setRowCount(15);
    pModel->setHorizontalHeaderLabels(QStringList() << "Property name" << "Property value");

    pItem = new QStandardItem("AC-Line status:");
    pModel->setItem(0, 0, pItem);

    pItem = new QStandardItem(this->m_data->ACLineStatus);
    pModel->setItem(0, 1, pItem);

    pItem = new QStandardItem("Battery status:");
    pModel->setItem(1, 0, pItem);

    pItem = new QStandardItem(this->m_data->BatteryStatus);
    pModel->setItem(1, 1, pItem);

    pItem = new QStandardItem("Life percent:");
    pModel->setItem(2, 0, pItem);

    pItem = new QStandardItem(this->m_data->LifePercent);
    pModel->setItem(2, 1, pItem);

    pItem = new QStandardItem("Life time:");
    pModel->setItem(3, 0, pItem);

    pItem = new QStandardItem(this->m_data->LifeTime);
    pModel->setItem(3, 1, pItem);

    pItem = new QStandardItem("Full life-time:");
    pModel->setItem(4, 0, pItem);

    pItem = new QStandardItem(this->m_data->FullLifeTime);
    pModel->setItem(4, 1, pItem);

    pItem = new QStandardItem("Technology:");
    pModel->setItem(5, 0, pItem);

    pItem = new QStandardItem(this->m_data->Technology);
    pModel->setItem(5, 1, pItem);

    pItem = new QStandardItem("Chemistry:");
    pModel->setItem(6, 0, pItem);

    pItem = new QStandardItem(this->m_data->Chemistry);
    pModel->setItem(6, 1, pItem);

    pItem = new QStandardItem("Designed capacity:");
    pModel->setItem(7, 0, pItem);

    pItem = new QStandardItem(this->m_data->DesignedCapacity);
    pModel->setItem(7, 1, pItem);

    pItem = new QStandardItem("Full charged capacity:");
    pModel->setItem(8, 0, pItem);

    pItem = new QStandardItem(this->m_data->FullChargedCapacity);
    pModel->setItem(8, 1, pItem);

    pItem = new QStandardItem("Current capacity:");
    pModel->setItem(9, 0, pItem);

    pItem = new QStandardItem(this->m_data->CurrentCapacity);
    pModel->setItem(9, 1, pItem);

    pItem = new QStandardItem("Low Battery Alert 1:");
    pModel->setItem(10, 0, pItem);

    pItem = new QStandardItem(this->m_data->LowBatteryAlert1);
    pModel->setItem(10, 1, pItem);

    pItem = new QStandardItem("Low Battery Alert 2:");
    pModel->setItem(11, 0, pItem);

    pItem = new QStandardItem(this->m_data->LowBatteryAlert2);
    pModel->setItem(11, 1, pItem);

    pItem = new QStandardItem("Cycle count:");
    pModel->setItem(12, 0, pItem);

    pItem = new QStandardItem(this->m_data->CycleCount);
    pModel->setItem(12, 1, pItem);

    pItem = new QStandardItem("Voltage:");
    pModel->setItem(13, 0, pItem);

    pItem = new QStandardItem(this->m_data->Voltage);
    pModel->setItem(13, 1, pItem);

    pItem = new QStandardItem("Wear level:");
    pModel->setItem(14, 0, pItem);

    pItem = new QStandardItem(this->m_data->WearLevel);
    pModel->setItem(14, 1, pItem);

    return pModel;
}
