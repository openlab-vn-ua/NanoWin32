#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinError.h"
 #include "NanoWinOther.h"
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinErrorTestGroup)

NW_TEST(NanoWinErrorTestGroup, FormatMessageATest)
{
  char buff[128];

  DWORD res = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL, 
                             ERROR_INVALID_FUNCTION,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res != 0);
  NW_CHECK_EQUAL_STRCMP("Incorrect function", buff);
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageAUnexistErrorTest)
{
  char buff[128];

  DWORD res = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             10000,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res != 0);
  NW_CHECK_EQUAL_STRCMP("Generic error (10000)", buff);
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageAUnexistErrSmallBuffTest)
{
  char buff[8];

  DWORD res = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             10000,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res == 0);
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageASmallBuffTest)
{
  char buff[8];

  DWORD res = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             ERROR_NOT_SUPPORTED,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res == 0);
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageAAllocateBuffTest)
{
  char *buff;

  DWORD res = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             ERROR_NOT_ENOUGH_MEMORY,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             (LPSTR)&buff,
                             0,
                             NULL);

  NW_CHECK_TRUE(res != 0);
  NW_CHECK_EQUAL_STRCMP("Not enough storage is available to process this command", buff);

  LocalFree(buff);
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageWTest)
{
  wchar_t buff[128];

  DWORD res = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             ERROR_RESOURCE_LANG_NOT_FOUND,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res != 0);
  NW_CHECK_EQUAL_MEMCMP(L"The specified resource language ID cannot be found in the image file", buff, res * sizeof(wchar_t));
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageWUnexistErrorTest)
{
  wchar_t buff[128];

  DWORD res = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             10000,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res != 0);
  NW_CHECK_EQUAL_MEMCMP(L"Generic error (10000)", buff, res * sizeof(wchar_t));
}
#ifndef __GNUC__
NW_TEST(NanoWinErrorTestGroup, FormatMessageWUnexistErrSmallBuffTest)
{
  wchar_t buff[8];

  DWORD res = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             10000,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res == 0);
}
#endif
NW_TEST(NanoWinErrorTestGroup, FormatMessageWSmallBuffTest)
{
  wchar_t buff[8];

  DWORD res = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             ERROR_INSUFFICIENT_BUFFER,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             buff,
                             sizeof(buff),
                             NULL);

  NW_CHECK_TRUE(res == 0);
}

NW_TEST(NanoWinErrorTestGroup, FormatMessageWAllocateBuffTest)
{
  wchar_t *buff;
  
  DWORD res = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             ERROR_ACCESS_DENIED,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             (LPWSTR)&buff,
                             0,
                             NULL);

  NW_CHECK_TRUE(res != 0);
  NW_CHECK_EQUAL_MEMCMP(L"Access is denied", buff, res * sizeof(wchar_t));

  LocalFree(buff);
}

NW_END_TEST_GROUP()