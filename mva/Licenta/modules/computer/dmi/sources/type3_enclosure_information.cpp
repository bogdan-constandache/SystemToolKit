#include "../headers/type3_enclosure_information.h"

CType3EnclosureInformation::CType3EnclosureInformation()
{
    m_StructureType = ENCLOSURE_INFORMATION_TYPE;
    m_data = NULL;
    m_data = new T3EnclosureInformation;
}

CType3EnclosureInformation::~CType3EnclosureInformation()
{
    if( NULL != m_data )
    {
        delete m_data;
        m_data = 0;
    }
}

SMStructureType CType3EnclosureInformation::GetStructureType()
{
    return m_StructureType;
}

int CType3EnclosureInformation::AddInformation(BYTE *pData)
{
    PSMHeader pHeader = (PSMHeader)(pData);
    PType3EnclosureInfomation pInformation = (PType3EnclosureInfomation)(pData + sizeof(SMHeader));
    int count = 1;
    char *pTempString = 0;

    if( ENCLOSURE_INFORMATION_TYPE != pHeader->bType )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    switch( pInformation->Type )
    {
    case 0x1:
        m_data->Type = "Other"; break;
    case 0x2:
        m_data->Type = "Unknown"; break;
    case 0x3:
        m_data->Type = "Desktop"; break;
    case 0x4:
        m_data->Type = "Low Profile Desktop"; break;
    case 0x5:
        m_data->Type = "Pizza Box"; break;
    case 0x6:
        m_data->Type = "Mini Tower"; break;
    case 0x7:
        m_data->Type = "Tower"; break;
    case 0x8:
        m_data->Type = "Portable"; break;
    case 0x9:
        m_data->Type = "Laptop"; break;
    case 0xA:
        m_data->Type = "Notebook"; break;
    case 0xB:
        m_data->Type = "Hand Held"; break;
    case 0xC:
        m_data->Type = "Docking Station"; break;
    case 0xD:
        m_data->Type = "All in One"; break;
    case 0xE:
        m_data->Type = "Sub Notebook"; break;
    case 0xF:
        m_data->Type = "Space-saving"; break;
    case 0x10:
        m_data->Type = "Lunch Box"; break;
    case 0x11:
        m_data->Type = "Main Server Chassis"; break;
    case 0x12:
        m_data->Type = "Expansion Chassis"; break;
    case 0x13:
        m_data->Type = "SubChassis"; break;
    case 0x14:
        m_data->Type = "Bus Expansion Chassis"; break;
    case 0x15:
        m_data->Type = "Peripheral Chassis"; break;
    case 0x16:
        m_data->Type = "RAID Chassis"; break;
    case 0x17:
        m_data->Type = "Rack Mount Chassis"; break;
    case 0x18:
        m_data->Type = "Sealed-case PC"; break;
    case 0x19:
        m_data->Type = "Multi-system Chassis"; break;
    case 0x1A:
        m_data->Type = "Compact PCI"; break;
    case 0x1B:
        m_data->Type = "Advanced TCA"; break;
    case 0x1C:
        m_data->Type = "Blade"; break;
    case 0x1D:
        m_data->Type = "Blade Enclosure"; break;
    }

    switch( pInformation->BootUpState )
    {
    case 0x1:
        m_data->BootUpState = "Other"; break;
    case 0x2:
        m_data->BootUpState = "Unknown"; break;
    case 0x3:
        m_data->BootUpState = "Safe"; break;
    case 0x4:
        m_data->BootUpState = "Warning"; break;
    case 0x5:
        m_data->BootUpState = "Critical"; break;
    case 0x6:
        m_data->BootUpState = "Non-recoverable"; break;
    }

    switch( pInformation->PowerSupplyState )
    {
    case 0x1:
        m_data->PowerSupplyState = "Other"; break;
    case 0x2:
        m_data->PowerSupplyState = "Unknown"; break;
    case 0x3:
        m_data->PowerSupplyState = "Safe"; break;
    case 0x4:
        m_data->PowerSupplyState = "Warning"; break;
    case 0x5:
        m_data->PowerSupplyState = "Critical"; break;
    case 0x6:
        m_data->PowerSupplyState = "Non-recoverable"; break;
    }

    switch( pInformation->ThermalState )
    {
    case 0x1:
        m_data->ThermalState = "Other"; break;
    case 0x2:
        m_data->ThermalState = "Unknown"; break;
    case 0x3:
        m_data->ThermalState = "Safe"; break;
    case 0x4:
        m_data->ThermalState = "Warning"; break;
    case 0x5:
        m_data->ThermalState = "Critical"; break;
    case 0x6:
        m_data->ThermalState = "Non-recoverable"; break;
    }

    switch( pInformation->SecurityStatus )
    {
    case 0x1:
        m_data->SecurityStatus = "Other"; break;
    case 0x2:
        m_data->SecurityStatus = "Unknown"; break;
    case 0x3:
        m_data->SecurityStatus = "None"; break;
    case 0x4:
        m_data->SecurityStatus = "External interface locked out"; break;
    case 0x5:
        m_data->SecurityStatus = "External interface enabled"; break;
    }

    m_data->OEMSpecific.sprintf("0x%x", pInformation->OEMDefined);
    m_data->Height.sprintf("%d", pInformation->Height);
    m_data->NumberOfPowerCords.sprintf("%d", pInformation->NumberOfPowerCords);

    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->Manufacturer )
            m_data->Manufacturer = QString(pTempString);
        if( count == pInformation->SerialNumber )
            m_data->SerialNumber = QString(pTempString);
        if( count == pInformation->Version )
            m_data->Version = QString(pTempString);
        count++;
        pTempString = (char*)(pTempString + strlen(pTempString) + 1);
    }while(pTempString[0] != 0);

    return Success;
}

void *CType3EnclosureInformation::GetStructureData()
{
    return (void*)m_data;
}
