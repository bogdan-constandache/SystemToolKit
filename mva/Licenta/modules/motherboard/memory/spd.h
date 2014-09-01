#ifndef _SPD_H
#define _SPD_H

#include <Windows.h>
#include <heapapi.h>

#include <QStandardItemModel>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "../../computer/sensors/headers/stk_driver_wrapper.h"
#include "../../../utils/headers/utils.h"

#include "spd_defines.h"

class CSPDInformation
{
private:
    QStandardItemModel          *m_pDimmsModel;
    QStandardItemModel          *m_pDimmInformationModel;

    CSTKDriverWrapper           *m_pDriver;

    DWORD                       m_dwDataAddr, m_dwBaseAddr, m_dwPortVal;

    QMap<int, int>              m_qDimmsType;
    QMap<int, SpdInformation*>  m_qDimmsInformation;
    QList<ManufacturerDetails*> m_qManufacturerDetails;

    BYTE                        *m_pDIMM0SPD;
    BYTE                        *m_pDIMM1SPD;
    BYTE                        *m_pDIMM2SPD;
    BYTE                        *m_pDIMM3SPD;

    BOOL                        m_bIsDIMM0Present;
    BOOL                        m_bIsDIMM1Present;
    BOOL                        m_bIsDIMM2Present;
    BOOL                        m_bIsDIMM3Present;

    BYTE                        m_ppASCIIDecodingMatrix[7][16];

public:
    CSPDInformation();
    ~CSPDInformation();

    QStandardItemModel *GetDimmsModel();
    QStandardItemModel *GetDimmsInformationModel();

    void OnRefreshData(int nDimm);

private:
    BYTE ReadSPDByte(DWORD dwBase, BYTE bOffset, BYTE bDevID);
    BOOL IsWaitReady(DWORD dwBase);
    void Delay(__int64 uiSeconds);

    int ReadDDR3Informations(int nDIMM);
    int InterpretDDR3SPDArray(BYTE *pbArray, int nDimm);

    int CollectSPDInformations();
    BOOL CheckIfDIMMIsAvailable(int nDIMM);
    int CollectDIMMSType();
    int InitializeASCIIDecodingMatrix();
    CHAR DecodeASCIICharacter(BYTE bLow, BYTE bHigh);

    int ReadManufacturersDataFromDB();
    ManufacturerDetails *GetManufacturerDetails(int nContinuation, int nManufacturerID);
};

#endif
