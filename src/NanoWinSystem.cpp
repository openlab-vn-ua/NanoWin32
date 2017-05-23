// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// System functions

#include "NanoWinSystem.h"

#include "NanoWinModule.h"
#include "NanoWinError.h"

#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>

NW_EXTERN_C_BEGIN

// Retrieves the current local date and time. (in local time zone)
extern void WINAPI GetLocalTime(_Out_ LPSYSTEMTIME lpSystemTime)
{
  // Note: Under win32 expected that this call should never fall (and, probably, should never alter SetLastError)
  if (lpSystemTime == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return; }
  memset(lpSystemTime, 0, sizeof(*lpSystemTime));
  struct timespec result;
  if (clock_gettime(CLOCK_REALTIME, &result) != 0) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); return; }
  struct tm parts;
  if (localtime_r(&result.tv_sec, &parts) == NULL) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); return; }
  lpSystemTime->wYear      = parts.tm_year+1900;
  lpSystemTime->wMonth     = parts.tm_mon+1;
  lpSystemTime->wDayOfWeek = parts.tm_wday;
  lpSystemTime->wDay       = parts.tm_mday;
  lpSystemTime->wHour      = parts.tm_hour;
  lpSystemTime->wMinute    = parts.tm_min;
  lpSystemTime->wSecond    = parts.tm_sec;
  lpSystemTime->wMilliseconds = (result.tv_nsec / 1000000);
  lpSystemTime->wMilliseconds %= 1000; // just in case
}

// Retrieves the current system date and time. The system time is expressed in Coordinated Universal Time (UTC).
extern void WINAPI GetSystemTime(_Out_ LPSYSTEMTIME lpSystemTime)
{
  // Note: Under win32 expected that this call should never fall (and, probably, should never alter SetLastError)
  if (lpSystemTime == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return; }
  memset(lpSystemTime, 0, sizeof(*lpSystemTime));
  struct timespec result;
  if (clock_gettime(CLOCK_REALTIME, &result) != 0) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); return; }
  struct tm parts;
  if (gmtime_r(&result.tv_sec, &parts) == NULL) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); return; }
  lpSystemTime->wYear      = parts.tm_year+1900;
  lpSystemTime->wMonth     = parts.tm_mon+1;
  lpSystemTime->wDayOfWeek = parts.tm_wday;
  lpSystemTime->wDay       = parts.tm_mday;
  lpSystemTime->wHour      = parts.tm_hour;
  lpSystemTime->wMinute    = parts.tm_min;
  lpSystemTime->wSecond    = parts.tm_sec;
  lpSystemTime->wMilliseconds = (result.tv_nsec / 1000000);
  lpSystemTime->wMilliseconds %= 1000; // just in case
}

// Psapi.h
// -----------------------------

// inspired by
// https://stackoverflow.com/questions/1558402/memory-usage-of-current-process-in-c

static bool getIntFromLine(const char* line, int *result)
{
  if (sscanf(line, "%d", &result) != 1) { return(false); }
  return(true);
}

static bool assignMemValue(UINT_PTR *result, int value)
{
  //Note: this value is in KB!
  if (value < 0) { return(false); }
  *result = value * 1024;
  return(true);
}

static int fillProcStatValues(PPROCESS_MEMORY_COUNTERS_EX ppsmemCounters)
{
  FILE* file = fopen("/proc/self/status", "r");

  if (file == NULL) { return(false); }

  char line[128];

  while (fgets(line, sizeof(line), file) != NULL)
  {
    int value;
    const char *pfx; int plen;

	pfx = "VmSize:";
	plen = strlen(pfx);
    if (strncasecmp(line, pfx, plen) == 0)
	{
	  if (!getIntFromLine(&line[plen], &value) != 0) { return(false); }
	  if (!assignMemValue(&ppsmemCounters->PagefileUsage, value)) { return(false); }
	  if (!assignMemValue(&ppsmemCounters->PrivateUsage, value)) { return(false); }
	  continue;
    }

	pfx = "VmPeak:";
	plen = strlen(pfx);
    if (strncasecmp(line, pfx, plen) == 0)
	{
	  if (!getIntFromLine(&line[plen], &value) != 0) { return(false); }
	  if (!assignMemValue(&ppsmemCounters->PeakPagefileUsage, value)) { return(false); }
	  continue;
    }

	pfx = "VmRSS:";
	plen = strlen(pfx);
    if (strncasecmp(line, pfx, plen) == 0)
	{
	  if (!getIntFromLine(&line[plen], &value) != 0) { return(false); }
	  if (!assignMemValue(&ppsmemCounters->WorkingSetSize, value)) { return(false); }
	  continue;
    }

	pfx = "VmHWM:";
	plen = strlen(pfx);
    if (strncasecmp(line, pfx, plen) == 0)
	{
	  if (!getIntFromLine(&line[plen], &value) != 0) { return(false); }
	  if (!assignMemValue(&ppsmemCounters->PeakWorkingSetSize, value)) { return(false); }
	  continue;
    }
  }

  fclose(file);
  return(true);
}

extern BOOL WINAPI GetProcessMemoryInfo
                   (
                     _In_  HANDLE                   Process,
                     _Out_ PPROCESS_MEMORY_COUNTERS ppsmemCounters,
                     _In_  DWORD                    cb
                   )
{
  if (ppsmemCounters == NULL)       { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE); }
  if (cb < sizeof(*ppsmemCounters)) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE); }

  if ((Process == NULL) || NanoWinIsCurrentProccessHandle(Process))
  {
    // OK, we support only current proccess
  }
  else
  {
    SetLastError(ERROR_NOT_SUPPORTED); return(FALSE);
  }

  if ((sizeof(cb) == sizeof(PROCESS_MEMORY_COUNTERS)) || (sizeof(cb) == sizeof(PROCESS_MEMORY_COUNTERS_EX)))
  {
    // OK, we support these cases
  }
  else
  {
    SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE);
  }

  // Fill what we have with zeros
  memset(ppsmemCounters, 0 , cb);
  ppsmemCounters->cb = cb;

  PROCESS_MEMORY_COUNTERS_EX result;
  if (!fillProcStatValues(&result))
  {
    SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE);
  }

  memcpy(ppsmemCounters, &result, cb);

  return(TRUE);
}

NW_EXTERN_C_END
