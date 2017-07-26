// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Module functions

#include "NanoWinModule.h"

#include "NanoWinError.h"
#include "NanoWinStrConvert.h"

#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>

NW_EXTERN_C_BEGIN

extern HANDLE  WINAPI GetCurrentProcess()
{
  return(NW_HANDLE_CURR_PROCCESS);
}

extern DWORD   WINAPI GetCurrentProcessId(void)
{
  return(getpid());
}

extern HMODULE WINAPI GetModuleHandleA(LPCSTR_NULLONLY  lpModuleName)
{
  if (lpModuleName == NULL)
  {
    return(GetCurrentProcess());
  }

  return(NULL);
}

extern HMODULE WINAPI GetModuleHandleW(LPCWSTR_NULLONLY lpModuleName)
{
  if (lpModuleName == NULL)
  {
    return(GetCurrentProcess());
  } 

  return(NULL);
}

// Dlls

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL. (Use GetLastError)
extern HMODULE WINAPI LoadLibraryA(_In_ LPCSTR  lpFileName)
{
  if (lpFileName == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  int iLoadFlags = RTLD_NOW; // RTLD_LAZY
  HMODULE result = dlopen(lpFileName, iLoadFlags);
  // if (result == NULL) { printf("Dl error : %s\n", dlerror()); } // DEBUG
  if (result == NULL) { SetLastError(ERROR_INVALID_PARAMETER); } // TODO: (not critical) Provide more details leter (somehow?)
  return(result);
}

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL (use GetLastError to get error code)
extern HMODULE WINAPI LoadLibraryW(_In_ LPCWSTR lpFileName)
{
  if (lpFileName == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  try
  {
    std::string mbFileName = NanoWin::StrConverter::Convert(lpFileName);
    return LoadLibraryA(mbFileName.c_str());
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    return FALSE;
  }
}

// Frees the loaded dynamic-link library
extern BOOL    WINAPI FreeLibrary(_In_ HMODULE hModule)
{
  if (hModule    == NULL)    { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE); }
  if (dlclose(hModule) != 0) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); return(FALSE); }
  return(TRUE);
}

// Retrieves the address of an exported function or variable from the specified dynamic-link library
// If the function fails, the return value is NULL (use GetLastError to get error code)
// Note: The only version exists in Win32 is ASCII?
extern FARPROC WINAPI GetProcAddress(_In_ HMODULE hModule, _In_ LPCSTR lpProcName)
{
  if (hModule    == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  if (lpProcName == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  PVOID result = dlsym(hModule, lpProcName);
  if (result == NULL) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); }
  return(result);
}

NW_EXTERN_C_END
