#ifndef SMBIOS_ENTRY_POINT_H
#define SMBIOS_ENTRY_POINT_H

// WinAPI includes
#include <Windows.h>
#include <WbemIdl.h>

// QT includes
#include <QList>
#include <QDebug>
#include <QSemaphore>

// My includes
#include "../../../../interfaces/module.h"
#include "../../../../utils/headers/utils.h"
#include "smbios_api_classes.h"

// Defines
#define MAX_DATA 0xFA00 // 64KB

class CSMBiosEntryPoint : public IModule
{
    Q_OBJECT
private:
    PSmbiosVersion m_pSmbiosVersion;
    QList<ISMBiosGenericStructure*> qSMBiosData;
    unsigned char *m_pSmbiosTableData;
    int m_nSmbiosTableSize;

    QStandardItemModel *m_pDataTreeModel;
    QStandardItemModel *m_pDescTreeModel;
    QTreeView *m_pDataTree;
    QTreeView *m_pDescTree;

    QStandardItemModel *m_pBiosModel;
    QStandardItemModel *m_pBaseBoardModel;
    QStandardItemModel *m_pCpuModel;

public:
    CSMBiosEntryPoint();
    ~CSMBiosEntryPoint();

    int InitializeData(void);
    ISMBiosGenericStructure* GetStructure(SMStructureType type);
    virtual int OnCreateWidget(QWidget **ppWidget);

public slots:
    virtual void OnStartLoadingModuleDataSlot();

private slots:
    void DescriptionTreeItemClicked(QModelIndex);

private:
    void PopulateModels();
    int ParseData();
    QList<unsigned char*> ReturnTableOfTypeAndLength(int nType, int nLength);
    bool bBiosFound;
};

#endif
