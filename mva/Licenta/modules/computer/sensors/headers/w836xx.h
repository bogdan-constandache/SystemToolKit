#ifndef W836XX_H
#define W836XX_H

#include "sensor_defines.h"
#include "stk_driver_wrapper.h"
#include "../interfaces/isensor.h"
#include "../../../../utils/headers/utils.h"


#define W836_CHIP_ADDRESS_REGISTER_OFFSET         0x05
#define W836_CHIP_DATA_REGISTER_OFFSET            0x06
#define W836_CHIP_BANK_SELECT_REGISTER            0x4E
#define W836_CHIP_TEMPERATURE_SOURCE_FLAG         0x49
#define W836_CHIP_VENDOR_ID_REGISTER              0x4F
#define W836_CHIP_VOLTAGE_VBAT_REGISTER           0x51


class CW836XX : public ISensor
{
private:
    USHORT usAddress;
    BYTE bRevision;

    Chip m_eChip;

    CSTKDriverWrapper *m_pDriver;

    VoltageReading *m_pVoltages;
    double m_pTemps[3];
    double m_pFans[5];
    bool m_bPCITemps[3];

    int nVoltages;

    double dVoltageGain;

    BYTE *m_bVoltageReg;
    BYTE *m_bVoltageBank;

    BYTE *m_bTempReg;
    BYTE *m_bTempBank;

    BYTE *m_bFanTachoReg;
    BYTE *m_bFanTachoBank;
    BYTE *m_bFanBitReg;
    BYTE *m_bFanDivBit0;
    BYTE *m_bFanDivBit1;
    BYTE *m_bFanDivBit2;

    int ReadByteFromBank(BYTE bBank, BYTE bReg, BYTE *bResult);

public:
    CW836XX(Chip, USHORT, BYTE);
    virtual ~CW836XX();

    virtual int Initialize();
    virtual int Destroy();
    virtual int Update();
    virtual double* GetTemps();
    virtual double* GetFanSpeeds();
    virtual VoltageReading* GetVoltages();
    virtual QString GetChipName();
};

#endif // W836XX_H
