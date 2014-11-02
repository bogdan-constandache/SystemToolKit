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

#define ModuleTypeDDR3Undefined                         0x0
#define ModuleTypeDDR3RDIMM                             0x1
#define ModuleTypeDDR3UDIMM                             0x2
#define ModuleTypeDDR3SODIMM                            0x3
#define ModuleTypeDDR3MicroDIMM                         0x4
#define ModuleTypeDDR3MiniRDIMM                         0x5
#define ModuleTypeDDR3MiniUDIMM                         0x6
#define ModuleTypeDDR3MiniCDIMM                         0x7
#define ModuleTypeDDR372BSOUDIMM                        0x8
#define ModuleTypeDDR372BSORDIMM                        0x9
#define ModuleTypeDDR372BSOCDIMM                        0xA
#define ModuleTypeDDR3LRDIMM                            0xB
#define ModuleTypeDDR316BSODIMM                         0xC
#define ModuleTypeDDR332BSODIMM                         0xD

#define MODULE_DDR3_TYPE(X) \
    X == ModuleTypeDDR3Undefined ? "Undefined" : \
    X == ModuleTypeDDR3RDIMM ? "RDIMM" : \
    X == ModuleTypeDDR3UDIMM ? "UDIMM" : \
    X == ModuleTypeDDR3SODIMM ? "SO-DIMM" : \
    X == ModuleTypeDDR3MicroDIMM ? "Micro-DIMM" : \
    X == ModuleTypeDDR3MiniRDIMM ? "Mini-RDIMM" : \
    X == ModuleTypeDDR3MiniUDIMM ? "Mini-UDIMM" : \
    X == ModuleTypeDDR3MiniCDIMM ? "Mini-CDIMM" : \
    X == ModuleTypeDDR372BSOUDIMM ? "72b-SO-UDIMM" : \
    X == ModuleTypeDDR372BSORDIMM ? "72b-SO-RDIMM" : \
    X == ModuleTypeDDR372BSOCDIMM ? "72b-SO-CDIMM" : \
    X == ModuleTypeDDR3LRDIMM ? "LRDIMM" : \
    X == ModuleTypeDDR316BSODIMM ? "16b-SO-DIMM" : \
    X == ModuleTypeDDR332BSODIMM ? "32b-SO-DIMM" : \
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


#define ModuleTypeDDR2Undefined         0x0
#define ModuleTypeDDR2RDIMM             0x1
#define ModuleTypeDDR2UDIMM             0x2
#define ModuleTypeDDR2SODIMM            0x4
#define ModuleTypeDDR272BSOCDIMM        0x6
#define ModuleTypeDDR272BSORDIMM        0x7
#define ModuleTypeDDR2MICRODIMM         0x8
#define ModuleTypeDDR2MICRORDIMM        0x10
#define ModuleTypeDDR2MICRUUDIMM        0x20

#define MODULE_DDR2_TYPE(X) X == ModuleTypeDDR2Undefined ? "Undefined" : \
    X == ModuleTypeDDR2RDIMM ? "RDIMM" : \
    X == ModuleTypeDDR2UDIMM ? "Unbuffered DIMM" : \
    X == ModuleTypeDDR2SODIMM ? "SO-DIMM" : \
    X == ModuleTypeDDR272BSOCDIMM ? "72b-SO-CDIMM" : \
    X == ModuleTypeDDR272BSORDIMM ? "72b-SO-RDIMM" : \
    X == ModuleTypeDDR2MICRODIMM ? "Micro-DIMM" : \
    X == ModuleTypeDDR2MICRORDIMM ? "Micro-RDIMM" : \
    X == ModuleTypeDDR2MICRUUDIMM ? "Micro-UDIMM" : \
    "Undefined"

#define MODULE_DDR2_VOLTAGE(X) \
    0x0 == X ? "TTL 5V" : \
    0x1 == X ? "LVTTL" : \
    0x2 == X ? "1.5V" : \
    0x3 == X ? "3.3V" : \
    0x4 == X ? "2.5V" : \
    0x5 == X ? "1.8V" : \
    0x6 == X ? "TBD" : \
    "Undefined"

#define MODULE_DDR2_FREQ_STRING(X) \
    0xA0 == X ? "100MHz" : \
    0x75 == X ? "133MHz" : \
    0x60 == X ? "167MHz" : \
    0x50 == X ? "200MHz" : \
    0x46 == X ? "217MHz" : \
    0x42 == X ? "233MHz" : \
    0x3D == X ? "266MHz" : \
    0x30 == X ? "333MHz" : \
    0x25 == X ? "400MHz" : \
    0x20 == X ? "500MHz" : \
    "Undefined"

#define MODULE_DDR2_FREQ_INT(X) \
    0xA0 == X ? 100 : \
    0x75 == X ? 133 : \
    0x60 == X ? 167 : \
    0x50 == X ? 200 : \
    0x46 == X ? 217 : \
    0x42 == X ? 233 : \
    0x3D == X ? 266 : \
    0x30 == X ? 333 : \
    0x25 == X ? 400 : \
    0x20 == X ? 500 : \
    0

#define MODULE_DDR2_MAX_CAS_STRING(X) \
    0 == X ? "TBD" : \
    1 == X ? "TBD" : \
    2 == X ? "2" : \
    3 == X ? "3" : \
    4 == X ? "4" : \
    5 == X ? "5" : \
    6 == X ? "6" : \
    7 == X ? "7" : \
    "Undefined"

#define MODULE_DDR2_GET_FREQ_CLOCK_NS(X, dRes) \
{ \
    double dFreq = (double)X / 1000.0; \
    dRes = 1.0 / dFreq; \
}

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
    QString qsMaxTimings;
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
