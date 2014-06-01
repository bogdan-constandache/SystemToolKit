#include "memory_data.h"

CRAMInformation::CRAMInformation()
{

}

CRAMInformation::~CRAMInformation()
{
    ClearRamDataList();
}

QStandardItemModel *CRAMInformation::GetRAMInformations()
{
    int nStatus = ClearRamDataList();
    CHECK_OPERATION_STATUS(nStatus);
    if( Success != nStatus )
        return 0;

    nStatus = InitializeInformation();
    CHECK_OPERATION_STATUS(nStatus);
    if( Success != nStatus )
        return 0;

    QStandardItemModel  *pModel = new QStandardItemModel;
    RamData             *pRamData = 0;

    pModel->setHorizontalHeaderLabels(QStringList() << "Field" << "Value");

    for(int i = 0; i < m_qRamData.count(); i++)
    {
        pRamData = m_qRamData.at(i);

        pModel->setItem(i * 11 + 0, 0, new QStandardItem("Bank label:"));
        pModel->setItem(i * 11 + 0, 1, new QStandardItem(pRamData->qzBankLabel == "" ? "N/A" : pRamData->qzBankLabel));

        pModel->setItem(i * 11 + 1, 0, new QStandardItem("Capacity:"));
        pModel->setItem(i * 11 + 1, 1, new QStandardItem(pRamData->qzCapacity == "" ? "N/A" : pRamData->qzCapacity));

        pModel->setItem(i * 11 + 2, 0, new QStandardItem("Data width:"));
        pModel->setItem(i * 11 + 2, 1, new QStandardItem(pRamData->qzDataWidth == "" ? "N/A" : pRamData->qzDataWidth));

        pModel->setItem(i * 11 + 3, 0, new QStandardItem("Description:"));
        pModel->setItem(i * 11 + 3, 1, new QStandardItem(pRamData->qzDescription == "" ? "N/A" : pRamData->qzDescription));

        pModel->setItem(i * 11 + 4, 0, new QStandardItem("Form factor:"));
        pModel->setItem(i * 11 + 4, 1, new QStandardItem(pRamData->qzFormFactor == "" ? "N/A" : pRamData->qzFormFactor));

        pModel->setItem(i * 11 + 5, 0, new QStandardItem("Manufacturer:"));
        pModel->setItem(i * 11 + 5, 1, new QStandardItem(pRamData->qzManufacturer == "" ? "N/A" : pRamData->qzManufacturer));

        pModel->setItem(i * 11 + 6, 0, new QStandardItem("Memory type:"));
        pModel->setItem(i * 11 + 6, 1, new QStandardItem(pRamData->qzMemoryType == "" ? "N/A" : pRamData->qzMemoryType));

        pModel->setItem(i * 11 + 7, 0, new QStandardItem("Model:"));
        pModel->setItem(i * 11 + 7, 1, new QStandardItem(pRamData->qzModel == "" ? "N/A" : pRamData->qzModel));

        pModel->setItem(i * 11 + 8, 0, new QStandardItem("Name:"));
        pModel->setItem(i * 11 + 8, 1, new QStandardItem(pRamData->qzName == "" ? "N/A" : pRamData->qzName));

        pModel->setItem(i * 11 + 9, 0, new QStandardItem("Speed:"));
        pModel->setItem(i * 11 + 9, 1, new QStandardItem(pRamData->qzSpeed == "" ? "N/A" : pRamData->qzSpeed));
    }

    return pModel;
}

int CRAMInformation::ClearRamDataList()
{
    foreach(RamData *pRam, m_qRamData)
        SAFE_DELETE(pRam);

    m_qRamData.clear();

    return Success;
}

int CRAMInformation::InitializeInformation()
{
    int nStatus = ClearRamDataList();
    CHECK_OPERATION_STATUS(nStatus);

    IWbemLocator            *pLocator = 0;
    IWbemServices           *pService = 0;
    IEnumWbemClassObject    *pEnumerator = 0;
    IWbemClassObject        *pClassObject = 0;
    ULONG                   ulReturn = 0;
    HRESULT                 hRes;

    RamData                 *pRamData = 0;

    VARIANT                 vtProp;

    hRes = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                            IID_IWbemLocator, (LPVOID*) &pLocator);
    if( FAILED(hRes) )
        return Unsuccessful;

    hRes = pLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0,
                                   NULL, 0, 0, &pService);
    if( FAILED(hRes) )
    {
        pLocator->Release();
        return Unsuccessful;
    }

    hRes = CoSetProxyBlanket(pService, RPC_C_AUTHN_WINNT,
                             RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,
                             RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if( FAILED(hRes) )
    {
        pService->Release();
        pLocator->Release();
        return Unsuccessful;
    }

    hRes = pService->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_PhysicalMemory"),
                               WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                               NULL, &pEnumerator);
    if( FAILED(hRes) )
    {
        pService->Release();
        pLocator->Release();
        return Unsuccessful;
    }

    while( pEnumerator )
    {
        hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &ulReturn);
        if( 0 == ulReturn )
            break;

        pRamData = new RamData;
        CHECK_ALLOCATION_STATUS(pRamData);

        VariantInit(&vtProp);
        hRes = pClassObject->Get(L"BankLabel", 0, &vtProp, 0, 0);
        if( VT_BSTR == vtProp.vt )
            pRamData->qzBankLabel = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"Capacity", 0, &vtProp, 0, 0);
        if( VT_BSTR == vtProp.vt ) // when query for UINT64 -> return STRING (MSDN REF).
        {
            UINT64 uiVal = _atoi64(_com_util::ConvertBSTRToString(vtProp.bstrVal));
            pRamData->qzCapacity.sprintf("%d GB", (uiVal / (1024*1024*1024)));
        }

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"DataWidth", 0, &vtProp, 0, 0);
        if( VT_I4 == vtProp.vt)
            pRamData->qzDataWidth.sprintf("%d bits", vtProp.iVal);

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"Description", 0, &vtProp, 0, 0);
        if( VT_BSTR == vtProp.vt)
            pRamData->qzDescription = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"FormFactor", 0, &vtProp, 0, 0);
        if( VT_I4 == vtProp.vt)
        {
            switch(vtProp.intVal)
            {
            case 0:
                pRamData->qzFormFactor = "Unknown"; break;
            case 1:
                pRamData->qzFormFactor = "Other"; break;
            case 2:
                pRamData->qzFormFactor = "SIP"; break;
            case 3:
                pRamData->qzFormFactor = "DIP"; break;
            case 4:
                pRamData->qzFormFactor = "ZIP"; break;
            case 5:
                pRamData->qzFormFactor = "SOJ"; break;
            case 6:
                pRamData->qzFormFactor = "Proprietary"; break;
            case 7:
                pRamData->qzFormFactor = "SIMM"; break;
            case 8:
                pRamData->qzFormFactor = "DIMM"; break;
            case 9:
                pRamData->qzFormFactor = "TSOP"; break;
            case 10:
                pRamData->qzFormFactor = "PGA"; break;
            case 11:
                pRamData->qzFormFactor = "RIMM"; break;
            case 12:
                pRamData->qzFormFactor = "SODIMM"; break;
            case 13:
                pRamData->qzFormFactor = "SRIMM"; break;
            case 14:
                pRamData->qzFormFactor = "SMD"; break;
            case 15:
                pRamData->qzFormFactor = "SSMP"; break;
            case 16:
                pRamData->qzFormFactor = "QFP"; break;
            case 17:
                pRamData->qzFormFactor = "TQFP"; break;
            case 18:
                pRamData->qzFormFactor = "SOIC"; break;
            case 19:
                pRamData->qzFormFactor = "LCC"; break;
            case 20:
                pRamData->qzFormFactor = "PLCC"; break;
            case 21:
                pRamData->qzFormFactor = "BGA"; break;
            case 22:
                pRamData->qzFormFactor = "FPBGA"; break;
            case 23:
                pRamData->qzFormFactor = "LGA"; break;
            default:
                pRamData->qzFormFactor = "Unknown";
            }
        }

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        if( VT_BSTR == vtProp.vt)
            pRamData->qzManufacturer = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"Model", 0, &vtProp, 0, 0);
        if( VT_BSTR == vtProp.vt)
            pRamData->qzModel = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"Name", 0, &vtProp, 0, 0);
        if( VT_BSTR == vtProp.vt)
            pRamData->qzName = _com_util::ConvertBSTRToString(vtProp.bstrVal);

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"FormFactor", 0, &vtProp, 0, 0);
        if( VT_I4 == vtProp.vt)
        {
            switch(vtProp.intVal)
            {
            case 0:
                pRamData->qzMemoryType = "Unknown"; break;
            case 1:
                pRamData->qzMemoryType = "Other"; break;
            case 2:
                pRamData->qzMemoryType = "DRAM"; break;
            case 3:
                pRamData->qzMemoryType = "Synchronous DRAM"; break;
            case 4:
                pRamData->qzMemoryType = "Cache DRAM"; break;
            case 5:
                pRamData->qzMemoryType = "EDO"; break;
            case 6:
                pRamData->qzMemoryType = "EDRAM"; break;
            case 7:
                pRamData->qzMemoryType = "VRAM"; break;
            case 8:
                pRamData->qzMemoryType = "SRAM"; break;
            case 9:
                pRamData->qzMemoryType = "RAM"; break;
            case 10:
                pRamData->qzMemoryType = "ROM"; break;
            case 11:
                pRamData->qzMemoryType = "Flash"; break;
            case 12:
                pRamData->qzMemoryType = "EEPROM"; break;
            case 13:
                pRamData->qzMemoryType = "FEPROM"; break;
            case 14:
                pRamData->qzMemoryType = "ERPOM"; break;
            case 15:
                pRamData->qzMemoryType = "CDRAM"; break;
            case 16:
                pRamData->qzMemoryType = "3DRAM"; break;
            case 17:
                pRamData->qzMemoryType = "SDRAM"; break;
            case 18:
                pRamData->qzMemoryType = "SGRAM"; break;
            case 19:
                pRamData->qzMemoryType = "RDRAM"; break;
            case 20:
                pRamData->qzMemoryType = "DDR"; break;
            case 21:
                pRamData->qzMemoryType = "DDR-2"; break;
            default:
                pRamData->qzMemoryType = "Unknown";
            }
        }

        VariantClear(&vtProp);
        hRes = pClassObject->Get(L"Speed", 0, &vtProp, 0, 0);
        if( VT_I4 == vtProp.vt)
            pRamData->qzSpeed.sprintf("%d MHz", vtProp.intVal);

        m_qRamData.append(pRamData);
    }

    pService->Release();
    pLocator->Release();

    return Success;
}


