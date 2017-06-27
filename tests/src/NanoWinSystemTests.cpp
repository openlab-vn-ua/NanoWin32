#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinSystem.h"
 #include <time.h>
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinSystemTestGroup)


NW_TEST(NanoWinSystemTestGroup, GetLocalTimeSimpleTest)
{
	SYSTEMTIME systemtime;

	systemtime.wYear          = 0xFF;
	systemtime.wMonth         = 0xFF;
	systemtime.wDayOfWeek     = 0xFF;
	systemtime.wDay           = 0xFF;
	systemtime.wHour          = 0xFF;
	systemtime.wMinute        = 0xFF;
	systemtime.wSecond        = 0xFF;
	systemtime.wMilliseconds  = 0xFF;

	GetLocalTime(&systemtime);

	NW_CHECK(systemtime.wYear          != 0xFF);
	NW_CHECK(systemtime.wMonth         != 0xFF);
	NW_CHECK(systemtime.wDayOfWeek     != 0xFF);
	NW_CHECK(systemtime.wDay           != 0xFF);
	NW_CHECK(systemtime.wHour          != 0xFF);
	NW_CHECK(systemtime.wMinute        != 0xFF);
	NW_CHECK(systemtime.wSecond        != 0xFF);
	NW_CHECK(systemtime.wMilliseconds  != 0xFF);
}

NW_TEST(NanoWinSystemTestGroup, GetSystemTimeSimpleTest)
{
	SYSTEMTIME systemtime;

	systemtime.wYear          = 0xFF;
	systemtime.wMonth         = 0xFF;
	systemtime.wDayOfWeek     = 0xFF;
	systemtime.wDay           = 0xFF;
	systemtime.wHour          = 0xFF;
	systemtime.wMinute        = 0xFF;
	systemtime.wSecond        = 0xFF;
	systemtime.wMilliseconds  = 0xFF;

	GetSystemTime(&systemtime);

	NW_CHECK(systemtime.wYear          != 0xFF);
	NW_CHECK(systemtime.wMonth         != 0xFF);
	NW_CHECK(systemtime.wDayOfWeek     != 0xFF);
	NW_CHECK(systemtime.wDay           != 0xFF);
	NW_CHECK(systemtime.wHour          != 0xFF);
	NW_CHECK(systemtime.wMinute        != 0xFF);
	NW_CHECK(systemtime.wSecond        != 0xFF);
	NW_CHECK(systemtime.wMilliseconds  != 0xFF);
}

NW_TEST(NanoWinSystemTestGroup, GetLocalTimeTest)
{
	SYSTEMTIME systemtime;

	struct tm structMyTime;

	time_t minTime = time(NULL);

	GetLocalTime(&systemtime);

	time_t maxTime = time(NULL);

	structMyTime.tm_year  = systemtime.wYear - 1900;
	structMyTime.tm_mon   = systemtime.wMonth - 1;
	structMyTime.tm_wday  = systemtime.wDayOfWeek;
	structMyTime.tm_mday  = systemtime.wDay;
	structMyTime.tm_hour  = systemtime.wHour;
	structMyTime.tm_min   = systemtime.wMinute;
	structMyTime.tm_sec   = systemtime.wSecond;
	structMyTime.tm_isdst = -1;

	time_t myTime = mktime(&structMyTime);

	NW_CHECK(myTime >= minTime);
	NW_CHECK(myTime <= maxTime);
}


NW_TEST(NanoWinSystemTestGroup, GetSystemTimeTest)
{
	SYSTEMTIME systemtime;

	struct tm structMyTime;

	time_t minTime = time(NULL);

	GetSystemTime(&systemtime);

	time_t maxTime = time(NULL);

	structMyTime.tm_year  = systemtime.wYear - 1900;
	structMyTime.tm_mon   = systemtime.wMonth - 1;
	structMyTime.tm_wday  = systemtime.wDayOfWeek;
	structMyTime.tm_mday  = systemtime.wDay;
	structMyTime.tm_hour  = systemtime.wHour;
	structMyTime.tm_min   = systemtime.wMinute;
	structMyTime.tm_sec   = systemtime.wSecond;
	structMyTime.tm_isdst = -1;

	time_t myTime = timegm(&structMyTime);

	NW_CHECK(myTime >= minTime);
	NW_CHECK(myTime <= maxTime);
}

NW_TEST(NanoWinSystemTestGroup, GetProcessMemoryInfoTest)
{
	PROCESS_MEMORY_COUNTERS MemCounters;

	int MemResult = 0;
	
	MemResult = GetProcessMemoryInfo(NULL, &MemCounters, sizeof(PROCESS_MEMORY_COUNTERS));

	NW_CHECK(MemResult != 0);
	
	NW_CHECK_EQUAL_BYTES(sizeof(MemCounters), MemCounters.cb);

	NW_CHECK(MemCounters.WorkingSetSize <= MemCounters.PeakWorkingSetSize);
	NW_CHECK(MemCounters.PagefileUsage  <= MemCounters.PeakPagefileUsage);
}

NW_TEST(NanoWinSystemTestGroup, GetProcessMemoryInfoExTest)
{
	PROCESS_MEMORY_COUNTERS_EX MemCounters;

	int MemResult = 0;

	MemResult = GetProcessMemoryInfo(NULL, (PROCESS_MEMORY_COUNTERS*)&MemCounters, sizeof(PROCESS_MEMORY_COUNTERS_EX));

	NW_CHECK(MemResult != 0);

	NW_CHECK_EQUAL_BYTES(sizeof(MemCounters), MemCounters.cb);

	NW_CHECK(MemCounters.WorkingSetSize <= MemCounters.PeakWorkingSetSize);
	NW_CHECK(MemCounters.PagefileUsage  <= MemCounters.PeakPagefileUsage);
}

NW_TEST(NanoWinSystemTestGroup, GetTimeFormatASystemDefaultTest)
{
  SYSTEMTIME systemtime;

  char buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 6;
  systemtime.wDayOfWeek = 1;
  systemtime.wDay = 26;
  systemtime.wHour = 20;
  systemtime.wMinute = 30;
  systemtime.wSecond = 40;
  systemtime.wMilliseconds = 0;

  int count = GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(12, count);
  NW_CHECK_EQUAL_STRCMP("08:30:40 PM", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetTimeFormatAUserDefaultTest)
{
  SYSTEMTIME systemtime;

  char buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 6;
  systemtime.wDayOfWeek = 1;
  systemtime.wDay = 26;
  systemtime.wHour = 0;
  systemtime.wMinute = 0;
  systemtime.wSecond = 0;
  systemtime.wMilliseconds = 0;

  int count = GetTimeFormatA(LOCALE_USER_DEFAULT, 0, &systemtime, NULL, buff, 64);
  
  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(12, count);
  NW_CHECK_EQUAL_STRCMP("12:00:00 AM", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetDateFormatASystemDefaultTest)
{
  SYSTEMTIME systemtime;

  char buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 6;
  systemtime.wDayOfWeek = 1;
  systemtime.wDay = 26;
  systemtime.wHour = 20;
  systemtime.wMinute = 30;
  systemtime.wSecond = 40;
  systemtime.wMilliseconds = 0;

  int count = GetDateFormatA(LOCALE_SYSTEM_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(9, count);
  NW_CHECK_EQUAL_STRCMP("06/26/17", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetDateFormatAUserDefaultTest)
{
  SYSTEMTIME systemtime;

  char buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 1;
  systemtime.wDayOfWeek = 4;
  systemtime.wDay = 1;
  systemtime.wHour = 0;
  systemtime.wMinute = 0;
  systemtime.wSecond = 0;
  systemtime.wMilliseconds = 0;

  int count = GetDateFormatA(LOCALE_USER_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(9, count);
  NW_CHECK_EQUAL_STRCMP("01/01/17", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetTimeFormatWSystemDefaultTest)
{
  SYSTEMTIME systemtime;

  wchar_t buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 6;
  systemtime.wDayOfWeek = 1;
  systemtime.wDay = 26;
  systemtime.wHour = 20;
  systemtime.wMinute = 30;
  systemtime.wSecond = 40;
  systemtime.wMilliseconds = 0;

  int count = GetTimeFormatW(LOCALE_SYSTEM_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(12, count);
  NW_CHECK_EQUAL_STRCMP(L"08:30:40 PM", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetTimeFormatWUserDefaultTest)
{
  SYSTEMTIME systemtime;

  wchar_t buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 6;
  systemtime.wDayOfWeek = 1;
  systemtime.wDay = 26;
  systemtime.wHour = 0;
  systemtime.wMinute = 0;
  systemtime.wSecond = 0;
  systemtime.wMilliseconds = 0;

  int count = GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(12, count);
  NW_CHECK_EQUAL_STRCMP(L"12:00:00 AM", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetDateFormatWSystemDefaultTest)
{
  SYSTEMTIME systemtime;

  wchar_t buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 6;
  systemtime.wDayOfWeek = 1;
  systemtime.wDay = 26;
  systemtime.wHour = 20;
  systemtime.wMinute = 30;
  systemtime.wSecond = 40;
  systemtime.wMilliseconds = 0;

  int count = GetDateFormatW(LOCALE_SYSTEM_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(9, count);
  NW_CHECK_EQUAL_STRCMP(L"06/26/17", buff);
}

NW_TEST(NanoWinSystemTestGroup, GetDateFormatWUserDefaultTest)
{
  SYSTEMTIME systemtime;

  wchar_t buff[128];

  systemtime.wYear = 2017;
  systemtime.wMonth = 1;
  systemtime.wDayOfWeek = 4;
  systemtime.wDay = 1;
  systemtime.wHour = 0;
  systemtime.wMinute = 0;
  systemtime.wSecond = 0;
  systemtime.wMilliseconds = 0;

  int count = GetDateFormatW(LOCALE_USER_DEFAULT, 0, &systemtime, NULL, buff, 64);

  NW_CHECK(count > 0);
  NW_CHECK_EQUAL(9, count);
  NW_CHECK_EQUAL_STRCMP(L"01/01/17", buff);
}

NW_END_TEST_GROUP()
