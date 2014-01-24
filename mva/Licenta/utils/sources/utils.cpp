#include "../headers/utils.h"

///
/// \brief Convert wchar_t array parameter into QString
/// \param pwszParam - wchar_t array
/// \return returnValue - QString or empty QString
///
QString WcharArrayToQString(wchar_t* pwszParam)
{
    QString returnValue = "";
    if( 0 != pwszParam )
    {
        returnValue = QString::fromWCharArray(pwszParam);
    }
    return returnValue;
}

///
/// \brief Convert wchar_t array to chat array
/// \param pwszParam - wchat_t array
/// \return returnValue - char array or NULL
///
char* WCharArrayToCharArray(wchar_t* pwszParam)
{
    char *returnValue = 0;
    size_t requiredSize = 0;
    ::wcstombs_s(&requiredSize, NULL, 0, pwszParam, 0);
    if( 0 == pwszParam)
    {
        return 0;
    }

    returnValue = (char*) malloc (sizeof(char) * wcslen(pwszParam));
    if( 0 == returnValue )
    {
        return 0;
    }
    memset(returnValue, '\0', sizeof(returnValue));
    wcstombs_s(&requiredSize, returnValue, requiredSize + 1, pwszParam, requiredSize);

    return returnValue;
}

///
/// \brief Convert char array to wchar_t array
/// \param pszParam - char array
/// \return returnValue - wchar_t array NULL
///
wchar_t* CharArrayToWcharArray(char* pszParam)
{
    wchar_t *returnValue = 0;
    size_t requiredSize = 0;
    ::mbstowcs_s(&requiredSize, NULL, 0, pszParam, 0);
    if( 0 == pszParam)
    {
        return 0;
    }

    returnValue = (wchar_t*) malloc (sizeof(wchar_t) * strlen(pszParam));
    if( 0 == returnValue )
    {
        return 0;
    }
    memset(returnValue, '\0', sizeof(returnValue));
    mbstowcs_s(&requiredSize, returnValue, requiredSize + 1, pszParam, requiredSize);

    return returnValue;
}

///
/// \brief Safe delete any piece of memory
/// \param pParam - void*
/// \return Success
///
int SafeDelete(void *pParam)
{
    if( 0 != pParam )
    {
        delete pParam;
        pParam = 0;
    }

    return Success;
}

///
/// \brief Convert byte array to QString
/// \param pbParam - byte array
/// \param dwSize - size of array
/// \return returnValue - QString
///
QString ByteToQString(LPBYTE pbParam, DWORD dwSize)
{
    QString returnValue = "";
    for( unsigned int i = 0; i < dwSize; i+=2)
    {
        returnValue.append(pbParam[i]);
    }

    return returnValue;
}


bool WIN32_FROM_HRESULT(HRESULT hr, DWORD *pdwWin32)
{
    if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0))
    {
        *pdwWin32 = HRESULT_CODE(hr);
        return TRUE;
    }

    if (hr == S_OK)
    {
        *pdwWin32 = HRESULT_CODE(hr);
        return TRUE;
    }

    return FALSE;
}
