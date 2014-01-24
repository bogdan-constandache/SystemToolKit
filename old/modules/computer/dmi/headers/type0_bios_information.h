#ifndef TYPE0_BIOS_INFORMATION
#define TYPE0_BIOS_INFORMATION

// WinAPI includes

// QT includes

// My includes
#include "../interfaces/smbios_generic_structure.h"
#include "smbios_structures.h"

class CType0BiosInformation : public ISMBiosGenericStructure
{
private:
    PT0BiosInformation m_data;
    bool m_bBiosCHSupported;
public:
    CType0BiosInformation();
    ~CType0BiosInformation();
    SMStructureType GetStructureType(void);
    int AddInformation(BYTE *pData);
    void *GetStructureData();
};

#endif
