#ifndef UTILS_H
#define UTILS_H

//WinAPI includes
#include <Windows.h>
#include <WinBase.h>
#include <wchar.h>

//Qt includes
#include <QString>
#include <QDebug>
#include <QIcon>
#include <QPixmap>
#include <QtWinExtras>
#include <QtWin>
#include <QDir>

//Defines
#define UNUSED(expr) do { (void)(expr); } while (0)

#define SAFE_DELETE(X) if(X) { delete (X); X = 0; }

#ifdef STK_WINDOWS
    #define DEBUG_STATUS(expr) qDebug("\nEXCEPTION ON: \"%s\"(%d)\nFUNCTION: \"%s\"\nStatus: %d\n",__FILE__, __LINE__, __FUNCTION__, expr)
    #define CHECK_ALLOCATION(X) if(!X) { qDebug("Allocation failed in: %s at line %d: ", __FILE__, __LINE__); return; }
    #define CHECK_ALLOCATION_STATUS(X) if(!X) { qDebug("Allocation failed in: %s at line %d: ", __FILE__, __LINE__); return 5; }
    #define CHECK_OPERATION_STATUS(X) if(0 != X) { qDebug("Operation unsuccessful in: %s at line %d with status %d", __FILE__, __LINE__, X); }
    #define CHECK_OPERATION_STATUS_EX(X) if(0 != X) { qDebug("Operation unsuccessful in: %s at line %d with status %d", __FILE__, __LINE__, X); return X; }
#endif

#define AMD_CHIPSET_VENDOR_STRING QString("AuthenticAMD")
#define INTEL_CHIPSET_VENDOR_STRING QString("GenuineIntel")


typedef enum _SysToolError
{
    Uninitialized = -1,
    Success = 0,
    Unsuccessful = 1,
    NullException = 2,
    DllNotFound = 3,
    InvalidParameter = 4,
    NotAllocated = 5,
    InvalidHandle = 6,
    DBOpenError = 7,
    DBExecError = 8
}SysToolError;

QString WcharArrayToQString(wchar_t* pwszParam);
QString ByteToQString(LPBYTE pbParam, DWORD dwSize);
char* WCharArrayToCharArray(wchar_t* pwszParam);
wchar_t* CharArrayToWcharArray(char* pszParam);
int SafeDelete(void *pParam);
bool WIN32_FROM_HRESULT(HRESULT hr, OUT DWORD *pdwWin32);
QIcon GetIconFromHICON(QString qzFileName);
QString GetChipsetType();

#endif // UTILS_H
