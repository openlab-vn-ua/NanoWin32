// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Environment functions

#if !defined(NanoWinEnvIncluded)
#define NanoWinEnvIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

extern  DWORD GetEnvironmentVariableA
              (
                LPCSTR  lpName,
                LPSTR   lpBuffer,
                DWORD   nSize
              );

extern  DWORD GetEnvironmentVariableW
              (
                LPCWSTR lpName,
                LPWSTR  lpBuffer,
                DWORD   nSize
              );

#if defined(UNICODE) || defined(_UNICODE)
#define	GetEnvironmentVariable  GetEnvironmentVariableW
#else
#define GetEnvironmentVariable  GetEnvironmentVariableA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
