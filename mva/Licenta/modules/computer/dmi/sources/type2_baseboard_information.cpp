#include "../headers/type2_baseboard_information.h"

CType2BaseBoardInformation::CType2BaseBoardInformation()
{
    this->m_StructureType = BASEBOARD_INFORMATION_TYPE;
    this->m_data = NULL;
    this->m_data = new T2BaseBoardInformation;
}

CType2BaseBoardInformation::~CType2BaseBoardInformation()
{
    if( 0 != this->m_data )
    {
        delete this->m_data;
        this->m_data = 0;
    }
}

int CType2BaseBoardInformation::AddInformation(BYTE *pData)
{
    PSMHeader pHeader = (PSMHeader)(pData);
    PType2BaseBoardInformation pInformation = (PType2BaseBoardInformation)(pData + sizeof(SMHeader));
    int count = 1;
    char *pTempString = 0;

    if( BASEBOARD_INFORMATION_TYPE != pHeader->bType )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    if( pInformation->FeatureFlags & 0x10 )
    {
        m_data->HotSwappable = QString("Yes");
    }
    else
    {
        m_data->HotSwappable = QString("No");
    }
    if( pInformation->FeatureFlags & 0x08 )
    {
        m_data->Replaceable = QString("Yes");
    }
    else
    {
        m_data->Replaceable = QString("No");
    }
    if( pInformation->FeatureFlags & 0x04 )
    {
        m_data->Removable = QString("Yes");
    }
    else
    {
        m_data->Removable = QString("No");
    }
    if( pInformation->FeatureFlags & 0x01 )
    {
        m_data->HostingBoard = QString("Yes");
    }
    else
    {
        m_data->HostingBoard = QString("No");
    }

    switch(pInformation->BoardType)
    {
    case 0x01:
        m_data->Type = QString("Unknown");
        break;
    case 0x02:
        m_data->Type = QString("Other");
        break;
    case 0x03:
        m_data->Type = QString("Server Blades");
        break;
    case 0x04:
        m_data->Type = QString("Connectivity Switch");
        break;
    case 0x05:
        m_data->Type = QString("System Management Module");
        break;
    case 0x06:
        m_data->Type = QString("Processor Module");
        break;
    case 0x07:
        m_data->Type = QString("I/O Module");
        break;
    case 0x08:
        m_data->Type = QString("Memory Module");
        break;
    case 0x09:
        m_data->Type = QString("Daughter board");
        break;
    case 0x0A:
        m_data->Type = QString("Motherboard (includes processor, memory, and I/O)");
        break;
    case 0x0B:
        m_data->Type = QString("Processor/Memory Module");
        break;
    case 0x0C:
        m_data->Type = QString("Processor/IO Module");
        break;
    case 0x0D:
        m_data->Type = QString("Interconnect board");
        break;
    }

    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->Manufacturer )
            m_data->Manufacturer = QString(pTempString);
        if( count == pInformation->Product )
            m_data->Product = QString(pTempString);
        if( count == pInformation->SerialNumber )
            m_data->SerialNumber = QString(pTempString);
        if( count == pInformation->Location )
            m_data->Location = QString(pTempString);
        if( count == pInformation->Version )
            m_data->Version = QString(pTempString);
        count++;
        pTempString = (char*)(pTempString + strlen(pTempString) + 1);
    }while(pTempString[0] != 0);

    return Success;
}

void *CType2BaseBoardInformation::GetStructureData()
{
    return (void*)this->m_data;
}


SMStructureType CType2BaseBoardInformation::GetStructureType()
{
    return this->m_StructureType;
}
