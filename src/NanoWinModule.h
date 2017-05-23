// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Module functions

#if !defined(NanoWinModuleIncluded)
#define NanoWinModuleIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

#define NW_HANDLE_CURR_PROCCESS        NW_HANDLE_SPEC_VALUE(-2) // Under win32 is "-1", this is bad since -1 is same as INVALID_HANDLE_VALUE (we use -2 that better a bit)

// The return value is a pseudo handle to the current process.
// The pseudo handle as a special handle value used to identify current proccess
extern HANDLE  WINAPI GetCurrentProcess();

// Retrieves the process identifier of the calling (current) process.
// Until the process terminates, the process identifier uniquely identifies the process throughout the system.
extern DWORD   WINAPI GetCurrentProcessId(void);

extern HMODULE WINAPI GetModuleHandleA(LPCSTR_NULLONLY  lpModuleName);
extern HMODULE WINAPI GetModuleHandleW(LPCWSTR_NULLONLY lpModuleName);

#if defined(UNICODE) || defined(_UNICODE)
#define GetModuleHandle       GetModuleHandleW
#else
#define GetModuleHandle       GetModuleHandleA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
