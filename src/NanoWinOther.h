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

NW_EXTERN_C_BEGIN

// SEH stub
#define __try		try
#define __except(a)	catch(...)

extern BOOL IsBadReadPtr (const void *lp, UINT_PTR ucb); 
extern BOOL IsBadWritePtr (const void *lp, UINT_PTR ucb);

#define Sleep(dwMilliseconds)    usleep((dwMilliseconds)*1000)
extern  DWORD GetTickCount();    // Return value in ms

#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
