// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Other functions

#include "NanoWinOther.h"

#include "NanoWinQueryPerf.h"
//#include "NanoWinError.h"

#include <stdlib.h>

NW_EXTERN_C_BEGIN

extern PVOID SecureZeroMemory (_In_ volatile PVOID dest, _In_ SIZE_T destszb)
{
  // dest marked as volatile to prevent compiler to optimize-out call to this func
  return(memset(dest,0,destszb));
}

extern BOOL WINAPI IsBadReadPtr    (_In_ const VOID  *lp,   _In_ UINT_PTR ucb)
{ 
  if (ucb <= 0) { return(FALSE); };
  if (lp == NULL) { return(TRUE); }; 
  return(FALSE);
}

extern BOOL WINAPI IsBadWritePtr   (_In_ LPVOID       lp,   _In_ UINT_PTR ucb)
{
  if (ucb <= 0) { return(FALSE); };
  if (lp == NULL) { return(TRUE); };
  return(FALSE);
}

extern BOOL WINAPI IsBadStringPtrA (_In_ LPCSTR       lpsz, _In_ UINT_PTR ucchMax)
{
  if (ucchMax <= 0) { return(FALSE); };
  if (lpsz == NULL) { return(TRUE); };
  return(FALSE);
}

extern BOOL WINAPI IsBadStringPtrW (_In_ LPCWSTR      lpsz, _In_ UINT_PTR ucchMax)
{
  if (ucchMax <= 0) { return(FALSE); };
  if (lpsz == NULL) { return(TRUE); };
  return(FALSE);
}

extern BOOL WINAPI IsBadCodePtr    (_In_ FARPROC      lpfn)
{
  if (lpfn == NULL) { return(TRUE); };
  return(FALSE);
}

extern DWORD GetTickCount()
{
  LARGE_INTEGER result; 
  QueryPerformanceCounter(&result); 
  return (DWORD)(result.QuadPart / 1000000);
}

// Legacy heap functions (subset)

extern  HGLOBAL WINAPI  GlobalAlloc(_In_ UINT uFlags, _In_ SIZE_T sz)
{
  // TODO: Result pointer should me aligned to 8-bytes boundary
  HGLOBAL result;
  result = malloc(sz);
  if (result != NULL)
  {
    if ((uFlags & GMEM_ZEROINIT) != 0)
    {
      ZeroMemory(result, sz);
    }
  }

  return(result);
}

extern  HGLOBAL WINAPI  GlobalFree(_In_ HGLOBAL hMem)
{
  free(hMem);
  return(NULL);
}

extern  HLOCAL WINAPI   LocalAlloc(_In_ UINT   uFlags, _In_ SIZE_T sz)
{
  HLOCAL result;
  result = malloc(sz);
  if (result != NULL)
  {
    if ((uFlags & LMEM_ZEROINIT) != 0)
    {
      ZeroMemory(result, sz);
    }
  }

  return(result);
}

extern  HLOCAL WINAPI   LocalFree(_In_ HLOCAL hMem)
{
  free(hMem);
  return(NULL);
}

// Returns system metrics SM_* or 0 if error found (does not update GetLastError())
extern int WINAPI GetSystemMetrics(_In_ int nIndex)
{
  // TODO: Refine me somehow
  if (nIndex == SM_CXSCREEN) { return(640); }
  if (nIndex == SM_CYSCREEN) { return(480); }
  return(0);
}

NW_EXTERN_C_END
