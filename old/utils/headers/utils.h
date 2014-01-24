#ifndef UTILS_H
#define UTILS_H

//WinAPI includes
#include <Windows.h>
#include <WinBase.h>
#include <wchar.h>

//Qt includes
#include <QString>
#include <QDebug>

//Defines
#define UNUSED(expr) do { (void)(expr); } while (0)
#define DEBUG_STATUS(expr) printf("\nEXCEPTION ON: \"%s\"(%d)\nFUNCTION: \"%s\"\nStatus: %d\n",__FILE__, __LINE__, __FUNCTION__, expr)

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


#endif // UTILS_H
