#ifndef _MEMORY_DATA_H
#define _MEMORY_DATA_H

#include <Windows.h>
#include <comdef.h>
#include <WbemIdl.h>

#include <QStandardItemModel>

#include "../../../utils/headers/utils.h"

typedef struct _RAM_DATA_
{
    QString qzBankLabel;
    QString qzCapacity;
    QString qzDataWidth;
    QString qzDescription;
    QString qzFormFactor;
    QString qzManufacturer;
    QString qzMemoryType;
    QString qzModel;
    QString qzName;
    QString qzSpeed;
}RamData;

class CRAMInformation
{
private:
    QList<RamData*> m_qRamData;

    int ClearRamDataList();
    int InitializeInformation();

public:
    CRAMInformation();
    ~CRAMInformation();

    QStandardItemModel *GetRAMInformations();
};

#endif
