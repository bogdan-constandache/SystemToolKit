#include "../headers/type17_memory_device_information.h"

CType17MemoryDeviceInformation::CType17MemoryDeviceInformation()
{
    this->m_StructureType = MEMORY_DEVICE_INFORMATION_TYPE;
    this->m_data = NULL;
    m_data = new T17MemoryDeviceInformation;
}

CType17MemoryDeviceInformation::~CType17MemoryDeviceInformation()
{

}

int CType17MemoryDeviceInformation::AddInformation(BYTE *pData)
{
    PSMHeader pHeader = (PSMHeader)(pData);
    PType17MemoryDeviceInformation pInformation = (PType17MemoryDeviceInformation)(pData + sizeof(SMHeader));
    int count = 1;
    char *pTempString = 0;

    if( MEMORY_DEVICE_INFORMATION_TYPE != pHeader->bType )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    switch(pInformation->FormFactor)
    {
    case 0x01:
        m_data->FormFactor = QString("Other");
        break;
    case 0x02:
        m_data->FormFactor = QString("Unknown");
        break;
    case 0x03:
        m_data->FormFactor = QString("SIMM");
        break;
    case 0x04:
        m_data->FormFactor = QString("SIP");
        break;
    case 0x05:
        m_data->FormFactor = QString("Chip");
        break;
    case 0x06:
        m_data->FormFactor = QString("DIP");
        break;
    case 0x07:
        m_data->FormFactor = QString("ZIP");
        break;
    case 0x08:
        m_data->FormFactor = QString("PC");
        break;
    case 0x09:
        m_data->FormFactor = QString("DIMM");
        break;
    case 0x0A:
        m_data->FormFactor = QString("TSOP");
        break;
    case 0x0B:
        m_data->FormFactor = QString("ROC");
        break;
    case 0x0C:
        m_data->FormFactor = QString("RIMM");
        break;
    case 0x0D:
        m_data->FormFactor = QString("SO-DIMM");
        break;
    case 0x0E:
        m_data->FormFactor = QString("S-RIMM");
        break;
    case 0x0F:
        m_data->FormFactor = QString("FB-DIMM");
        break;
    }

    m_data->TotalWidth.setNum(pInformation->TotalWidth);
    m_data->TotalWidth.append(" bits");

    m_data->DataWidth.setNum(pInformation->DataWidth);
    m_data->DataWidth.append(" bits");

    if( 0xFFFF == pInformation->Size )
    {
        m_data->Size = "Unknown";
    }

    if( 0 < pInformation->Size )
    {
        if( 0 == (0x8000 % pInformation->Size) )
        {
            m_data->Size.setNum(pInformation->Size);
            m_data->Size.append(" MB");
        }
        else
        {
            m_data->Size.setNum(pInformation->Size);
            m_data->Size.append(" KB");
        }
    }

    if( 0 == pInformation->Speed )
    {
        m_data->Speed = QString("Unknown");
    }
    else
    {
        m_data->Speed.setNum(pInformation->Speed);
        m_data->Speed.append(" MHz");
    }

    if( 0 == pInformation->ConfiguredMemoryClockSpeed )
    {
        m_data->ConfiguredMemoryClockSpeed = QString("Unknown");
    }
    else
    {
        m_data->ConfiguredMemoryClockSpeed.setNum(pInformation->ConfiguredMemoryClockSpeed);
        m_data->ConfiguredMemoryClockSpeed.append(" MHz");
    }

    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->DeviceLocator )
            m_data->DeviceLocator = QString(pTempString);
        if( count == pInformation->Manufacturer )
            m_data->Manufacturer = QString(pTempString);
        if( count == pInformation->SerialNumber )
            m_data->SerialNumber = QString(pTempString);
        count++;
        pTempString = (char*)(pTempString + strlen(pTempString) + 1);
    }while(pTempString[0] != 0);

    return Success;
}

void *CType17MemoryDeviceInformation::GetStructureData()
{
    return (void*)this->m_data;
}


SMStructureType CType17MemoryDeviceInformation::GetStructureType()
{
    return this->m_StructureType;
}
