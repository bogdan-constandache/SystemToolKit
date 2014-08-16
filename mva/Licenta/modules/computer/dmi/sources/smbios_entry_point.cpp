#include "../headers/smbios_entry_point.h"

CSMBiosEntryPoint::CSMBiosEntryPoint() : bBiosFound(false), m_pSmbiosTableData(0)
{
    this->m_pSmbiosVersion = new SmbiosVersion;

    m_pDataModel = new QStandardItemModel();
    m_pStructsModel = new QStandardItemModel();

    m_pStructsModel->setHorizontalHeaderLabels(QStringList() << "DMI modules");

    m_pStructsModel->appendRow(new QStandardItem("DMI - Access level"));
    m_pStructsModel->appendRow(new QStandardItem("BIOS"));
    m_pStructsModel->item(1, 0)->setData(DMI_BIOS);
    m_pStructsModel->appendRow(new QStandardItem("Baseboard"));
    m_pStructsModel->item(2, 0)->setData(DMI_BASEBOARD);
    m_pStructsModel->appendRow(new QStandardItem("Chassis"));
    m_pStructsModel->item(3, 0)->setData(DMI_ENCLOSURE);
    m_pStructsModel->appendRow(new QStandardItem("CPU"));
    m_pStructsModel->item(4, 0)->setData(DMI_CPU);
    m_pStructsModel->appendRow(new QStandardItem("Memory devices"));
    m_pStructsModel->item(5, 0)->setData(DMI_MEMORY_DEVICE);
}

CSMBiosEntryPoint::~CSMBiosEntryPoint()
{
    SAFE_DELETE(m_pStructsModel);
    SAFE_DELETE(m_pDataModel);
    SAFE_DELETE(m_pSmbiosVersion);
    m_pSmbiosTableData = 0;
}

QStandardItemModel *CSMBiosEntryPoint::GetItemsModel()
{
    // Get SMBios data
    InitializeData();

    return m_pStructsModel;
}

QStandardItemModel *CSMBiosEntryPoint::GetItemPropertiesModel()
{
    return m_pDataModel;
}

void CSMBiosEntryPoint::OnRefreshData(DMIModuleType ItemType)
{
    QList<ISMBiosGenericStructure *> qStructureList;
    QList<ISMBiosGenericStructure *>::const_iterator it;
    PT4ProcessorInformation pData = 0;
    PT0BiosInformation pData0 = 0;
    PT3EnclosureInformation pData3 = 0;
    PT2BaseBoardInformation pData2 = 0;
    PT17MemoryDeviceInformation pData17 = 0;

    m_pDataModel->clear();
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    QList<QStandardItem*> qList;

    switch (ItemType)
    {
    case DMI_BIOS:
        qStructureList = GetStructure(BIOS_INFORMATION_TYPE);
        for(it = qStructureList.begin(); it != qStructureList.end(); it++)
        {
            pData0 = (PT0BiosInformation)(*it)->GetStructureData();

            qList << new QStandardItem(QString("Vendor:")) << new QStandardItem(pData0->Vendor == "" ? "N/A" : pData0->Vendor);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Version:")) << new QStandardItem(pData0->BiosVersion == "" ? "N/A" : pData0->BiosVersion);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("System BIOS version:")) << new QStandardItem(pData0->SystemBiosVersion == "" ? "N/A" : pData0->SystemBiosVersion);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Release date:")) << new QStandardItem(pData0->ReleaseDate == "" ? "N/A" : pData0->ReleaseDate);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("ROM memory size:")) << new QStandardItem(pData0->BiosROMSize == "" ? "N/A" : pData0->BiosROMSize);
            m_pDataModel->appendRow(qList);
            qList.clear();

            m_pDataModel->appendRow(qList);
        }

        m_pDataModel->removeRow(m_pDataModel->rowCount() - 1);
        break;

    case DMI_BASEBOARD:
        qStructureList = GetStructure(BASEBOARD_INFORMATION_TYPE);
        for(it = qStructureList.begin(); it != qStructureList.end(); it++)
        {
            pData2 = (PT2BaseBoardInformation)(*it)->GetStructureData();

            qList << new QStandardItem(QString("Manufacturer:")) << new QStandardItem(pData2->Manufacturer == "" ? "N/A" : pData2->Manufacturer);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Product:")) << new QStandardItem(pData2->Product == "" ? "N/A" : pData2->Product);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Version:")) << new QStandardItem(pData2->Version == "" ? "N/A" : pData2->Version);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Serial number:")) << new QStandardItem(pData2->SerialNumber == "" ? "N/A" : pData2->SerialNumber);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Location:")) << new QStandardItem(pData2->Location == "" ? "N/A" : pData2->Location);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Hot swappable:")) << new QStandardItem(pData2->HotSwappable == "" ? "N/A" : pData2->HotSwappable);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Replaceable:")) << new QStandardItem(pData2->Replaceable == "" ? "N/A" : pData2->Replaceable);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Removable:")) << new QStandardItem(pData2->Removable == "" ? "N/A" : pData2->Removable);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("HostingBoard:")) << new QStandardItem(pData2->HostingBoard == "" ? "N/A" : pData2->HostingBoard);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Type:")) << new QStandardItem(pData2->Type == "" ? "N/A" : pData2->Type);
            m_pDataModel->appendRow(qList);
            qList.clear();

            m_pDataModel->appendRow(qList);
        }

        m_pDataModel->removeRow(m_pDataModel->rowCount() - 1);
        break;

    case DMI_ENCLOSURE:
        qStructureList = GetStructure(ENCLOSURE_INFORMATION_TYPE);
        for(it = qStructureList.begin(); it != qStructureList.end(); it++)
        {
            pData3 = (PT3EnclosureInformation)(*it)->GetStructureData();

            qList << new QStandardItem(QString("Manufacturer:")) << new QStandardItem(pData3->Manufacturer == "" ? "N/A" : pData3->Manufacturer);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Type:")) << new QStandardItem(pData3->Type == "" ? "N/A" : pData3->Type);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Version:")) << new QStandardItem(pData3->Version == "" ? "N/A" : pData3->Version);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Serial number:")) << new QStandardItem(pData3->SerialNumber == "" ? "N/A" : pData3->SerialNumber);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Boot-up state:")) << new QStandardItem(pData3->BootUpState == "" ? "N/A" : pData3->BootUpState);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Power-supply state:")) << new QStandardItem(pData3->PowerSupplyState == "" ? "N/A" : pData3->PowerSupplyState);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Thermal state:")) << new QStandardItem(pData3->ThermalState == "" ? "N/A" : pData3->ThermalState);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Security status:")) << new QStandardItem(pData3->SecurityStatus == "" ? "N/A" : pData3->SecurityStatus);
            m_pDataModel->appendRow(qList);
            qList.clear();

            m_pDataModel->appendRow(qList);
        }

        m_pDataModel->removeRow(m_pDataModel->rowCount() - 1);
        break;

    case DMI_CPU:
        qStructureList = GetStructure(PROCESSOR_INFORMATION_TYPE);
        for(it = qStructureList.begin(); it != qStructureList.end(); it++)
        {
            pData = (PT4ProcessorInformation)(*it)->GetStructureData();

            qList << new QStandardItem(QString("Type:")) << new QStandardItem(pData->ProcessorType == "" ? "N/A" : pData->ProcessorType);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Version:")) << new QStandardItem(pData->Version == "" ? "N/A" : pData->Version);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Family:")) << new QStandardItem(pData->ProcessorFamily == "" ? "N/A" : pData->ProcessorFamily);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Manufacturer:")) << new QStandardItem(pData->ProcessorManufacturer == "" ? "N/A" : pData->ProcessorManufacturer);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Voltage:")) << new QStandardItem(pData->Voltage == "" ? "N/A" : pData->Voltage);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("External clock:")) << new QStandardItem(pData->ExtClock == "" ? "N/A" : pData->ExtClock);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Serial number:")) << new QStandardItem(pData->SerialNumber == "" ? "N/A" : pData->SerialNumber);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Max speed:")) << new QStandardItem(pData->MaxSpeed == "" ? "N/A" : pData->MaxSpeed);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Current speed:")) << new QStandardItem(pData->CurrentSpeed == "" ? "N/A" : pData->CurrentSpeed);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Core count:")) << new QStandardItem(pData->CoreCount == "" ? "N/A" : pData->CoreCount);
            m_pDataModel->appendRow(qList);
            qList.clear();

            m_pDataModel->appendRow(qList);
        }

        m_pDataModel->removeRow(m_pDataModel->rowCount() - 1);
        break;

    case DMI_MEMORY_DEVICE:
        qStructureList = GetStructure(MEMORY_DEVICE_INFORMATION_TYPE);
        for(it = qStructureList.begin(); it != qStructureList.end(); it++)
        {
            pData17 = (PT17MemoryDeviceInformation)(*it)->GetStructureData();

            qList << new QStandardItem(QString("Form factor:")) << new QStandardItem(pData17->FormFactor == "" ? "N/A" : pData17->FormFactor);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Type")) << new QStandardItem(pData17->Type == "" ? "N/A" : pData17->Type);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Type details")) << new QStandardItem(pData17->TypeDetails == "" ? "N/A" : pData17->TypeDetails);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Size:")) << new QStandardItem(pData17->Size == "" ? "N/A" : pData17->Size);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Speed:")) << new QStandardItem(pData17->CurrentClockSpeed == "" ? "N/A" : pData17->CurrentClockSpeed);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Total width:")) << new QStandardItem(pData17->TotalWidth == "" ? "N/A" : pData17->TotalWidth);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Data width:")) << new QStandardItem(pData17->DataWidth == "" ? "N/A" : pData17->DataWidth);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Device locator:")) << new QStandardItem(pData17->DeviceLocator == "" ? "N/A" : pData17->DeviceLocator);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Bank locator:")) << new QStandardItem(pData17->BankLocator == "" ? "N/A" : pData17->BankLocator);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Manufacturer:")) << new QStandardItem(pData17->Manufacturer == "" ? "N/A" : pData17->Manufacturer);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Serial number:")) << new QStandardItem(pData17->SerialNumber == "" ? "N/A" : pData17->SerialNumber);
            m_pDataModel->appendRow(qList);
            qList.clear();

            qList << new QStandardItem(QString("Part number:")) << new QStandardItem(pData17->PartNumber == "" ? "N/A" : pData17->PartNumber);
            m_pDataModel->appendRow(qList);
            qList.clear();

            m_pDataModel->appendRow(qList);
        }

        m_pDataModel->removeRow(m_pDataModel->rowCount() - 1);
        break;
    }
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

    hResult = CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator, (LPVOID *) &pLocator);

    if( 0 > hResult )
    {
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

    return ERROR_SUCCESS;
}

QList<ISMBiosGenericStructure *> CSMBiosEntryPoint::GetStructure(SMStructureType type)
{
    QList<ISMBiosGenericStructure *> qList;
    for(int i = 0; i < qSMBiosData.count(); i++)
    {
        if( type == qSMBiosData.at(i)->GetStructureType() )
        {
            qList.append(qSMBiosData.at(i));
        }
    }
    return qList;
}

int CSMBiosEntryPoint::ParseData()
{
    ISMBiosGenericStructure *pItem = NULL;
    QList<unsigned char *> pTable;

    // bios information
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

    // baseboard information
    pTable = ReturnTableOfTypeAndLength(BASEBOARD_INFORMATION_TYPE, 0x0f);
    if( 0 == pTable.count() )
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

    // enclosure information
    if( '0' == m_pSmbiosVersion->Minor[0].toLatin1() )
        pTable = ReturnTableOfTypeAndLength(ENCLOSURE_INFORMATION_TYPE, 0x9);
    else if( '1' == m_pSmbiosVersion->Minor[0].toLatin1() )
        pTable = ReturnTableOfTypeAndLength(ENCLOSURE_INFORMATION_TYPE, 0xD);
    else
        pTable = ReturnTableOfTypeAndLength(ENCLOSURE_INFORMATION_TYPE, 0x16);
    for(int k = 0; k < pTable.count(); k++)
    {
        pItem = new CType3EnclosureInformation;
        if( 0 == pItem )
        {
            DEBUG_STATUS(NotAllocated);
            return NotAllocated;
        }
        pItem->AddInformation(pTable.at(k));
        qSMBiosData.append(pItem);
    }
    pTable.clear();

    // processor information
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
