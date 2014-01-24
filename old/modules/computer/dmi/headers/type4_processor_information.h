#ifndef TYPE4_PROCESSOR_INFORMATION
#define TYPE4_PROCESSOR_INFORMATION

// WinAPI includes

// QT includes

// My includes
#include "../interfaces/smbios_generic_structure.h"
#include "smbios_structures.h"

class CType4ProcessorInformation : public ISMBiosGenericStructure
{
private:
    PT4ProcessorInformation m_data;
public:
    CType4ProcessorInformation();
    ~CType4ProcessorInformation();
    SMStructureType GetStructureType(void);
    int AddInformation(BYTE *pData);
    void *GetStructureData();
};

#endif
