#include "../../include/smart/csmartinfo.h"

// static member for getting details out of database
QMap<BYTE, SmartDetails*>* CSmartInfo::m_dbSmartDetails = 0;

CSmartInfo::CSmartInfo() : m_data(0)
{
    // read details out of database
    m_dbSmartDetails = new QMap<BYTE, SmartDetails*>;
    ReadSMARTDetailsFromDB();
    m_PhysicalDrives = GetPhysicalDrivesList();
}

CSmartInfo::~CSmartInfo()
{
}

// callback for SQLite necessary for processing query results
int CSmartInfo::SQLiteCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
    UNUSED(NotUsed);
    UNUSED(argc);
    UNUSED(azColName);
    int nStatus = Uninitialized;

    SmartDetails *pSmartDetails = 0;
    pSmartDetails = new SmartDetails;
    if( 0 == pSmartDetails )
    {
        nStatus = NotAllocated;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    pSmartDetails->m_ucAttribId = 0;
    pSmartDetails->m_bCritical = 0;

    pSmartDetails->m_ucAttribId = QString(argv[0]).toInt();
    pSmartDetails->m_bCritical = QString(argv[1]).toInt();
    pSmartDetails->m_csAttribName = QString(argv[2]);
    pSmartDetails->m_csAttribDetails = QString(argv[3]);

    m_dbSmartDetails->insert(pSmartDetails->m_ucAttribId, pSmartDetails);

    nStatus = Success;
    return nStatus;
}

int CSmartInfo::Initialize(int nDriveIndex)
{
    int nStatus = Uninitialized;
    QString qzDriveName = "";
    DriveInfo *pDriveInfo = NULL;
    bool bResult;

    if( 0 == m_data )
    {
        m_data = new DriveInfo;
    }
    else
    {
        delete m_data;
        m_data = 0;
        m_data = new DriveInfo;
    }
    if( 0 == m_data )
    {
        nStatus = NotAllocated;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    if( nDriveIndex >= m_PhysicalDrives.count() )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }
    qzDriveName = m_PhysicalDrives.at(nDriveIndex);
    pDriveInfo = new DriveInfo;
    if( 0 == pDriveInfo )
    {
        nStatus = NotAllocated;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    // we save the drive letter
    pDriveInfo->DriveIndex = qzDriveName.right(qzDriveName.count() - 1).toShort();

    // check for cmd commands;
    bResult = CheckForCMDCommands(qzDriveName.toStdWString().c_str());
    if( false == bResult )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    // check for smart flag
    bResult = CheckForSmartFlag(qzDriveName.toStdWString().c_str());
    if( false == bResult )
    {
        nStatus = Unsuccessful;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    // getDriveInformation
    m_data->DeviceInformation = GetATADriveData(qzDriveName.toStdWString().c_str());

    nStatus = CollectSmartAttributes(qzDriveName.toStdWString().c_str());
    if( Success != nStatus )
    {
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    nStatus = Success;
    return nStatus;
}

bool CSmartInfo::CheckForCMDCommands(const wchar_t *tszDriveName)
{
    HANDLE hDevice = NULL;
    DWORD dwRet = 0;
    GETVERSIONINPARAMS params;

    if( NULL == tszDriveName )
    {
        DEBUG_STATUS(InvalidParameter);
    }

    // open a handle to the physical drive
    hDevice = CreateFile(
                tszDriveName,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_SYSTEM,
                NULL);
    if( INVALID_HANDLE_VALUE == hDevice )
    {
        DEBUG_STATUS(InvalidHandle);
    }

    // send IOCTL -> SMART_GET_VERSION to check if disk accepts CMD commands
    bool bResult = DeviceIoControl(
                hDevice,
                SMART_GET_VERSION,
                NULL,
                0,
                &params,
                sizeof(GETVERSIONINPARAMS),
                &dwRet,
                NULL);
    if( 0 == bResult )
    {
        DEBUG_STATUS(Unsuccessful);
    }
    CloseHandle(hDevice);

    //check if we can send cmd commands to the disk.
    if( (params.fCapabilities & CAP_SMART_CMD) == CAP_SMART_CMD )
    {
        return true;
    }
    return false;
}

bool CSmartInfo::CheckForSmartFlag(const wchar_t *tszDriveName)
{
    DWORD dwResult = 0;
    HANDLE hDevice = NULL;
    SENDCMDINPARAMS paramsIN;
    SENDCMDOUTPARAMS paramsOUT;
    memset(&paramsIN, 0, sizeof(SENDCMDINPARAMS));
    memset(&paramsOUT, 0, sizeof(SENDCMDOUTPARAMS));

    if( NULL == tszDriveName )
    {
        DEBUG_STATUS(InvalidParameter);
    }

    paramsIN.cBufferSize=0;
    paramsIN.irDriveRegs.bFeaturesReg=ENABLE_SMART;
    paramsIN.irDriveRegs.bSectorCountReg = 1;
    paramsIN.irDriveRegs.bSectorNumberReg = 1;
    paramsIN.irDriveRegs.bCylLowReg = SMART_CYL_LOW;
    paramsIN.irDriveRegs.bCylHighReg = SMART_CYL_HI;
    paramsIN.irDriveRegs.bDriveHeadReg = DRIVE_HEAD_REG;
    paramsIN.irDriveRegs.bCommandReg = SMART_CMD;

    // open a handle to the physical drive
    hDevice = CreateFile(
                tszDriveName,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_SYSTEM,
                NULL);
    if( INVALID_HANDLE_VALUE == hDevice )
    {
        DEBUG_STATUS(InvalidHandle);
    }

    bool bResult = DeviceIoControl(
                hDevice,
                SMART_SEND_DRIVE_COMMAND,
                &paramsIN,
                sizeof(SENDCMDINPARAMS),
                &paramsOUT,
                sizeof(SENDCMDOUTPARAMS),
                &dwResult,
                NULL );
    return bResult;
}

// get SMART Identify information
DeviceData *CSmartInfo::GetATADriveData(const wchar_t *tszDriveName)
{
    DWORD dwResult=0;
    SENDCMDINPARAMS paramsIN;
    HANDLE hDevice = NULL;
    DeviceData *pData = NULL;
    BYTE paramOUT[OUT_BUFFER_SIZE];
    memset(&paramOUT, 0, sizeof(paramOUT));
    memset(&paramsIN, 0, sizeof(SENDCMDINPARAMS));

    if( NULL == tszDriveName )
    {
        DEBUG_STATUS(InvalidParameter);
    }

    // prepare parameter for DeviceIoControl function
    // telling that i need Identify information
    // ID_CMD
    paramsIN.cBufferSize=IDENTIFY_BUFFER_SIZE;
    paramsIN.irDriveRegs.bFeaturesReg=0;
    paramsIN.irDriveRegs.bSectorCountReg = 1;
    paramsIN.irDriveRegs.bSectorNumberReg = 1;
    paramsIN.irDriveRegs.bCylLowReg = 0;
    paramsIN.irDriveRegs.bCylHighReg = 0;
    paramsIN.irDriveRegs.bDriveHeadReg = DRIVE_HEAD_REG;
    paramsIN.irDriveRegs.bCommandReg = ID_CMD;

    hDevice = CreateFile(
                tszDriveName,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_SYSTEM,
                NULL);
    if( INVALID_HANDLE_VALUE == hDevice )
    {
        DEBUG_STATUS(InvalidHandle);
    }

    bool bResult = DeviceIoControl(
                hDevice,
                SMART_RCV_DRIVE_DATA,
                &paramsIN,
                sizeof(SENDCMDINPARAMS),
                paramOUT,
                OUT_BUFFER_SIZE,
                &dwResult,
                NULL );
    // if function was successful
    if( bResult )
    {
        pData = new DeviceData;
        if( NULL == pData )
        {
            DEBUG_STATUS(NotAllocated);
        }
        // fill struct members
        pData->FirmwareRevision = ParseFirmware(paramOUT);
        pData->Model = ParseModel(paramOUT);
        pData->SerialNumber = ParseSerial(paramOUT);
    }
    return pData;
}

QString CSmartInfo::ParseFirmware(PBYTE pbData)
{
    QString retValue = "";
    WCHAR wzTemp[9];
    memset(wzTemp, '\0', 9);
    PBYTE offSet = pbData + 16 + 46;
    for(int i = 0; i < 8; i+=2)
    {
        wzTemp[i] = offSet[i+1];
        wzTemp[i+1] = offSet[i];
    }
    wzTemp[8] = L'\0';
    retValue = QString::fromWCharArray(wzTemp);
    return retValue.trimmed();
}

QString CSmartInfo::ParseModel(PBYTE pbData)
{
    QString retValue = "";
    WCHAR wzTemp[41];
    memset(wzTemp, '\0', 41);
    PBYTE offSet = pbData + 16 + 54;
    for(int i = 0; i < 40; i+=2)
    {
        wzTemp[i] = offSet[i+1];
        wzTemp[i+1] = offSet[i];
    }
    wzTemp[40] = L'\0';
    retValue = QString::fromWCharArray(wzTemp);
    return retValue.trimmed();
}

QString CSmartInfo::ParseSerial(PBYTE pbData)
{
    QString retValue = "";
    WCHAR wzTemp[21];
    memset(wzTemp, '\0', 21);
    PBYTE offSet = pbData + 16 + 20;
    for(int i = 0; i < 20; i+=2)
    {
        wzTemp[i] = offSet[i+1];
        wzTemp[i+1] = offSet[i];
    }
    wzTemp[20] = L'\0';
    retValue = QString::fromWCharArray(wzTemp);
    return retValue.trimmed();
}

// get the attributes from database
int CSmartInfo::ReadSMARTDetailsFromDB()
{
    int nStatus = Uninitialized;
    sqlite3 *db;
    QString qszSQLStatement;
    char *pszErrMsg = 0;

    nStatus = sqlite3_open("config/databases/config.db", &db);
    if( Success != nStatus )
    {
        nStatus = DBOpenError;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    qszSQLStatement = QString("SELECT * from SMART_DETAILS");

    /* Execute SQL statement */
    nStatus = sqlite3_exec(db, qszSQLStatement.toStdString().c_str(), this->SQLiteCallback, 0, &pszErrMsg);

    if( Success != nStatus )
    {
        sqlite3_free(pszErrMsg);
        nStatus = DBExecError;
        DEBUG_STATUS(nStatus);
        return nStatus;
    }

    sqlite3_close(db);
    SafeDelete(pszErrMsg);

    return nStatus;
}

// getting smart details for required index
SmartDetails *CSmartInfo::GetSMARTDetails(short sAttribIndex)
{
    QMap<BYTE, SmartDetails*>::iterator pIt;
    SmartDetails *pRet=NULL;

    pIt = m_dbSmartDetails->find(sAttribIndex);
    if(pIt != m_dbSmartDetails->end())
    {
        pRet=pIt.value();
    }
    return pRet;
}

// read SMART data attributs
int CSmartInfo::CollectSmartAttributes(const wchar_t *tszDriveName)
{
    int nStatus = Uninitialized;

    SENDCMDINPARAMS paramIN;
    BYTE paramOUT[sizeof(ATAOutParam) + READ_ATTRIBUTE_BUFFER_SIZE - 1];
    DWORD dwResult = 0;
    PBYTE pbATABuffer = NULL, pbTempBuffer = NULL;
    PDWORD pTemp2;
    SmartData *pSmartValues = NULL;
    HANDLE hDevice = NULL;

    memset(&paramIN, 0, sizeof(SENDCMDINPARAMS));

    if( NULL == tszDriveName )
    {
        DEBUG_STATUS(InvalidParameter);
    }

    // prepare params for DeviceIoControl
    // telling that i need attribute details
    // SMART_CMD

    // open a handle to the physical drive
    hDevice = CreateFile(
                tszDriveName,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_SYSTEM,
                NULL);
    if( INVALID_HANDLE_VALUE == hDevice )
    {
        DEBUG_STATUS(InvalidHandle);
    }

    paramIN.cBufferSize = READ_ATTRIBUTE_BUFFER_SIZE;
    paramIN.bDriveNumber = (BYTE)m_data->DriveIndex;
    paramIN.irDriveRegs.bFeaturesReg = READ_ATTRIBUTES;
    paramIN.irDriveRegs.bSectorCountReg = 1;
    paramIN.irDriveRegs.bSectorNumberReg = 1;
    paramIN.irDriveRegs.bCylLowReg = SMART_CYL_LOW;
    paramIN.irDriveRegs.bCylHighReg = SMART_CYL_HI;
    paramIN.irDriveRegs.bDriveHeadReg = DRIVE_HEAD_REG;
    paramIN.irDriveRegs.bCommandReg = SMART_CMD;

    bool bResult = DeviceIoControl(
                hDevice,
                SMART_RCV_DRIVE_DATA,
                &paramIN,
                sizeof(SENDCMDINPARAMS),
                paramOUT,
                sizeof(ATAOutParam) + READ_ATTRIBUTE_BUFFER_SIZE - 1,
                &dwResult,
                NULL);
    //if function was successful
    if(bResult)
    {
        // then we parse data
        pbATABuffer=(PBYTE)(((ATAOutParam*)paramOUT)->bBuffer);
        for(int i = 0; i < 30; ++i)
        {
            pbTempBuffer = &pbATABuffer[2 + i * 12];
            pTemp2 = (PDWORD)&pbTempBuffer[INDEX_ATTRIB_RAW];
            pbTempBuffer[INDEX_ATTRIB_RAW+2] = 0;
            pbTempBuffer[INDEX_ATTRIB_RAW+3] = 0;
            pbTempBuffer[INDEX_ATTRIB_RAW+4] = 0;
            pbTempBuffer[INDEX_ATTRIB_RAW+5] = 0;
            pbTempBuffer[INDEX_ATTRIB_RAW+6] = 0;

            if(pbTempBuffer[INDEX_ATTRIB_INDEX] != 0)
            {
                pSmartValues = new SmartData;
                pSmartValues->m_ucAttribIndex=pbTempBuffer[INDEX_ATTRIB_INDEX];
                pSmartValues->m_ucValue=pbTempBuffer[INDEX_ATTRIB_VALUE];
                pSmartValues->m_ucWorst=pbTempBuffer[INDEX_ATTRIB_WORST];
                pSmartValues->m_dwAttribValue=pTemp2[0];
                pSmartValues->m_dwThreshold=MAXDWORD;
                m_data->SmartEntries.append(pSmartValues);
            }
        }
    }
    else
        DEBUG_STATUS(Unsuccessful);

    // prepare params for another DeviceIoControl
    // this time i neeed threshold details
    paramIN.irDriveRegs.bFeaturesReg=READ_THRESHOLDS;
    paramIN.cBufferSize=READ_THRESHOLD_BUFFER_SIZE; // Is same as attrib size
    bResult = DeviceIoControl(
                hDevice,
                SMART_RCV_DRIVE_DATA,
                &paramIN,
                sizeof(SENDCMDINPARAMS),
                paramOUT,
                sizeof(ATAOutParam) + READ_ATTRIBUTE_BUFFER_SIZE - 1,
                &dwResult,
                NULL);
    // if function was successful then we parse data and fill the drive info
    if(bResult)
    {
        pbATABuffer=(PBYTE)(((ATAOutParam*)paramOUT)->bBuffer);
        for(int i = 0; i < 30; ++i)
        {
            pTemp2=(PDWORD)&pbATABuffer[2 + i*12 + 1];
            pbTempBuffer=&pbATABuffer[2 + i * 12];

            if(pbTempBuffer[0]!=0)
            {
                pSmartValues = 0;
                for(int i = 0; i < m_data->SmartEntries.count(); i++)
                {
                    if( pbTempBuffer[0] == m_data->SmartEntries.at(i)->m_ucAttribIndex )
                    {
                        pSmartValues = m_data->SmartEntries.at(i);
                        break;
                    }
                }
                if(pSmartValues)
                    pSmartValues->m_dwThreshold = pTemp2[0];
            }
        }
    }
    else
        DEBUG_STATUS(Unsuccessful);
    nStatus = Success;
    return nStatus;
}

// return drive info struct according to the required index
DeviceData *CSmartInfo::GetDriveInfo()
{
    return m_data->DeviceInformation;
}

QList<SmartData *> CSmartInfo::GetSmartData()
{
    return m_data->SmartEntries;
}

QStringList CSmartInfo::GetAvailablePhysicalDrives()
{
    return m_PhysicalDrives;
}
