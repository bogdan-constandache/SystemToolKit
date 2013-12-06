#ifndef SMBIOS_GENERIC_STRUCTURE
#define SMBIOS_GENERIC_STRUCTURE

// WinAPI includes
#include <Windows.h>

// QT includes

// My includes
#include "../headers/smbios_internal_structures.h"
#include "../../../../utils/headers/utils.h"

class ISMBiosGenericStructure
{
protected:
    SMStructureType m_StructureType;
public:
    virtual ~ISMBiosGenericStructure(){}

    virtual SMStructureType GetStructureType(void) = 0;
    virtual int AddInformation(BYTE *pData) = 0;
    virtual void *GetStructureData() = 0;
};

#endif
