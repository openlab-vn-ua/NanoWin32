// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Additional functions (not part of Win API, but provided by NanoWin)

#if !defined(NanoWinPlusIncluded)
#define NanoWinPlusIncluded

#include "NanoWinTypes.h"

#if defined LINUX

// Return Win32 API error code
NW_EXTERN_C DWORD  NanoWinCreateDirectoryPathA
                   (
                     LPCSTR                     pszPath,
                     const SECURITY_ATTRIBUTES *psa
                   );

// Return Win32 API error code
NW_EXTERN_C DWORD  NanoWinCreateDirectoryPathW
                   (
                     LPCWSTR                    pszPath,
                     const SECURITY_ATTRIBUTES *psa
                   );

#if defined(UNICODE) || defined(_UNICODE)
#define NanoWinCreateDirectoryPath              NanoWinCreateDirectoryPathW
#else
#define NanoWinCreateDirectoryPath              NanoWinCreateDirectoryPathA
#endif

#endif // linux
#endif // ...Included
