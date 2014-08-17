#include "../headers/type7_cache_information.h"


CType7CacheInformation::CType7CacheInformation()
{
    m_StructureType = CACHE_INFORMATION_TYPE;
    m_data = NULL;
    m_data = new T7CacheInformation;
}

CType7CacheInformation::~CType7CacheInformation()
{
    if( NULL != m_data )
    {
        delete m_data;
        m_data = NULL;
    }
}

SMStructureType CType7CacheInformation::GetStructureType()
{
    return m_StructureType;
}

int CType7CacheInformation::AddInformation(BYTE *pData)
{
    PSMHeader pHeader = (PSMHeader)(pData);
    PType7CacheInformation pInformation = (PType7CacheInformation)(pData + sizeof(SMHeader));
    int count = 1;
    char *pTempString;

    if( CACHE_INFORMATION_TYPE != pHeader->bType )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    // Operational mode
    switch(pInformation->CacheConfiguration & 0x300)
    {
    case 0x300:
        m_data->OperationalMode = "Unknown"; break;
    case 0x200:
        m_data->OperationalMode = "Varies with memory address"; break;
    case 0x100:
        m_data->OperationalMode = "Write back"; break;
    case 0:
        m_data->OperationalMode = "Write through"; break;
    }

    // Type
    switch(pInformation->CacheConfiguration & 0x60)
    {
    case 0x60:
        m_data->Type = "Unknown"; break;
    case 0x40:
        m_data->Type = "Reserved"; break;
    case 0x20:
        m_data->Type = "External"; break;
    case 0x0:
        m_data->Type = "Internal"; break;
    }

    // status
    if( pInformation->CacheConfiguration & 0x80 )
        m_data->Status = "Enabled";
    else
        m_data->Status = "Disabled";

    // associativity
    switch(pInformation->Associativity)
    {
    case 0x01:
        m_data->Associativity = "Other"; break;
    case 0x02:
        m_data->Associativity = "Unknown"; break;
    case 0x03:
        m_data->Associativity = "Direct mapped"; break;
    case 0x04:
        m_data->Associativity = "2-way Set-Associative"; break;
    case 0x05:
        m_data->Associativity = "4-way Set-Associative"; break;
    case 0x06:
        m_data->Associativity = "Fully Associative"; break;
    case 0x07:
        m_data->Associativity = "8-way Set-Associative"; break;
    case 0x08:
        m_data->Associativity = "16-way Set-Associative"; break;
    case 0x09:
        m_data->Associativity = "12-way Set-Associative"; break;
    case 0x0A:
        m_data->Associativity = "24-way Set-Associative"; break;
    case 0x0B:
        m_data->Associativity = "32-way Set-Associative"; break;
    case 0x0C:
        m_data->Associativity = "48-way Set-Associative"; break;
    case 0x0D:
        m_data->Associativity = "64-way Set-Associative"; break;
    case 0x0E:
        m_data->Associativity = "20-way Set-Associative"; break;
    }

    // max size
    m_data->MaxSize.sprintf("%d KB", pInformation->MaximumCacheSize & 0x7FFF);
    // installed size
    m_data->InstalledSize.sprintf("%d KB", pInformation->InstalledSize & 0x7FFF);

    // error correction
    switch(pInformation->ErrorCorrectionType)
    {
    case 0x01:
        m_data->ErrorCorrection = "Other"; break;
    case 0x02:
        m_data->ErrorCorrection = "Unknown"; break;
    case 0x03:
        m_data->ErrorCorrection = "None"; break;
    case 0x04:
        m_data->ErrorCorrection = "Parity"; break;
    case 0x05:
        m_data->ErrorCorrection = "Single-bit ECC"; break;
    case 0x06:
        m_data->ErrorCorrection = "Multi-bit ECC"; break;
    }

    // system cache type
    switch(pInformation->SystemCacheType)
    {
    case 0x01:
        m_data->SystemCacheType = "Other"; break;
    case 0x02:
        m_data->SystemCacheType = "Unknown"; break;
    case 0x03:
        m_data->SystemCacheType = "Instruction"; break;
    case 0x04:
        m_data->SystemCacheType = "Data"; break;
    case 0x05:
        m_data->SystemCacheType = "Unified"; break;
    }

    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->SocketDesignation )
            m_data->SocketDesignation = QString(pTempString);
        count++;
        pTempString = (char*)(pTempString + strlen(pTempString) + 1);
    }while(pTempString[0] != 0);

    return Success;
}

void *CType7CacheInformation::GetStructureData()
{
    return (void *)m_data;
}
