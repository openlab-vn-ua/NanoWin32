// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// ShellEx functions

#include <string.h>

#include "NanoWinShellEx.h"
#include "NanoWinError.h"
#include "NanoWinPlus.h"

NW_EXTERN_C_BEGIN

extern int SHCreateDirectoryExA
           (
            HWND                       hwnd,
            LPCSTR                     pszPath,
            const SECURITY_ATTRIBUTES *psa
           )
{
  if (hwnd != NULL) { return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL)); }
  return(NanoWinCreateDirectoryPathA(pszPath, psa));
}

extern int SHCreateDirectoryExW
           (
            HWND                       hwnd,
            LPCWSTR                    pszPath,
            const SECURITY_ATTRIBUTES *psa
           )
{
  if (hwnd != NULL) { return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL)); }
  return(NanoWinCreateDirectoryPathW(pszPath, psa));
}

NW_EXTERN_C_END
