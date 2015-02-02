#include "amd_card.h"

#include "coverdrive5.h"
#include "coverdrive6.h"

void* __cdecl ADL_Main_Memory_Alloc ( int iSize )
{
    void* lpBuffer = malloc ( iSize );
    return lpBuffer;
}

void __cdecl ADL_Main_Memory_Free ( void** lpBuffer )
{
    if ( NULL != *lpBuffer )
    {
        free ( *lpBuffer );
        *lpBuffer = NULL;
    }
}

CAmdCard::CAmdCard():
    m_pOverdrive(NULL), m_nAdapterIndex(-1), m_hDll(NULL),
    m_nNumberOfAdapters(0), m_nInformationIndex(-1), m_nOverdriveVersion(-1),
    m_pAdapterInfo(NULL), m_pGpuModel(NULL), m_pGpuDataModel(NULL)
{
    if( !Initialize() )
        return;

    m_pGpuModel = new QStandardItemModel();
    CHECK_ALLOCATION(m_pGpuModel);
    m_pGpuDataModel = new QStandardItemModel();
    CHECK_ALLOCATION(m_pGpuDataModel);

    m_pGpuModel->setHorizontalHeaderLabels(QStringList() << "Devices: ");

    QStandardItem* pStandardItem = new QStandardItem(GetAdapterName());
    pStandardItem->setData((int)m_nInformationIndex);

    m_pGpuModel->appendRow(pStandardItem);


}

CAmdCard::~CAmdCard()
{
    if( NULL != m_pAdapterInfo )
        free(m_pAdapterInfo);

    if( NULL != m_pOverdrive )
        delete m_pOverdrive;

    if( NULL != m_hDll )
        FreeLibrary(m_hDll);
}

QStandardItemModel *CAmdCard::GetPhysicalGPUModel()
{
    return m_pGpuModel;
}

QStandardItemModel *CAmdCard::GetGPUDetailsModel()
{
    return m_pGpuDataModel;
}

void CAmdCard::OnRefreshData(int)
{
    double *dAux = 0;
    QList<QStandardItem*> qRow;
    int nCount = 0;
    double dAux2 = 0;

    m_pGpuDataModel->clear();

    QStandardItem *pIndex = 0;

    dAux = GetTemperatures();
    if( dAux )
    {
        pIndex = new QStandardItem("Temperatures: ");
        m_pGpuDataModel->appendRow(pIndex);
        nCount = 0;
        for(int i = 0; i < 10; i++)
        {
            if( dAux[i] == 0 )
                continue;

            qRow.clear();
            qRow << new QStandardItem(QString().sprintf("Temp #%d", ++nCount));
            qRow << new QStandardItem(QString().sprintf("%.2f",dAux[i]));

            pIndex->appendRow(qRow);
        }
    }

    dAux = GetFanSpeedsRPM();
    if( dAux )
    {
        pIndex = new QStandardItem("Fan speed: ");
        m_pGpuDataModel->appendRow(pIndex);
        nCount = 0;
        for(int i = 0; i < 10; i++)
        {
            if( dAux[i] == 0 )
                continue;

            qRow.clear();
            qRow << new QStandardItem(QString().sprintf("Fan #%d", ++nCount));
            qRow << new QStandardItem(QString().sprintf("%.2f",dAux[i]));

            pIndex->appendRow(qRow);
        }
    }

    dAux2 = GetEngineClock();
    qRow.clear();
    qRow << new QStandardItem("Engine Clock: ");
    qRow << new QStandardItem("%d MHz");
    m_pGpuDataModel->appendRow(qRow);

    dAux2 = GetEngineClock();
    qRow.clear();
    qRow << new QStandardItem("Memory Clock: ");
    qRow << new QStandardItem("%d MHz");
    m_pGpuDataModel->appendRow(qRow);

    dAux2 = GetEngineClock();
    qRow.clear();
    qRow << new QStandardItem("Core Voltage: ");
    qRow << new QStandardItem("%d V");
    m_pGpuDataModel->appendRow(qRow);
}

bool CAmdCard::Initialize()
{
    m_hDll = LoadLibrary(L"atiadlxx.dll");
    if( NULL == m_hDll )
        m_hDll = LoadLibrary(L"atiadlxy.dll");

    if( NULL == m_hDll )
        return false;

    ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE) GetProcAddress (m_hDll, "ADL_Main_Control_Create");
    ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY) GetProcAddress (m_hDll, "ADL_Main_Control_Destroy");
    ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET) GetProcAddress(m_hDll, "ADL_Adapter_NumberOfAdapters_Get");
    ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET) GetProcAddress(m_hDll, "ADL_Adapter_AdapterInfo_Get");
    ADL_Adapter_Active_Get = (ADL_ADAPTER_ACTIVE_GET)GetProcAddress(m_hDll, "ADL_Adapter_Active_Get");
    ADL_Overdrive_Caps = (ADL_OVERDRIVE_CAPS)GetProcAddress(m_hDll, "ADL_Overdrive_Caps");

    if( NULL == ADL_Main_Control_Create ||
        NULL == ADL_Main_Control_Destroy ||
        NULL == ADL_Adapter_NumberOfAdapters_Get ||
        NULL == ADL_Adapter_AdapterInfo_Get ||
        NULL == ADL_Adapter_Active_Get ||
        NULL == ADL_Overdrive_Caps )
    {
        return false;
    }

    if( ADL_OK != ADL_Main_Control_Create (ADL_Main_Memory_Alloc, 1) )
       return false;

    if( ADL_OK != ADL_Adapter_NumberOfAdapters_Get(&m_nNumberOfAdapters) )
        return false;

    if( 0 < m_nNumberOfAdapters )
    {
        m_pAdapterInfo = (LPAdapterInfo) malloc (sizeof(AdapterInfo) * m_nNumberOfAdapters);
        memset(m_pAdapterInfo, '\0', sizeof(AdapterInfo) * m_nNumberOfAdapters);

        ADL_Adapter_AdapterInfo_Get(m_pAdapterInfo, sizeof(AdapterInfo) * m_nNumberOfAdapters);
    }

    for(int i = 0; i < m_nNumberOfAdapters; i++)
    {
        int nIsAdapterActive = 0;
        AdapterInfo adapterInfo = m_pAdapterInfo[i];
        ADL_Adapter_Active_Get(adapterInfo.iAdapterIndex , &nIsAdapterActive);
        if( nIsAdapterActive && adapterInfo.iVendorID == AMDVENDORID )
        {
            m_nAdapterIndex = adapterInfo.iAdapterIndex;
            m_nInformationIndex = i;
            break;
        }
    }

    if( -1 == m_nAdapterIndex )
        return false;

    int  nOverdriveSupported = 0;
    int  nOverdriveEnabled = 0;

    int nAdlRes;
    if( ADL_OK != (nAdlRes = ADL_Overdrive_Caps(m_nAdapterIndex, &nOverdriveSupported, &nOverdriveEnabled, &m_nOverdriveVersion)) )   
    {
        m_pOverdrive = new COverdrive5(m_hDll);
        return true;
    }

    if( !nOverdriveSupported )
        return false;

    if( 5 == m_nOverdriveVersion )
        m_pOverdrive = new COverdrive5(m_hDll);
    else  if( 6 == m_nOverdriveVersion )
        m_pOverdrive = new COverdrive6(m_hDll);
    else
        return false;

    return true;
}

QString CAmdCard::GetAdapterName()
{
    AdapterInfo adapterInfo = m_pAdapterInfo[m_nInformationIndex];
    return QString(adapterInfo.strAdapterName);
}

double *CAmdCard::GetTemperatures()
{
    return m_pOverdrive->OnGetTemperatures(m_nAdapterIndex);
}

double *CAmdCard::GetFanSpeedsRPM()
{
    return m_pOverdrive->OnGetFanSpeedsRPM(m_nAdapterIndex);
}

double *CAmdCard::GetFanSpeedsPercentage()
{
    return m_pOverdrive->OnGetFanSpeedsPercentage(m_nAdapterIndex);
}

double CAmdCard::GetEngineClock()
{
    return m_pOverdrive->OnGetEngineClock(m_nAdapterIndex);
}

double CAmdCard::GetMemoryClock()
{
    return m_pOverdrive->OnGetMemoryClock(m_nAdapterIndex);
}

double CAmdCard::GetCoreVoltage()
{
    return m_pOverdrive->OnGetCoreVoltage(m_nAdapterIndex);
}


