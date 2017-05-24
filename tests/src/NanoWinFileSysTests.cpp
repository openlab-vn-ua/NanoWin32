#include "NWUnitTest.h"

#ifdef __linux
 #include "unistd.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include "NanoWinFileSys.h"
#else
 #error "unsupported platform"
// #include <windows.h>
#endif

static void CreateFile(const int size, const char *fileName, const char symbol)
{
	FILE *file;

	file = fopen(fileName, "w");

	if (file != NULL)
	{
		for (int i = 0; i < size; i++)
		{
			fputc(symbol, file);
		}
	}

	fclose(file);
}

static void RemoveTestDir()
{
	unlink("testDir/testFile256.txt");
	unlink("testDir/testFile256-copy.txt");
	unlink("testDir/testFile512.txt");
	rmdir("testDir");
}

static void CreateTestDir()
{
	mkdir("testDir", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	CreateFile(512, "testDir/testFile512.txt", 'a');
	CreateFile(256, "testDir/testFile256.txt", 'a');
}

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

NW_BEGIN_TEST_GROUP(NanoWinFileSysCopyFileATestGroup)

NW_BEGIN_SETUP_TEARDOWN

NW_SETUP_METHOD()
{
  CreateTestDir();
}

NW_TEARDOWN_METHOD()
{
  RemoveTestDir();
}

NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinFileSysCopyFileATestGroup, UnexistingFileCopyTest)
{
  int    copyRes = CopyFileA("testDir/unexistFile.txt", "testDir/testFile111.txt", FALSE);

  NW_CHECK(copyRes == 0);
  
  struct stat statFileUnexist;
  int    statRes = stat("testDir/testFile111.txt", &statFileUnexist);

  NW_CHECK_EQUAL(-1, statRes);
}

NW_TEST(NanoWinFileSysCopyFileATestGroup, FileCopySimpleTest)
{
  struct stat statFileSource;
  struct stat statFileCopy;

  chmod("testDir/testFile256.txt", S_IRUSR | S_IWUSR | S_IRGRP);

  int copyRes = CopyFileA("testDir/testFile256.txt", "testDir/testFile256-copy.txt", FALSE);

  NW_CHECK(copyRes != 0);

  stat("testDir/testFile256.txt",      &statFileSource);
  stat("testDir/testFile256-copy.txt", &statFileCopy);

  NW_CHECK_EQUAL_BYTES(statFileSource.st_size, statFileCopy.st_size);

  NW_CHECK_EQUAL(statFileSource.st_uid,  statFileCopy.st_uid);
  NW_CHECK_EQUAL(statFileSource.st_gid,  statFileCopy.st_gid);
  NW_CHECK_EQUAL(statFileSource.st_mode, statFileCopy.st_mode);
}

NW_TEST(NanoWinFileSysCopyFileATestGroup, FileCopyExistingFileNotRewriteTest)
{
  int copyRes = CopyFileA("testDir/testFile256.txt", "testDir/testFile512.txt", TRUE);

  NW_CHECK(copyRes == 0);
}

NW_TEST(NanoWinFileSysCopyFileATestGroup, FileCopyExistingFileRewritingTest)
{
  struct stat statFileSource;
  struct stat statFileCopy;

  int copyRes = CopyFileA("testDir/testFile256.txt", "testDir/testFile512.txt", FALSE);

  NW_CHECK(copyRes != 0);

  stat("testDir/testFile256.txt", &statFileSource);
  stat("testDir/testFile512.txt", &statFileCopy);

  NW_CHECK_EQUAL_BYTES(statFileSource.st_size, statFileCopy.st_size);

  NW_CHECK_EQUAL(statFileSource.st_uid,  statFileCopy.st_uid);
  NW_CHECK_EQUAL(statFileSource.st_gid,  statFileCopy.st_gid);
  NW_CHECK_EQUAL(statFileSource.st_mode, statFileCopy.st_mode);
}

NW_END_TEST_GROUP()