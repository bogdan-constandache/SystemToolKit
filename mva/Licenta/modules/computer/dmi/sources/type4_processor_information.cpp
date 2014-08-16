#include "../headers/type4_processor_information.h"


CType4ProcessorInformation::CType4ProcessorInformation()
{
    this->m_StructureType = PROCESSOR_INFORMATION_TYPE;
    this->m_data = NULL;
    this->m_data = new T4ProcessorInformation;
}

CType4ProcessorInformation::~CType4ProcessorInformation()
{
    if( 0 == this->m_data )
    {
        delete this->m_data;
        this->m_data = 0;
    }
}

SMStructureType CType4ProcessorInformation::GetStructureType()
{
    return this->m_StructureType;
}

int CType4ProcessorInformation::AddInformation(BYTE *pData)
{
    PSMHeader pHeader = (PSMHeader)(pData);
    PType4ProcessorInformation pInformation = (PType4ProcessorInformation)(pData + sizeof(SMHeader));
    char *pTempString = 0;
    int count = 1;

    if( PROCESSOR_INFORMATION_TYPE != pHeader->bType )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    short x = 0;
    memcpy(&x,(pData+0x12),2);
    m_data->ProcessorID = QString("N/A");

    switch(pInformation->ProcessorType)
    {
    case 0x01:
        m_data->ProcessorType = QString("Other");
        break;
    case 0x02:
        m_data->ProcessorType = QString("Unknown");
        break;
    case 0x03:
        m_data->ProcessorType = QString("Central Processor");
        break;
    case 0x04:
        m_data->ProcessorType = QString("Math Processor");
        break;
    case 0x05:
        m_data->ProcessorType = QString("DSP Processor");
        break;
    case 0x06:
        m_data->ProcessorType = QString("Video Processor");
        break;
    }

    if( 0 == (pInformation->Voltage % 0x80) )
    {
        if(pInformation->Voltage % 0x01)
            m_data->Voltage = QString("5V");
        if(pInformation->Voltage % 0x02)
            m_data->Voltage = QString("3.3V");
        if(pInformation->Voltage % 0x04)
            m_data->Voltage = QString("2.9V");
    }
    else
    {
        double fVoltage = (double)(pInformation->Voltage - 0x80);
        m_data->Voltage.sprintf("%.1f V", (fVoltage / 10));
    }

    if( 0 == pInformation->MaxSpeed )
        m_data->MaxSpeed = "N/A";
    else
    {
        qDebug() << pInformation->MaxSpeed;
        m_data->MaxSpeed.sprintf("%d MHz", pInformation->MaxSpeed);
        qDebug() << m_data->MaxSpeed;
    }

    if( 0 == pInformation->CurrentSpeed )
        m_data->CurrentSpeed = "N/A";
    else
        m_data->CurrentSpeed.sprintf("%d MHz", pInformation->CurrentSpeed);

    m_data->ProcessorFamily.sprintf("0x%x", pInformation->ProcessorFamily);

    if( 0 == pInformation->ExternalClock )
        m_data->ExtClock = "N/A";
    else
    {
        m_data->ExtClock.sprintf("%d MHz", pInformation->ExternalClock);
    }

    m_data->CoreCount.sprintf("%d", pInformation->CoreCount);
    qDebug() << m_data->CoreCount;

    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->SerialNumber )
            m_data->SerialNumber = QString(pTempString);
        if( count == pInformation->ProcessorManufactor )
            m_data->ProcessorManufacturer = QString(pTempString);
        if( count == pInformation->ProcessorVersion )
            m_data->Version = QString(pTempString);
        count++;
        pTempString = (char*)(pTempString + strlen(pTempString) + 1);
    }while(pTempString[0] != 0);

    return Success;
}

void *CType4ProcessorInformation::GetStructureData()
{
    return (void*)m_data;
}
