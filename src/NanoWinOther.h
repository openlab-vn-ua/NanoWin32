// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Other functions

#if !defined(NanoWinOtherIncluded)
#define NanoWinOtherIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <unistd.h>
#include <string.h>

NW_EXTERN_C_BEGIN

// SEH stub
#define __try		try
#define __except(a)	catch(...)

// Memory functions

#define ZeroMemory(dest,destszb)       memset((dest),0,(destszb))

extern PVOID SecureZeroMemory (_In_ volatile PVOID dest, _In_ SIZE_T destszb);

extern BOOL WINAPI IsBadReadPtr    (_In_ const VOID  *lp,   _In_ UINT_PTR ucb); 
extern BOOL WINAPI IsBadWritePtr   (_In_ LPVOID       lp,   _In_ UINT_PTR ucb);
extern BOOL WINAPI IsBadStringPtrA (_In_ LPCSTR       lpsz, _In_ UINT_PTR ucchMax);
extern BOOL WINAPI IsBadStringPtrW (_In_ LPCWSTR      lpsz, _In_ UINT_PTR ucchMax);
extern BOOL WINAPI IsBadCodePtr    (_In_ FARPROC      lpfn);

// Legacy heap functions (subset)

typedef HANDLE HGLOBAL;

#define GMEM_FIXED      (0x0000)                     // Allocates fixed memory. The return value is a pointer.
#define GMEM_ZEROINIT   (0x0040)                     // Initializes memory contents to zero.
#define GPTR            (GMEM_FIXED | GMEM_ZEROINIT) // Combines GMEM_FIXED and GMEM_ZEROINIT.

extern  HGLOBAL WINAPI  GlobalAlloc(_In_ UINT uFlags, _In_ SIZE_T sz);
extern  HGLOBAL WINAPI  GlobalFree(_In_ HGLOBAL hMem);

typedef HANDLE HLOCAL;

#define LMEM_FIXED      (0x0000)                     // Allocates fixed memory. The return value is a pointer to the memory object.
#define LMEM_ZEROINIT   (0x0040)                     // Initializes memory contents to zero.
#define LPTR            (LMEM_FIXED | LMEM_ZEROINIT) // Combines LMEM_FIXED and LMEM_ZEROINIT.
#define NONZEROLPTR     (LMEM_FIXED)                 // Same as LMEM_FIXED.

extern  HLOCAL WINAPI   LocalAlloc(_In_ UINT   uFlags, _In_ SIZE_T sz);
extern  HLOCAL WINAPI   LocalFree(_In_ HLOCAL hMem);

// Timer

#define Sleep(dwMilliseconds)    usleep((dwMilliseconds)*1000)
extern  DWORD GetTickCount();    // Return value in ms

// Metrics

#define SM_CYSCREEN ('X')
#define SM_CXSCREEN ('Y')

// Returns system metrics SM_* or 0 if error found (does not update GetLastError())
extern int WINAPI GetSystemMetrics(_In_ int nIndex);

#if defined(UNICODE) || defined(_UNICODE)
#define IsBadStringPtr  IsBadStringPtrW
#else
#define IsBadStringPtr  IsBadStringPtrA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
