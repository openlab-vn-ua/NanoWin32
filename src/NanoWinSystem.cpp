// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// System functions

#include "NanoWinSystem.h"

#include "NanoWinModule.h"
#include "NanoWinError.h"

#include <limits.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>

NW_EXTERN_C_BEGIN

static void BrokenTimeToSystemTime(SYSTEMTIME      *systemTime,
                                   const struct tm *brokenTime)
{
  systemTime->wYear          = (WORD)(brokenTime->tm_year+1900);
  systemTime->wMonth         = (WORD)(brokenTime->tm_mon+1);
  systemTime->wDayOfWeek     = (WORD)brokenTime->tm_wday;
  systemTime->wDay           = (WORD)brokenTime->tm_mday;
  systemTime->wHour          = (WORD)brokenTime->tm_hour;
  systemTime->wMinute        = (WORD)brokenTime->tm_min;
  systemTime->wSecond        = (WORD)brokenTime->tm_sec;
  systemTime->wMilliseconds  = 0;
}

static void SystemTimeToBrokenTime(struct tm        *brokenTime,
                                   const SYSTEMTIME *systemTime)
{
  brokenTime->tm_year = systemTime->wYear - 1900;
  brokenTime->tm_mon  = systemTime->wMonth - 1;
  brokenTime->tm_wday = systemTime->wDayOfWeek;
  brokenTime->tm_mday = systemTime->wDay;
  brokenTime->tm_hour = systemTime->wHour;
  brokenTime->tm_min  = systemTime->wMinute;
  brokenTime->tm_sec  = systemTime->wSecond;
}

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

  BrokenTimeToSystemTime(lpSystemTime,&parts);

  lpSystemTime->wMilliseconds = (WORD)(result.tv_nsec / 1000000);
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

  BrokenTimeToSystemTime(lpSystemTime,&parts);

  lpSystemTime->wMilliseconds = (WORD)(result.tv_nsec / 1000000);
  lpSystemTime->wMilliseconds %= 1000; // just in case
}

extern int  WINAPI GetTimeFormat(_In_            LCID       Locale,
                                 _In_            DWORD      dwFlags,
                                 _In_opt_  const SYSTEMTIME *lpTime,
                                 _In_opt_        LPCTSTR    lpFormat,
                                 _Out_opt_       LPTSTR     lpTimeStr,
                                 _In_            int        cchTime)
{
  if (Locale != LOCALE_USER_DEFAULT && Locale != LOCALE_SYSTEM_DEFAULT) { SetLastError(ERROR_INVALID_PARAMETER); return 0; } // not supported
  if (dwFlags != 0)                                                     { SetLastError(ERROR_INVALID_FLAGS); return 0; }     // not supported
  if (lpFormat != NULL)                                                 { SetLastError(ERROR_INVALID_PARAMETER); return 0; } // not supported

  if (lpTimeStr == NULL && cchTime != 0)                                { SetLastError(ERROR_INSUFFICIENT_BUFFER); return 0; }

  struct tm brokenTime;

  if (lpTime == NULL)
  {
    SYSTEMTIME currTime;

    GetLocalTime(&currTime);

    SystemTimeToBrokenTime(&brokenTime,&currTime);
  }
  else
  {
    SystemTimeToBrokenTime(&brokenTime,lpTime);
  }

  size_t charCount = strftime(cchTime == 0 ? NULL : lpTimeStr,cchTime != 0 ? cchTime : INT_MAX,"%r",&brokenTime);

  if (charCount == 0)
  {
    SetLastError(ERROR_INSUFFICIENT_BUFFER);

    return 0;
  }
  else
  {
    return charCount + 1; // must return number of character including '\0'
  }
}

extern int  WINAPI GetDateFormat(_In_            LCID       Locale,
                                 _In_            DWORD      dwFlags,
                                 _In_opt_  const SYSTEMTIME *lpDate,
                                 _In_opt_        LPCTSTR    lpFormat,
                                 _Out_opt_       LPTSTR     lpDateStr,
                                 _In_            int        cchDate)
{
  if (Locale != LOCALE_USER_DEFAULT && Locale != LOCALE_SYSTEM_DEFAULT) { SetLastError(ERROR_INVALID_PARAMETER); return 0; } // not supported
  if (dwFlags != 0)                                                     { SetLastError(ERROR_INVALID_FLAGS); return 0; }     // not supported
  if (lpFormat != NULL)                                                 { SetLastError(ERROR_INVALID_PARAMETER); return 0; } // not supported

  if (lpDateStr == NULL && cchDate != 0)                                { SetLastError(ERROR_INSUFFICIENT_BUFFER); return 0; }

  struct tm brokenTime;

  if (lpDate == NULL)
  {
    SYSTEMTIME currDate;

    GetLocalTime(&currDate);

    SystemTimeToBrokenTime(&brokenTime,&currDate);
  }
  else
  {
    SystemTimeToBrokenTime(&brokenTime,lpDate);
  }

  size_t charCount = strftime(cchDate == 0 ? NULL : lpDateStr,cchDate != 0 ? cchDate : INT_MAX,"%x",&brokenTime);

  if (charCount == 0)
  {
    SetLastError(ERROR_INSUFFICIENT_BUFFER);

    return 0;
  }
  else
  {
    return charCount + 1; // must return number of character including '\0'
  }
}

// Psapi.h
// -----------------------------

// inspired by
// https://stackoverflow.com/questions/1558402/memory-usage-of-current-process-in-c

static bool getIntFromLine(const char* line, int *result)
{
  if (sscanf(line, "%d", result) != 1) { return(false); }
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
    const char *pfx; size_t plen;

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

  if ((cb == sizeof(PROCESS_MEMORY_COUNTERS)) || (cb == sizeof(PROCESS_MEMORY_COUNTERS_EX)))
  {
    // OK, we support these cases
  }
  else
  {
    SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE);
  }

  // Fill what we have with zeros
  memset(ppsmemCounters, 0 , cb);

  PROCESS_MEMORY_COUNTERS_EX result;
  if (!fillProcStatValues(&result))
  {
    SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE);
  }

  memcpy(ppsmemCounters, &result, cb);

  ppsmemCounters->cb = cb;

  return(TRUE);
}

NW_EXTERN_C_END
