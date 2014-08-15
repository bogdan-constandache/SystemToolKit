#include "../headers/csmartinfo.h"

CSmartInfo::CSmartInfo():
    m_data(0), m_bErrorFlag(false)
{
    // read details out of database
    int nStatus = ReadSMARTDetailsFromDB();
    if( Success != nStatus )
    {
        m_bErrorFlag = true;
        return;
    }
    // get physical drives list (e.g. PhysicalDrive0, PhysicalDrive1, etc..)
    m_PhysicalDrives = GetPhysicalDrivesList();
    for(int i = 0; i < m_PhysicalDrives.count(); i++)
    {
        ATADeviceProperties *pProp = GetATADeviceProperties(m_PhysicalDrives.at(i).toStdWString().c_str());
        if( !pProp )
            continue;
        m_PhysicalDrivesToModel.insert(pProp->Model, m_PhysicalDrives.at(i));
        delete pProp;
    }
}

CSmartInfo::~CSmartInfo()
{
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
//    pDriveInfo = new DriveInfo;
//    if( 0 == pDriveInfo )
//    {
//        nStatus = NotAllocated;
//        DEBUG_STATUS(nStatus);
//        return nStatus;
//    }

    // we save the drive index
    m_data->DriveIndex = qzDriveName.right(qzDriveName.count() - 1).toShort();

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

// get the attributes from database
int CSmartInfo::ReadSMARTDetailsFromDB()
{
    {
        SmartDetails *pSmartDetails = 0;
        QString qzAppPath = QCoreApplication::applicationDirPath();
        qzAppPath.append("/config/databases/config.db");
        QSqlDatabase qDB = QSqlDatabase::addDatabase("QSQLITE");
        qDB.setDatabaseName(qzAppPath);

        bool bRet = qDB.open();
        if( !bRet )
        {
            return DBOpenError;
        }

        QSqlQuery qQuery("SELECT * FROM SMART_DETAILS", qDB);

        while( qQuery.next() )
        {
            pSmartDetails = new SmartDetails;
            if( 0 == pSmartDetails )
            {
                return NotAllocated;
            }
            pSmartDetails->m_ucAttribId = 0;
            pSmartDetails->m_bCritical = 0;

            pSmartDetails->m_ucAttribId = qQuery.value(0).toInt();
            pSmartDetails->m_bCritical = qQuery.value(1).toInt();
            pSmartDetails->m_csAttribName = qQuery.value(2).toString();
            pSmartDetails->m_csAttribDetails = qQuery.value(3).toString();

            m_dbSmartDetails.insert(pSmartDetails->m_ucAttribId, pSmartDetails);
        }

        qDB.close();
    }

    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    return Success;
}

// getting smart details for required index
SmartDetails *CSmartInfo::GetSMARTDetailsFromDB(short sAttribIndex)
{
    QMap<BYTE, SmartDetails*>::iterator pIt;
    SmartDetails *pRet=NULL;

    pIt = m_dbSmartDetails.find(sAttribIndex);
    if(pIt != m_dbSmartDetails.end())
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

QList<SmartData*> CSmartInfo::GetSmartData()
{
    return m_data->SmartEntries;
}

QStandardItemModel *CSmartInfo::GetAvailableHDD()
{
    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pItem = 0;

    pModel->setColumnCount(1);
    pModel->setHorizontalHeaderLabels(QStringList() << "Device description");

    if( m_bErrorFlag )
    {
        pItem = new QStandardItem("There was a problem detecting SMART properties for your HDD");
        pModel->setItem(0, 0, pItem);
        return pModel;
    }

    for(int i = 0; i < m_PhysicalDrivesToModel.keys().count(); i++)
    {
        pItem = new QStandardItem(m_PhysicalDrivesToModel.keys().at(i));
        pItem->setIcon(QIcon(":/img/hdd.png"));
        pModel->setItem(i, 0, pItem);
    }

    return pModel;
}

QStandardItemModel *CSmartInfo::GetSMARTPropertiesForHDD(QString qzModel)
{
    if( m_bErrorFlag )
        return 0;

    QStandardItemModel *pModel = new QStandardItemModel();
    SmartDetails *pSmartDetails = 0;
    SmartData *pSmartData = 0;
    QString qzDrive = m_PhysicalDrivesToModel.value(qzModel);

    int nStatus = Initialize(qzDrive.right(qzDrive.count() - 1).toShort());
    if (Success != nStatus)
        return 0;

    pModel->setHorizontalHeaderLabels(QStringList() << "ID" << "Property name" << "Raw" << "Value" << "Worst" << "Threshold");

    QList<QStandardItem*> qList;

    for(int i = 0; i < m_data->SmartEntries.count(); i++)
    {
        pSmartData = m_data->SmartEntries.at(i);
        pSmartDetails = this->GetSMARTDetailsFromDB(pSmartData->m_ucAttribIndex);

        if( !pSmartData || !pSmartDetails )
            continue;

        qList << new QStandardItem(QString().setNum(pSmartDetails->m_ucAttribId));
        qList << new QStandardItem(pSmartDetails->m_csAttribName);
        // raw
        qList << new QStandardItem(QString().setNum(pSmartData->m_dwAttribValue));
        // value
        qList << new QStandardItem(QString().setNum(pSmartData->m_ucValue));
        // worst
        qList << new QStandardItem(QString().setNum(pSmartData->m_ucWorst));
        // threshold
        qList << new QStandardItem(QString().setNum(pSmartData->m_dwThreshold));

        pModel->appendRow(qList);
        qList.clear();
    }

    return pModel;
}
