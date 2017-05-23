// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Module functions

#include "NanoWinModule.h"

#include <unistd.h>

#include "NanoWinError.h"

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

NW_EXTERN_C_END
