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

#if defined(__cplusplus)
inline bool NanoWinIsCurrentProccessHandle(HANDLE proccess)
{
  if (proccess == NW_HANDLE_CURR_PROCCESS) { return(true); }
  if (proccess == INVALID_HANDLE_VALUE)    { return(true); } // for Win32 compatibility, since curr proccess pseudo handle is -1 (same value as INVALID_HANDLE_VALUE)
  return(false);
}
#endif

// The return value is a pseudo handle to the current process.
// The pseudo handle as a special handle value used to identify current proccess
extern HANDLE  WINAPI GetCurrentProcess();

// Retrieves the process identifier of the calling (current) process.
// Until the process terminates, the process identifier uniquely identifies the process throughout the system.
extern DWORD   WINAPI GetCurrentProcessId(void);

extern HMODULE WINAPI GetModuleHandleA(LPCSTR_NULLONLY  lpModuleName);
extern HMODULE WINAPI GetModuleHandleW(LPCWSTR_NULLONLY lpModuleName);

// Dlls

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL. (Use GetLastError)
extern HMODULE WINAPI LoadLibraryA(_In_ LPCSTR  lpFileName);

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL (use GetLastError to get error code)
extern HMODULE WINAPI LoadLibraryW(_In_ LPCWSTR lpFileName);

// Frees the loaded dynamic-link library
extern BOOL    WINAPI FreeLibrary(_In_ HMODULE hModule);

// Retrieves the address of an exported function or variable from the specified dynamic-link library
// If the function fails, the return value is NULL (use GetLastError to get error code)
// Note: The only version exists in Win32 is ASCII?
extern FARPROC WINAPI GetProcAddress(_In_ HMODULE hModule, _In_ LPCSTR lpProcName); 

#if defined(UNICODE) || defined(_UNICODE)
#define GetModuleHandle       GetModuleHandleW
#define LoadLibrary           LoadLibraryW
#else
#define GetModuleHandle       GetModuleHandleA
#define LoadLibrary           LoadLibraryA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
