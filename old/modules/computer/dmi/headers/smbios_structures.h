#ifndef SMBIOS_STRUCTURES
#define SMBIOS_STRUCTURES

// WinAPI includes

// QT includes
#include <QString>

// My includes

typedef struct _SMBIOS_VERSION_
{
    QString Minor;
    QString Major;
}SmbiosVersion, *PSmbiosVersion;

typedef struct _BIOS_CHARACTERISTICS
{
    bool ISASupported;
    bool MCASupported;
    bool EISASupported;
    bool PCISupported;
    bool PCMCIASupported;
    bool PlugAndPlaySupported;
    bool APMSupported;
    bool BiosUpgradable;
    bool BiosShadowingAllowed;
    bool VL_VESASupported;
    bool ESCDSupported;
    bool BootFromCDSupported;
    bool SelectableBootSupported;
    bool BiosRomIsSocketed;
    bool BootFromPCMCIASupported;
    bool EDDSpecSupported;
    bool JapanFloppyNEC9800Supported;
    bool JapanFloppyToshibaSupported;
    bool Floppy5_25_360Supported;
    bool Floppy5_25_1_2Supported;
    bool Floppy3_5_720Supported;
    bool Floppy3_5_2_88Supported;
    bool PrintScreenServiceSupported;
    bool Keyboard8042ServiceSupported;
    bool SerialServiceSupported;
    bool PrinterServiceSupported;
    bool CGAVideoServiceSupported;
}BiosCharacteristics, *PBiosCharacteristics;

typedef struct _T0_BIOS_INFORMATION
{
    QString Vendor;
    QString BiosVersion;
    QString ReleaseDate;
    QString SystemBiosVersion;
    QString BiosROMSize;
    BiosCharacteristics BiosChar;
}T0BiosInformation, *PT0BiosInformation;

typedef struct _T2_BASEBOARD_INFORMATION
{
    QString Manufacturer;
    QString Product;
    QString Version;
    QString SerialNumber;
    QString Location;
    QString Type; // done
    QString HotSwappable; //done
    QString Replaceable; // done
    QString Removable; // done
    QString HostingBoard; // done
}T2BaseBoardInformation, *PT2BaseBoardInformation;

typedef struct _T4_PROCESSOR_INFORMATION
{
    QString ProcessorID; // N/A
    QString ProcessorType; // done
    QString ProcessorFamily;
    QString ProcessorManufacturer; // string
    QString Voltage; // done
    QString MaxSpeed; // done
    QString ExtClock; // done
    QString SerialNumber; // done
    QString CoreCount; // done
}T4ProcessorInformation, *PT4ProcessorInformation;

typedef struct _T17_MEMORY_DEVICE_INFORMATION
{
    QString TotalWidth;     // done
    QString DataWidth;      // done
    QString Size;           // done
    QString FormFactor;     // done
    QString DeviceLocator;  // string
    QString Speed;          // done
    QString Manufacturer;   // string
    QString SerialNumber;   // string
    QString ConfiguredMemoryClockSpeed;
}T17MemoryDeviceInformation, *PT17MemoryDeviceInformation;

#endif