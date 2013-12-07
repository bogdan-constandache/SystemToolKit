#include "../headers/identify.h"

CIdentifyData::CIdentifyData():
    m_pData(NULL)
{
    m_pData = new _IDENTIFY_DEVICE_DATA_;
}

CIdentifyData::~CIdentifyData()
{
    if( 0 != m_pData )
    {
        delete m_pData;
        m_pData = 0;
    }
}

int CIdentifyData::Initialize(const wchar_t *wszDriveName)
{
    DWORD dwResult = 0;
    SENDCMDINPARAMS pIN;
    HANDLE hDevice = NULL;
    WORD *bData = new WORD[OUT_IDENTIFY_SIZE];

    WORD *pIdentifyData = 0;

    memset(&pIN, 0, sizeof(SENDCMDINPARAMS));
    memset(bData, 0, OUT_IDENTIFY_SIZE);

    if( NULL == wszDriveName )
        return InvalidParameter;

    hDevice = CreateFile(
                wszDriveName,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_SYSTEM,
                NULL);

    if( INVALID_HANDLE_VALUE == hDevice )
        return InvalidHandle;

    pIN.cBufferSize = IDENTIFY_BUFFER_SIZE;
    pIN.irDriveRegs.bFeaturesReg = 0;
    pIN.irDriveRegs.bSectorCountReg = 1;
    pIN.irDriveRegs.bSectorNumberReg = 1;
    pIN.irDriveRegs.bCylLowReg = 0;
    pIN.irDriveRegs.bCylHighReg = 0;
    pIN.irDriveRegs.bDriveHeadReg = DRIVE_HEAD_REG;
    pIN.irDriveRegs.bCommandReg = ID_CMD;

    bool bResult = DeviceIoControl(
                hDevice,
                SMART_RCV_DRIVE_DATA,
                &pIN,
                sizeof(SENDCMDINPARAMS),
                bData,
                OUT_BUFFER_SIZE,
                &dwResult,
                NULL);

    CloseHandle(hDevice);

    DWORD dwErr = GetLastError();

    if( bResult )
    {
        pIdentifyData = bData + 8;
        ParseData(pIdentifyData);
    }
    else
    {

    }

    delete bData;

    return Success;
}

void CIdentifyData::ParseData(WORD *pData)
{
    m_pData->Model = ConvertToASCII(pData + START_MODEL, LENGTH_MODEL);

    m_pData->FirmwareRevision = ConvertToASCII(pData + START_FIRMWARE, LENGTH_FIRMWARE);

    m_pData->SerialNumber = ConvertToASCII(pData + START_SERIAL, LENGTH_SERIAL);

    m_pData->Manufacturer = ConvertToASCII(pData + START_MANUFACTURER, LENGTH_MANUFACTURER);
    qDebug() << m_pData->Manufacturer;

    // form factor
    m_pData->FormFactor.sprintf("0x%04x", pData[FORM_FACTOR]);
    qDebug() << m_pData->FormFactor;
    if( pData[FORM_FACTOR] && (pData[FORM_FACTOR] & 0xfff8) == 0 )
    {
        switch(pData[FORM_FACTOR]) {
        case 1:
            m_pData->FormFactor = QString("5.25 inch");
            break;
        case 2:
            m_pData->FormFactor = QString("3.5 inch");
            break;
        case 3:
            m_pData->FormFactor = QString("2.5 inch");
            break;
        case 4:
            m_pData->FormFactor = QString("1.8 inch");
            break;
        case 5:
            m_pData->FormFactor = QString("Less than 1.8 inch");
            break;
        default:
            m_pData->FormFactor.sprintf("Unknown (0x%04x]", pData[FORM_FACTOR]);
            break;
        }
    }

    if (pData[20] <= 3 && pData[BUFFER_SIZE] && pData[BUFFER_SIZE] != 0xffff)
    {
        m_pData->BufferSize.sprintf("%u KBytes", pData[BUFFER_SIZE] / 2);
    }
    qDebug() << m_pData->BufferSize;

    if( 1 == pData[NMRR] )
        m_pData->DeviceType.sprintf("Nominal Media Rotation Rate: Solid State Device");
    else
        if( pData[NMRR] > 0x401 )
            m_pData->DeviceType.sprintf("Nominal Media Rotation Rate: %u", pData[NMRR]);

    qDebug() << m_pData->DeviceType;
}

QString CIdentifyData::ConvertToASCII(WORD *pData, unsigned int nLength)
{
    QString retValue = "";

    for(unsigned int i = 0; i < nLength; i++)
    {
        retValue.append((char)(0x00ff & pData[i] >> 8 ));
        retValue.append((char)(0x00ff & pData[i] ));
    }
    return retValue.trimmed();
}
