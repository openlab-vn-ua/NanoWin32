// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// AfxMt functions (subset)

#include "NanoWinMFCAfxMt.h"

#include "NanoWinError.h"
#include "NanoWinInternal.h"

CMutex::CMutex(BOOL                  bInitiallyOwn)
{
  pthread_mutex_init(&mutex,NULL); // always succeeds on GNU

  if (bInitiallyOwn)
  {
    pthread_mutex_lock(&mutex); // there can be two reasons to fail - invalid mutex and "already locked", both impossible here
  }
}

CMutex::~CMutex()
{
  pthread_mutex_destroy(&mutex);
}

BOOL CMutex::Lock(DWORD dwTimeout)
{
  if (dwTimeout != INFINITE)
  {
    struct timespec finishTime;

    if (clock_gettime(CLOCK_REALTIME, &finishTime) != 0)
    {
      return FALSE;
    }

    NanoWinTimeSpecAddTimeoutInMillis(&finishTime,dwTimeout);

    return pthread_mutex_timedlock(&mutex,&finishTime) == 0;
  }
  else
  {
    return pthread_mutex_lock(&mutex) == 0;
  }
}

BOOL CMutex::Unlock()
{
  return pthread_mutex_unlock(&mutex) == 0;
}

NW_EXTERN_C_BEGIN


NW_EXTERN_C_END
