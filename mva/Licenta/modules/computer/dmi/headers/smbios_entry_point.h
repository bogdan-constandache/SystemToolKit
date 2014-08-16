#ifndef SMBIOS_ENTRY_POINT_H
#define SMBIOS_ENTRY_POINT_H

// WinAPI includes
#include <Windows.h>
#include <WbemIdl.h>

// QT includes
#include <QList>
#include <QDebug>
#include <QSemaphore>
#include <QStandardItemModel>

// My includes
#include "../../../../utils/headers/utils.h"
#include "../../../../controller/enums.h"
#include "smbios_api_classes.h"

// Defines
#define MAX_DATA 0xFA00 // 64KB

class CSMBiosEntryPoint
{
private:
    PSmbiosVersion                  m_pSmbiosVersion;
    QList<ISMBiosGenericStructure*> qSMBiosData;
    unsigned char                   *m_pSmbiosTableData;
    int                             m_nSmbiosTableSize;

    QStandardItemModel              *m_pStructsModel;
    QStandardItemModel              *m_pDataModel;

public:
    CSMBiosEntryPoint();
    ~CSMBiosEntryPoint();
    QStandardItemModel* GetItemsModel();
    QStandardItemModel* GetItemPropertiesModel();

    void OnRefreshData(DMIModuleType);

private:
    QList<ISMBiosGenericStructure*> GetStructure(SMStructureType type);
    int InitializeData(void);
    void PopulateModels();
    int ParseData();
    QList<unsigned char*> ReturnTableOfTypeAndLength(int nType, int nLength);
    bool bBiosFound;
};

#endif
