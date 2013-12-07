#ifndef IDENTIFY_H
#define IDENTIFY_H

// WinAPI includes

// QT includes

// My includes
#include "../../../../utils/headers/utils.h"
#include "smart_structs.h"
#include "smart_defines.h"

#define OUT_IDENTIFY_SIZE 264

class CIdentifyData
{
private:
    IdentifyDeviceData *m_pData;

    void ParseData(WORD *pData);
    QString ConvertToASCII(WORD *pData, unsigned int nLength);

public:
    CIdentifyData();
    ~CIdentifyData();

    int Initialize(const wchar_t *wszDriveName);
};

#endif
