#include "../headers/type0_bios_information.h"

CType0BiosInformation::CType0BiosInformation()
{
    this->m_StructureType = BIOS_INFORMATION_TYPE;
    this->m_data = NULL;
    this->m_bBiosCHSupported = false;
    this->m_data = new T0BiosInformation;
}

CType0BiosInformation::~CType0BiosInformation()
{
    if( 0 == this->m_data )
    {
        delete this->m_data;
        this->m_data = 0;
    }
}

int CType0BiosInformation::AddInformation(BYTE *pData)
{
    PSMHeader pHeader = (PSMHeader)(pData);
    PType0BiosInformation pInformation = (PType0BiosInformation)(pData + sizeof(SMHeader));
    int count = 1;
    char *pTempString = 0;
    unsigned long biosCH = pInformation->qwBiosCharacteristics;

    if( BIOS_INFORMATION_TYPE != pHeader->bType )
    {
        DEBUG_STATUS(Unsuccessful);
        return Unsuccessful;
    }

    // getting bios version
    if( 255 != pInformation->bSystemBiosMajorRelease )
        this->m_data->SystemBiosVersion.sprintf("%d.%d", pInformation->bSystemBiosMajorRelease, pInformation->bSystemBiosMinorRelease);
    else
        this->m_data->SystemBiosVersion.sprintf("N/A");

    // getting bios rom size
    UINT64 RomSize = (65536 * (pInformation->bBiosROMSize + 1)) / 1024;
    m_data->BiosROMSize.sprintf("%llu KB", RomSize);

    // parse unformatted data
    pTempString = (char*)(pData + pHeader->bLength);
    do{
        if( 0x00 == pTempString[0] )
            break;
        if( count == pInformation->bBiosReleaseDate )
            m_data->ReleaseDate = QString(pTempString);
        if( count == pInformation->bVendor )
            m_data->Vendor = QString(pTempString);
        if( count == pInformation->bBiosVersion )
            m_data->BiosVersion = QString(pTempString);
        count++;
        pTempString = (char*)(pTempString + strlen(pTempString) + 1);
    }while(pTempString[0] != 0);

    if( biosCH & BC_NOT_SUPPORTED )
        m_bBiosCHSupported = false;
    else
    {
        m_bBiosCHSupported = true;
        memset(&this->m_data->BiosChar, 0, sizeof(BiosCharacteristics));
        if( biosCH & ISA_SUPPORTED )
            m_data->BiosChar.ISASupported = 1;
        if( biosCH & MCA_SUPPORTED )
            m_data->BiosChar.MCASupported = 1;
        if( biosCH & EISA_SUPPORTED )
            m_data->BiosChar.EISASupported = 1;
        if( biosCH & PCI_SUPPORTED )
            m_data->BiosChar.PCISupported = 1;
        if( biosCH & PCMCIA_SUPPORTED )
            m_data->BiosChar.PCMCIASupported = 1;
        if( biosCH & PnP_SUPPORTED )
            m_data->BiosChar.PlugAndPlaySupported = 1;
        if( biosCH & APM_SUPPORTED )
            m_data->BiosChar.APMSupported = 1;
        if( biosCH & IS_UPGRADEABLE )
            m_data->BiosChar.BiosUpgradable = 1;
        if( biosCH & SHADOWING_SUPPORTED )
            m_data->BiosChar.BiosShadowingAllowed = 1;
        if( biosCH & VL_VESA_SUPPORTED )
            m_data->BiosChar.VL_VESASupported = 1;
        if( biosCH & ESCD_SUPPORTED )
            m_data->BiosChar.ESCDSupported = 1;
        if( biosCH & BOOT_FROM_CD_SUPPORTED )
            m_data->BiosChar.BootFromCDSupported = 1;
        if( biosCH & SELECTABLE_BOOT_SUPPORTED )
            m_data->BiosChar.SelectableBootSupported = 1;
        if( biosCH & BIOS_ROM_SOCKETED )
            m_data->BiosChar.BiosRomIsSocketed = 1;
        if( biosCH & BOOT_FROM_PCMCIA_SUPPORTED )
            m_data->BiosChar.BootFromPCMCIASupported = 1;
        if( biosCH & EDD_SUPPORTED )
            m_data->BiosChar.EDDSpecSupported = 1;
        if( biosCH & INT_13H_JAP_FLOPPY_NEC_SUPPORTED )
            m_data->BiosChar.JapanFloppyNEC9800Supported = 1;
        if( biosCH & INT_13H_JAP_FLOPPY_TOSHIBA_SUPPORTED )
            m_data->BiosChar.JapanFloppyToshibaSupported = 1;
        if( biosCH & INT_13H_525_360KB_FLOPPY_SUPPORTED )
            m_data->BiosChar.Floppy5_25_360Supported = 1;
        if( biosCH & INT_13H_525_12MB_FLOPPY_SUPPORTED )
            m_data->BiosChar.Floppy5_25_1_2Supported = 1;
        if( biosCH & INT_13H_35_720KB_FLOPPY_SUPPORTED )
            m_data->BiosChar.Floppy3_5_720Supported = 1;
        if( biosCH & INT_13H_35_288MB_FLOPPY_SUPPORTED )
            m_data->BiosChar.Floppy3_5_2_88Supported = 1;
        if( biosCH & INT_5H_PRINT_SCREEN_SUPPORTED )
            m_data->BiosChar.PrintScreenServiceSupported = 1;
        if( biosCH & INT_9H_842_KEYBD_SUPPORTED )
            m_data->BiosChar.Keyboard8042ServiceSupported = 1;
        if( biosCH & INT_14H_SERIAL_SUPPORTED )
            m_data->BiosChar.SerialServiceSupported = 1;
        if( biosCH & INT_17H_PRINTER_SUPPORTED )
            m_data->BiosChar.PrinterServiceSupported = 1;
        if( biosCH & INT_10H_CGA_MONO_VIDEO_SUPPORTED )
            m_data->BiosChar.CGAVideoServiceSupported = 1;
    }
    return Success;
}

void *CType0BiosInformation::GetStructureData()
{
    return (void*)this->m_data;
}


SMStructureType CType0BiosInformation::GetStructureType()
{
    return this->m_StructureType;
}
