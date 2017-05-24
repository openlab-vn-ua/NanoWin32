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

  int resultlen = strlen(result);

  if ((nSize > 0) && (lpBuffer != NULL))
  {
    strcpy_s(lpBuffer, nSize, result);
  }

  return(resultlen);
}

DWORD GetEnvironmentVariableW
              (
                LPCWSTR lpName,
                LPWSTR  lpBuffer,
                DWORD   nSize
              )
{
  // Note: will return number of char, not wchar_t required to store value (this seems not a big problem since the value returned is bigger wchar_t count then need)

  if ((nSize > 0) && (lpBuffer != NULL))
  {
    lpBuffer[0] = '\0';
  }

  DWORD result;
  WStrToStrClone     sName(lpName);
  WStrByStrResultBag sBuffer(nSize, lpBuffer);
  result = GetEnvironmentVariableA(sName.c_str(), sBuffer.bag(), sBuffer.bagSize()); // TODO: Adjust result length
  sBuffer.copyBag(lpBuffer, nSize); // we allow truncate here
  return(result);
}

NW_EXTERN_C_END
