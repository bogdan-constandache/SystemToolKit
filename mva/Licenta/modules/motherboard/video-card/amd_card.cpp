#include "amd_card.h"

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
    m_pAdapterInfo(NULL)
{

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
        return false;

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
