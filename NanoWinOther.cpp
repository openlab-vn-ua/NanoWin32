// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Other functions

#include "NanoWinOther.h"

#include "NanoWinQueryPerf.h"
//#include "NanoWinError.h"

NW_EXTERN_C_BEGIN

extern BOOL IsBadReadPtr (const void *lp, UINT_PTR ucb) 
{ 
  if (ucb <= 0) { return(FALSE); };
  if (lp == NULL) { return(TRUE); }; 
  return(FALSE);
}

extern BOOL IsBadWritePtr (const void *lp, UINT_PTR ucb)
{
  if (ucb <= 0) { return(FALSE); };
  if (lp == NULL) { return(TRUE); };
  return(FALSE);
}

extern DWORD GetTickCount()
{
  LARGE_INTEGER result; 
  QueryPerformanceCounter(&result); 
  return (DWORD)(result.QuadPart / 1000000);
}

NW_EXTERN_C_END
