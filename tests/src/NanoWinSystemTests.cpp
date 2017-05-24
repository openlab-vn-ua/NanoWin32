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

NW_END_TEST_GROUP()
