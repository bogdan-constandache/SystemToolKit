#ifndef TYPE7_CACHE_INFORMATION_H
#define TYPE7_CACHE_INFORMATION_H

// WinAPI includes

// QT includes

// My includes
#include "../interfaces/smbios_generic_structure.h"
#include "smbios_structures.h"

class CType7CacheInformation : public ISMBiosGenericStructure
{
private:
    PT7CacheInformation m_data;

public:
    CType7CacheInformation();
    ~CType7CacheInformation();

    SMStructureType GetStructureType(void);
    int AddInformation(BYTE *pData);
    void *GetStructureData();
};

#endif // TYPE7_CACHE_INFORMATION_H
