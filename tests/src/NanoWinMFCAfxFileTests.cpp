#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxFile.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
#else
 //#include <windows.h>
 #include <afx.h>
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
#ifdef __GNUC__
	unlink("testDir/testSubDir/testFile16.txt");
	unlink("testDir/testSubDir/testFile512.txt");
	rmdir("testDir/testSubDir");

	unlink("testDir/testFile512.txt");
	unlink("testDir/testFile256.txt");
	rmdir("testDir");
#else
	DeleteFileA("testDir/testSubDir/testFile16.txt");
	DeleteFileA("testDir/testSubDir/testFile512.txt");
	RemoveDirectoryA("testDir/testSubDir");

	DeleteFileA("testDir/testFile512.txt");
	DeleteFileA("testDir/testFile256.txt");
	RemoveDirectoryA("testDir");
#endif
}

static void CreateTestDir()
{
#ifdef __GNUC__
	mkdir("testDir", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	mkdir("testDir/testSubDir", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	CreateFile(16,  "testDir/testSubDir/testFile16.txt", 'a');
	CreateFile(512, "testDir/testSubDir/testFile512.txt", 'a');
	CreateFile(512, "testDir/testFile512.txt", 'a');
	CreateFile(256, "testDir/testFile256.txt", 'a');
#else
	CreateDirectoryA("testDir", NULL);
	CreateDirectoryA("testDir/testSubDir", NULL);

	CreateFile(16, "testDir/testSubDir/testFile16.txt", 'a');
	CreateFile(512, "testDir/testSubDir/testFile512.txt", 'a');
	CreateFile(512, "testDir/testFile512.txt", 'a');
	CreateFile(256, "testDir/testFile256.txt", 'a');
#endif
}

NW_BEGIN_TEST_GROUP(NanoWinMFCAfxFileTestGroup)

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

#ifndef __GNUC__

NW_TEST(NanoWinMFCAfxFileTestGroup, CFileFindUnexistTest)
{
	CFileFind find;
	BOOL      res;

	CString str("testDir/unexist.txt");

	res = find.FindFile(str);

	NW_CHECK_FALSE(res);
}

NW_TEST(NanoWinMFCAfxFileTestGroup, FindFileTest)
{
	CFileFind find;
	BOOL      res;

	CString str("testDir/testFile5*");

	res = find.FindFile(str);

	NW_CHECK_TRUE(res);
}

NW_TEST(NanoWinMFCAfxFileTestGroup, FindNextFileUnexistTest)
{
	CFileFind find;
	BOOL      res;

	CString str("testDir/testFile5*");

	res = find.FindFile(str);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);
}

NW_TEST(NanoWinMFCAfxFileTestGroup, FindNextFileTest)
{
	CFileFind find;
	BOOL      res;

	CString resStr("testDir/testFile*");

	res = find.FindFile(resStr);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_TRUE(res);

	resStr = find.GetFileName();


	if (strcmp("testFile512.txt", resStr.GetString()) == 0)
	{
		NW_CHECK_EQUAL_STRCMP("testFile512.txt", resStr.GetString());
	}
	else
	{
		NW_CHECK_EQUAL_STRCMP("testFile256.txt", resStr.GetString());
	}
}


#endif

NW_END_TEST_GROUP()