#include "../headers/csmartinfo.h"

CSmartInfo::CSmartInfo():
    m_data(0),
    m_pAvailableHDDModel(NULL),
    m_pAvailableHDDTableView(NULL),
    m_pHDDDetailsModel(NULL),
    m_pHDDDetailsTableView(NULL)
{
    // read details out of database
    ReadSMARTDetailsFromDB();
    m_PhysicalDrives = GetPhysicalDrivesList();

    m_pAvailableHDDModel = new QStandardItemModel();
    m_pHDDDetailsModel = new QStandardItemModel();
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

    ATADeviceProperties *pDev = GetATADeviceProperties(qzDriveName.toStdWString().c_str());
    qDebug() << "Rotation Speed " << pDev->RotationSpeed;
    qDebug() << "UDMA " << pDev->UDMATransferMode;
    qDebug() << "Active UDMA " << pDev->ActiveUDMATransferMode;
    qDebug() << "PIO " << pDev->PIOTransferMode;
    qDebug() << "MWDMA " << pDev->MWDMATransferMode;
    qDebug() << "Model " << pDev->Model;
    qDebug() << "Firmware " << pDev->FirmwareRevision;
    qDebug() << "Serial " << pDev->SerialNumber;
    qDebug() << "Buffer size " << pDev->BufferSize;
    qDebug() << "Device type " << pDev->DeviceType;
    qDebug() << "Cylinders" << pDev->Cylinders;
    qDebug() << "Logical Heads" << pDev->Heads;
    qDebug() << "Sector per track " << pDev->SectorPerTrack;
    qDebug() << "Bytes per sector " << pDev->BytesPerSector;
    qDebug() << "ATA Standard " << pDev->ATAStandard;

    delete pDev;

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
//    BYTE paramOUT[OUT_BUFFER_SIZE];
//    memset(&paramOUT, 0, sizeof(paramOUT));
    memset(&paramsIN, 0, sizeof(SENDCMDINPARAMS));

    int nBufferSize = 0;
    nBufferSize = IDENTIFY_BUFFER_SIZE + sizeof(SENDCMDOUTPARAMS);
    BYTE paramOUT[IDENTIFY_BUFFER_SIZE + sizeof(SENDCMDOUTPARAMS)];

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
        BYTE *pTemp = paramOUT + 16;
        // fill struct members
        pData->FirmwareRevision = ParseFirmware(paramOUT);
        pData->Model = ParseModel(paramOUT);
        pData->SerialNumber = ParseSerial(paramOUT);
    }

    CloseHandle(hDevice);

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
    return Success;
}

// getting smart details for required index
SmartDetails *CSmartInfo::GetSMARTDetails(short sAttribIndex)
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

int CSmartInfo::OnCreateWidget(QWidget **ppWidget)
{
    QWidget *pWidget = new QWidget();
    pWidget->setLayout(new QVBoxLayout());
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);
    pWidget->layout()->setSpacing(2);

    m_pAvailableHDDTableView = new QTableView();
    m_pAvailableHDDTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pAvailableHDDTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pAvailableHDDTableView->verticalHeader()->hide();
    m_pAvailableHDDTableView->setShowGrid(false);
    m_pAvailableHDDTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pAvailableHDDTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pAvailableHDDTableView->setFixedHeight(75);

    QObject::connect(m_pAvailableHDDTableView, SIGNAL(clicked(QModelIndex)),
                     this, SLOT(OnAvailableHDDTableItemClicked(QModelIndex)),
                     Qt::QueuedConnection);

    m_pAvailableHDDModel->setColumnCount(1);
    m_pAvailableHDDModel->setRowCount(this->m_qInfomations.count());

    for(int i = 0; i < this->m_qInfomations.count(); i++)
    {
        QStandardItem *pItem = new QStandardItem(m_qInfomations.at(i)->DeviceInformation->Model);
        m_pAvailableHDDModel->setItem(i, 0, pItem);
    }
    m_pAvailableHDDTableView->setModel(m_pAvailableHDDModel);

    pWidget->layout()->addWidget(m_pAvailableHDDTableView);

    m_pHDDDetailsTableView = new QTableView();
    m_pHDDDetailsTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pHDDDetailsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pHDDDetailsTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_pHDDDetailsTableView->verticalHeader()->hide();
    m_pHDDDetailsTableView->setShowGrid(false);
    m_pHDDDetailsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pHDDDetailsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pHDDDetailsTableView->setColumnWidth(0, 10);
    m_pHDDDetailsTableView->setRowHeight(0, 5);

    pWidget->layout()->addWidget(m_pHDDDetailsTableView);

    (*ppWidget) = pWidget;

    return Success;
}

void CSmartInfo::OnStartLoadingModuleDataSlot()
{
    for(int i = 0; i < m_PhysicalDrives.count(); i++)
    {
        this->Initialize(i);
        this->m_qInfomations.append(m_data);
    }

    emit OnLoadingModuleDataCompleteSignal();
    emit OnCreateWidgetSignal(STORAGE_SMART, this);
}

void CSmartInfo::OnAvailableHDDTableItemClicked(QModelIndex index)
{
    QStandardItem *pItem = m_pAvailableHDDModel->itemFromIndex(index);
    QString qzItemText = pItem->text();
    DriveInfo *pDriveInfo = 0;

    for(int i = 0; i < m_qInfomations.count(); i++)
    {
        if( qzItemText == m_qInfomations.at(i)->DeviceInformation->Model )
        {
            pDriveInfo = m_qInfomations.at(i);
            break;
        }
    }

    this->PopulateModel(pDriveInfo);
}

void CSmartInfo::PopulateModel(DriveInfo *pDriveInfo)
{
    SmartDetails *pSmartDetails = 0;
    QStandardItem *pItem = 0;
    SmartData *pSmartData = 0;
    m_pHDDDetailsModel->clear();

    m_pHDDDetailsModel->setColumnCount(6);
    m_pHDDDetailsModel->setRowCount(pDriveInfo->SmartEntries.count());

    m_pHDDDetailsModel->setHorizontalHeaderLabels(QStringList() << "ID"
                                                  << "Property name" << "Raw" << "Value" <<
                                                  "Worst" << "Threshold");

    for(int i = 0; i < pDriveInfo->SmartEntries.count(); i++)
    {
        pSmartData = pDriveInfo->SmartEntries.at(i);
        pSmartDetails = this->GetSMARTDetails(pSmartData->m_ucAttribIndex);

        pItem = new QStandardItem(QString().setNum(pSmartDetails->m_ucAttribId));
        m_pHDDDetailsModel->setItem(i, 0, pItem);

        pItem = new QStandardItem(pSmartDetails->m_csAttribName);
        m_pHDDDetailsModel->setItem(i, 1, pItem);

        // raw
        pItem = new QStandardItem(QString().setNum(pSmartData->m_dwAttribValue));
        m_pHDDDetailsModel->setItem(i, 2, pItem);

        // value
        pItem = new QStandardItem(QString().setNum(pSmartData->m_ucValue));
        m_pHDDDetailsModel->setItem(i, 3, pItem);

        // worst
        pItem = new QStandardItem(QString().setNum(pSmartData->m_ucWorst));
        m_pHDDDetailsModel->setItem(i, 4, pItem);

        // threshold
        pItem = new QStandardItem(QString().setNum(pSmartData->m_dwThreshold));
        m_pHDDDetailsModel->setItem(i, 5, pItem);
    }

    emit OnSendModuleDataToGUISignal(STORAGE_SMART, m_pHDDDetailsTableView, m_pHDDDetailsModel);
}
