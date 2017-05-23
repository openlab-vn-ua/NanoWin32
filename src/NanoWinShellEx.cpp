// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// ShellEx functions

#include "NanoWinShellEx.h"

#include "NanoWinError.h"
#include "NanoWinFileSys.h"

NW_EXTERN_C_BEGIN

extern int SHCreateDirectoryExW
           (
            HWND                       hwnd,
            LPCWSTR                    pszPath,
            const SECURITY_ATTRIBUTES *psa
           )
{
  // TODO: Refine implemenation (create folders among full path)

  // Error codes: (at least): [see doc]
  // ERROR_SUCCESS:
  // ERROR_FILE_EXISTS:
  // ERROR_ALREADY_EXISTS:

  if (hwnd != NULL) { return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL)); }
  if (CreateDirectoryW(pszPath, const_cast<SECURITY_ATTRIBUTES*>(psa)))
  {
    return(ERROR_SUCCESS);
  }
  else if (GetLastError() != ERROR_SUCCESS)
  {
    return(GetLastError());
  }
  else // GetLastError() not set, but we have failed to action
  {
    return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL));
  }
}

extern int SHCreateDirectoryExA
           (
            HWND                       hwnd,
            LPCSTR                     pszPath,
            const SECURITY_ATTRIBUTES *psa
           )
{
  // TODO: Refine implemenation (create folders among full path)

  // Error codes: (at least): [see doc]
  // ERROR_SUCCESS:
  // ERROR_FILE_EXISTS:
  // ERROR_ALREADY_EXISTS:

  if (hwnd != NULL) { return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL)); }
  if (CreateDirectoryA(pszPath, const_cast<SECURITY_ATTRIBUTES*>(psa)))
  {
    return(ERROR_SUCCESS);
  }
  else if (GetLastError() != ERROR_SUCCESS)
  {
    return(GetLastError());
  }
  else // GetLastError() not set, but we have failed to action
  {
    return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL));
  }
}

NW_EXTERN_C_END
