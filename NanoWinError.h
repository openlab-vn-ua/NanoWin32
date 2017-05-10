// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Error suuport functions and constants

#if !defined(NanoWinErrorIncluded)
#define NanoWinErrorIncluded

#include "NanoWinTypes.h"

#if defined LINUX

// Error constants
// -----------------------------------------------------------------------
// Ordered by error code value, values match values in Win32 API

#define ERROR_SUCCESS                  (0x0)

#define ERROR_INVALID_FUNCTION         (0x1)
#define ERROR_NOT_SUPPORTED            (0x50)

// Error functions
// -----------------------------------------------------------------------

NW_EXTERN_C_BEGIN

extern  DWORD NanoWinGetLastError(void); // Return last eeror code

extern  DWORD NanoWinSetLastError  // Set last error code (Returns dwErrCode)
              (
                DWORD  dwErrCode
              );

extern  DWORD NanoWinSetLastErrorEx // Set last error code (Returns dwErrCode)
              (
                DWORD  dwErrCode,
                DWORD  dwType     // Ignored (as in Win32)
              );

// Windows gates

#define GetLastError         NanoWinGetLastError
#define SetLastError(x)      ((void)NanoWinSetLastError((x)))
#define SetLastErrorEx(x,y)  ((void)NanoWinSetLastErrorEx((x),(y)))

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
