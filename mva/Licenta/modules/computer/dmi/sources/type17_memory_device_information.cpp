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
        m_data->FormFactor = QString("Other"); break;
    case 0x02:
        m_data->FormFactor = QString("Unknown"); break;
    case 0x03:
        m_data->FormFactor = QString("SIMM"); break;
    case 0x04:
        m_data->FormFactor = QString("SIP"); break;
    case 0x05:
        m_data->FormFactor = QString("Chip"); break;
    case 0x06:
        m_data->FormFactor = QString("DIP"); break;
    case 0x07:
        m_data->FormFactor = QString("ZIP"); break;
    case 0x08:
        m_data->FormFactor = QString("PC"); break;
    case 0x09:
        m_data->FormFactor = QString("DIMM"); break;
    case 0x0A:
        m_data->FormFactor = QString("TSOP"); break;
    case 0x0B:
        m_data->FormFactor = QString("ROC"); break;
    case 0x0C:
        m_data->FormFactor = QString("RIMM"); break;
    case 0x0D:
        m_data->FormFactor = QString("SO-DIMM"); break;
    case 0x0E:
        m_data->FormFactor = QString("S-RIMM"); break;
    case 0x0F:
        m_data->FormFactor = QString("FB-DIMM"); break;
    }

    switch(pInformation->MemoryType)
    {
    case 0x01:
        m_data->Type = "Other"; break;
    case 0x02:
        m_data->Type = "Unknown"; break;
    case 0x03:
        m_data->Type = "DRAM"; break;
    case 0x04:
        m_data->Type = "EDRAM"; break;
    case 0x05:
        m_data->Type = "VRAM"; break;
    case 0x06:
        m_data->Type = "SRAM"; break;
    case 0x07:
        m_data->Type = "RAM"; break;
    case 0x08:
        m_data->Type = "ROM"; break;
    case 0x09:
        m_data->Type = "FLASH"; break;
    case 0x0A:
        m_data->Type = "EEPROM"; break;
    case 0x0B:
        m_data->Type = "FEPROM"; break;
    case 0x0C:
        m_data->Type = "EPROM"; break;
    case 0x0D:
        m_data->Type = "CDRAM"; break;
    case 0x0E:
        m_data->Type = "3DRAM"; break;
    case 0x0F:
        m_data->Type = "SDRAM"; break;
    case 0x10:
        m_data->Type = "SGRAM"; break;
    case 0x11:
        m_data->Type = "RDRAM"; break;
    case 0x12:
        m_data->Type = "DDR"; break;
    case 0x13:
        m_data->Type = "DDR2"; break;
    case 0x14:
        m_data->Type = "DDR2 FB-DIMM"; break;
    case 0x15:
        m_data->Type = "N/A"; break;
    case 0x16:
        m_data->Type = "N/A"; break;
    case 0x17:
        m_data->Type = "N/A"; break;
    case 0x18:
        m_data->Type = "DDR3"; break;
    case 0x19:
        m_data->Type = "FBD2"; break;
    }

    if( pInformation->TypeDetail & 0x1 )
        m_data->TypeDetails = "Reserved";
    else if( pInformation->TypeDetail & 0x2 )
        m_data->TypeDetails = "Other";
    else if( pInformation->TypeDetail & 0x4 )
        m_data->TypeDetails = "Unknown";
    else if( pInformation->TypeDetail & 0x8 )
        m_data->TypeDetails = "Fast-paged";
    else if( pInformation->TypeDetail & 0x10 )
        m_data->TypeDetails = "Static column";
    else if( pInformation->TypeDetail & 0x20 )
        m_data->TypeDetails = "Pseudo-static";
    else if( pInformation->TypeDetail & 0x40 )
        m_data->TypeDetails = "RAMBUS";
    else if( pInformation->TypeDetail & 0x80 )
        m_data->TypeDetails = "Synchronous";
    else if( pInformation->TypeDetail & 0x100 )
        m_data->TypeDetails = "CMOS";
    else if( pInformation->TypeDetail & 0x200 )
        m_data->TypeDetails = "EDO";
    else if( pInformation->TypeDetail & 0x400 )
        m_data->TypeDetails = "Window DRAM";
    else if( pInformation->TypeDetail & 0x800 )
        m_data->TypeDetails = "Cache DRAM";
    else if( pInformation->TypeDetail & 0x1000 )
        m_data->TypeDetails = "Non-volatile";
    else if( pInformation->TypeDetail & 0x2000 )
        m_data->TypeDetails = "Registered (Buffered)";
    else if( pInformation->TypeDetail & 0x4000 )
        m_data->TypeDetails = "Unbuffered (Unregistered)";
    else if( pInformation->TypeDetail & 0x8000 )
        m_data->TypeDetails = "LRDIMM";

    if( 0xFFFF == pInformation->Size )
        m_data->Size = "Unknown";
    else
        m_data->Size.setNum(pInformation->Size & 0x7FFF);
    if( pInformation->Size & 0x8000 )
        m_data->Size.append(" KB");
    else
        m_data->Size.append(" MB");

    if( 0 == pInformation->Speed )
        m_data->CurrentClockSpeed = "Unknown";
    else
        m_data->CurrentClockSpeed.sprintf("%d MHz", pInformation->Speed);

    m_data->TotalWidth.setNum(pInformation->TotalWidth);
    m_data->TotalWidth.append(" bits");

    m_data->DataWidth.setNum(pInformation->DataWidth);
    m_data->DataWidth.append(" bits");

    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->DeviceLocator )
            m_data->DeviceLocator = QString(pTempString);
        if( count == pInformation->BankLocator )
            m_data->BankLocator = QString(pTempString);
        if( count == pInformation->Manufacturer )
            m_data->Manufacturer = QString(pTempString);
        if( count == pInformation->SerialNumber )
            m_data->SerialNumber = QString(pTempString);
        if( count == pInformation->PartNumber )
            m_data->PartNumber = QString(pTempString);
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
