#ifndef _DRIVER_DEFINES_
#define _DRIVER_DEFINES_


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


//#pragma pack(push, 4)

typedef struct STK_MSR_INPUT
{
    ULONG ulRegister;
    ULARGE_INTEGER uliValue;
}STK_MSR_INPUT;

typedef struct STK_IO_PORT_INPUT {
    ULONG   ulPort;
    UCHAR   CharData;
}STK_IO_PORT_INPUT;

typedef struct STK_IO_PORT_DWORD {
    ULONG   ulPort;
    ULONG   dwData;
}STK_IO_PORT_DWORD;

typedef struct STK_READ_PCI_CONFIG_INPUT
{
    ULONG ulPCIAddress;
    ULONG ulPCIOffset;

}STK_READ_PCI_CONFIG_INPUT;

typedef struct STK_WRITE_PCI_CONFIG_INPUT
{
    ULONG ulPCIAddress;
    ULONG ulPCIOffset;
    ULONG ulData;
}STK_WRITE_PCI_CONFIG_INPUT;

//#pragma pack(pop)


#endif
