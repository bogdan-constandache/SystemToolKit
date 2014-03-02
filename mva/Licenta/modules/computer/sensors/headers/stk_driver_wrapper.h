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
};

#endif

