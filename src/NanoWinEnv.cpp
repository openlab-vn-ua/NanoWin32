// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Environment functions

#include "NanoWinEnv.h"

#include <string.h>
#include <stdlib.h>

#include "NanoWinMsSafeString.h"
#include "NanoWinError.h"

#include "NanoWinStrConvert.h"
using namespace NanoWin;

NW_EXTERN_C_BEGIN

DWORD GetEnvironmentVariableA
              (
                LPCSTR lpName,
                LPSTR  lpBuffer,
                DWORD  nSize
              )
{
  if ((nSize > 0) && (lpBuffer != NULL))
  {
    lpBuffer[0] = '\0';
  }

  LPCSTR result = getenv(lpName);
  if (result == NULL)
  {
    NanoWinSetLastError(ERROR_ENVVAR_NOT_FOUND);
    return(0);
  }

  size_t resultlen = strlen(result);

  if ((nSize > 0) && (lpBuffer != NULL))
  {
    strcpy_s(lpBuffer, nSize, result);
  }

  return(static_cast<DWORD>(nSize < resultlen + 1 ? resultlen + 1 : resultlen));
}

DWORD GetEnvironmentVariableW
              (
                LPCWSTR lpName,
                LPWSTR  lpBuffer,
                DWORD   nSize
              )
{
  // Note: will return number of char, not wchar_t required to store value (in case var not fit or target buffer is null)
  // (this seems not a big problem since the value returned is bigger wchar_t count then need)

  if ((nSize > 0) && (lpBuffer != NULL))
  {
    lpBuffer[0] = '\0';
  }

  DWORD resulta;
  WStrToStrClone     sName(lpName);
  WStrByStrResultBag sBuffer(nSize, lpBuffer);
  resulta = GetEnvironmentVariableA(sName.c_str(), sBuffer.bag(), static_cast<DWORD>(sBuffer.bagSize()));
  sBuffer.copyBag(lpBuffer, nSize); // we allow truncate here

  if (resulta == 0)
  {
    return(0); // No var found
  }
  else if (lpBuffer != NULL)
  {
    DWORD resultw = static_cast<DWORD>(wcslen(lpBuffer));
    if ((resultw+1) < nSize)
    {
      return(resultw); // Fit, return real length
    }
    else
    {
      return(resulta); // Return ascii lenght (pessimistic expectation)
    }
  }
  else
  {
    return(resulta); // Return ascii lenght (pessimistic expectation)
  }
}

extern BOOL WINAPI SetEnvironmentVariableA(_In_ const char *lpName, _In_opt_ const char *lpValue)
{
  int result = lpValue != NULL ? setenv(lpName,lpValue,1) : unsetenv(lpName);

  if (result != 0)
  {
    switch (errno)
    {
      case (EINVAL) :
      {
        SetLastError(ERROR_INVALID_PARAMETER);
      } break;

      case (ENOMEM) :
      {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
      } break;

      default :
      {
        SetLastError(NanoWinErrorByErrnoAtFail(errno));
      }
    }

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

extern BOOL WINAPI SetEnvironmentVariableW(_In_ const wchar_t *lpName, _In_opt_ const wchar_t *lpValue)
{
  try
  {
    std::string lpNameMb = NanoWin::StrConverter::Convert(lpName);

    if (lpValue != NULL)
    {
      std::string lpValueMb = NanoWin::StrConverter::Convert(lpValue);

      return SetEnvironmentVariableA(lpNameMb.c_str(),lpValueMb.c_str());
    }
    else
    {
      return SetEnvironmentVariableA(lpNameMb.c_str(),NULL);
    }
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);

    return FALSE;
  }
}

NW_EXTERN_C_END
