// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// CriticalSection functions

#include "NanoWinCriticalSection.h"

//#include "NanoWinError.h"

NW_EXTERN_C_BEGIN

static void log_error(int error_code, const char *func_name)
{
  // NanoWin::log(func_name, " Unexpected error in CRITICAL_SECTION:", error_code);
}

#define check_error(expr) { int err = (expr); if (err != 0) { log_error(err, #expr); } }

void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
  check_error(pthread_mutexattr_init(&lpCriticalSection->mutexattr));
  check_error(pthread_mutexattr_settype(&lpCriticalSection->mutexattr, PTHREAD_MUTEX_NORMAL));
  check_error(pthread_mutex_init(&lpCriticalSection->mutex, &lpCriticalSection->mutexattr));
}

//TODO: implement spin count support (need to store spin count in CRITICAL_SECTION structure
//      and spin inside EnterCriticalSection (only on multi-core processors))
void InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION lpCriticalSection,
                                           DWORD              /*dwSpinCount*/)
{
  InitializeCriticalSection(lpCriticalSection);
}

void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
  check_error(pthread_mutex_lock(&lpCriticalSection->mutex));
}

void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
  check_error(pthread_mutex_unlock(&lpCriticalSection->mutex));
}

void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
  check_error(pthread_mutex_destroy(&lpCriticalSection->mutex));
  check_error(pthread_mutexattr_destroy(&lpCriticalSection->mutexattr));
}


NW_EXTERN_C_END
