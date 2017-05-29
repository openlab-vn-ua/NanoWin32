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

extern BOOL WINAPI SetEnvironmentVariableA(_In_ const char *lpName, _In_opt_ const char *lpValue);
extern BOOL WINAPI SetEnvironmentVariableW(_In_ const wchar_t *lpName,	_In_opt_ const wchar_t *lpValue);

#if defined(UNICODE) || defined(_UNICODE)
#define GetEnvironmentVariable  GetEnvironmentVariableW
#define SetEnvironmentVariable  SetEnvironmentVariableW
#else
#define GetEnvironmentVariable  GetEnvironmentVariableA
#define SetEnvironmentVariable  SetEnvironmentVariableA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
