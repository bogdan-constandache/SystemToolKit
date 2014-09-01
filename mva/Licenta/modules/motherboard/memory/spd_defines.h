#ifndef _SPD_DEFINES_H
#define _SPD_DEFINES_H

#define IO_CONFIG_SPACE_CONTROL_ADDRESS             0x0cf8 //config  space  control  address
#define IO_CONFIG_SPACE_DATA_ADDRESS                0x0cfc //config  space  data  address
#define BASEADDRESS                                 0x80000000 //PCI-SMBus  init  address
#define DIMM0                                       0x0a0 //memory  device  ID
#define DIMM1                                       0x0a2
#define DIMM2                                       0x0a4
#define DIMM3                                       0x0a6

#define DDR                                         7
#define DDR2                                        8
#define DDR3                                        11

#define DIMM_TYPE(X) \
    X == DDR ? "DDR" : \
    X == DDR2 ? "DDR2" : \
    X == DDR3 ? "DDR3" : \
    "UNKNOWN"

#define DIMM_TO_STRING(X) \
    X == DIMM0 ? "DIMM1" : \
    X == DIMM1 ? "DIMM2" : \
    X == DIMM2 ? "DIMM3" : \
    X == DIMM3 ? "DIMM4" : \
    "UNKNOWN"

#define ModuleTypeUndefined                         0x0
#define ModuleTypeRDIMM                             0x1
#define ModuleTypeUDIMM                             0x2
#define ModuleTypeSODIMM                            0x3
#define ModuleTypeMicroDIMM                         0x4
#define ModuleTypeMiniRDIMM                         0x5
#define ModuleTypeMiniUDIMM                         0x6
#define ModuleTypeMiniCDIMM                         0x7
#define ModuleType72BSOUDIMM                        0x8
#define ModuleType72BSORDIMM                        0x9
#define ModuleType72BSOCDIMM                        0xA
#define ModuleTypeLRDIMM                            0xB
#define ModuleType16BSODIMM                         0xC
#define ModuleType32BSODIMM                         0xD

#define MODULE_DDR3_TYPE(X) \
    X == ModuleTypeUndefined ? "Undefined" : \
    X == ModuleTypeRDIMM ? "RDIMM" : \
    X == ModuleTypeUDIMM ? "UDIMM" : \
    X == ModuleTypeSODIMM ? "SO-DIMM" : \
    X == ModuleTypeMicroDIMM ? "Micro-DIMM" : \
    X == ModuleTypeMiniRDIMM ? "Mini-RDIMM" : \
    X == ModuleTypeMiniUDIMM ? "Mini-UDIMM" : \
    X == ModuleTypeMiniCDIMM ? "Mini-CDIMM" : \
    X == ModuleType72BSOUDIMM ? "72b-SO-UDIMM" : \
    X == ModuleType72BSORDIMM ? "72b-SO-RDIMM" : \
    X == ModuleType72BSOCDIMM ? "72b-SO-CDIMM" : \
    X == ModuleTypeLRDIMM ? "LRDIMM" : \
    X == ModuleType16BSODIMM ? "16b-SO-DIMM" : \
    X == ModuleType32BSODIMM ? "32b-SO-DIMM" : \
    "Reserved"

#define MODULE_DDR3_VOLTAGE(X) \
    X == 0x0 ? "1.5V" : \
    X == 0x1 ? "Custom voltage" : \
    X == 0x2 ? "1.35V, 1.5V" : \
    X == 0x3 ? "1.35V" : \
    X == 0x4 ? "1.25V, 1.5V" : \
    X == 0x5 ? "1.25V" : \
    X == 0x6 ? "1.25V, 1.35V, 1.5V" : \
    X == 0x7 ? "1.25V, 1.35V" : \
    "Undefined"

#define SDRAM_DDR3_CAPACITY(X) \
    X == 0x0 ? 256 : \
    X == 0x1 ? 512 : \
    X == 0x2 ? 1024 : \
    X == 0x3 ? 2048 : \
    X == 0x4 ? 4096 : \
    X == 0x5 ? 8192 : \
    X == 0x6 ? 16386 : \
    0

#define PRIMARY_BUS_WIDTH_DDR3(X) \
    X == 0x0 ? 8 : \
    X == 0x1 ? 16 : \
    X == 0x2 ? 32 : \
    X == 0x3 ? 64 : \
    0

#define SDRAM_DDR3_DEVICE_WIDTH(X) \
    X == 0x0 ? 4 : \
    X == 0x1 ? 8 : \
    X == 0x2 ? 16 : \
    X == 0x3 ? 32 : \
    0

#define RANKS_DDR3(X) \
    X == 0x0 ? 1 : \
    X == 0x1 ? 2 : \
    X == 0x2 ? 3 : \
    X == 0x3 ? 4 : \
    0

typedef struct _SPD_INFORMATION_
{
    QString qsSPDSize;
    QString qsSPDVersion;
    QString qsName;
    QString qsManufacturer;
    QString qsSerial;
    QString qsPartNumber;
    QString qsManufactureDate;
    QString qsSize;
    QString qsCASLatencies;
    QString qsModuleType;
    QString qsModuleBusWidth;
    QString qsModuleVoltage;
    QString qsDeviceType;
    QString qsSDRAMDeviceType;
    QString qsECCMethod;
    QStringList qTimings;
    QStringList qFeatures;
}SpdInformation;

typedef struct _MANUFACTURER_DETAILS_
{
    int nID;
    int nContinuation;
    QString qsManufacturer;
    int nManufacturerID;
}ManufacturerDetails;

#endif
