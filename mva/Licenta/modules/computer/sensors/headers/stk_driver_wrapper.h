#ifndef _STK_DRIVER_WRAPPER_
#define _STK_DRIVER_WRAPPER_

#include <Windows.h>

#include "../driver/driver_defines.h"
#include "../../../../utils/headers/utils.h"

class CSTKDriverWrapper
{
	HANDLE m_hDriver;
public:
	CSTKDriverWrapper(void);
    ~CSTKDriverWrapper(void);

	int Initialize();
	int Destroy();

    int ReadMsr(ULONG ulIndex, ULONG *ulEAX, ULONG *ulEDX);
    int WriteMsr(ULONG ulIndex, ULONG ulEAX, ULONG ulEDX);

    int ReadIoPortByte(ULONG ulPort, BYTE *bValue);
    int WriteIoPortByte(ULONG ulPort, BYTE bValue);

    int ReadIoPortDWORD(ULONG ulPort, DWORD *dwValue);
    int WriteIoPortDWORD(ULONG ulPort, DWORD dwValue);

    int ReadPCIConfiguration(ULONG ulPCIAddress, ULONG ulRegAddress, ULONG *ulValue);
    int WritePCIConfiguration(ULONG ulPCIAddress, ULONG ulRegAddress, ULONG ulData);
};

#endif

