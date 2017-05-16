// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// QueryPerformance functions

#include "NanoWinQueryPerf.h"

#include <time.h>

NW_EXTERN_C_BEGIN

/* Implementation notes:
1. CLOCK_MONOTONIC is used to get high-resolution timer value
2. PerformanceFrequency is constant and set to 1000000000 (resolution is 1ns). While it is possible to
check resolution by calling clock_getres but there is no guarantee that we can precisely represent
frequency as an integer value for any possible values of resolution.
3. In order to correctly detect presence of clock QueryPerformanceFrequency function calls clock_getres
the first time it is called.
*/

#define PERFORMANCE_COUNTER_CLOCK_ID                CLOCK_MONOTONIC

///////////////////////////////////////////////////////////////////////////////
//
// WIN32 API
// Retrieves the frequency of the performance counter. 
//
// OUT lpFrequency - A pointer to a variable that receives the current performance-counter frequency,
//                   in counts per second. If the installed hardware doesn't support a high-resolution
//                   performance counter, this parameter can be zero (this will not occur on systems
//                   that run Windows XP or later).
//
// RETURN  If the installed hardware supports a high-resolution performance counter, the return value is nonzero.
//
///////////////////////////////////////////////////////////////////////////////
BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency)
{
  static bool clockSupportChecked = false;
  static bool clockIsSupported = false;

  if (!clockSupportChecked)
  {
    clockIsSupported = clock_getres(PERFORMANCE_COUNTER_CLOCK_ID, NULL) == 0;
    clockSupportChecked = true;
  }

  if (clockIsSupported)
  {
    lpFrequency->QuadPart = PERFORMANCE_COUNTER_FREQUENCY;
  }

  return clockIsSupported ? TRUE : FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
//  WIN32 API
//  Retrieves the current value of the performance counter, which is a high resolution
//  (<1us) time stamp that can be used for time-interval measurements.
//
//   OUT lpPerformanceCount - A pointer to a variable that receives the current
//                            performance-counter value, in counts.
//
//   RETURN If the function succeeds, the return value is nonzero.
//
///////////////////////////////////////////////////////////////////////////////
BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount)
{
  struct timespec timestamp;

  if (clock_gettime(PERFORMANCE_COUNTER_CLOCK_ID, &timestamp) == 0)
  {
    lpPerformanceCount->QuadPart =
      (int64_t)timestamp.tv_sec * PERFORMANCE_COUNTER_FREQUENCY +
      (int64_t)timestamp.tv_nsec * PERFORMANCE_COUNTER_FREQUENCY / PERFORMANCE_COUNTER_NANOSECONDS_IN_SECOND;

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

NW_EXTERN_C_END
