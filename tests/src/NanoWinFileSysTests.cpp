#include "NWUnitTest.h"

#ifdef __linux
 #include "unistd.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include "NanoWinFileSys.h"
 #include "NanoWinStrConvert.h"
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
		fclose(file);
	}
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

static void CreateTestDirForFileSysMoveFileA()
{
	mkdir("testDirForFileSysMoveFileA_0", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	mkdir("testDirForFileSysMoveFileA_1", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	CreateFile(256, "testDirForFileSysMoveFileA_0/testFile", 'a');
}

static void RemoveTestDirForFileSysMoveFileA()
{
	unlink("testDirForFileSysMoveFileA_0/testFile");
	unlink("testDirForFileSysMoveFileA_1/testFileMoved");
	unlink("testDirForFileSysMoveFileA_0/testFileRenamed");

	rmdir("testDirForFileSysMoveFileA_0");
	rmdir("testDirForFileSysMoveFileA_1");
}

static void CreateTestDirForFileSysMoveFileW()
{
	mkdir(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_0").c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	mkdir(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_1").c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	CreateFile(256, NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_0/testFile").c_str(), 'a');
}

static void RemoveTestDirForFileSysMoveFileW()
{
	
	unlink(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_0/testFile").c_str());
	unlink(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_1/testFileMoved").c_str());
	unlink(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_0/testFileRenamed").c_str());

	rmdir(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_0").c_str());
	rmdir(NanoWin::WStrToStrClone(L"testDirForFileSysMoveFileW_1").c_str());
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

NW_BEGIN_TEST_GROUP(NanoWinFileSysMoveFileATestGroup)

NW_BEGIN_SETUP_TEARDOWN

NW_SETUP_METHOD()
{
	CreateTestDirForFileSysMoveFileA();
}

NW_TEARDOWN_METHOD()
{
	RemoveTestDirForFileSysMoveFileA();
}

NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinFileSysMoveFileATestGroup, FileMoveSimpleTest)
{
	char source[] = "testDirForFileSysMoveFileA_0/testFile";
	char destination[] = "testDirForFileSysMoveFileA_1/testFileMoved";

	struct stat statFileSource;
	struct stat statFileMoved;

	stat(source, &statFileSource);

	BOOL result = MoveFileA(source, destination);

	NW_CHECK(result);

	stat(destination, &statFileMoved);

	NW_CHECK_EQUAL_BYTES(statFileSource.st_size, statFileMoved.st_size);

	NW_CHECK_EQUAL(statFileSource.st_uid, statFileMoved.st_uid);
	NW_CHECK_EQUAL(statFileSource.st_gid, statFileMoved.st_gid);
	NW_CHECK_EQUAL(statFileSource.st_mode, statFileMoved.st_mode);
}

NW_TEST(NanoWinFileSysMoveFileATestGroup, FileRenameTest)
{
	char source[]      = "testDirForFileSysMoveFileA_0/testFile";
	char destination[] = "testDirForFileSysMoveFileA_0/testFileRenamed";
	
	struct stat statFileSource;
	struct stat statFileMoved;

	stat(source, &statFileSource);

	BOOL result = MoveFileA(source, destination);
	NW_CHECK(result);

	stat(destination, &statFileMoved);

	NW_CHECK_EQUAL_BYTES(statFileSource.st_size, statFileMoved.st_size);

	NW_CHECK_EQUAL(statFileSource.st_uid, statFileMoved.st_uid);
	NW_CHECK_EQUAL(statFileSource.st_gid, statFileMoved.st_gid);
	NW_CHECK_EQUAL(statFileSource.st_mode, statFileMoved.st_mode);
}

NW_TEST(NanoWinFileSysMoveFileATestGroup, UnexistingFileMoveTest)
{
	char source[]      = "testDirForFileSysMoveFileA_0/unexistFile";
	char destination[] = "testDirForFileSysMoveFileA_1/testUnexistFileRenamed";

	BOOL result = MoveFileA(source, destination);
	NW_CHECK(!result);
}

NW_TEST(NanoWinFileSysMoveFileATestGroup, FileMoveToUnexistingDirTest)
{
	char source[]      = "testDirForFileSysMoveFileA_0/testFile";
	char destination[] = "testDirForFileSysMoveFileA_unexist/testFileMoved";

	BOOL result = MoveFileA(source, destination);
	NW_CHECK(!result);
}

NW_END_TEST_GROUP()

NW_BEGIN_TEST_GROUP(NanoWinFileSysMoveFileWTestGroup)

NW_BEGIN_SETUP_TEARDOWN

NW_SETUP_METHOD()
{
	CreateTestDirForFileSysMoveFileW();
}

NW_TEARDOWN_METHOD()
{
	RemoveTestDirForFileSysMoveFileW();
}

NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinFileSysMoveFileWTestGroup, FileMoveSimpleTest)
{
	const wchar_t source[] = L"testDirForFileSysMoveFileW_0/testFile";
	const wchar_t destination[] = L"testDirForFileSysMoveFileW_1/testFileMoved";

	struct stat statFileSource;
	struct stat statFileMoved;

	stat(NanoWin::WStrToStrClone(source).c_str(), &statFileSource);

	BOOL result = MoveFileW(source, destination);
	
	NW_CHECK(result);

	stat(NanoWin::WStrToStrClone(destination).c_str(), &statFileMoved);

	NW_CHECK_EQUAL_BYTES(statFileSource.st_size, statFileMoved.st_size);

	NW_CHECK_EQUAL(statFileSource.st_uid, statFileMoved.st_uid);
	NW_CHECK_EQUAL(statFileSource.st_gid, statFileMoved.st_gid);
	NW_CHECK_EQUAL(statFileSource.st_mode, statFileMoved.st_mode);
}

NW_TEST(NanoWinFileSysMoveFileWTestGroup, FileRenameTest)
{
	const wchar_t source[]      = L"testDirForFileSysMoveFileW_0/testFile";
	const wchar_t destination[] = L"testDirForFileSysMoveFileW_0/testFileRenamed";
	
	struct stat statFileSource;
	struct stat statFileMoved;

	stat(NanoWin::WStrToStrClone(source).c_str(), &statFileSource);

	BOOL result = MoveFileW(source, destination);
	NW_CHECK(result);

	stat(NanoWin::WStrToStrClone(destination).c_str(), &statFileMoved);

	NW_CHECK_EQUAL_BYTES(statFileSource.st_size, statFileMoved.st_size);

	NW_CHECK_EQUAL(statFileSource.st_uid, statFileMoved.st_uid);
	NW_CHECK_EQUAL(statFileSource.st_gid, statFileMoved.st_gid);
	NW_CHECK_EQUAL(statFileSource.st_mode, statFileMoved.st_mode);
}

NW_TEST(NanoWinFileSysMoveFileWTestGroup, UnexistingFileMoveTest)
{
	wchar_t source[]      = L"testDirForFileSysMoveFileW_0/unexistFile";
	wchar_t destination[] = L"testDirForFileSysMoveFileW_1/testUnexistFileRenamed";

	BOOL result = MoveFileW(source, destination);
	NW_CHECK(!result);
}

NW_TEST(NanoWinFileSysMoveFileWTestGroup, FileMoveToUnexistingDirTest)
{
	wchar_t source[]      = L"testDirForFileSysMoveFileW_0/testFile";
	wchar_t destination[] = L"testDirForFileSysMoveFileW_unexist/testFileMoved";

	BOOL result = MoveFileW(source, destination);
	NW_CHECK(!result);
}

#if 0

// this one should lead to link error
// "undefined reference to "__wrap_basename" if path translation is active
// (this is done intentionaly, since basename from string.h is not translated)

#include <string.h>

NW_TEST(NanoWinFileSysMoveFileWTestGroup, BasenameStringTest)
{
    // This one can have arg as const char *
    NW_CHECK_EQUAL_STRCMP("myfile.txt", basename("nydir/myfile.txt"));
}

#endif

#include <libgen.h>

static bool IsPathTranslationActive()
{
  #ifdef LINUX
  char input[] = { '\\', 0 };
  return(strcmp(dirname(input),"/") == 0); // path was translated
  #else
  return(false);
  #endif
}

NW_TEST(NanoWinFileSysMoveFileWTestGroup, BasenameTest)
{
  #define TEST_STEP(input, expect_dirname, expect_basename) \
  { \
    char *input_tmp; \
    const char *result_dirname; \
    const char *result_basename; \
    input_tmp = strdup(input); \
    result_dirname = dirname(input_tmp); \
    NW_CHECK_EQUAL_STRCMP(expect_dirname, result_dirname); \
    free(input_tmp); \
    input_tmp = strdup(input); \
    result_basename = basename(input_tmp); \
    NW_CHECK_EQUAL_STRCMP(expect_basename, result_basename); \
    free(input_tmp); \
  }

  // from man page

  TEST_STEP("/usr/lib",    "/usr",    "lib");
  TEST_STEP("/usr/",       "/",       "usr");
  TEST_STEP("usr",         ".",       "usr");
  TEST_STEP("/",           "/",       "/");
  TEST_STEP(".",           ".",       ".");
  TEST_STEP("..",          ".",       "..");

  // additional

  TEST_STEP("mypath/myfile.txt", "mypath", "myfile.txt");
  TEST_STEP("mypath/../myfile.txt", "mypath/..", "myfile.txt");

  if (!IsPathTranslationActive()) { return; }

  // Test translation

  TEST_STEP("mypath\\myfile.txt", "mypath", "myfile.txt");
  TEST_STEP("mypath\\..\\myfile.txt", "mypath/..", "myfile.txt");

  #undef TEST_STEP
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP(NanoWinFileSysSetCurrDirTestGroup)

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

NW_TEST(NanoWinFileSysSetCurrDirTestGroup, SetCurrentDirectoryAUnexistTest)
{
  bool res = SetCurrentDirectoryA("/unexist/dir");
  NW_CHECK_FALSE(res);
}

NW_TEST(NanoWinFileSysSetCurrDirTestGroup, SetCurrentDirectoryATest)
{
  int  BUFF_SIZE = 512;
  char buff[BUFF_SIZE];

  bool res = SetCurrentDirectoryA("/home/vmbuilder/testDir");
  NW_CHECK_TRUE(res);

  if (getcwd(buff, BUFF_SIZE) != NULL)
  {
    NW_CHECK_EQUAL_STRCMP(buff, "/home/vmbuilder/testDir");
  }
}

NW_TEST(NanoWinFileSysSetCurrDirTestGroup, SetCurrentDirectoryWUnexistTest)
{
  bool res = SetCurrentDirectoryW(L"/unexist/dir");
  NW_CHECK_FALSE(res);
}

NW_TEST(NanoWinFileSysSetCurrDirTestGroup, SetCurrentDirectoryWTest)
{
  int     BUFF_SIZE = 512;
  char    buff[BUFF_SIZE];

  bool res = SetCurrentDirectoryW(L"/home/vmbuilder/testDir");
  NW_CHECK_TRUE(res);

  if (getcwd(buff, BUFF_SIZE) != NULL)
  {
    NW_CHECK_EQUAL_STRCMP(buff, "/home/vmbuilder/testDir");
  }
}

NW_END_TEST_GROUP()