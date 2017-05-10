// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Error suuport functions and constants

#include "NanoWinError.h"

NW_EXTERN_C_BEGIN

static __thread DWORD LastError = ERROR_SUCCESS;

extern  DWORD NanoWinGetLastError(void)
{
  return(LastError);
}

extern  DWORD NanoWinSetLastError  // Set last error code (Returns dwErrCode)
              (
                DWORD  dwErrCode
              )
{
  LastError = dwErrCode;
  return(dwErrCode);
}

extern  DWORD NanoWinSetLastErrorEx // Set last error code (Returns dwErrCode)
              (
                DWORD  dwErrCode,
                DWORD  dwType     // Ignored (as in Win32)
              )
{
  return(NanoWinSetLastError(dwErrCode));
}

NW_EXTERN_C_END
