#include "NWUnitTest.h"

#ifdef __linux
 #include "unistd.h"
 #include "NanoWinFileSys.h"
#else
 #error "unsupported platform"
// #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinFileSysGetCurrDirTestGroup)

NW_TEST(NanoWinFileSysGetCurrDirTestGroup, GetCurrentDirectoryASimpleTest)
{
  char initialDir[PATH_MAX];
  char testDir[PATH_MAX];

  NW_CHECK(getcwd(initialDir, sizeof(initialDir)) != NULL);
  NW_CHECK(chdir("/etc") == 0)

  DWORD result = GetCurrentDirectoryA(sizeof(testDir),testDir);

  NW_CHECK_EQUAL_ULONGS(4, result);
  NW_CHECK_EQUAL_STRCMP("/etc", testDir);

  NW_CHECK(chdir(initialDir) == 0);
}

NW_TEST(NanoWinFileSysGetCurrDirTestGroup, GetCurrentDirectoryAGetReqBufferSizeTest)
{
  char initialDir[PATH_MAX];

  NW_CHECK(getcwd(initialDir, sizeof(initialDir)) != NULL);
  NW_CHECK(chdir("/etc") == 0)

  DWORD result = GetCurrentDirectoryA(0,NULL);

  NW_CHECK_EQUAL_ULONGS(5, result);

  NW_CHECK(chdir(initialDir) == 0);
}

NW_TEST(NanoWinFileSysGetCurrDirTestGroup, GetCurrentDirectoryASmallBufferSizeTest)
{
  char initialDir[PATH_MAX];
  char testDir[4];

  NW_CHECK(getcwd(initialDir, sizeof(initialDir)) != NULL);
  NW_CHECK(chdir("/etc") == 0)

  DWORD result = GetCurrentDirectoryA(sizeof(testDir),testDir);

  NW_CHECK_EQUAL_ULONGS(5, result);

  NW_CHECK(chdir(initialDir) == 0);
}

NW_TEST(NanoWinFileSysGetCurrDirTestGroup, GetCurrentDirectoryWSimpleTest)
{
  char initialDir[PATH_MAX];
  WCHAR testDir[PATH_MAX];

  NW_CHECK(getcwd(initialDir, sizeof(initialDir)) != NULL);
  NW_CHECK(chdir("/etc") == 0)

  DWORD result = GetCurrentDirectoryW(sizeof(testDir),testDir);

  NW_CHECK_EQUAL_ULONGS(4, result);
  NW_CHECK_EQUAL_MEMCMP(L"/etc", testDir, 10);

  NW_CHECK(chdir(initialDir) == 0);
}

NW_TEST(NanoWinFileSysGetCurrDirTestGroup, GetCurrentDirectoryWGetReqBufferSizeTest)
{
  char initialDir[PATH_MAX];

  NW_CHECK(getcwd(initialDir, sizeof(initialDir)) != NULL);
  NW_CHECK(chdir("/etc") == 0)

  DWORD result = GetCurrentDirectoryW(0,NULL);

  NW_CHECK_EQUAL_ULONGS(5, result);

  NW_CHECK(chdir(initialDir) == 0);
}

NW_TEST(NanoWinFileSysGetCurrDirTestGroup, GetCurrentDirectoryWSmallBufferSizeTest)
{
  char initialDir[PATH_MAX];
  WCHAR testDir[4];

  NW_CHECK(getcwd(initialDir, sizeof(initialDir)) != NULL);
  NW_CHECK(chdir("/etc") == 0)

  DWORD result = GetCurrentDirectoryW(sizeof(testDir) / sizeof(testDir[0]),testDir);

  NW_CHECK_EQUAL_ULONGS(5, result);

  NW_CHECK(chdir(initialDir) == 0);
}

NW_END_TEST_GROUP()
