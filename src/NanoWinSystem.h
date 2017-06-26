// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// System functions

#if !defined(NanoWinSystemIncluded)
#define NanoWinSystemIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

typedef struct _SYSTEMTIME // Specifies a date and time, using individual members for the month, day, year, weekday, hour, minute, second, and millisecond
{
  WORD wYear;          // The year. The valid values 1900..2xxx
  WORD wMonth;         // The month. This member can be 1..12
  WORD wDayOfWeek;     // The day of the week. This member can 0..6 [Sunday=0, Monday=1, ...]
  WORD wDay;           // The day of the month. The valid values are 1..31
  WORD wHour;          // The hour. The valid values 0..23
  WORD wMinute;        // The minute. The valid values 0..59
  WORD wSecond;        // The second. The valid values 0..59 [Note: Under Linux you may see 60 here for leap second]
  WORD wMilliseconds;  // The millisecond. The valid values 0..999
} SYSTEMTIME; NW_MAKE_PLP_TYPES_BY(SYSTEMTIME);

// Retrieves the current local date and time. (in local time zone)
extern void WINAPI GetLocalTime(_Out_ LPSYSTEMTIME lpSystemTime);

// Retrieves the current system date and time. The system time is expressed in Coordinated Universal Time (UTC).
extern void WINAPI GetSystemTime(_Out_ LPSYSTEMTIME lpSystemTime);

// Current implementation supports only LOCALE_USER_DEFAULT and LOCALE_SYSTEM_DEFAULT locales,
// no flags support and lpFormat string must be NULL (default time and date formats for
// locale are used)
extern int  WINAPI GetTimeFormatA(_In_            LCID       Locale,
                                  _In_            DWORD      dwFlags,
                                  _In_opt_  const SYSTEMTIME *lpTime,
                                  _In_opt_        LPCSTR     lpFormat,
                                  _Out_opt_       LPSTR      lpTimeStr,
                                  _In_            int        cchTime);

extern int  WINAPI GetTimeFormatW(_In_            LCID       Locale,
                                  _In_            DWORD      dwFlags,
                                  _In_opt_  const SYSTEMTIME *lpTime,
                                  _In_opt_        LPCWSTR    lpFormat,
                                  _Out_opt_       LPWSTR     lpTimeStr,
                                  _In_            int        cchTime);

extern int  WINAPI GetDateFormatA(_In_            LCID       Locale,
                                  _In_            DWORD      dwFlags,
                                  _In_opt_  const SYSTEMTIME *lpDate,
                                  _In_opt_        LPCSTR     lpFormat,
                                  _Out_opt_       LPSTR      lpDateStr,
                                  _In_            int        cchDate);

extern int  WINAPI GetDateFormatW(_In_            LCID       Locale,
                                  _In_            DWORD      dwFlags,
                                  _In_opt_  const SYSTEMTIME *lpDate,
                                  _In_opt_        LPCWSTR    lpFormat,
                                  _Out_opt_       LPWSTR     lpDateStr,
                                  _In_            int        cchDate);

// Psapi.h
// -----------------------------

typedef struct // Reasonable subset of proccess system resources consumption
{
  DWORD  cb;
//DWORD  PageFaultCount;
  SIZE_T PeakWorkingSetSize; // The peak working set size, in bytes.
  SIZE_T WorkingSetSize;     // The current working set size, in bytes.
  SIZE_T PagefileUsage;      // The Commit Charge value in bytes for this process. Commit Charge is the total amount of memory that the memory manager has committed for a running process. [Win7 or earlier may not fill it, check PagefileUsage]
  SIZE_T PeakPagefileUsage;  // The peak value in bytes of the Commit Charge during the lifetime of this process.
} PROCESS_MEMORY_COUNTERS; NW_MAKE_PLP_TYPES_BY(PROCESS_MEMORY_COUNTERS);

typedef  struct // Reasonable subset of proccess system resources consumption [ex]
{
  DWORD  cb;
//DWORD  PageFaultCount;
  SIZE_T PeakWorkingSetSize; // The peak working set size, in bytes.
  SIZE_T WorkingSetSize;     // The current working set size, in bytes.
  SIZE_T PagefileUsage;      // The Commit Charge value in bytes for this process. Commit Charge is the total amount of memory that the memory manager has committed for a running process. [Win7 or earlier may not fill it, check PagefileUsage]
  SIZE_T PeakPagefileUsage;  // The peak value in bytes of the Commit Charge during the lifetime of this process.
  // Ex
  SIZE_T PrivateUsage;       // The Commit Charge value in bytes for this process. Commit Charge is the total amount of memory that the memory manager has committed for a running process. [Same as PagefileUsage]
} PROCESS_MEMORY_COUNTERS_EX; NW_MAKE_PLP_TYPES_BY(PROCESS_MEMORY_COUNTERS_EX);

// Supports only current proccess (so Process must be result of GetCurrentProcess() or be NULL)
extern BOOL WINAPI GetProcessMemoryInfo
                   (
                     _In_  HANDLE                   Process, // NW_NULLONLY(HANDLE)
                     _Out_ PPROCESS_MEMORY_COUNTERS ppsmemCounters, // can be PPROCESS_MEMORY_COUNTERS_EX as well
                     _In_  DWORD                    cb
                   );

#if defined(UNICODE) || defined(_UNICODE)
 #define GetTimeFormat GetTimeFormatW
 #define GetDateFormat GetDateFormatW
#else
 #define GetTimeFormat GetTimeFormatA
 #define GetDateFormat GetDateFormatA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
