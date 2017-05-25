#include "NWUnitTest.h"

#ifdef __linux
 #include "errno.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
 #include "NanoWinError.h"
 #include "NanoWinShellEx.h"
#else
 #include <windows.h>
#endif

static bool NanoWinDirectoryExists(const char *dirName)
{
  struct stat info;

  return (stat(dirName,&info) == 0);
}

static const char *SHCreateDirectoryExTestsSubdirs[] =
{
  "shtopdir",
  "shtopdir/subdir1",
  "shtopdir/subdir1/",
  "shtopdir/subdir1/subsubdir1"
};

#define SHCreateDirectoryExTestsSubdirsCount \
 (sizeof(SHCreateDirectoryExTestsSubdirs) / sizeof(SHCreateDirectoryExTestsSubdirs[0]))

static void SHCreateDirectoryExTestsSubdirsRemove()
{
  for (int i = SHCreateDirectoryExTestsSubdirsCount - 1; i >= 0; i--)
  {
    rmdir(SHCreateDirectoryExTestsSubdirs[i]);
  }
}

NW_BEGIN_TEST_GROUP(NanoWinSHCreateDirectoryExTestGroup)

NW_BEGIN_SETUP_TEARDOWN
NW_SETUP_METHOD()
 {
  SHCreateDirectoryExTestsSubdirsRemove();
 }

NW_TEARDOWN_METHOD()
 {
  SHCreateDirectoryExTestsSubdirsRemove();
 }
NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinSHCreateDirectoryExTestGroup, SHCreateDirectoryASingleDirTest)
{
  int result = SHCreateDirectoryExA(NULL,SHCreateDirectoryExTestsSubdirs[0],NULL);

  NW_CHECK_EQUAL_INTS(ERROR_SUCCESS,result);
  NW_CHECK_TRUE(NanoWinDirectoryExists(SHCreateDirectoryExTestsSubdirs[0]));
}

NW_TEST(NanoWinSHCreateDirectoryExTestGroup, SHCreateDirectorySubDirOnly)
{
  NW_CHECK(mkdir(SHCreateDirectoryExTestsSubdirs[0],0777) == 0);

  int result = SHCreateDirectoryExA(NULL,SHCreateDirectoryExTestsSubdirs[1],NULL);

  NW_CHECK_EQUAL_INTS(ERROR_SUCCESS,result);
  NW_CHECK_TRUE(NanoWinDirectoryExists(SHCreateDirectoryExTestsSubdirs[1]));
}

NW_TEST(NanoWinSHCreateDirectoryExTestGroup, SHCreateDirectorySubDirOnlyTrailingSlashTest)
{
  NW_CHECK(mkdir(SHCreateDirectoryExTestsSubdirs[0],0777) == 0);

  int result = SHCreateDirectoryExA(NULL,SHCreateDirectoryExTestsSubdirs[2],NULL);

  NW_CHECK_EQUAL_INTS(ERROR_SUCCESS,result);
  NW_CHECK_TRUE(NanoWinDirectoryExists(SHCreateDirectoryExTestsSubdirs[2]));
}

NW_TEST(NanoWinSHCreateDirectoryExTestGroup, SHCreateDirectoryDeepSubDir)
{
  int result = SHCreateDirectoryExA(NULL,SHCreateDirectoryExTestsSubdirs[3],NULL);

  NW_CHECK_EQUAL_INTS(ERROR_SUCCESS,result);
  NW_CHECK_TRUE(NanoWinDirectoryExists(SHCreateDirectoryExTestsSubdirs[3]));
}

NW_TEST(NanoWinSHCreateDirectoryExTestGroup, SHCreateDirectoryDeepSubDirBackslashesTest)
{
  int result = SHCreateDirectoryExA(NULL,"shtopdir\\subdir1\\subsubdir1",NULL);
  
  NW_CHECK_EQUAL_INTS(ERROR_SUCCESS,result);
  NW_CHECK_TRUE(NanoWinDirectoryExists("shtopdir\\subdir1\\subsubdir1"));
}

NW_TEST(NanoWinSHCreateDirectoryExTestGroup, SHCreateDirectoryAlreadyExistsTest)
{
  NW_CHECK(mkdir(SHCreateDirectoryExTestsSubdirs[0],0777) == 0);
  NW_CHECK(mkdir(SHCreateDirectoryExTestsSubdirs[1],0777) == 0);

  int result = SHCreateDirectoryExA(NULL,SHCreateDirectoryExTestsSubdirs[1],NULL);

  NW_CHECK_EQUAL_INTS(ERROR_ALREADY_EXISTS,result);
}

NW_END_TEST_GROUP()
