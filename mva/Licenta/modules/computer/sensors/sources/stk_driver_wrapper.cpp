#include "../headers/stk_driver_wrapper.h"


CSTKDriverWrapper::CSTKDriverWrapper(void):
	m_hDriver(NULL)
{
}


CSTKDriverWrapper::~CSTKDriverWrapper(void)
{
}


int CSTKDriverWrapper::Initialize()
{
	m_hDriver = CreateFile(L"\\\\.\\STKDriver",
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);

	if (INVALID_HANDLE_VALUE == m_hDriver)
	{
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
	}

    return Success;
}

int CSTKDriverWrapper::Destroy()
{
	if(m_hDriver)
		CloseHandle(m_hDriver);
    return Success;
}

int CSTKDriverWrapper::ReadMsr(ULONG ulIndex, ULONG *ulEAX, ULONG *ulEDX)
{
    ULONGLONG ulBuffer = 0;
    DWORD dwBytesReturned = 0;
    bool bResult = false;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_READ_MSR,
                              (LPVOID)&ulIndex,
                              sizeof(ULONG),
                              (LPVOID)&ulBuffer,
                              sizeof(ULONGLONG),
                              &dwBytesReturned,
                              NULL);

    if (false == bResult)
    {
        *ulEAX = 0;
        *ulEDX = 0;
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }
    else
    {
        *ulEDX = (ULONGLONG)((ulBuffer >> 32) & 0xFFFFFFFF);
        *ulEAX = (ULONGLONG)(ulBuffer & 0xFFFFFFFF);
    }

    return Success;
}

int CSTKDriverWrapper::WriteMsr(ULONG ulIndex, ULONG ulEAX, ULONG ulEDX)
{
    STK_MSR_INPUT *pInput = new STK_MSR_INPUT;
    bool bResult = false;

    pInput->ulRegister = ulIndex;
    pInput->uliValue.HighPart = ulEDX;
    pInput->uliValue.LowPart = ulEAX;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_WRITE_MSR,
                              (LPVOID)pInput,
                              sizeof(STK_MSR_INPUT),
                              NULL,
                              0,
                              NULL,
                              NULL);

    if (false == bResult)
    {
        DEBUG_STATUS(Unsuccessful);
        return Success;
    }

    return Success;
}

int CSTKDriverWrapper::ReadIoPortByte(ULONG ulPort, BYTE *bValue)
{
    ULONG ulBuffer = 0;
    bool bResult = false;
    DWORD dwBytesReturned = 0;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_READ_IO_PORT_BYTE,
                              (LPVOID)&ulPort,
                              sizeof(ULONG),
                              (LPVOID)&ulBuffer,
                              sizeof(ULONG),
                              &dwBytesReturned,
                              NULL);

    if (false == bResult)
    {
        *bValue = 0;
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    *bValue = (BYTE) ulBuffer & 0xFF;

    return Success;
}

int CSTKDriverWrapper::WriteIoPortByte(ULONG ulPort, BYTE bValue)
{
    STK_IO_PORT_INPUT *pInput = new STK_IO_PORT_INPUT;
    bool bResult = false;

    pInput->ulPort = ulPort;
    pInput->CharData = bValue;

    DWORD dwRetParam = 0;
    DWORD dwBytesReceived = 0;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_WRITE_IO_PORT_BYTE,
                              (LPVOID)pInput,
                              sizeof(STK_IO_PORT_INPUT),
                              &dwRetParam,
                              sizeof(dwRetParam),
                              &dwBytesReceived,
                              NULL);

    if (false == bResult)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    return Success;
}

int CSTKDriverWrapper::ReadIoPortDWORD(ULONG ulPort, DWORD *dwValue)
{
    ULONG ulBuffer = 0;
    bool bResult = false;
    DWORD dwBytesReturned = 0;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_READ_IO_PORT_DWORD,
                              (LPVOID)&ulPort,
                              sizeof(ULONG),
                              (LPVOID)&ulBuffer,
                              sizeof(ULONG),
                              &dwBytesReturned,
                              NULL);

    if (false == bResult)
    {
        *dwValue = 0;
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    *dwValue = (ULONG) ulBuffer & 0xFFFFFFFF;

    return Success;
}

int CSTKDriverWrapper::WriteIoPortDWORD(ULONG ulPort, ULONG dwValue)
{
    STK_IO_PORT_DWORD *pInput = new STK_IO_PORT_DWORD;
    bool bResult = false;

    pInput->ulPort = ulPort;
    pInput->dwData = dwValue;

    DWORD dwRetParam = 0;
    DWORD dwBytesReceived = 0;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_WRITE_IO_PORT_DWORD,
                              (LPVOID)pInput,
                              sizeof(STK_IO_PORT_DWORD),
                              &dwRetParam,
                              sizeof(dwRetParam),
                              &dwBytesReceived,
                              NULL);

    if (false == bResult)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    return Success;
}

int CSTKDriverWrapper::ReadPCIConfiguration(ULONG ulPCIAddress, ULONG ulRegAddress, ULONG *ulValue)
{
    if ((ulRegAddress & 3) != 0)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    STK_READ_PCI_CONFIG_INPUT *pInput = new STK_READ_PCI_CONFIG_INPUT;
    bool bResult = false;
    DWORD dwBytesReceived = 0;

    pInput->ulPCIAddress = ulPCIAddress;
    pInput->ulPCIOffset = ulRegAddress;

    ULONG ulVal = 0;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_READ_PCI_CONFIG,
                              (LPVOID)pInput,
                              sizeof(STK_READ_PCI_CONFIG_INPUT),
                              &ulVal,
                              sizeof(ulVal),
                              &dwBytesReceived,
                              NULL);

    if (false == bResult)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    *ulValue = ulVal;

    return Success;
}

int CSTKDriverWrapper::WritePCIConfiguration(ULONG ulPCIAddress, ULONG ulRegAddress, ULONG ulData)
{
    if ((ulRegAddress & 3) != 0)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    STK_WRITE_PCI_CONFIG_INPUT *pInput = new STK_WRITE_PCI_CONFIG_INPUT;
    bool bResult = false;
    DWORD dwBytesReceived = 0;
    DWORD dwReturnVal = 0;

    pInput->ulPCIAddress = ulPCIAddress;
    pInput->ulPCIOffset = ulRegAddress;
    pInput->ulData = ulData;

    bResult = DeviceIoControl(m_hDriver,
                              IOCTL_STK_WRITE_PCI_CONFIG,
                              (LPVOID)pInput,
                              sizeof(STK_WRITE_PCI_CONFIG_INPUT),
                              &dwReturnVal,
                              sizeof(dwReturnVal),
                              &dwBytesReceived,
                              NULL);

    if (false == bResult)
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    return Success;
}
