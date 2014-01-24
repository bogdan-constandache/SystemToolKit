#ifndef TYPE2_BASEBOARD_INFORMATION_H
#define TYPE2_BASEBOARD_INFORMATION_H

// WinAPI includes

// QT includes

// My includes
#include "../interfaces/smbios_generic_structure.h"
#include "smbios_structures.h"

class CType2BaseBoardInformation : public ISMBiosGenericStructure
{
private:
    PT2BaseBoardInformation m_data;
public:
    CType2BaseBoardInformation();
    ~CType2BaseBoardInformation();
    SMStructureType GetStructureType(void);
    int AddInformation(BYTE *pData);
    void *GetStructureData();
};

#endif // TYPE2_BASEBOARD_INFORMATION_H
