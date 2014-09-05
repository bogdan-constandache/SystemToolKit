#include "spd.h"

CSPDInformation::CSPDInformation():
    m_pDimmsModel(NULL), m_pDimmInformationModel(NULL), m_pDriver(NULL),
    m_dwBaseAddr(0), m_dwDataAddr(0), m_dwPortVal(0),
    m_bIsDIMM0Present(FALSE), m_bIsDIMM1Present(FALSE), m_bIsDIMM2Present(FALSE), m_bIsDIMM3Present(FALSE),
    m_pDIMM0SPD(NULL), m_pDIMM1SPD(NULL), m_pDIMM2SPD(NULL), m_pDIMM3SPD(NULL)
{
    m_pDimmsModel = new QStandardItemModel;
    m_pDimmInformationModel = new QStandardItemModel;

    InitializeASCIIDecodingMatrix();

    m_pDIMM0SPD = (BYTE*)malloc(sizeof(BYTE) * 256);
    CHECK_ALLOCATION(m_pDIMM0SPD);
    m_pDIMM1SPD = (BYTE*)malloc(sizeof(BYTE) * 256);
    CHECK_ALLOCATION(m_pDIMM1SPD);
    m_pDIMM2SPD = (BYTE*)malloc(sizeof(BYTE) * 256);
    CHECK_ALLOCATION(m_pDIMM2SPD);
    m_pDIMM3SPD = (BYTE*)malloc(sizeof(BYTE) * 256);
    CHECK_ALLOCATION(m_pDIMM3SPD);

    ZeroMemory(m_pDIMM0SPD, 256);
    ZeroMemory(m_pDIMM1SPD, 256);
    ZeroMemory(m_pDIMM2SPD, 256);
    ZeroMemory(m_pDIMM3SPD, 256);

    m_pDriver = new CSTKDriverWrapper;
    CHECK_ALLOCATION(m_pDriver);
    int nStatus = m_pDriver->Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    m_dwDataAddr = 0x8000fb20; // INTEL only;

    nStatus = m_pDriver->WriteIoPortDWORD(IO_CONFIG_SPACE_CONTROL_ADDRESS, m_dwDataAddr);
    CHECK_OPERATION_STATUS(nStatus);
    nStatus = m_pDriver->ReadIoPortDWORD(IO_CONFIG_SPACE_DATA_ADDRESS, &m_dwPortVal);

    m_dwBaseAddr = m_dwPortVal & 0xFFFFFFF0;

    nStatus = ReadManufacturersDataFromDB();
    CHECK_OPERATION_STATUS(nStatus);


    if( IsWaitReady(m_dwBaseAddr) )
    {
        CollectSPDInformations();

        CollectDIMMSType();

        QMap<int, int>::const_iterator it;
        for(it = m_qDimmsType.begin(); it != m_qDimmsType.end(); it++)
        {
            if( it.value() == DDR3 )
                ReadDDR3Informations(it.key());
        }
    }

    m_pDimmsModel->setHorizontalHeaderLabels(QStringList() << "Description:");

    // Fill main model
    QMap<int, SpdInformation*>::const_iterator it2;
    for(it2 = m_qDimmsInformation.begin(); it2 != m_qDimmsInformation.end(); it2++)
    {
        QStandardItem *pItem = new QStandardItem(it2.value()->qsName);
        pItem->setData(it2.key());

        m_pDimmsModel->appendRow(pItem);
    }
}

CSPDInformation::~CSPDInformation()
{
    m_qDimmsType.clear();

    for(int i = 0; i < m_qManufacturerDetails.count(); i++)
    {
        SAFE_DELETE(m_qManufacturerDetails[i]);
    }
    m_qManufacturerDetails.clear();

    QMap<int, SpdInformation*>::iterator it;
    for(it = m_qDimmsInformation.begin(); it != m_qDimmsInformation.end(); it++)
    {
        SAFE_DELETE(it.value());
    }
    m_qDimmsInformation.clear();

    int nStatus = m_pDriver->Destroy();
    CHECK_OPERATION_STATUS(nStatus);
    SAFE_DELETE(m_pDriver);

    SAFE_DELETE(m_pDIMM0SPD);
    SAFE_DELETE(m_pDIMM1SPD);
    SAFE_DELETE(m_pDIMM2SPD);
    SAFE_DELETE(m_pDIMM3SPD);

    SAFE_DELETE(m_pDimmInformationModel);
    SAFE_DELETE(m_pDimmsModel);
}

QStandardItemModel *CSPDInformation::GetDimmsModel()
{
    return m_pDimmsModel;
}

QStandardItemModel *CSPDInformation::GetDimmsInformationModel()
{
    return m_pDimmInformationModel;
}

void CSPDInformation::OnRefreshData(int nDimm)
{
    SpdInformation *pData = m_qDimmsInformation[nDimm];

    m_pDimmInformationModel->clear();

    m_pDimmInformationModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    QList<QStandardItem*> qList;

    qList << new QStandardItem("SPD size: ") << new QStandardItem(pData->qsSPDSize);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("SPD version: ") << new QStandardItem(pData->qsSPDVersion);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Manufacturer: ") << new QStandardItem(pData->qsManufacturer);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Manufacturing date: ") << new QStandardItem(pData->qsManufactureDate);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Serial number: ") << new QStandardItem(pData->qsSerial);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Part number: ") << new QStandardItem(pData->qsPartNumber);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Size: ") << new QStandardItem(pData->qsSize);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("CAS latencies: ") << new QStandardItem(pData->qsCASLatencies);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Device type: ") << new QStandardItem(pData->qsDeviceType);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Module type: ") << new QStandardItem(pData->qsModuleType);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Module bus width: ") << new QStandardItem(pData->qsModuleBusWidth);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("Module voltage: ") << new QStandardItem(pData->qsModuleVoltage);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("SDRAM device type: ") << new QStandardItem(pData->qsSDRAMDeviceType);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
    qList << new QStandardItem("ECC method: ") << new QStandardItem(pData->qsECCMethod);
    m_pDimmInformationModel->appendRow(qList); qList.clear();
}

BYTE CSPDInformation::ReadSPDByte(DWORD dwBase, BYTE bOffset, BYTE bDevID)
{
    BYTE bRetVal = 0;

    int nStatus = m_pDriver->WriteIoPortByte(dwBase, 0x0FE);
    CHECK_OPERATION_STATUS(nStatus);
    nStatus = m_pDriver->WriteIoPortByte(dwBase + 0x04, bDevID + 1);
    CHECK_OPERATION_STATUS(nStatus);
    nStatus = m_pDriver->WriteIoPortByte(dwBase + 0x03, bOffset);
    CHECK_OPERATION_STATUS(nStatus);
    nStatus = m_pDriver->WriteIoPortByte(dwBase + 0x02, 0x48);

    //wait 5ms
    Delay(5*1000);

    //wait  smbus  ready
    if( IsWaitReady(dwBase) )
    {
        nStatus = m_pDriver->ReadIoPortByte(dwBase + 0x05, &bRetVal);
        CHECK_OPERATION_STATUS(nStatus);
    }

    return bRetVal;
}

BOOL CSPDInformation::IsWaitReady(DWORD dwBase)
{
    int nStatus = Uninitialized;
    BYTE bVal = 0;
    do
    {
        nStatus = m_pDriver->ReadIoPortByte(dwBase, &bVal);
        CHECK_OPERATION_STATUS(nStatus);
    }while( 0 != (bVal & 0x01) );

    return  TRUE;
}

void CSPDInformation::Delay(__int64 uiSeconds)
{
    LARGE_INTEGER  CurrTicks,TicksCount;
    QueryPerformanceFrequency(&TicksCount);
    QueryPerformanceCounter(&CurrTicks);

    TicksCount.QuadPart = TicksCount.QuadPart * uiSeconds/1000000i64;
    TicksCount.QuadPart += CurrTicks.QuadPart;

    while( CurrTicks.QuadPart < TicksCount.QuadPart )
        QueryPerformanceCounter(&CurrTicks);
}

int CSPDInformation::ReadDDR3Informations(int nDIMM)
{
    switch( nDIMM )
    {
    case DIMM0:
        if( !m_bIsDIMM0Present ) return Unsuccessful;
        InterpretDDR3SPDArray(m_pDIMM0SPD, nDIMM); break;
    case DIMM1:
        if( !m_bIsDIMM1Present ) return Unsuccessful;
        InterpretDDR3SPDArray(m_pDIMM1SPD, nDIMM); break;
    case DIMM2:
        if( !m_bIsDIMM2Present ) return Unsuccessful;
        InterpretDDR3SPDArray(m_pDIMM2SPD, nDIMM); break;
    case DIMM3:
        if( !m_bIsDIMM3Present ) return Unsuccessful;
        InterpretDDR3SPDArray(m_pDIMM3SPD, nDIMM); break;
    default:
        return Unsuccessful;
    }

    return Success;
}

int CSPDInformation::InterpretDDR3SPDArray(BYTE *pbArray, int nDimm)
{
    SpdInformation *pData = new SpdInformation;
    CHECK_ALLOCATION_STATUS(pData);
    BYTE bCurrentVal = 0; BYTE bAdditional = 0;

    // SPD total size - BYTE 0
    bCurrentVal = pbArray[0];
    bCurrentVal &= 0x70; // CHECK BITS 4-6
    switch( bCurrentVal )
    {
    case 0x0:
        pData->qsSPDSize = "Undefined"; break;
    case 0x10:
        pData->qsSPDSize = "256 Bytes"; break;
    default:
        pData->qsSPDSize = "Reserved";
    }

    // SPD revision - BYTE 1
    bCurrentVal = pbArray[1];
    bAdditional = bCurrentVal >> 4;
    pData->qsSPDVersion.sprintf("%d.%d", bAdditional, bCurrentVal & 0xF);

    // DRAM device type - BYTE 2
    bCurrentVal = pbArray[2];
    pData->qsDeviceType = DIMM_TYPE(bCurrentVal);

    // Module type - BYTE 3
    bCurrentVal = pbArray[3];
    pData->qsModuleType = MODULE_DDR3_TYPE((bCurrentVal & 0xF));

    // Voltage - BYTE 6
    bCurrentVal = pbArray[6];
    pData->qsModuleVoltage = MODULE_DDR3_VOLTAGE((bCurrentVal & 0x07));

    // Size - BYTE 4, 7, 8
    DWORD dwSDRAMCap = 0, dwBusWidth = 0, dwSDRAMWidth = 0, dwRanks = 0;
    bCurrentVal = pbArray[4] & 0xF; // BYTE 4 3~0
    dwSDRAMCap = SDRAM_DDR3_CAPACITY(bCurrentVal);
    bCurrentVal = pbArray[8] & 0x7; // BYTE 8 2~0
    dwBusWidth = PRIMARY_BUS_WIDTH_DDR3(bCurrentVal);
    bCurrentVal = pbArray[7] & 0x7; // BYTE 7 2~0
    dwSDRAMWidth = SDRAM_DDR3_DEVICE_WIDTH(bCurrentVal);
    bCurrentVal = (pbArray[7] & 0x38) >> 3; // BYTE 7 5~3
    dwRanks = RANKS_DDR3(bCurrentVal);

    DWORD dwSize = dwSDRAMCap / 8 * dwBusWidth / dwSDRAMWidth * dwRanks;
    pData->qsSize.sprintf("%d MB", dwSize);
    pData->qsModuleBusWidth.sprintf("%d - bit", dwBusWidth);

    // ECC - BYTE 8 4~3
    if( pbArray[8] & 0x8 )
        pData->qsECCMethod = "8 bit ECC";
    else
        pData->qsECCMethod = "None";

    // CAS Latency - BYTE 14, 15
    bCurrentVal = pbArray[14];
    pData->qsCASLatencies = "CL ";
    if( bCurrentVal & 0x1 )
        pData->qsCASLatencies.append("4, ");
    if( bCurrentVal & 0x2 )
        pData->qsCASLatencies.append("5, ");
    if( bCurrentVal & 0x4 )
        pData->qsCASLatencies.append("6, ");
    if( bCurrentVal & 0x8 )
        pData->qsCASLatencies.append("7, ");
    if( bCurrentVal & 0x10 )
        pData->qsCASLatencies.append("8, ");
    if( bCurrentVal & 0x20 )
        pData->qsCASLatencies.append("9, ");
    if( bCurrentVal & 0x40 )
        pData->qsCASLatencies.append("10, ");
    if( bCurrentVal & 0x80 )
        pData->qsCASLatencies.append("11, ");

    bCurrentVal = pbArray[15];
    if( bCurrentVal & 0x1 )
        pData->qsCASLatencies.append("12, ");
    if( bCurrentVal & 0x2 )
        pData->qsCASLatencies.append("13, ");
    if( bCurrentVal & 0x4 )
        pData->qsCASLatencies.append("14, ");
    if( bCurrentVal & 0x8 )
        pData->qsCASLatencies.append("15, ");
    if( bCurrentVal & 0x10 )
        pData->qsCASLatencies.append("16, ");
    if( bCurrentVal & 0x20 )
        pData->qsCASLatencies.append("17, ");
    if( bCurrentVal & 0x40 )
        pData->qsCASLatencies.append("18, ");
    pData->qsCASLatencies.chop(2);

    // SDRAM Device type - BYTE 33
    bCurrentVal = pbArray[33];
    if( bCurrentVal & 0x80 )
        pData->qsSDRAMDeviceType = "Non-standard device";
    else
        pData->qsSDRAMDeviceType = "Standard Monolithic DRAM Device";

    switch( (bCurrentVal & 0x70) >> 4 )
    {
    case 1:
        pData->qsSDRAMDeviceType += " - Single die"; break;
    case 2:
        pData->qsSDRAMDeviceType += " - 2 die"; break;
    case 3:
        pData->qsSDRAMDeviceType += " - 4 die"; break;
    case 4:
        pData->qsSDRAMDeviceType += " - 8 die"; break;
    }

    // Module manufacturer BYTE 117, 118
    BYTE bContinuation = pbArray[117] & 0x7F;
    BYTE bID = pbArray[118];

    ManufacturerDetails *pDetails = GetManufacturerDetails(bContinuation + 1, bID);
    if( pDetails )
    {
        pData->qsManufacturer = pDetails->qsManufacturer;
    }

    // Module manufacturing date
    pData->qsManufactureDate = "Week $1, Year $2";
    QString qsYear = "20$1$2", qsWeek = "$1$2";

    bCurrentVal = (pbArray[120] & 0xF0) >> 4;
    qsYear.replace("$1", QString().sprintf("%d", bCurrentVal));
    bCurrentVal = (pbArray[120] & 0xF);
    qsYear.replace("$2", QString().sprintf("%d", bCurrentVal));

    bCurrentVal = (pbArray[121] & 0xF0) >> 4;
    qsWeek.replace("$1", QString().sprintf("%d", bCurrentVal));
    bCurrentVal = (pbArray[121] & 0xF);
    qsWeek.replace("$2", QString().sprintf("%d", bCurrentVal));

    pData->qsManufactureDate.replace("$1", qsWeek);
    pData->qsManufactureDate.replace("$2", qsYear);

    // Serial number - BYTE 122, 123, 124, 125
    DWORD dwSerial = 0;
    dwSerial |= pbArray[122] << 24;
    dwSerial |= pbArray[123] << 16;
    dwSerial |= pbArray[124] << 8;
    dwSerial |= pbArray[125];
    pData->qsSerial.sprintf("%Xh", dwSerial);

    // Part number - BYTE 128-145
    for(int i = 128; i <= 145; i++)
    {
        CHAR chLetter = DecodeASCIICharacter(((pbArray[i] & 0xF0) >> 4), (pbArray[i] & 0xF));
        QString qsLetter = QString(chLetter);
        pData->qsPartNumber += qsLetter;
    }

    // Name - Manufacturer - Partnumber;
    pData->qsName = QString(DIMM_TO_STRING(nDimm)) + QString(": ") + pData->qsManufacturer + " " + pData->qsPartNumber;

    m_qDimmsInformation.insert(nDimm, pData);

    return Success;
}

int CSPDInformation::CollectSPDInformations()
{
    // CHECK FOR DIMM0
    if( IsWaitReady(m_dwBaseAddr) )
    {
        if( CheckIfDIMMIsAvailable(DIMM0) )
        {
            m_bIsDIMM0Present = TRUE;
            for(int i = 0; i < 256; i++)
            {
                m_pDIMM0SPD[i] = ReadSPDByte(m_dwBaseAddr, i, DIMM0);
            }
        }
    }

    // CHECK FOR DIMM1
    if( IsWaitReady(m_dwBaseAddr) )
    {
        if( CheckIfDIMMIsAvailable(DIMM1) )
        {
            m_bIsDIMM1Present = TRUE;
            for(int i = 0; i < 256; i++)
            {
                m_pDIMM1SPD[i] = ReadSPDByte(m_dwBaseAddr, i, DIMM1);
            }
        }
    }

    // CHECK FOR DIMM2
    if( IsWaitReady(m_dwBaseAddr) )
    {
        if( CheckIfDIMMIsAvailable(DIMM2) )
        {
            m_bIsDIMM2Present = TRUE;
            for(int i = 0; i < 256; i++)
            {
                m_pDIMM2SPD[i] = ReadSPDByte(m_dwBaseAddr, i, DIMM2);
            }
        }
    }

    // CHECK FOR DIMM3
    if( IsWaitReady(m_dwBaseAddr) )
    {
        if( CheckIfDIMMIsAvailable(DIMM3) )
        {
            m_bIsDIMM3Present = TRUE;
            for(int i = 0; i < 256; i++)
            {
                m_pDIMM3SPD[i] = ReadSPDByte(m_dwBaseAddr, i, DIMM3);
            }
        }
    }

    return Success;
}

BOOL CSPDInformation::CheckIfDIMMIsAvailable(int nDIMM)
{
    BYTE bVal = 0;

    bVal = ReadSPDByte(m_dwBaseAddr, 0, nDIMM);
    if( 128 > bVal )
        return FALSE;

    bVal = ReadSPDByte(m_dwBaseAddr, 1, nDIMM);
    if( 128 > (1 << bVal) )
        return FALSE;

    return TRUE;
}

int CSPDInformation::CollectDIMMSType()
{
    if( m_bIsDIMM0Present )
        m_qDimmsType.insert(DIMM0, m_pDIMM0SPD[2]);
    if( m_bIsDIMM1Present )
        m_qDimmsType.insert(DIMM1, m_pDIMM1SPD[2]);
    if( m_bIsDIMM2Present )
        m_qDimmsType.insert(DIMM2, m_pDIMM2SPD[2]);
    if( m_bIsDIMM3Present )
        m_qDimmsType.insert(DIMM3, m_pDIMM3SPD[2]);

    return Success;
}

int CSPDInformation::InitializeASCIIDecodingMatrix()
{
    m_ppASCIIDecodingMatrix[0][0] = ' ';
    m_ppASCIIDecodingMatrix[0][1] = 0;
    m_ppASCIIDecodingMatrix[0][2] = 0;
    m_ppASCIIDecodingMatrix[0][3] = 0;
    m_ppASCIIDecodingMatrix[0][4] = 0;
    m_ppASCIIDecodingMatrix[0][5] = 0;
    m_ppASCIIDecodingMatrix[0][6] = 0;
    m_ppASCIIDecodingMatrix[0][7] = 0;
    m_ppASCIIDecodingMatrix[0][8] = '(';
    m_ppASCIIDecodingMatrix[0][9] = ')';
    m_ppASCIIDecodingMatrix[0][10] = 0;
    m_ppASCIIDecodingMatrix[0][11] = 0;
    m_ppASCIIDecodingMatrix[0][12] = 0;
    m_ppASCIIDecodingMatrix[0][13] = '-';
    m_ppASCIIDecodingMatrix[0][14] = '.';
    m_ppASCIIDecodingMatrix[0][15] = 0;

    m_ppASCIIDecodingMatrix[1][0] = '0';
    m_ppASCIIDecodingMatrix[1][1] = '1';
    m_ppASCIIDecodingMatrix[1][2] = '2';
    m_ppASCIIDecodingMatrix[1][3] = '3';
    m_ppASCIIDecodingMatrix[1][4] = '4';
    m_ppASCIIDecodingMatrix[1][5] = '5';
    m_ppASCIIDecodingMatrix[1][6] = '6';
    m_ppASCIIDecodingMatrix[1][7] = '7';
    m_ppASCIIDecodingMatrix[1][8] = '8';
    m_ppASCIIDecodingMatrix[1][9] = '9';
    m_ppASCIIDecodingMatrix[1][10] = 0;
    m_ppASCIIDecodingMatrix[1][11] = 0;
    m_ppASCIIDecodingMatrix[1][12] = 0;
    m_ppASCIIDecodingMatrix[1][13] = 0;
    m_ppASCIIDecodingMatrix[1][14] = 0;
    m_ppASCIIDecodingMatrix[1][15] = 0;

    m_ppASCIIDecodingMatrix[2][0] = 0;
    m_ppASCIIDecodingMatrix[2][1] = 'A';
    m_ppASCIIDecodingMatrix[2][2] = 'B';
    m_ppASCIIDecodingMatrix[2][3] = 'C';
    m_ppASCIIDecodingMatrix[2][4] = 'D';
    m_ppASCIIDecodingMatrix[2][5] = 'E';
    m_ppASCIIDecodingMatrix[2][6] = 'F';
    m_ppASCIIDecodingMatrix[2][7] = 'G';
    m_ppASCIIDecodingMatrix[2][8] = 'H';
    m_ppASCIIDecodingMatrix[2][9] = 'I';
    m_ppASCIIDecodingMatrix[2][10] = 'J';
    m_ppASCIIDecodingMatrix[2][11] = 'K';
    m_ppASCIIDecodingMatrix[2][12] = 'L';
    m_ppASCIIDecodingMatrix[2][13] = 'M';
    m_ppASCIIDecodingMatrix[2][14] = 'N';
    m_ppASCIIDecodingMatrix[2][15] = 'O';

    m_ppASCIIDecodingMatrix[3][0] = 'P';
    m_ppASCIIDecodingMatrix[3][1] = 'Q';
    m_ppASCIIDecodingMatrix[3][2] = 'R';
    m_ppASCIIDecodingMatrix[3][3] = 'S';
    m_ppASCIIDecodingMatrix[3][4] = 'T';
    m_ppASCIIDecodingMatrix[3][5] = 'U';
    m_ppASCIIDecodingMatrix[3][6] = 'V';
    m_ppASCIIDecodingMatrix[3][7] = 'W';
    m_ppASCIIDecodingMatrix[3][8] = 'X';
    m_ppASCIIDecodingMatrix[3][9] = 'Y';
    m_ppASCIIDecodingMatrix[3][10] = 'Z';
    m_ppASCIIDecodingMatrix[3][11] = 0;
    m_ppASCIIDecodingMatrix[3][12] = 0;
    m_ppASCIIDecodingMatrix[3][13] = 0;
    m_ppASCIIDecodingMatrix[3][14] = 0;
    m_ppASCIIDecodingMatrix[3][15] = 0;

    m_ppASCIIDecodingMatrix[4][0] = 0;
    m_ppASCIIDecodingMatrix[4][1] = 'a';
    m_ppASCIIDecodingMatrix[4][2] = 'b';
    m_ppASCIIDecodingMatrix[4][3] = 'c';
    m_ppASCIIDecodingMatrix[4][4] = 'd';
    m_ppASCIIDecodingMatrix[4][5] = 'e';
    m_ppASCIIDecodingMatrix[4][6] = 'f';
    m_ppASCIIDecodingMatrix[4][7] = 'g';
    m_ppASCIIDecodingMatrix[4][8] = 'h';
    m_ppASCIIDecodingMatrix[4][9] = 'i';
    m_ppASCIIDecodingMatrix[4][10] = 'j';
    m_ppASCIIDecodingMatrix[4][11] = 'k';
    m_ppASCIIDecodingMatrix[4][12] = 'l';
    m_ppASCIIDecodingMatrix[4][13] = 'm';
    m_ppASCIIDecodingMatrix[4][14] = 'n';
    m_ppASCIIDecodingMatrix[4][15] = 'o';

    m_ppASCIIDecodingMatrix[5][0] = 'p';
    m_ppASCIIDecodingMatrix[5][1] = 'q';
    m_ppASCIIDecodingMatrix[5][2] = 'r';
    m_ppASCIIDecodingMatrix[5][3] = 's';
    m_ppASCIIDecodingMatrix[5][4] = 't';
    m_ppASCIIDecodingMatrix[5][5] = 'u';
    m_ppASCIIDecodingMatrix[5][6] = 'v';
    m_ppASCIIDecodingMatrix[5][7] = 'w';
    m_ppASCIIDecodingMatrix[5][8] = 'x';
    m_ppASCIIDecodingMatrix[5][9] = 'y';
    m_ppASCIIDecodingMatrix[5][10] = 'z';
    m_ppASCIIDecodingMatrix[5][11] = 0;
    m_ppASCIIDecodingMatrix[5][12] = 0;
    m_ppASCIIDecodingMatrix[5][13] = 0;
    m_ppASCIIDecodingMatrix[5][14] = 0;
    m_ppASCIIDecodingMatrix[5][15] = 0;

    return Success;
}

CHAR CSPDInformation::DecodeASCIICharacter(BYTE bLow, BYTE bHigh)
{
    if( bLow - 2 < 0 || bLow - 2 > 5 )
        return 0;
    if( bHigh < 0 || bHigh > 15 )
        return 0;

    return m_ppASCIIDecodingMatrix[bLow - 2][bHigh];
}

int CSPDInformation::ReadManufacturersDataFromDB()
{
    {
        ManufacturerDetails *pDetails = 0;
        QString qzAppPath = QCoreApplication::applicationDirPath();
        qzAppPath.append("/config/databases/config.db");
        QSqlDatabase qDB = QSqlDatabase::addDatabase("QSQLITE");
        qDB.setDatabaseName(qzAppPath);

        bool bRet = qDB.open();
        if( !bRet )
        {
            return DBOpenError;
        }

        QSqlQuery qQuery("SELECT * FROM SPD_MANUFACTURER", qDB);

        while( qQuery.next() )
        {
            pDetails = new ManufacturerDetails;
            if( 0 == pDetails )
            {
                return NotAllocated;
            }
            pDetails->nID = 0;
            pDetails->nContinuation = 0;
            pDetails->nManufacturerID = 0;

            pDetails->nID = qQuery.value(0).toInt();
            pDetails->nContinuation = qQuery.value(1).toInt();
            pDetails->qsManufacturer = qQuery.value(2).toString();
            pDetails->nManufacturerID = qQuery.value(3).toInt();

            m_qManufacturerDetails.append(pDetails);
        }

        qDB.close();
    }

    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    return Success;
}

ManufacturerDetails *CSPDInformation::GetManufacturerDetails(int nContinuation, int nManufacturerID)
{
    QList<ManufacturerDetails*>::const_iterator it;

    for(it = m_qManufacturerDetails.begin(); it != m_qManufacturerDetails.end(); it++)
    {
        if( (*it)->nContinuation == nContinuation && (*it)->nManufacturerID == nManufacturerID )
            return (*it);
    }

    return 0;
}
