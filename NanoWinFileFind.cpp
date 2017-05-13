// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileFind functions

#include "NanoWinFileFind.h"

#include "NanoWinError.h"

NW_EXTERN_C_BEGIN

// TODO: Implement me

extern HANDLE FindFirstFileA(const char *Mask, WIN32_FIND_DATAA *state)
{ 
  return INVALID_HANDLE_VALUE; 
}

extern BOOL   FindNextFileA(HANDLE handle, WIN32_FIND_DATAA *state)
{
  return FALSE;
}

extern HANDLE FindFirstFileW(const char *Mask, WIN32_FIND_DATAW *state)
{
  return INVALID_HANDLE_VALUE;
}

extern BOOL   FindNextFileW(HANDLE handle, WIN32_FIND_DATAW *state)
{
  return FALSE;
}

extern BOOL   FindClose(HANDLE hFindFile)
{
  return TRUE;
}

NW_EXTERN_C_END
