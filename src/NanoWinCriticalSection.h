// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// CriticalSection functions

#if !defined(NanoWinCriticalSectionIncluded)
#define NanoWinCriticalSectionIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <pthread.h>

typedef struct
{
  pthread_mutex_t     mutex;
  pthread_mutexattr_t mutexattr;
} CRITICAL_SECTION;
typedef CRITICAL_SECTION *LPCRITICAL_SECTION;


NW_EXTERN_C_BEGIN

extern void InitializeCriticalSection             (LPCRITICAL_SECTION lpCriticalSection);
extern BOOL InitializeCriticalSectionAndSpinCount (LPCRITICAL_SECTION lpCriticalSection,
                                                   DWORD              dwSpinCount);
extern void EnterCriticalSection                  (LPCRITICAL_SECTION lpCriticalSection);
extern void LeaveCriticalSection                  (LPCRITICAL_SECTION lpCriticalSection);
extern void DeleteCriticalSection                 (LPCRITICAL_SECTION lpCriticalSection);

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
