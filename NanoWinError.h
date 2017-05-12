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

#define ERROR_SUCCESS                  (0x0)     //     0 // OK, no error

#define ERROR_INVALID_FUNCTION         (0x1)     //     1 // Incorrect function

#define ERROR_FILE_NOT_FOUND           (0x2)     //     2 // The system cannot find the file specified
#define ERROR_PATH_NOT_FOUND           (0x3)     //     3 // The system cannot find the path specified
#define ERROR_TOO_MANY_OPEN_FILES      (0x4)     //     4 // The system cannot open the file
#define ERROR_ACCESS_DENIED            (0x5)     //     5 // Access is denied
#define ERROR_INVALID_HANDLE           (0x6)     //     6 // The handle is invalid
#define ERROR_NOT_ENOUGH_MEMORY        (0x8)     //     8 // Not enough storage is available to process this command
#define ERROR_INVALID_DATA             (0xd)     //    13 // The data is invalid

#define ERROR_NOT_SUPPORTED            (0x32)    //    50 // The request is not supported
#define ERROR_ENVVAR_NOT_FOUND         (0XCB)    //   203 // The system could not find the environment option that was entered

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
