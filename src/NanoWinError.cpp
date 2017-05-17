// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Error suuport functions and constants

#include "NanoWinError.h"

#include <errno.h>

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

// Converts errno to Win32Error, converts errno=0 to specified value win32 error value
extern  DWORD NanoWinErrorByErrnoRaw (errno_t errno_value, DWORD winErrorForZeroErrno)
{
  #define CASE_RETURN(errno_val,win32_error_val) case(errno_val): { return(win32_error_val); } break;

  switch(errno_value)
  {
    // Success
    case(0): { return(winErrorForZeroErrno); } break;

    #if defined(ENOMEM)
    CASE_RETURN(ENOMEM,ERROR_NOT_ENOUGH_MEMORY);
    #endif

    #if defined(EINVAL)
    CASE_RETURN(EINVAL,ERROR_INVALID_DATA); // ?
    #endif

    #if defined(EACCES)
    CASE_RETURN(EACCES,ERROR_ACCESS_DENIED);
    #endif

    #if defined(ENFILE)
    CASE_RETURN(ENFILE,ERROR_TOO_MANY_OPEN_FILES);
    #endif

    #if defined(EMFILE)
    CASE_RETURN(EMFILE,ERROR_TOO_MANY_OPEN_FILES); // ?
    #endif

    #if defined(ENOENT)
    CASE_RETURN(ENOENT,ERROR_FILE_NOT_FOUND); // ERROR_PATH_NOT_FOUND ?
    #endif

    // Unknown errno
	default:
    {
      // return(NW_DEFAULT_ERROR_AT_FAIL); // will be at end of function
    }
  }

  return(NW_DEFAULT_ERROR_AT_FAIL);
  #undef CASE_RETURN
}

#define NW_DEFAULT_ERROR_AT_FAIL   ERROR_INVALID_DATA

// Converts errno to Win32Error, converts errno=0 to NW_DEFAULT_ERROR_AT_FAIL(ERROR_INVALID_DATA)
extern  DWORD NanoWinErrorByErrnoAtFail(errno_t errno_value)
{
  return NanoWinErrorByErrnoRaw (errno_value, NW_DEFAULT_ERROR_AT_FAIL);
}

NW_EXTERN_C_END
