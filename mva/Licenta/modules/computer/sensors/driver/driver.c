#include <ntddk.h>
#include <stddef.h>
#include <devioctl.h>

//-----------------------------------------------------------------------------
//
// The Device type codes form 32768 to 65535 are for customer use.
//
//-----------------------------------------------------------------------------

#define STK_TYPE 45000


#define IOCTL_STK_READ_MSR \
        CTL_CODE(STK_TYPE, 0x821, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_STK_WRITE_MSR \
        CTL_CODE(STK_TYPE, 0x822, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_STK_READ_IO_PORT \
        CTL_CODE(STK_TYPE, 0x831, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_STK_WRITE_IO_PORT \
        CTL_CODE(STK_TYPE, 0x832, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_STK_READ_IO_PORT_BYTE \
        CTL_CODE(STK_TYPE, 0x833, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_STK_READ_IO_PORT_WORD \
        CTL_CODE(STK_TYPE, 0x834, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_STK_READ_IO_PORT_DWORD \
        CTL_CODE(STK_TYPE, 0x835, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_STK_WRITE_IO_PORT_BYTE \
        CTL_CODE(STK_TYPE, 0x836, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_STK_WRITE_IO_PORT_WORD \
        CTL_CODE(STK_TYPE, 0x837, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_STK_WRITE_IO_PORT_DWORD \
        CTL_CODE(STK_TYPE, 0x838, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_STK_READ_MEMORY \
        CTL_CODE(STK_TYPE, 0x841, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_STK_WRITE_MEMORY \
        CTL_CODE(STK_TYPE, 0x842, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_STK_READ_PCI_CONFIG \
        CTL_CODE(STK_TYPE, 0x851, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_STK_WRITE_PCI_CONFIG \
        CTL_CODE(STK_TYPE, 0x852, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#pragma pack(push, 4)

typedef struct STK_MSR_INPUT
{
	ULONG ulRegister;
	ULARGE_INTEGER uliValue;
}STK_MSR_INPUT;

typedef struct STK_IO_PORT_INPUT {
    ULONG   ulPort; 
 //   union
	//{
 //       ULONG   LongData;
 //       USHORT  ShortData;
        UCHAR   CharData;
    //} uData;
}STK_IO_PORT_INPUT;

#pragma pack(pop)


NTSTATUS ReadMsr(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned);
NTSTATUS WriteMsr(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned);
NTSTATUS ReadIOPort(ULONG nIoCode, VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned);
NTSTATUS WriteIOPort(ULONG nIoCode, VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned);
//NTSTATUS ReadPCI(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned);
//NTSTATUS WritePCI(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned);


VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
	UNICODE_STRING szDosDeviceName;
	DbgPrint("Hello from unload");

	PAGED_CODE();

	RtlInitUnicodeString(&szDosDeviceName, L"\\DosDevices\\STKDriver");
    IoDeleteSymbolicLink(&szDosDeviceName);

	if(pDriverObj->DeviceObject != NULL)
		IoDeleteDevice(pDriverObj->DeviceObject);
}

NTSTATUS UnsupportedFunction()
{
	DbgPrint("Unsupported function");
	return STATUS_SUCCESS;
}

NTSTATUS Dispatcher(PDEVICE_OBJECT pDeviceObj, PIRP pIrp)
{
	NTSTATUS nStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIrpStack;
	UNREFERENCED_PARAMETER(pDeviceObj);

	pIrp->IoStatus.Information = 0;
	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	switch(pIrpStack->MajorFunction)
	{
	case IRP_MJ_CREATE:
		DbgPrint("IRP_MJ_CREATE");
		nStatus = STATUS_SUCCESS;
		break;
	case IRP_MJ_CLOSE:
		DbgPrint("IRP_MJ_CLOSE");
		nStatus = STATUS_SUCCESS;
		break;
	case IRP_MJ_DEVICE_CONTROL:
		DbgPrint("IRP_MJ_DEVICE_CONTROL");
		switch(pIrpStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_STK_READ_MSR:
			DbgPrint("READ_MSR");
			nStatus = ReadMsr(
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
				(PULONG)&pIrp->IoStatus.Information);
			break;
		case IOCTL_STK_WRITE_MSR:
			DbgPrint("WRITE_MSR");
			nStatus = WriteMsr(
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
				(PULONG)&pIrp->IoStatus.Information);
			break;
		case IOCTL_STK_READ_IO_PORT:
		case IOCTL_STK_READ_IO_PORT_BYTE:
		case IOCTL_STK_READ_IO_PORT_WORD:
		case IOCTL_STK_READ_IO_PORT_DWORD:
			DbgPrint("READ_IO_PORT");
			nStatus = ReadIOPort(
				pIrpStack->Parameters.DeviceIoControl.IoControlCode,
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
				(PULONG)&pIrp->IoStatus.Information);
			break;
		case IOCTL_STK_WRITE_IO_PORT:
		case IOCTL_STK_WRITE_IO_PORT_BYTE:
		case IOCTL_STK_WRITE_IO_PORT_WORD:
		case IOCTL_STK_WRITE_IO_PORT_DWORD:
			DbgPrint("WRITE_IO_PORT");
			nStatus = WriteIOPort(
				pIrpStack->Parameters.DeviceIoControl.IoControlCode,
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
				pIrp->AssociatedIrp.SystemBuffer,
				pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
				(PULONG)&pIrp->IoStatus.Information);
			break;
		}
		break;
	}
	pIrp->IoStatus.Status = nStatus;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return nStatus;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	NTSTATUS nStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pDevObj = NULL;
	UNICODE_STRING szDriverName, szDosDeviceName;
	UNREFERENCED_PARAMETER(pRegistryPath);

	RtlInitUnicodeString(&szDriverName, L"\\Device\\STKDriver");
	RtlInitUnicodeString(&szDosDeviceName, L"\\DosDevices\\STKDriver");

	nStatus = IoCreateDevice(pDriverObject,
								0,
								&szDriverName,
								FILE_DEVICE_UNKNOWN,
								FILE_DEVICE_SECURE_OPEN,
								FALSE,
								&pDevObj);

	if( STATUS_SUCCESS == nStatus )
	{
		pDriverObject->DriverUnload = DriverUnload;
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = Dispatcher;
		pDriverObject->MajorFunction[IRP_MJ_CLOSE] = Dispatcher;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Dispatcher;
	}

	IoCreateSymbolicLink(&szDosDeviceName, &szDriverName);

	DbgPrint("Hello from Entry");

	return nStatus;
}

NTSTATUS ReadMsr(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned)
{
	UNREFERENCED_PARAMETER(nInBufferSize);
	UNREFERENCED_PARAMETER(nOutBufferSize);
	__try
	{
		ULONGLONG ullData = __readmsr(*(ULONG*)lpInBuffer);
		memcpy((PULONG)lpOutBuffer, &ullData, 8);
		*lpBytesReturned = 8;
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		*lpBytesReturned = 0;
		DbgPrint("ReadMsr - Exception");
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS WriteMsr(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned)
{
	UNREFERENCED_PARAMETER(nInBufferSize);
	UNREFERENCED_PARAMETER(nOutBufferSize);
	UNREFERENCED_PARAMETER(lpOutBuffer);
	__try
	{
		STK_MSR_INPUT *pInput = (STK_MSR_INPUT*)lpInBuffer;
		__writemsr(pInput->ulRegister, pInput->uliValue.QuadPart);
		*lpBytesReturned = 0;
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		*lpBytesReturned = 0;
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS ReadIOPort(ULONG nIoCode, VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned)
{
	ULONG ulPort = *(ULONG*)lpInBuffer;
	UNREFERENCED_PARAMETER(nOutBufferSize);

	switch(nIoCode)
	{
	case IOCTL_STK_READ_IO_PORT_BYTE:
		DbgPrint("READ_IO_PORT : READ_BYTE");
		*(PUCHAR)lpOutBuffer = READ_PORT_UCHAR((PUCHAR)(ULONG_PTR)ulPort);
		DbgPrint("\nVALUE: %d", *(PUCHAR)lpOutBuffer);
		break;
	//case IOCTL_STK_READ_IO_PORT_WORD:
	//	DbgPrint("READ_IO_PORT : READ_WORD");
	//	*(PUSHORT)lpOutBuffer = READ_PORT_USHORT((PUSHORT)(ULONG_PTR)ulPort);
	//	break;
	//case IOCTL_STK_READ_IO_PORT_DWORD:
	//	DbgPrint("READ_IO_PORT : READ_DWORD");
	//	*(PULONG)lpOutBuffer = READ_PORT_ULONG((PULONG)(ULONG_PTR)ulPort);
	//	break;
	default:
		*lpBytesReturned = 0;
		return STATUS_INVALID_PARAMETER;
	}

	*lpBytesReturned = nInBufferSize;

	return STATUS_SUCCESS;
}

NTSTATUS WriteIOPort(ULONG nIoCode, VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned)
{
	STK_IO_PORT_INPUT *pInput = (STK_IO_PORT_INPUT*)lpInBuffer;
	UNREFERENCED_PARAMETER(nInBufferSize);
	UNREFERENCED_PARAMETER(nOutBufferSize);
	UNREFERENCED_PARAMETER(lpOutBuffer);

	switch(nIoCode)
	{
	case IOCTL_STK_WRITE_IO_PORT_BYTE:
		DbgPrint("Port: %d\nValue: %d", pInput->ulPort, pInput->CharData);
		WRITE_PORT_UCHAR((PUCHAR)(ULONG_PTR)pInput->ulPort, pInput->CharData);
		break;
	//case IOCTL_STK_READ_IO_PORT_WORD:
	//	WRITE_PORT_USHORT((PUSHORT)(ULONG_PTR)pInput->ulPort, pInput->uData.ShortData);
	//	break;
	//case IOCTL_STK_READ_IO_PORT_DWORD:
	//	WRITE_PORT_ULONG((PULONG)(ULONG_PTR)pInput->ulPort, pInput->uData.LongData);
	//	break;
	default:
		*lpBytesReturned = 0;
		return STATUS_INVALID_PARAMETER;
	}

	*lpBytesReturned = 0;
	return STATUS_SUCCESS;
}

//NTSTATUS ReadPCI(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned)
//{
//
//}
//
//NTSTATUS WritePCI(VOID *lpInBuffer, ULONG nInBufferSize, VOID *lpOutBuffer, ULONG nOutBufferSize, ULONG *lpBytesReturned)
//{
//
//}