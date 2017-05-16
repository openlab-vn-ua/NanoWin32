// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// QueryPerformance functions

#if !defined(NanoWinQueryPerfIncluded)
#define NanoWinQueryPerfIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#define PERFORMANCE_COUNTER_NANOSECONDS_IN_SECOND   1000000000LL
#define PERFORMANCE_COUNTER_FREQUENCY               PERFORMANCE_COUNTER_NANOSECONDS_IN_SECOND

NW_EXTERN_C_BEGIN

extern BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency);
extern BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
