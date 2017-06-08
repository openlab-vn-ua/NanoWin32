#include "NWUnitTest.h"

#ifdef __linux
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include "NanoWinFile.h"
#include "NanoWinError.h"
#include "NanoWinStrConvert.h"
#include "NanoWinFileSys.h"
#else
	#include <windows.h>
	#include <direct.h> 
	#include <Shlwapi.h>
	#include "NanoWinStrConvert.h"

    #define CloseFileHandle CloseHandle
	#define unlink _unlink
	#define rmdir _rmdir
	#define ONLY_FOR_WIN

#endif

static void NewFile(const int size, const char *fileName, const char symbol)
{
	FILE *file;

	file = fopen(fileName, "w+");

	if (file != NULL)
	{
		for (int i = 0; i < size; i++)
		{
			fputc(symbol, file);
		}
		fclose(file);
	}
}

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

std::string root_path = "TestDirForNanoWinFileCreateFileW";
std::string test_path[] =
{
	"ReadCreateAlways",
	"ReadCreateNew",
	"ReadOpenAlways",
	"ReadOpenExisting",
	"ReadTruncateExisting",
	"WriteCreateAlways",
	"WriteCreateNew",
	"WriteOpenAlways",
	"WriteOpenExisting",
	"WriteTruncateExisting",
	"ReadWriteCreateAlways",
	"ReadWriteCreateNew",
	"ReadWriteOpenAlways",
	"ReadWriteOpenExisting",
	"ReadWriteTruncateExisting",
};
static std::string file = "/file";

class TestDir
{
public:
	TestDir(std::string root, std::string sub_dir, std::string file)
	{
		sub_dir_A = root + std::string("/") +sub_dir;
		file_A = root + std::string("/") + sub_dir + std::string("/") + file;
		sub_dir_W = NanoWin::StrConverter::Convert(sub_dir_A);
		file_W = NanoWin::StrConverter::Convert(file_A);

		CreateDirectoryA(sub_dir_A.c_str(), NULL);
	}
	~TestDir()
	{
		unlink(file_A.c_str());
		rmdir(sub_dir_A.c_str());
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
	std::string sub_dir_A = "null";
	std::string file_A = "null";
	std::wstring sub_dir_W = L"null";
	std::wstring file_W = L"null";
};

static void CreateTestDirForNanoWinFileCreateFileW()
{
	CreateDirectoryA(root_path.c_str(), NULL);
}

static void RemoveTestDirForNanoWinFileCreateFileW()
{
	rmdir(root_path.c_str());
}

NW_BEGIN_TEST_GROUP(NanoWinFileCreateFileW)

NW_BEGIN_SETUP_TEARDOWN

NW_SETUP_METHOD()
{
	CreateTestDirForNanoWinFileCreateFileW();
}

NW_TEARDOWN_METHOD()
{
	RemoveTestDirForNanoWinFileCreateFileW();
}

NW_END_SETUP_TEARDOWN

/* dwDesiredAccess can be only: GENERIC_READ GENERIC_WRITE */
/* dwShareMode isn't used */
/* lpSecurityAttributes isn't used */
/* dwCreationDisposition can be only: CREATE_ALWAYS || CREATE_NEW || OPEN_ALWAYS || OPEN_EXISTING || TRUNCATE_EXISTING */
/* dwFlagsAndAttributes can be only: FILE_ATTRIBUTE_NORMAL */
/* hTemplateFile isn't used */

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadCreateAlways)
{
	TestDir homed(root_path, test_path[0], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
#endif

	CloseFileHandle(fd);

	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist
	NW_CHECK_EQUAL(0, is_file_created);
	

	NewFile(10, homed.getFileA(), 'a');
	
	fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif

	is_file_created = stat(homed.getFileA(), &statBefore);
	NW_CHECK_EQUAL(0, int(statBefore.st_size));


	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadCreateNew)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);


	NewFile(10, homed.getFileA(), 'a');
	is_file_created = stat(homed.getFileA(), &statBefore);

	fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_EXISTS);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadOpenAlways)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
#endif

	int is_file_created = stat(homed.getFileA(), &statBefore);
	NW_CHECK_EQUAL(0, is_file_created);

	
	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	
	fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif

	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(0, is_file_created);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadOpenExisting)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND); //last error code must be zero

	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist
	NW_CHECK_EQUAL(-1, is_file_created); // if == 0 -- file is exist
	

	NewFile(10, homed.getFileA(), 'a');

	fd = CreateFileW(homed.getFileW(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	is_file_created = stat(homed.getFileA(), &statBefore);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);


	char buff[] = "a";
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadTruncateExisting)
{
	NW_CHECK(true); 
	//GENERIC_READ does not use with TruncateExisting
}



NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteCreateAlways)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif

	CloseFileHandle(fd);

	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist


	NewFile(10, homed.getFileA(), 'a');

	fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //must rewrite the file

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	
	is_file_created = stat(homed.getFileA(), &statBefore);
	NW_CHECK_EQUAL(0, int(statBefore.st_size));


	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteCreateNew)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	
	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	
	NewFile(10, homed.getFileA(), 'a');
	is_file_created = stat(homed.getFileA(), &statBefore);
	
	fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_EXISTS);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteOpenAlways)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif

	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist


	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	
	fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(0, is_file_created);

	CloseFileHandle(fd);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteOpenExisting)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist
	NW_CHECK_EQUAL(-1, is_file_created); // if == 0 -- file is exist
	

	NewFile(10, homed.getFileA(), 'a');

	fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	is_file_created = stat(homed.getFileA(), &statBefore);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);


	char buff[] = "a";
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteTruncateExisting)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);


	NewFile(10, homed.getFileA(), 'a');
	stat(homed.getFileA(), &statBefore);
	fd = CreateFileW(homed.getFileW(), GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(homed.getFileA(), &statAfter);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK_EQUAL(10, int(statBefore.st_size));
	NW_CHECK_EQUAL( 0, int(statAfter.st_size));


	char buff[] = "a";
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(2, int(statAfter.st_size));
}


NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteCreateAlways)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
#endif

	int is_file_created = stat(homed.getFileA(), &statBefore);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	
	NewFile(10, homed.getFileA(), 'a');
	
	fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //must rewrite the file

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif

	is_file_created = stat(homed.getFileA(), &statBefore);
	NW_CHECK_EQUAL(0, int(statBefore.st_size));

	
	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteCreateNew)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(homed.getFileA(), &statBefore);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created);


	char buff[] = "a";
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);


	NewFile(10, homed.getFileA(), 'a');
	is_file_created = stat(homed.getFileA(), &statBefore);

	fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_EXISTS);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteOpenAlways)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;


	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif

	int is_file_created = stat(homed.getFileA(), &statBefore); //is file exist
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	
	char buff[] = "a";
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	
	fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
#if defined( ONLY_FOR_WIN )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(0, is_file_created);
	
	CloseFileHandle(fd);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteOpenExisting)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;

	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	int is_file_created = stat(homed.getFileA(), &statBefore);
	NW_CHECK_EQUAL(-1, is_file_created);

	
	NewFile(10, homed.getFileA(), 'a');
	
	fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //must rewrite the file
	is_file_created = stat(homed.getFileA(), &statBefore);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	
	char buff[] = "a"; 
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteTruncateExisting)
{
	TestDir homed(root_path, test_path[1], file);
	struct stat statBefore;
	struct stat statAfter;

	HANDLE fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	
	NewFile(10, homed.getFileA(), 'a');
	stat(homed.getFileA(), &statBefore);
	fd = CreateFileW(homed.getFileW(), GENERIC_READ | GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(homed.getFileA(), &statAfter);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK_EQUAL(10, int(statBefore.st_size));
	NW_CHECK_EQUAL(0, int(statAfter.st_size));

	
	char buff[] = "a";
	DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);

	is_file_created = stat(homed.getFileA(), &statAfter);
	NW_CHECK_EQUAL(2, int(statAfter.st_size));
}

NW_END_TEST_GROUP()



NW_BEGIN_TEST_GROUP_SIMPLE(GetFileSizeTestGroup)

NW_TEST(GetFileSizeTestGroup, GetFileSizeATest)
{
	if (PathFileExistsA("/tmp/file.txt"))
	{
		unsigned long long expectedSize = 5LLU * 1024 * 1024 * 1024;

		HANDLE handle = CreateFileA("/tmp/file.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		NW_CHECK(handle != INVALID_HANDLE_VALUE);

		DWORD fileSizeHi = 0;
		DWORD fileSizeLo = GetFileSize(handle,&fileSizeHi);

		CloseFileHandle(handle);

		DWORD expectedFileSizeHi = (DWORD)(expectedSize >> (sizeof(DWORD) * 8));
		DWORD expectedFileSizeLo = (DWORD)(expectedSize & 0xFFFFFFFFLU);

		NW_CHECK_EQUAL_ULONGS(expectedFileSizeLo, fileSizeLo);
		NW_CHECK_EQUAL_ULONGS(expectedFileSizeHi, fileSizeHi);
	}
}

NW_END_TEST_GROUP()
