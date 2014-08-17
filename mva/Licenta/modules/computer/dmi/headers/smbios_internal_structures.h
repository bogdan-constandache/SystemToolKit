#ifndef SMBIOS_DEFINES
#define SMBIOS_DEFINES

#pragma pack(1)
typedef struct _SMBIOS_STRUCT_HEADER
{
    unsigned char    bType;
    unsigned char    bLength;
    unsigned short   wHandle;
}SMHeader, *PSMHeader;

typedef enum
{
    BIOS_INFORMATION_TYPE = 0x00,
    BASEBOARD_INFORMATION_TYPE = 0x02,
    ENCLOSURE_INFORMATION_TYPE = 0x03,
    PROCESSOR_INFORMATION_TYPE = 0x04,
    CACHE_INFORMATION_TYPE = 0x07,
    MEMORY_DEVICE_INFORMATION_TYPE = 0x11
}SMStructureType;

// type 0 bios info
typedef struct _SMBIOS_TYPE0_BIOS_INFORMATION
{
    unsigned char   bVendor;
    unsigned char   bBiosVersion;
    unsigned short  wBiosStartingAddress;
    unsigned char   bBiosReleaseDate;
    unsigned char   bBiosROMSize;
    unsigned __int64   qwBiosCharacteristics;
    unsigned short  wOptional;
    unsigned char   bSystemBiosMajorRelease;
    unsigned char   bSystemBiosMinorRelease;
    unsigned char   bEmbeddedMajorRelease;
    unsigned char   bEmbeddedMinorRelease;
}Type0BiosInformation, *PType0BiosInformation;

//type 0 BiosCharacteristics
#define RESERVED_1                              0x01
#define RESERVED_2                              0x02
#define UNKNOWN                                 0x04
#define BC_NOT_SUPPORTED                        0x08
#define ISA_SUPPORTED                           0x10
#define MCA_SUPPORTED                           0x20
#define EISA_SUPPORTED                          0x40
#define PCI_SUPPORTED                           0x80
#define PCMCIA_SUPPORTED                        0x100
#define PnP_SUPPORTED                           0x200
#define APM_SUPPORTED                           0x400
#define IS_UPGRADEABLE                          0x800
#define SHADOWING_SUPPORTED                     0x1000
#define VL_VESA_SUPPORTED                       0x2000
#define ESCD_SUPPORTED                          0x4000
#define BOOT_FROM_CD_SUPPORTED                  0x8000
#define SELECTABLE_BOOT_SUPPORTED               0x10000
#define BIOS_ROM_SOCKETED                       0x20000
#define BOOT_FROM_PCMCIA_SUPPORTED              0x40000
#define EDD_SUPPORTED                           0x80000
#define INT_13H_JAP_FLOPPY_NEC_SUPPORTED        0x100000
#define INT_13H_JAP_FLOPPY_TOSHIBA_SUPPORTED    0x200000
#define INT_13H_525_360KB_FLOPPY_SUPPORTED      0x400000
#define INT_13H_525_12MB_FLOPPY_SUPPORTED       0x800000
#define INT_13H_35_720KB_FLOPPY_SUPPORTED       0x1000000
#define INT_13H_35_288MB_FLOPPY_SUPPORTED       0x2000000
#define INT_5H_PRINT_SCREEN_SUPPORTED           0x4000000
#define INT_9H_842_KEYBD_SUPPORTED              0x8000000
#define INT_14H_SERIAL_SUPPORTED                0x10000000
#define INT_17H_PRINTER_SUPPORTED               0x20000000
#define INT_10H_CGA_MONO_VIDEO_SUPPORTED        0x40000000
#define NEC_PC_98                               0x80000000

//type 0 BIOS characteristics extension byte 1
#define ACPI_SUPPORTED              0x01
#define USB_LEGACY_SUPPORTED        0x02
#define AGP_SUPPORTED               0x04
#define I20_BOOT_SUPPORTED          0x08
#define LS120_BOOT_SUPPORTED        0x10
#define ATAPI_ZIP_BOOT_SUPPORTED    0x20
#define BOOT_1394_SUPPORTED         0x40
#define SMART_BATTERY_SUPPORTED     0x80

//type 0 BIOS characteristics extension byte 2
#define BIOS_BOOT_SPEC_SUPPORTED                0x01
#define FUNC_KEY_INIT_NET_BOOT_SUPPORTED        0x02
#define TARGET_CONTENT_DISTRIBUTION_SUPPORTED   0x04


//type 2 BASEBOARD_INFORMATION
typedef struct _SMBIOS_TYPE2_BASEBOARD_INFORMATION
{
    unsigned char   Manufacturer;
    unsigned char   Product;
    unsigned char   Version;
    unsigned char   SerialNumber;
    unsigned char   AssetTag;
    unsigned char   FeatureFlags;
    unsigned char   Location;
    unsigned short  ChassisHandle;
    unsigned char   BoardType;
    unsigned char   NrOfObjHandles;
}Type2BaseBoardInformation, *PType2BaseBoardInformation;

//type 3 ENCLOSURE_INFORMATION
typedef struct _SMBIOS_TYPE3_ENCLOSURE_INFORMATION
{
    unsigned char   Manufacturer;
    unsigned char   Type;
    unsigned char   Version;
    unsigned char   SerialNumber;
    unsigned char   AssetTagNumber;
    unsigned char   BootUpState;
    unsigned char   PowerSupplyState;
    unsigned char   ThermalState;
    unsigned char   SecurityStatus;
    unsigned int    OEMDefined;
    unsigned char   Height;
    unsigned char   NumberOfPowerCords;
    unsigned char   ContainedElementCount;
    unsigned char   ContainedElementRecordLength;
}Type3EnclosureInformation, *PType3EnclosureInfomation;

//type 4 PROCESSOR INFORMATION
typedef struct _SMBIOS_TYPE4_PROCESSOR_INFORMATION
{
    unsigned char       SocketDesignation;
    unsigned char       ProcessorType;
    unsigned char       ProcessorFamily;
    unsigned char       ProcessorManufactor;
    unsigned __int64    ProcessorID;
    unsigned char       ProcessorVersion;
    unsigned char       Voltage;
    unsigned short      ExternalClock;
    unsigned short      MaxSpeed;
    unsigned short      CurrentSpeed;
    unsigned char       Status;
    unsigned char       ProcessorUpgrade;
    unsigned short      L1CacheHandle;
    unsigned short      L2CacheHandle;
    unsigned short      L3CacheHandle;
    unsigned char       SerialNumber;
    unsigned char       AssetTag;
    unsigned char       PartNumber;
    unsigned char       CoreCount;
    unsigned char       CoreEnable;
    unsigned char       ThreadCount;
    unsigned short      ProcessorCharacteristics;
    unsigned short      ProcessorFamily2;
}Type4ProcessorInformation, *PType4ProcessorInformation;

// type 7 CACHE INFORMATION
typedef struct _SMBIOS_TYPE7_CACHE_INFORMATION
{
    unsigned char       SocketDesignation;
    unsigned short      CacheConfiguration;
    unsigned short      MaximumCacheSize;
    unsigned short      InstalledSize;
    unsigned short      SupportedSRAMType;
    unsigned short      CurrentSRAMType;
    unsigned char       CacheSpeed;
    unsigned char       ErrorCorrectionType;
    unsigned char       SystemCacheType;
    unsigned char       Associativity;
}Type7CacheInformation, *PType7CacheInformation;

//type 17 MEMORY DEVICE INFORMATION
typedef struct _SMBIOS_TYPE17_MEMORY_DEVICE_INFORMATION
{
    unsigned short  PhysicalMemoryArrayHandle;
    unsigned short  MemoryErrorInformationHandle;
    unsigned short  TotalWidth;
    unsigned short  DataWidth;
    unsigned short  Size;
    unsigned char   FormFactor;
    unsigned char   DeviceSet;
    unsigned char   DeviceLocator;
    unsigned char   BankLocator;
    unsigned char   MemoryType;
    unsigned short  TypeDetail;
    unsigned short  Speed;
    unsigned char   Manufacturer;
    unsigned char   SerialNumber;
    unsigned char   AssetTag;
    unsigned char   PartNumber;
    unsigned char   Attributes;
    unsigned int    ExtendedSize;
    unsigned short  ConfiguredMemoryClockSpeed;
    unsigned short  MinimumVoltage;
    unsigned short  MaximumVoltage;
    unsigned short  ConfiguredVoltage;
}Type17MemoryDeviceInformation, *PType17MemoryDeviceInformation;

#endif
