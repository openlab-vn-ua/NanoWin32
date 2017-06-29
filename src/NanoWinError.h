// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Error suuport functions and constants

#if !defined(NanoWinErrorIncluded)
#define NanoWinErrorIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <stdarg.h>

// Error constants
// -----------------------------------------------------------------------
// Ordered by error code value, values match values in Win32 API
// Note, that NanoWinError.cpp contains message text table for each 
// constant (is used in FormatMessage implementation), so if you add
// new constant definition, you should add entry to that table too

#define ERROR_SUCCESS                  (0x0)     //     0 // OK, no error

#define ERROR_INVALID_FUNCTION         (0x1)     //     1 // Incorrect function

#define ERROR_FILE_NOT_FOUND           (0x2)     //     2 // The system cannot find the file specified
#define ERROR_PATH_NOT_FOUND           (0x3)     //     3 // The system cannot find the path specified
#define ERROR_TOO_MANY_OPEN_FILES      (0x4)     //     4 // The system cannot open the file
#define ERROR_ACCESS_DENIED            (0x5)     //     5 // Access is denied
#define ERROR_INVALID_HANDLE           (0x6)     //     6 // The handle is invalid
#define ERROR_NOT_ENOUGH_MEMORY        (0x8)     //     8 // Not enough storage is available to process this command
#define ERROR_INVALID_DATA             (0xd)     //    13 // The data is invalid
#define ERROR_SHARING_VIOLATION        (0x20)    //    32 // The process cannot access the file because it is being used by another process

#define ERROR_NOT_SUPPORTED            (0x32)    //    50 // The request is not supported
#define ERROR_FILE_EXISTS              (0x50)    //    80 // The file exists
#define ERROR_INVALID_PARAMETER        (0x57)    //    87 // The parameter is incorrect
#define ERROR_INSUFFICIENT_BUFFER      (0x7A)    //   122 // The data area passed to a system call is too small
#define ERROR_ALREADY_EXISTS           (0XB7)    //   183 // Cannot create a file when that file already exists
#define ERROR_ENVVAR_NOT_FOUND         (0XCB)    //   203 // The system could not find the environment option that was entered
#define ERROR_FILENAME_EXCED_RANGE     (0xCE)    //   206 // The filename or extension is too long

#define ERROR_INVALID_FLAGS            (0x3EC)   //  1004 // Invalid flags
#define ERROR_NO_UNICODE_TRANSLATION   (0x459)   //  1113 // No mapping for the Unicode character exists in the target multi-byte code page

#define ERROR_RESOURCE_LANG_NOT_FOUND  (0x717)   //  1815 // The specified resource language ID cannot be found in the image file

// Additional constants

#define NO_ERROR                       ERROR_SUCCESS

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

#define NW_DEFAULT_ERROR_AT_FAIL   ERROR_INVALID_DATA

// Converts errno to Win32Error, converts errno=0 to specified value win32 error value
extern  DWORD NanoWinErrorByErrnoRaw (errno_t errno_value, DWORD winErrorForZeroErrno);  

// Converts errno to Win32Error, converts errno=0 to NW_DEFAULT_ERROR_AT_FAIL(ERROR_INVALID_DATA)
// equvalent to call NanoWinErrorByErrnoRaw (errno_value, NW_DEFAULT_ERROR_AT_FAIL)
extern  DWORD NanoWinErrorByErrnoAtFail(errno_t errno_value); 

// Windows gates

#ifdef __cplusplus
inline  DWORD GetLastError()                                 { return NanoWinGetLastError(); }
inline  void  SetLastError(DWORD dwErrCode)                  { NanoWinSetLastError(dwErrCode); }
inline  void  SetLastErrorEx(DWORD dwErrCode, DWORD dwType)  { NanoWinSetLastErrorEx(dwErrCode, dwType); }
#else
#define GetLastError         NanoWinGetLastError
#define SetLastError(x)      ((void)(NanoWinSetLastError((x))))
#define SetLastErrorEx(x,y)  ((void)(NanoWinSetLastErrorEx((x),(y))))
#endif

#define FORMAT_MESSAGE_ALLOCATE_BUFFER  0x00000100 
#define FORMAT_MESSAGE_FROM_SYSTEM      0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS   0x00000200

// FormatMessage implementation has the following limitations:
// 1. FORMAT_MESSAGE_ARGUMENT_ARRAY, FORMAT_MESSAGE_FROM_HMODULE, FORMAT_MESSAGE_FROM_STRING flags
//    are not supported
// 2. FORMAT_MESSAGE_FROM_SYSTEM bit in dwFlags must be set
// 3. lpSource parameter is ignored (since neither FORMAT_MESSAGE_FROM_HMODULE, nor FORMAT_MESSAGE_FROM_STRING are supported)
// 4. dwLanguageId must be equal to MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT)
// 5. Arguments parameter is ignored
extern DWORD WINAPI FormatMessageA(_In_     DWORD   dwFlags,
                                   _In_opt_ LPCVOID lpSource,
                                   _In_     DWORD   dwMessageId,
                                   _In_     DWORD   dwLanguageId,
                                   _Out_    LPSTR   lpBuffer,
                                   _In_     DWORD   nSize,
                                   _In_opt_ va_list *Arguments);

extern DWORD WINAPI FormatMessageW(_In_     DWORD   dwFlags,
                                   _In_opt_ LPCVOID lpSource,
                                   _In_     DWORD   dwMessageId,
                                   _In_     DWORD   dwLanguageId,
                                   _Out_    LPWSTR  lpBuffer,
                                   _In_     DWORD   nSize,
                                   _In_opt_ va_list *Arguments);

NW_EXTERN_C_END

#if defined(UNICODE) || defined(_UNICODE)
 #define FormatMessage FormatMessageW
#else
 #define FormatMessage FormatMessageA
#endif

#endif // linux
#endif // ...Included
