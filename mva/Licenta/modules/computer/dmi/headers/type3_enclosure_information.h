#ifndef TYPE3_ENCLOSURE_INFORMATION_H
#define TYPE3_ENCLOSURE_INFORMATION_H

// WinAPI includes

// QT includes

// My includes
#include "../interfaces/smbios_generic_structure.h"
#include "smbios_structures.h"

class CType3EnclosureInformation : public ISMBiosGenericStructure
{
private:
    PT3EnclosureInformation m_data;

public:
    CType3EnclosureInformation();
    ~CType3EnclosureInformation();

    SMStructureType GetStructureType(void);
    int AddInformation(BYTE *pData);
    void *GetStructureData();
};

#endif // TYPE3_ENCLOSURE_INFORMATION_H
