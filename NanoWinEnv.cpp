// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Environment functions

#include "NanoWinEnv.h"

#include <string.h>
#include <stdlib.h>

#include "NanoMsSafeString.h"

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
    return(0);
  }

  int resultlen = strlen(result);

  strcpy_s(lpBuffer, nSize, result);

  return(resultlen);
}

NW_EXTERN_C_END
