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

NW_END_TEST_GROUP()