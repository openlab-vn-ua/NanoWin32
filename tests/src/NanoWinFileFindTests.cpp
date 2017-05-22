#include "NWUnitTest.h"

#ifdef __linux
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "NanoWinFileFind.h"
#else
#include <windows.h>
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

static void RemoveTestDir ()
{
	unlink  ("testDir/testSubDir");
	rmdir   ("testDir/testSubDir");

	unlink  ("testDir");
	rmdir   ("testDir");
}

static void CreateTestDir ()
{
	mkdir  ("testDir",            S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	mkdir  ("testDir/testSubDir", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	CreateFile(16,   "testDir/testSubDir/testFile16.txt", 'a');
	CreateFile(512,  "testDir/testSubDir/testFile512.txt", 'a');
	CreateFile(512,  "testDir/testFile512.txt", 'a');
	CreateFile(256,  "testDir/testFile256.txt", 'a');
}

NW_BEGIN_TEST_GROUP(NanoWinFileFindTestGroup)

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

NW_TEST(NanoWinFileFindTestGroup, FindFileFirstAUnexistTest)
{
	
	WIN32_FIND_DATA state;

	HANDLE result = FindFirstFileA("testDir/unexist.txt", &state);

	NW_CHECK_EQUAL(result, INVALID_HANDLE_VALUE);

	FindClose(result);
}

NW_TEST(NanoWinFileFindTestGroup, FindFileFirstATest)
{
	WIN32_FIND_DATA state;
	char            firstFoundFileName   [WIN32_FIND_DATA_FNAME_MAX];
	char            secondFoundFileName  [WIN32_FIND_DATA_FNAME_MAX];

	HANDLE searchHandle = FindFirstFileA("testDir/testFile5*", &state);

	NW_CHECK(searchHandle != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL_STRCMP("testFile512.txt", state.cFileName);

	NW_CHECK_FALSE(FindNextFileA(searchHandle, &state));

	FindClose(searchHandle);
}

NW_TEST(NanoWinFileFindTestGroup, FindFileNextATest)
{

	WIN32_FIND_DATA state;
	char            firstFoundFileName   [WIN32_FIND_DATA_FNAME_MAX];
	char            secondFoundFileName  [WIN32_FIND_DATA_FNAME_MAX];

	HANDLE searchHandle = FindFirstFileA("testDir/testSubDir/testFile*.*", &state);

	NW_CHECK(searchHandle != INVALID_HANDLE_VALUE);

	strcpy(firstFoundFileName, state.cFileName);

	NW_CHECK_TRUE(FindNextFileA(searchHandle, &state));

	strcpy(secondFoundFileName, state.cFileName);

	NW_CHECK_FALSE(FindNextFileA(searchHandle, &state));

	if (strcmp(firstFoundFileName, "testFile16.txt") == 0)
	{
		NW_CHECK_EQUAL_STRCMP("testFile16.txt",  firstFoundFileName);
		NW_CHECK_EQUAL_STRCMP("testFile512.txt", secondFoundFileName);
	}
	else
	{
		NW_CHECK_EQUAL_STRCMP("testFile512.txt", firstFoundFileName);
		NW_CHECK_EQUAL_STRCMP("testFile16.txt",  secondFoundFileName);
	}

	FindClose(searchHandle);
}

NW_TEST(NanoWinFileFindTestGroup, FindFileNextAFileSizeTest)
{
	WIN32_FIND_DATA state;
	DWORD           fileSizeHigh;
	DWORD           fileSizeLow;

	HANDLE searchHandle = FindFirstFileA("testDir/testSubDir/testFile1*", &state);

	NW_CHECK(searchHandle != INVALID_HANDLE_VALUE);

	fileSizeHigh = state.nFileSizeHigh;
	fileSizeLow  = state.nFileSizeLow;

	NW_CHECK_EQUAL(0, fileSizeHigh);
	NW_CHECK_EQUAL(16, fileSizeLow);

	FindClose(searchHandle);

	searchHandle = FindFirstFileA("testDir/testSubDir/testFile5*", &state);

	NW_CHECK(searchHandle != INVALID_HANDLE_VALUE);

	fileSizeHigh = state.nFileSizeHigh;
	fileSizeLow  = state.nFileSizeLow;

	NW_CHECK_EQUAL(0, fileSizeHigh);
	NW_CHECK_EQUAL(512, fileSizeLow);

	FindClose(searchHandle);
}

NW_END_TEST_GROUP()