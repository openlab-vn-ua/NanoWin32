#include "NWUnitTest.h"
#include "NanoWinStrConvert.h"

#ifdef __linux
 #include "NanoWinMFCAfxFile.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
#else
 //#include <windows.h>
 #include <direct.h> 
 #include <afx.h>
 #define unlink _unlink
 #define rmdir _rmdir
#endif

#define MAX_LENGTH     (1024)

#ifdef __linux
// #define PATH_DIR_SEPARATOR_CHAR  ('/')
	static char PATH_DIR_SEPARATOR_STR[] = "/";
#else
// #define PATH_DIR_SEPARATOR_CHAR  ('\\')
	static char PATH_DIR_SEPARATOR_STR[] = "\\";
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
	#ifdef __GNUC__

	unlink("testDir/testSubDir/testFile512.txt");
	rmdir("testDir/testSubDir");

	unlink("testDir/testFile512.txt");
	unlink("testDir/testFile256.txt");
	rmdir("testDir");

	#else

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

	#else

	CreateDirectoryA("testDir", NULL);
	CreateDirectoryA("testDir/testSubDir", NULL);

	#endif

	CreateFile(512, "testDir/testSubDir/testFile512.txt", 'a');
	CreateFile(512, "testDir/testFile512.txt", 'a');
	CreateFile(256, "testDir/testFile256.txt", 'a');

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

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	CString resStr = find.GetFileName();
	NW_CHECK_EQUAL_STRCMP(_T("testFile512.txt"), resStr.GetString());
}

NW_TEST(NanoWinMFCAfxFileTestGroup, FindFileNameNullTest)
{
	CFileFind find;
	BOOL      res;

	res = find.FindFile(NULL);
	
	NW_CHECK_TRUE(res);

	bool oneDotFound  = false;
	bool twoDotsFound = false;

	while (res)
	{
		res = find.FindNextFile();

		CString resStr = find.GetFileName();

		if (resStr == _T(".")) 
		{
			oneDotFound = true;
		}
		else if (resStr == _T(".."))
		{
			twoDotsFound = true;
		}
	}

	NW_CHECK_TRUE(oneDotFound);
	NW_CHECK_TRUE(twoDotsFound);
}

NW_TEST(NanoWinMFCAfxFileTestGroup, FindNextFileTest)
{
	CFileFind find;
	BOOL      res;

	CString resStr(_T("testDir/testFile*"));

	res = find.FindFile(resStr);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_TRUE(res);

	resStr = find.GetFileName();


	if (resStr == _T("testFile512.txt"))
	{
		NW_CHECK_EQUAL_STRCMP(_T("testFile512.txt"), resStr.GetString());
	}
	else
	{
		NW_CHECK_EQUAL_STRCMP(_T("testFile256.txt"), resStr.GetString());
	}

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	resStr = find.GetFileName();


	if (resStr == _T("testFile512.txt"))
	{
		NW_CHECK_EQUAL_STRCMP(_T("testFile512.txt"), resStr.GetString());
	}
	else
	{
		NW_CHECK_EQUAL_STRCMP(_T("testFile256.txt"), resStr.GetString());
	}
}

NW_TEST(NanoWinMFCAfxFileTestGroup, FindCloseTest)
{
	CFileFind find;
	BOOL      res;

	CString str1(_T("testDir/testFile5*"));
	CString str2(_T("testDir/testFile2*"));

	res = find.FindFile(str1);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	find.Close();

	CString resStr = find.GetFileName();
	NW_CHECK_EQUAL_STRCMP(_T(""), resStr.GetString());

	res = find.FindFile(str2);
	NW_CHECK_TRUE(res);
}

NW_TEST(NanoWinMFCAfxFileTestGroup, IsDirectoryTest)
{
	CFileFind find;
	BOOL      res;

	CString str1(_T("testDir/testFile5*"));
	CString str2(_T("testDir/testSub*"));

	res = find.FindFile(str1);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	CString resStr = find.GetFileName();

	NW_CHECK_EQUAL_STRCMP(_T("testFile512.txt"), resStr.GetString());
	NW_CHECK_TRUE(find.IsDirectory() == 0);

	find.Close();

	res = find.FindFile(str2);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	resStr = find.GetFileName();

	NW_CHECK_EQUAL_STRCMP(_T("testSubDir"), resStr.GetString());
	NW_CHECK_TRUE(find.IsDirectory() != 0);
}

NW_TEST(NanoWinMFCAfxFileTestGroup, IsDotsTest)
{
	CFileFind find;
	BOOL      res;

	CString str(_T("*.*"));

	res = find.FindFile(str);
	NW_CHECK_TRUE(res);

	while (res)
	{
		res = find.FindNextFile();

		CString resStr = find.GetFileName();

		if (resStr == _T(".") || resStr == _T(".."))
		{
			NW_CHECK_TRUE(find.IsDots());
		}
	}
}

NW_TEST(NanoWinMFCAfxFileTestGroup, GetLengthTest)
{
	CFileFind find;
	BOOL      res;

	CString resStr(_T("testDir/testFile*"));

	res = find.FindFile(resStr);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_TRUE(res);

	resStr = find.GetFileName();


	if (resStr == _T("testFile512.txt"))
	{
		NW_CHECK_EQUAL_LONGS(512, find.GetLength());
	}
	else
	{
		NW_CHECK_EQUAL_LONGS(256, find.GetLength());
	}

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	resStr = find.GetFileName();


	if (resStr == _T("testFile512.txt"))
	{
		NW_CHECK_EQUAL_LONGS(512, find.GetLength());
	}
	else
	{
		NW_CHECK_EQUAL_LONGS(256, find.GetLength());
	}
}

NW_TEST(NanoWinMFCAfxFileTestGroup, GetFilePathTest)
{
	CFileFind find;
	BOOL      res;
	char      fullFileName[MAX_LENGTH];

	CString str(_T("testDir/testFile5*"));

	res = find.FindFile(str);
	NW_CHECK_TRUE(res);

	res = find.FindNextFile();
	NW_CHECK_FALSE(res);

	CString resStr = find.GetFilePath();

	GetCurrentDirectoryA(sizeof(fullFileName), fullFileName);

	strcat(fullFileName, PATH_DIR_SEPARATOR_STR);
	strcat(fullFileName, "testDir");
	strcat(fullFileName, PATH_DIR_SEPARATOR_STR);
	strcat(fullFileName, "testFile512.txt");

	NW_CHECK_EQUAL_STRCMP(CA2T(fullFileName), resStr.GetString());
}

NW_END_TEST_GROUP()

static std::string root = "MFCAfxFileTestCFileClass";
#define ARR_SIZE(a) (sizeof(a)/sizeof(a[0]))
static std::string sub_dir[] =
{
	"MFCAfxCFileClassTestReadWrite",
	"MFCAfxCFileClassTestSeek",
	"MFCAfxCFileClassTestGetLength",
};
static std::string file = "file";
static std::string sep = "/";

class TestDir 
{
public:
	TestDir(std::string root, std::string sub_dir, std::string file)
	{
		sub_dir_A = root + sep + sub_dir;
		file_A = root + sep + sub_dir + sep + file;
		sub_dir_W = NanoWin::StrConverter::Convert(sub_dir_A);
		file_W = NanoWin::StrConverter::Convert(file_A);

		CreateDirectoryA(sub_dir_A.c_str(), NULL);
	}
	~TestDir()
	{
		unlink(file_A.c_str());
		rmdir( sub_dir_A.c_str());
	}

	const char *getFileA()
	{
		return file_A.c_str();
	}
	const wchar_t *getFileW()
	{
		return file_W.c_str();
	}
	const char *getSubDirA()
	{
		return sub_dir_A.c_str();
	}
	const wchar_t*getSubDirW()
	{
		return sub_dir_W.c_str();
	}

protected:
	std::string sub_dir_A  = "null";
	std::string file_A     = "null";
	std::wstring sub_dir_W = L"null";
	std::wstring file_W    = L"null";
};

static void CreateDirsForCFileClassTest()
{
	CreateDirectoryA(root.c_str(), NULL);
}

static void RemoveDirsForCFileClassTest()
{
	rmdir(root.c_str());
}

NW_BEGIN_TEST_GROUP(NanoWinMFCAfxFileTestCFileClass)

NW_SETUP_METHOD()
{
	CreateDirsForCFileClassTest();
}

NW_TEARDOWN_METHOD()
{
	RemoveDirsForCFileClassTest();
}

NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinMFCAfxFileTestCFileClass, MFCAfxCFileClassTestReadWrite)
{
	TestDir homed(root, sub_dir[0], file);
	CFile file;
	FILE *fd;
	const char written_buff[] = "test_string";
	char       readed_buff[64];

	file.Open(CA2T(homed.getFileA()), CFile::modeReadWrite | CFile::modeCreate);
	file.Write(written_buff, sizeof(written_buff));
	file.Close();

	fd = fopen(homed.getFileA(), "r");
	fscanf(fd, "%s", readed_buff);
	fclose(fd);

	NW_CHECK_EQUAL_STRCMP(written_buff, readed_buff);


	fd = fopen(homed.getFileA(), "w+");
	fprintf(fd, "%s", written_buff);
	fclose(fd);

	file.Open(CA2T(homed.getFileA()), CFile::modeRead | CFile::modeNoTruncate);
	file.Read(readed_buff, sizeof(readed_buff));
	file.Close();

	NW_CHECK_EQUAL_STRCMP(written_buff, readed_buff);
}

NW_TEST(NanoWinMFCAfxFileTestCFileClass, MFCAfxCFileClassTestSeek)
{
	TestDir homed(root, sub_dir[1], file);
	CFile file;
	FILE *fd;
	const char written_buff_0[] = "AAAA";
	const char written_buff_1[] = "bbbb";
	const char readed_buff_end[] = "AAAAbbbb";
	const char readed_buff_mid[] = "AAbbbb";
	char readed_buff[64];
	
	fd = fopen(homed.getFileA(), "w+");
	fprintf(fd, "%s", written_buff_0);
	fclose(fd);

	file.Open(CA2T(homed.getFileA()), CFile::modeReadWrite | CFile::modeNoTruncate);
	file.Seek(0, CFile::end);
	file.Write(written_buff_1, sizeof(written_buff_1));
	file.Close();

	fd = fopen(homed.getFileA(), "r");
	fscanf(fd, "%s", readed_buff);
	fclose(fd);

	NW_CHECK_EQUAL_STRCMP(readed_buff_end, readed_buff);


	fd = fopen(homed.getFileA(), "w+");
	fprintf(fd, "%s", written_buff_0);
	fclose(fd);

	file.Open(CA2T(homed.getFileA()), CFile::modeReadWrite | CFile::modeNoTruncate);
	file.Seek(2, CFile::begin);
	file.Write(written_buff_1, sizeof(written_buff_1));
	file.Close();

	fd = fopen(homed.getFileA(), "r");
	fscanf(fd, "%s", readed_buff);
	fclose(fd);

	NW_CHECK_EQUAL_STRCMP(readed_buff_mid, readed_buff);
}

NW_TEST(NanoWinMFCAfxFileTestCFileClass, MFCAfxCFileClassTestGetLength)
{
	TestDir homed(root, sub_dir[2], file);
	CFile file;
	FILE *fd;
	const char written_buff[] = "abcdef";
	ULONGLONG written_buff_len = sizeof(written_buff) / sizeof(written_buff[0]) - 1;

	fd = fopen(homed.getFileA(), "w+");
	fprintf(fd, "%s", written_buff);
	fclose(fd);
	
	file.Open(CA2T(homed.getFileA()), CFile::modeReadWrite | CFile::modeNoTruncate);
	ULONGLONG cfile_len = file.GetLength();
	file.Close();

	NW_CHECK(written_buff_len == cfile_len);
}

NW_END_TEST_GROUP()
















