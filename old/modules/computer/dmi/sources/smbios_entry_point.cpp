#include "../headers/smbios_entry_point.h"

#pragma comment(lib,"Wbemuuid.lib")


CSMBiosEntryPoint::CSMBiosEntryPoint() : bBiosFound(false), m_pSmbiosTableData(0)
{
    this->m_pSmbiosVersion = new SmbiosVersion;
}

CSMBiosEntryPoint::~CSMBiosEntryPoint()
{
}

QList<unsigned char *> CSMBiosEntryPoint::ReturnTableOfTypeAndLength(int nType, int nLength)
{
    int nIndex = 0;
    QList<unsigned char *> pData;

    while(nIndex < m_nSmbiosTableSize)
    {
        if( nType == (int)m_pSmbiosTableData[nIndex] && nLength == (int)m_pSmbiosTableData[nIndex+1] )
        {
            if( nIndex > 1 )
            {
                if( '\0' == m_pSmbiosTableData[nIndex - 1] && '\0' == m_pSmbiosTableData[nIndex - 2] )
                {
                    pData.append(m_pSmbiosTableData + nIndex);
                }
            }
            else
            {
                pData.append(m_pSmbiosTableData + nIndex);
            }
        }
        nIndex++;
    }
    return pData;
}

int CSMBiosEntryPoint::InitializeData(void)
{
    HRESULT hResult;
    LSTATUS nStatus;
    IWbemLocator* pLocator = 0;
    IWbemServices* pService = 0;
    IEnumWbemClassObject* pEnumerator = NULL;

    hResult = CoInitializeEx(0, COINIT_MULTITHREADED);
    if( 0 > hResult )
    {
        if( TRUE == WIN32_FROM_HRESULT(hResult,(DWORD*)&nStatus) )
            return nStatus;
        else
            return ERROR_CAN_NOT_COMPLETE;
    }

    hResult = CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator, (LPVOID *) &pLocator);

    if( 0 > hResult )
    {
        CoUninitialize();
        if(TRUE == WIN32_FROM_HRESULT(hResult,(DWORD*)&nStatus))
            return nStatus;
        else
            return ERROR_CAN_NOT_COMPLETE;
    }

    hResult = pLocator->ConnectServer(
                L"ROOT\\WMI",       // WMI namespace
                NULL,               // User name
                NULL,               // User password
                0,                  // Locale
                NULL,               // Security flags
                0,                  // Authority
                0,                  // Context object
                &pService           // IWbemServices proxy
                );

    if( 0 > hResult )
    {
        pLocator->Release();
        CoUninitialize();
        if(TRUE == WIN32_FROM_HRESULT(hResult,(DWORD*)&nStatus))
            return nStatus;
        else
            return ERROR_CAN_NOT_COMPLETE;
    }

    hResult = CoSetProxyBlanket(
                pService,                     // the proxy to set
                RPC_C_AUTHN_WINNT,            // authentication service
                RPC_C_AUTHZ_NONE,             // authorization service
                NULL,                         // Server principal name
                RPC_C_AUTHN_LEVEL_CALL,       // authentication level
                RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
                NULL,                         // client identity
                EOAC_NONE                     // proxy capabilities
                );

    if( 0 > hResult )
    {
        pService->Release();
        pLocator->Release();
        CoUninitialize();
        if(TRUE == WIN32_FROM_HRESULT(hResult,(DWORD*)&nStatus))
            return nStatus;
        else
            return ERROR_CAN_NOT_COMPLETE;
    }

    hResult = pService->CreateInstanceEnum(
                L"MSSmBios_RawSMBiosTables",
                0,
                NULL,
                &pEnumerator
                );

    if( 0 > hResult )
    {
        pService->Release();
        pLocator->Release();
        CoUninitialize();
        if(TRUE == WIN32_FROM_HRESULT(hResult,(DWORD*)&nStatus))
            return nStatus;
        else
            return ERROR_CAN_NOT_COMPLETE;
    }

    do
    {
        IWbemClassObject* pInstance = NULL;
        ULONG dwCount = NULL;

        hResult = pEnumerator->Next(
                    WBEM_INFINITE,
                    1,
                    &pInstance,
                    &dwCount);

        if(hResult>=0)
        {
            VARIANT varBiosData;
            VariantInit(&varBiosData);
            CIMTYPE type;

            hResult = pInstance->Get(L"SmbiosMajorVersion",0,&varBiosData,&type,NULL);

            if(hResult <0)
                VariantClear(&varBiosData);
            else
            {
                m_pSmbiosVersion->Major.setNum(varBiosData.bVal);
                VariantInit(&varBiosData);
                hResult = pInstance->Get(L"SmbiosMinorVersion",0,&varBiosData,&type,NULL);
                if(hResult<0)
                    VariantClear(&varBiosData);
                else
                {
                    m_pSmbiosVersion->Minor.setNum(varBiosData.bVal);
                    VariantInit(&varBiosData);
                    hResult = pInstance->Get(L"SMBiosData",0,&varBiosData,&type,NULL);
                    if(hResult>=0)
                    {
                        if ( ( VT_UI1 | VT_ARRAY  ) != varBiosData.vt )
                        {
                        }
                        else
                        {
                            SAFEARRAY* pArray = NULL;
                            pArray = V_ARRAY(&varBiosData);
                            unsigned char* p_data = (unsigned char*)pArray->pvData;
                            DWORD dwLength = pArray->rgsabound[0].cElements;
                            this->m_pSmbiosTableData = p_data;
                            this->m_nSmbiosTableSize = dwLength;
                            this->ParseData();
                        }
                    }
                    VariantClear(&varBiosData);
                }
            }
            break;
        }

    } while (hResult == WBEM_S_NO_ERROR);

    CoUninitialize();

    return ERROR_SUCCESS;
}

ISMBiosGenericStructure *CSMBiosEntryPoint::GetStructure(SMStructureType type)
{
    for(int i = 0; i < qSMBiosData.count(); i++)
    {
        if( type == qSMBiosData.at(i)->GetStructureType() )
        {
            return qSMBiosData.at(i);
        }
    }
    return 0;
}

void CSMBiosEntryPoint::OnStartLoadingModuleDataSlot()
{
    InitializeData();
    emit OnLoadingModuleDataCompleteSignal();
    emit OnCreateWidgetSignal(COMPUTER_DMI, this);

}

void CSMBiosEntryPoint::DescriptionTreeItemClicked(QModelIndex index)
{
    QStandardItem *pItem = m_pDescTreeModel->itemFromIndex(index);
    QString qzItemText = pItem->text();
    int nType = -1;

    if ("BIOS" == qzItemText)
        nType = 0;
    else
        if ("Base board" == qzItemText)
            nType = 1;
        else
            if ("CPU" == qzItemText)
                nType = 2;
            else
                nType = 3;

    switch(nType)
    {
    case 0:
        emit OnSendModuleDataToGUISignal(COMPUTER_DMI, m_pDataTree, m_pBiosModel);
        break;
    case 1:
        emit OnSendModuleDataToGUISignal(COMPUTER_DMI, m_pDataTree, m_pBaseBoardModel);
        break;
    case 2:
        emit OnSendModuleDataToGUISignal(COMPUTER_DMI, m_pDataTree, m_pCpuModel);
        break;
    }
}

void CSMBiosEntryPoint::PopulateModels()
{
    this->m_pBiosModel = new QStandardItemModel();
    this->m_pBaseBoardModel = new QStandardItemModel();
    this->m_pCpuModel = new QStandardItemModel();

    ISMBiosGenericStructure *pStructure = 0;
    PT4ProcessorInformation pData = 0;
    PT0BiosInformation pData0 = 0;
    PT2BaseBoardInformation pData2 = 0;
    PT17MemoryDeviceInformation pData17 = 0;

    QList<QStandardItem*> qList;

    QStandardItem *pDataRoot = m_pBiosModel->invisibleRootItem();
    QStandardItem *pDataItem = 0;
    pStructure = GetStructure(BIOS_INFORMATION_TYPE);
    pData0 = (PT0BiosInformation)pStructure->GetStructureData();

    pDataItem = new QStandardItem(QString("Vendor:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData0->Vendor == "" ? "N/A" : pData0->Vendor);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Version:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData0->BiosVersion == "" ? "N/A" : pData0->BiosVersion);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();


    pDataItem = new QStandardItem(QString("System BIOS version:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData0->SystemBiosVersion == "" ? "N/A" : pData0->SystemBiosVersion);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Release date:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData0->ReleaseDate == "" ? "N/A" : pData0->ReleaseDate);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("ROM memory size:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData0->BiosROMSize == "" ? "N/A" : pData0->BiosROMSize);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();


    pDataRoot = this->m_pBaseBoardModel->invisibleRootItem();
    pDataItem = 0;
    pStructure = GetStructure(BASEBOARD_INFORMATION_TYPE);
    pData2 = (PT2BaseBoardInformation)pStructure->GetStructureData();

    pDataItem = new QStandardItem(QString("Manufacturer:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Manufacturer == "" ? "N/A" : pData2->Manufacturer);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();


    pDataItem = new QStandardItem(QString("Product:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Product == "" ? "N/A" : pData2->Product);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Version:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Version == "" ? "N/A" : pData2->Version);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Serial number:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->SerialNumber == "" ? "N/A" : pData2->SerialNumber);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Location:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Location == "" ? "N/A" : pData2->Location);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Hot swappable:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->HotSwappable == "" ? "N/A" : pData2->HotSwappable);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Replaceable:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Replaceable == "" ? "N/A" : pData2->Replaceable);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Removable:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Removable == "" ? "N/A" : pData2->Removable);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("HostingBoard:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->HostingBoard == "" ? "N/A" : pData2->HostingBoard);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Type:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData2->Type == "" ? "N/A" : pData2->Type);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();


    pDataRoot = m_pCpuModel->invisibleRootItem();
    pDataItem = 0;
    pStructure = GetStructure(PROCESSOR_INFORMATION_TYPE);
    pData = (PT4ProcessorInformation)pStructure->GetStructureData();

    pDataItem = new QStandardItem(QString("Type:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->ProcessorType == "" ? "N/A" : pData->ProcessorType);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Family:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->ProcessorFamily == "" ? "N/A" : pData->ProcessorFamily);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Manufacturer:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->ProcessorManufacturer == "" ? "N/A" : pData->ProcessorManufacturer);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Voltage:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->Voltage == "" ? "N/A" : pData->Voltage);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("External clock:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->ExtClock == "" ? "N/A" : pData->ExtClock);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Serial number:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->SerialNumber == "" ? "N/A" : pData->SerialNumber);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Max Speed:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->MaxSpeed == "" ? "N/A" : pData->MaxSpeed);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();

    pDataItem = new QStandardItem(QString("Core count:"));
    qList.append(pDataItem);
    pDataItem = new QStandardItem(pData->CoreCount == "" ? "N/A" : pData->CoreCount);
    qList.append(pDataItem);
    pDataRoot->appendRow(qList);
    qList.clear();


//    pDataRoot->appendColumn(qList);
//    qList.clear();

//    pStructure = GetStructure(MEMORY_DEVICE_INFORMATION_TYPE);
//    pData17 = (PT17MemoryDeviceInformation)pStructure->GetStructureData();

//    pDataItem = new QStandardItem(QString("Total width:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Data width:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Size:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Form factor:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Device locator:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Speed:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Manufacturer:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Serial number:"));
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(QString("Configured memory clock speed:"));
//    qList.append(pDataItem);

//    pDataRoot->appendColumn(qList);
//    qList.clear();

//    pDataItem = new QStandardItem(pData17->TotalWidth == "" ? "N/A" : pData17->TotalWidth);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->DataWidth == "" ? "N/A" : pData17->DataWidth);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->Size == "" ? "N/A" : pData17->Size);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->FormFactor == "" ? "N/A" : pData17->FormFactor);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->DeviceLocator == "" ? "N/A" : pData17->DeviceLocator);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->Speed == "" ? "N/A" : pData17->Speed);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->Manufacturer == "" ? "N/A" : pData17->Manufacturer);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->SerialNumber == "" ? "N/A" : pData17->SerialNumber);
//    qList.append(pDataItem);
//    pDataItem = new QStandardItem(pData17->ConfiguredMemoryClockSpeed == "" ? "N/A" : pData17->ConfiguredMemoryClockSpeed);
//    qList.append(pDataItem);

//    pDataRoot->appendColumn(qList);
}

int CSMBiosEntryPoint::OnCreateWidget(QWidget **ppWidget)
{
    QWidget *pWidget = new QWidget();
    pWidget->setLayout(new QVBoxLayout());
    pWidget->layout()->setContentsMargins(0, 0, 0, 0);
    pWidget->layout()->setSpacing(2);

    this->m_pDescTree = new QTreeView();
    this->m_pDescTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->m_pDescTree->header()->setStretchLastSection(true);
    this->m_pDescTree->setFixedHeight(120);

    this->m_pDataTree = new QTreeView();
    this->m_pDataTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->m_pDataTree->header()->setStretchLastSection(true);

    this->m_pDataTreeModel = new QStandardItemModel();
    this->m_pDataTreeModel->setHorizontalHeaderItem(0, new QStandardItem("Property"));
    this->m_pDataTreeModel->setHorizontalHeaderItem(1, new QStandardItem("Value"));

    this->m_pDescTreeModel = new QStandardItemModel();
    this->m_pDescTreeModel->setHorizontalHeaderItem(0, new QStandardItem("DMI information"));

    // fill descmodel
    QStandardItem *pRootItem = this->m_pDescTreeModel->invisibleRootItem();
    QStandardItem *pStandardItem = new QStandardItem(QString("BIOS"));
    pRootItem->appendRow(pStandardItem);
    pStandardItem = new QStandardItem(QString("Base board"));
    pRootItem->appendRow(pStandardItem);
    pStandardItem = new QStandardItem(QString("CPU"));
    pRootItem->appendRow(pStandardItem);
    pStandardItem = new QStandardItem(QString("Memory"));
    pRootItem->appendRow(pStandardItem);

    this->m_pDescTree->setModel(this->m_pDescTreeModel);

    connect(m_pDescTree, SIGNAL(clicked(QModelIndex)),
            this, SLOT(DescriptionTreeItemClicked(QModelIndex)),
            Qt::QueuedConnection);

    this->PopulateModels();

    pWidget->layout()->addWidget(m_pDescTree);
    pWidget->layout()->addWidget(m_pDataTree);

    (*ppWidget) = pWidget;

    return Success;
}

int CSMBiosEntryPoint::ParseData()
{
    ISMBiosGenericStructure *pItem = NULL;
    QList<unsigned char *> pTable;

    pTable = ReturnTableOfTypeAndLength(BIOS_INFORMATION_TYPE, 0x18);
    for(int k = 0; k < pTable.count(); k++)
    {
        pItem = new CType0BiosInformation;
        if( 0 == pItem )
        {
            DEBUG_STATUS(NotAllocated);
            return NotAllocated;
        }
        pItem->AddInformation(pTable.at(k));
        qSMBiosData.append(pItem);
        this->bBiosFound = true;
    }
    pTable.clear();

    pTable = ReturnTableOfTypeAndLength(BASEBOARD_INFORMATION_TYPE, 0x10);
    for(int k = 0; k < pTable.count(); k++)
    {
        pItem = new CType2BaseBoardInformation;
        if( 0 == pItem )
        {
            DEBUG_STATUS(NotAllocated);
            return NotAllocated;
        }
        pItem->AddInformation(pTable.at(k));
        qSMBiosData.append(pItem);
    }
    pTable.clear();

    pTable = ReturnTableOfTypeAndLength(PROCESSOR_INFORMATION_TYPE, 0x2a);
    for(int k = 0; k < pTable.count(); k++)
    {
        pItem = new CType4ProcessorInformation;
        if( 0 == pItem )
        {
            DEBUG_STATUS(NotAllocated);
            return NotAllocated;
        }
        pItem->AddInformation(pTable.at(k));
        qSMBiosData.append(pItem);
    }
    pTable.clear();

    // memory device information
    switch(m_pSmbiosVersion->Minor[0].toLatin1())
    {
    case '1':
        pTable = ReturnTableOfTypeAndLength(MEMORY_DEVICE_INFORMATION_TYPE, 0x15);
        break;
    case '3':
        pTable = ReturnTableOfTypeAndLength(MEMORY_DEVICE_INFORMATION_TYPE, 0x1B);
        break;
    case '6':
        pTable = ReturnTableOfTypeAndLength(MEMORY_DEVICE_INFORMATION_TYPE, 0x1C);
        break;
    case '7':
        pTable = ReturnTableOfTypeAndLength(MEMORY_DEVICE_INFORMATION_TYPE, 0x22);
        break;
    case '8':
        pTable = ReturnTableOfTypeAndLength(MEMORY_DEVICE_INFORMATION_TYPE, 0x28);
        break;
    }
    for(int k = 0; k < pTable.count(); k++)
    {
        pItem = new CType17MemoryDeviceInformation;
        if( 0 == pItem )
        {
            DEBUG_STATUS(NotAllocated);
            return NotAllocated;
        }
        pItem->AddInformation(pTable.at(k));
        qSMBiosData.append(pItem);
    }
    pTable.clear();

    return Success;
}
