#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinModule.h"
 #include <string.h>
#else
 #include <windows.h>
#endif

#define FILE_NAME_A   "TestsLinux.out"
#define FILE_NAME_W   L"TestsLinux.out"

static bool isStrA(char *srcStr, const char *strToFind)
{
  bool res;

  long unsigned int srcStrLen    = strlen(srcStr);
  long unsigned int strToFindLen = strlen(strToFind);

  if (strToFindLen <= srcStrLen)
  {
    res = strcmp(srcStr + srcStrLen - strToFindLen, strToFind) == 0;
  }

  else
  {
    res = false;
  }

  return res;
}

static bool isStrW(wchar_t *srcStr, const wchar_t *strToFind)
{
  bool res;

  long unsigned int srcStrLen = wcslen(srcStr);
  long unsigned int strToFindLen = wcslen(strToFind);

  if (strToFindLen <= srcStrLen)
  {
    res = wcscmp(srcStr + srcStrLen - strToFindLen,strToFind) == 0;
  }
  else
  {
    res = false;
  }

  return res;
}

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinModuleTestGroup)

NW_TEST(NanoWinModuleTestGroup, GetModuleFileNameATest)
{
  int  BUFF_SIZE = 256;
  char buff[BUFF_SIZE];

  int size = GetModuleFileNameA(NULL, buff, BUFF_SIZE);

  NW_CHECK(size != 0);
  NW_CHECK_TRUE(isStrA(buff, FILE_NAME_A));
}

NW_TEST(NanoWinModuleTestGroup, GetModuleFileNameASmallBuffTest)
{
  int  BUFF_SIZE = 256;
  int  SMALL_BUFF_SIZE = 8;
  
  char buff1[BUFF_SIZE];
  char buff2[SMALL_BUFF_SIZE];

  int size1 = GetModuleFileNameA(NULL, buff1, BUFF_SIZE);
  int size2 = GetModuleFileNameA(NULL, buff2, SMALL_BUFF_SIZE);

  NW_CHECK(size1 != 0);
  NW_CHECK_EQUAL(SMALL_BUFF_SIZE, size2);

  for (int i = 0; i < size2 - 1; i++)
  {
    NW_CHECK_EQUAL(buff1[i], buff2[i]);
  }
}

NW_TEST(NanoWinModuleTestGroup, GetModuleFileNameWTest)
{
  int  BUFF_SIZE = 256;
  wchar_t buff[BUFF_SIZE];

  int size = GetModuleFileNameW(NULL, buff, BUFF_SIZE);

  NW_CHECK(size != 0);
  NW_CHECK_TRUE(isStrW(buff, FILE_NAME_W));
}

NW_TEST(NanoWinModuleTestGroup, GetModuleFileNameWSmallBuffTest)
{
  int  BUFF_SIZE = 256;
  int  SMALL_BUFF_SIZE = 8;

  wchar_t buff1[BUFF_SIZE];
  wchar_t buff2[SMALL_BUFF_SIZE];

  int size1 = GetModuleFileNameW(NULL, buff1, BUFF_SIZE);
  int size2 = GetModuleFileNameW(NULL, buff2, SMALL_BUFF_SIZE);

  NW_CHECK(size1 != 0);
  NW_CHECK_EQUAL(SMALL_BUFF_SIZE, size2);

  for (int i = 0; i < size2 - 1; i++)
  {
    NW_CHECK(buff1[i] == buff2[i]);
  }
}

NW_END_TEST_GROUP()