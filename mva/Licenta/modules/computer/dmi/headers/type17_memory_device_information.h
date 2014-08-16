#ifndef TYPE17_MEMORY_DEVICE_INFORMATION_H
#define TYPE17_MEMORY_DEVICE_INFORMATION_H

// WinAPI includes

// QT includes

// My includes
#include "../interfaces/smbios_generic_structure.h"
#include "smbios_structures.h"

class CType17MemoryDeviceInformation : public ISMBiosGenericStructure
{
private:
    PT17MemoryDeviceInformation m_data;
public:
    CType17MemoryDeviceInformation();
    ~CType17MemoryDeviceInformation();

    SMStructureType GetStructureType(void);
    int AddInformation(BYTE *pData);
    void *GetStructureData();
};

#endif // TYPE17_MEMORY_DEVICE_INFORMATION_H
