#ifndef AMD_CARD_H
#define AMD_CARD_H

#include "coverdrive5.h"
#include "coverdrive6.h"

#include <QString>

class CAmdCard
{
private:
    IOverdrive                              *m_pOverdrive;
    HINSTANCE                               m_hDll;
    int                                     m_nNumberOfAdapters;
    int                                     m_nAdapterIndex;
    int                                     m_nOverdriveVersion;
    int                                     m_nInformationIndex;

    ADL_MAIN_CONTROL_CREATE					ADL_Main_Control_Create;
    ADL_MAIN_CONTROL_DESTROY				ADL_Main_Control_Destroy;
    ADL_ADAPTER_NUMBEROFADAPTERS_GET		ADL_Adapter_NumberOfAdapters_Get;
    ADL_ADAPTER_ADAPTERINFO_GET				ADL_Adapter_AdapterInfo_Get;
    ADL_ADAPTER_ACTIVE_GET					ADL_Adapter_Active_Get;
    ADL_OVERDRIVE_CAPS						ADL_Overdrive_Caps;

    LPAdapterInfo						    m_pAdapterInfo;

public:
    CAmdCard();
    ~CAmdCard();
    bool Initialize();

    QString GetAdapterName();
    double *GetTemperatures();
    double *GetFanSpeedsRPM();
    double *GetFanSpeedsPercentage();
    double GetEngineClock();
    double GetMemoryClock();
    double GetCoreVoltage();
};

#endif // AMD_CARD_H
