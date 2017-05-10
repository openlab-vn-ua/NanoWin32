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
                LPCSTR lpName,
                LPSTR  lpBuffer,
                DWORD  nSize
              );

#if defined(UNICODE)
#define	GetEnvironmentVariable	GetEnvironmentVariableW // TODO: Impement me
#else
#define	GetEnvironmentVariable	GetEnvironmentVariableA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
