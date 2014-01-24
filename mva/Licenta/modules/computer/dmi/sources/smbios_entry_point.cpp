#include "../headers/smbios_entry_point.h"

CSMBiosEntryPoint::CSMBiosEntryPoint() : bBiosFound(false), m_pSmbiosTableData(0)
{
    this->m_pSmbiosVersion = new SmbiosVersion;
}

CSMBiosEntryPoint::~CSMBiosEntryPoint()
{
}

QStandardItemModel *CSMBiosEntryPoint::GetItemsModel()
{
    this->InitializeData();

    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pItem = 0;

    pModel->setColumnCount(1);
    pModel->setRowCount(5);
    pModel->setHorizontalHeaderLabels(QStringList() << "");

    pItem = new QStandardItem("DMI - Access level");
    pModel->setItem(0, 0, pItem);
    pItem = new QStandardItem("BIOS");
    pModel->setItem(1, 0, pItem);
    pItem = new QStandardItem("Baseboard");
    pModel->setItem(2, 0, pItem);
    pItem = new QStandardItem("CPU");
    pModel->setItem(3, 0, pItem);
    pItem = new QStandardItem("Memory devices");
    pModel->setItem(4, 0, pItem);

    return pModel;
}

QStandardItemModel *CSMBiosEntryPoint::GetItemPropertiesModel(DMIModuleType ItemType)
{
    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pItem = 0;

    ISMBiosGenericStructure *pStructure = 0;
    PT4ProcessorInformation pData = 0;
    PT0BiosInformation pData0 = 0;
    PT2BaseBoardInformation pData2 = 0;
    PT17MemoryDeviceInformation pData17 = 0;

    pModel->setColumnCount(2);
    pModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    switch (ItemType)
    {
    case DMI_BIOS:
        pStructure = GetStructure(BIOS_INFORMATION_TYPE);
        pData0 = (PT0BiosInformation)pStructure->GetStructureData();

        pModel->setRowCount(5);

        pItem = new QStandardItem(QString("Vendor:"));
        pModel->setItem(0, 0, pItem);
        pItem = new QStandardItem(pData0->Vendor == "" ? "N/A" : pData0->Vendor);
        pModel->setItem(0, 1, pItem);

        pItem = new QStandardItem(QString("Version:"));
        pModel->setItem(1, 0, pItem);
        pItem = new QStandardItem(pData0->BiosVersion == "" ? "N/A" : pData0->BiosVersion);
        pModel->setItem(1, 1, pItem);

        pItem = new QStandardItem(QString("System BIOS version:"));
        pModel->setItem(2, 0, pItem);
        pItem = new QStandardItem(pData0->SystemBiosVersion == "" ? "N/A" : pData0->SystemBiosVersion);
        pModel->setItem(2, 1, pItem);

        pItem = new QStandardItem(QString("Release date:"));
        pModel->setItem(3, 0, pItem);
        pItem = new QStandardItem(pData0->ReleaseDate == "" ? "N/A" : pData0->ReleaseDate);
        pModel->setItem(3, 1, pItem);

        pItem = new QStandardItem(QString("ROM memory size:"));
        pModel->setItem(4, 0, pItem);
        pItem = new QStandardItem(pData0->BiosROMSize == "" ? "N/A" : pData0->BiosROMSize);
        pModel->setItem(4, 1, pItem);
        break;
    case DMI_BASEBOARD:
        pStructure = GetStructure(BASEBOARD_INFORMATION_TYPE);
        pData2 = (PT2BaseBoardInformation)pStructure->GetStructureData();

        pModel->setRowCount(10);

        pItem = new QStandardItem(QString("Manufacturer:"));
        pModel->setItem(0, 0, pItem);
        pItem = new QStandardItem(pData2->Manufacturer == "" ? "N/A" : pData2->Manufacturer);
        pModel->setItem(0, 1, pItem);

        pItem = new QStandardItem(QString("Product:"));
        pModel->setItem(1, 0, pItem);
        pItem = new QStandardItem(pData2->Product == "" ? "N/A" : pData2->Product);
        pModel->setItem(1, 1, pItem);

        pItem = new QStandardItem(QString("Version:"));
        pModel->setItem(2, 0, pItem);
        pItem = new QStandardItem(pData2->Version == "" ? "N/A" : pData2->Version);
        pModel->setItem(2, 1, pItem);

        pItem = new QStandardItem(QString("Serial number:"));
        pModel->setItem(3, 0, pItem);
        pItem = new QStandardItem(pData2->SerialNumber == "" ? "N/A" : pData2->SerialNumber);
        pModel->setItem(3, 1, pItem);

        pItem = new QStandardItem(QString("Location:"));
        pModel->setItem(4, 0, pItem);
        pItem = new QStandardItem(pData2->Location == "" ? "N/A" : pData2->Location);
        pModel->setItem(4, 1, pItem);

        pItem = new QStandardItem(QString("Hot swappable:"));
        pModel->setItem(5, 0, pItem);
        pItem = new QStandardItem(pData2->HotSwappable == "" ? "N/A" : pData2->HotSwappable);
        pModel->setItem(5, 1, pItem);

        pItem = new QStandardItem(QString("Replaceable:"));
        pModel->setItem(6, 0, pItem);
        pItem = new QStandardItem(pData2->Replaceable == "" ? "N/A" : pData2->Replaceable);
        pModel->setItem(6, 1, pItem);

        pItem = new QStandardItem(QString("Removable:"));
        pModel->setItem(7, 0, pItem);
        pItem = new QStandardItem(pData2->Removable == "" ? "N/A" : pData2->Removable);
        pModel->setItem(7, 1, pItem);

        pItem = new QStandardItem(QString("HostingBoard:"));
        pModel->setItem(8, 0, pItem);
        pItem = new QStandardItem(pData2->HostingBoard == "" ? "N/A" : pData2->HostingBoard);
        pModel->setItem(8, 1, pItem);

        pItem = new QStandardItem(QString("Type:"));
        pModel->setItem(9, 0, pItem);
        pItem = new QStandardItem(pData2->Type == "" ? "N/A" : pData2->Type);
        pModel->setItem(9, 1, pItem);
        break;
    case DMI_CPU:
        pStructure = GetStructure(PROCESSOR_INFORMATION_TYPE);
        pData = (PT4ProcessorInformation)pStructure->GetStructureData();

        pModel->setRowCount(8);

        pItem = new QStandardItem(QString("Type:"));
        pModel->setItem(0, 0, pItem);
        pItem = new QStandardItem(pData->ProcessorType == "" ? "N/A" : pData->ProcessorType);
        pModel->setItem(0, 1, pItem);

        pItem = new QStandardItem(QString("Family:"));
        pModel->setItem(1, 0, pItem);
        pItem = new QStandardItem(pData->ProcessorFamily == "" ? "N/A" : pData->ProcessorFamily);
        pModel->setItem(1, 1, pItem);

        pItem = new QStandardItem(QString("Manufacturer:"));
        pModel->setItem(2, 0, pItem);
        pItem = new QStandardItem(pData->ProcessorManufacturer == "" ? "N/A" : pData->ProcessorManufacturer);
        pModel->setItem(2, 1, pItem);

        pItem = new QStandardItem(QString("Voltage:"));
        pModel->setItem(3, 0, pItem);
        pItem = new QStandardItem(pData->Voltage == "" ? "N/A" : pData->Voltage);
        pModel->setItem(3, 1, pItem);

        pItem = new QStandardItem(QString("External clock:"));
        pModel->setItem(4, 0, pItem);
        pItem = new QStandardItem(pData->ExtClock == "" ? "N/A" : pData->ExtClock);
        pModel->setItem(4, 1, pItem);

        pItem = new QStandardItem(QString("Serial number:"));
        pModel->setItem(5, 0, pItem);
        pItem = new QStandardItem(pData->SerialNumber == "" ? "N/A" : pData->SerialNumber);
        pModel->setItem(5, 1, pItem);

        pItem = new QStandardItem(QString("Max Speed:"));
        pModel->setItem(6, 0, pItem);
        pItem = new QStandardItem(pData->MaxSpeed == "" ? "N/A" : pData->MaxSpeed);
        pModel->setItem(6, 1, pItem);

        pItem = new QStandardItem(QString("Core count:"));
        pModel->setItem(7, 0, pItem);
        pItem = new QStandardItem(pData->CoreCount == "" ? "N/A" : pData->CoreCount);
        pModel->setItem(7, 1, pItem);
        break;
    case DMI_MEMORY_DEVICE:

        pStructure = GetStructure(MEMORY_DEVICE_INFORMATION_TYPE);
        pData17 = (PT17MemoryDeviceInformation)pStructure->GetStructureData();

        pModel->setRowCount(9);

        pItem = new QStandardItem(QString("Total width:"));
        pModel->setItem(0, 0, pItem);
        pItem = new QStandardItem(pData17->TotalWidth == "" ? "N/A" : pData17->TotalWidth);
        pModel->setItem(0, 1, pItem);

        pItem = new QStandardItem(QString("Data width:"));
        pModel->setItem(1, 0, pItem);
        pItem = new QStandardItem(pData17->DataWidth == "" ? "N/A" : pData17->DataWidth);
        pModel->setItem(1, 1, pItem);

        pItem = new QStandardItem(QString("Size:"));
        pModel->setItem(2, 0, pItem);
        pItem = new QStandardItem(pData17->Size == "" ? "N/A" : pData17->Size);
        pModel->setItem(2, 1, pItem);

        pItem = new QStandardItem(QString("Form factor:"));
        pModel->setItem(3, 0, pItem);
        pItem = new QStandardItem(pData17->FormFactor == "" ? "N/A" : pData17->FormFactor);
        pModel->setItem(3, 1, pItem);

        pItem = new QStandardItem(QString("Device locator:"));
        pModel->setItem(4, 0, pItem);
        pItem = new QStandardItem(pData17->DeviceLocator == "" ? "N/A" : pData17->DeviceLocator);
        pModel->setItem(4, 1, pItem);

        pItem = new QStandardItem(QString("Speed:"));
        pModel->setItem(5, 0, pItem);
        pItem = new QStandardItem(pData17->Speed == "" ? "N/A" : pData17->Speed);
        pModel->setItem(5, 1, pItem);

        pItem = new QStandardItem(QString("Manufacturer:"));
        pModel->setItem(6, 0, pItem);
        pItem = new QStandardItem(pData17->Manufacturer == "" ? "N/A" : pData17->Manufacturer);
        pModel->setItem(6, 1, pItem);

        pItem = new QStandardItem(QString("Serial number:"));
        pModel->setItem(7, 0, pItem);
        pItem = new QStandardItem(pData17->SerialNumber == "" ? "N/A" : pData17->SerialNumber);
        pModel->setItem(7, 1, pItem);

        pItem = new QStandardItem(QString("Configured memory clock speed:"));
        pModel->setItem(8, 0, pItem);
        pItem = new QStandardItem(pData17->ConfiguredMemoryClockSpeed == "" ? "N/A" : pData17->ConfiguredMemoryClockSpeed);
        pModel->setItem(8, 1, pItem);
        break;
    }

    return pModel;
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
