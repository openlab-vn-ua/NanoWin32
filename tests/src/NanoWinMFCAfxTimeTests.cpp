#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxTime.h"
 #include <time.h>
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxTimeTestGroup)

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCTimeSimpleTest)
{
  time_t timeT = time(NULL);
  char buff[64];
  struct tm *structTime = localtime(&timeT);

  CTime checkTime = CTime::GetCurrentTime();

  CString formatResult = checkTime.Format(_T("%Y %02d"));
  sprintf(buff, "%d %02d", structTime->tm_year+1900, structTime->tm_mday);

  NW_CHECK_EQUAL_STRCMP(CA2T(buff), formatResult.GetString());
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCTimeTest)
{
  time_t timeT = time(NULL);
  char buff[64];
  struct tm *structTime = localtime(&timeT);

  CTime checkTime = CTime::GetCurrentTime();

  CString formatResult = checkTime.Format(_T("year %Y, day of month %d, %H:%M"));
  sprintf(buff, "year %d, day of month %02d, %02d:%02d", structTime->tm_year + 1900, structTime->tm_mday, structTime->tm_hour, structTime->tm_min);

  NW_CHECK_EQUAL_STRCMP(CA2T(buff), formatResult.GetString());

  formatResult = checkTime.Format(_T("%w %j"));
  sprintf(buff, "%d %03d", structTime->tm_wday, structTime->tm_yday + 1);

  NW_CHECK_EQUAL_STRCMP(CA2T(buff), formatResult.GetString());
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCGetYearTest)
{
  time_t timeT = time(NULL);
  struct tm *structTime = localtime(&timeT);

  CTime checkTime = CTime::GetCurrentTime();

  int year = structTime->tm_year + 1900;
  int res  = checkTime.GetYear();

  NW_CHECK_EQUAL_INTS(year, res);
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCGetMonthTest)
{
  time_t timeT = time(NULL);
  struct tm *structTime = localtime(&timeT);

  CTime checkTime = CTime::GetCurrentTime();

  int month = structTime->tm_mon + 1;
  int res   = checkTime.GetMonth();

  NW_CHECK_EQUAL_INTS(month, res);
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCGetDayTest)
{
  time_t timeT = time(NULL);
  struct tm *structTime = localtime(&timeT);

  CTime checkTime = CTime::GetCurrentTime();

  int day = structTime->tm_mday;
  int res = checkTime.GetDay();

  NW_CHECK_EQUAL_INTS(day, res);
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCGetDayOfWeekTest)
{
  time_t timeT = time(NULL);
  struct tm *structTime = localtime(&timeT);

  CTime checkTime = CTime::GetCurrentTime();

  int day = structTime->tm_wday + 1;
  int res = checkTime.GetDayOfWeek();

  NW_CHECK_EQUAL_INTS(day, res);
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCSystemtimeTest)
{
  SYSTEMTIME systemtime;
  
  systemtime.wYear = 2017;
  systemtime.wMonth = 9;
  systemtime.wDayOfWeek = 2;
  systemtime.wDay = 5;
  systemtime.wHour = 10;
  systemtime.wMinute = 15;
  systemtime.wSecond = 30;
  
  CTime checkTime = CTime(systemtime);

  NW_CHECK_EQUAL(2017, checkTime.GetYear());
  NW_CHECK_EQUAL(9,    checkTime.GetMonth());
  NW_CHECK_EQUAL(2,    checkTime.GetDayOfWeek() - 1);
  NW_CHECK_EQUAL(5,    checkTime.GetDay());
  NW_CHECK_EQUAL(10,   checkTime.GetHour());
  NW_CHECK_EQUAL(15,   checkTime.GetMinute());
  NW_CHECK_EQUAL(30,   checkTime.GetSecond());
}

NW_TEST(NanoWinMFCAfxTimeTestGroup, MFCGetLocalTmTest)
{
  SYSTEMTIME systemtime;
  struct tm *structTm;

  systemtime.wYear = 2017;
  systemtime.wMonth = 9;
  systemtime.wDayOfWeek = 2;
  systemtime.wDay = 5;
  systemtime.wHour = 10;
  systemtime.wMinute = 15;
  systemtime.wSecond = 30;

  CTime checkTime = CTime(systemtime);

  checkTime.GetLocalTm(structTm);

  NW_CHECK_EQUAL(2017, structTm->tm_year + 1900);
  NW_CHECK_EQUAL(9, structTm->tm_mon + 1);
  NW_CHECK_EQUAL(2, structTm->tm_wday);
  NW_CHECK_EQUAL(5, structTm->tm_mday);
  NW_CHECK_EQUAL(10, structTm->tm_hour);
  NW_CHECK_EQUAL(15, structTm->tm_min);
  NW_CHECK_EQUAL(30, structTm->tm_sec);
}

NW_END_TEST_GROUP()