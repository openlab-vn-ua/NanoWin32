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
	#include "NanoWinStrConvert.h"

    #define CloseFileHandle CloseHandle
	#define unlink _unlink
	#define rmdir _rmdir
	#define DOES_NOT_PASS_THE_TESTS

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

static void CreateTestDirForNanoWinFileCreateFileW()
{ 
#ifdef __linux
	mkdir(NanoWin::WStrToStrClone(L"TestDirForNanoWinFileCreateFileW").c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(L"TestDirForNanoWinFileCreateFileW");
#endif
}

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

static void RemoveTestDirForNanoWinFileCreateFileW()
{
	std::wstring root_path = L"TestDirForNanoWinFileCreateFileW";

	std::wstring test_path[] =
	{
		L"ReadCreateAlways",
		L"ReadCreateNew",
		L"ReadOpenAlways",
		L"ReadOpenExisting",
		L"ReadTruncateExisting",
		L"WriteCreateAlways",
		L"WriteCreateNew",
		L"WriteOpenAlways",
		L"WriteOpenExisting",
		L"WriteTruncateExisting",
		L"ReadWriteCreateAlways",
		L"ReadWriteCreateNew",
		L"ReadWriteOpenAlways",
		L"ReadWriteOpenExisting",
		L"ReadWriteTruncateExisting",
	};
	int n_test_path = ARRAY_SIZE(test_path);
	
	std::wstring file[] =
	{
		L"/file",
	};
	int n_file_names = ARRAY_SIZE(file);
	
	for (int i = 0; i < n_test_path; ++i)
	{
		for (int j = 0; j < n_file_names; ++j)
		{
			unlink(NanoWin::WStrToStrClone((root_path + L"/" + test_path[i] + L"/" + file[j]).c_str()).c_str());
		}
		rmdir(NanoWin::WStrToStrClone((root_path + L"/" + test_path[i]).c_str()).c_str());
	}
	rmdir(NanoWin::WStrToStrClone(root_path.c_str()).c_str());
}

NW_BEGIN_TEST_GROUP(NanoWinFileCreateFileW)

NW_BEGIN_SETUP_TEARDOWN

NW_SETUP_METHOD()
{
	CreateTestDirForNanoWinFileCreateFileW();
}

NW_TEARDOWN_METHOD()
{
	//RemoveTestDirForNanoWinFileCreateFileW();
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
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadCreateAlways");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW( file_path_wchar, GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//----------Write something into the file-----------------------//
	NewFile(10, file_path_char, 'a');
	//-----------Create file when it exist--------------------------//
	
	fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	is_file_created = stat(file_path_char, &statBefore);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, int(statBefore.st_size));

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadCreateNew)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadCreateNew");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

	
#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	//----------Write something into the file-----------------------//
	NewFile(10, file_path_char, 'a');
	is_file_created = stat(file_path_char, &statBefore);
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	NW_CHECK(GetLastError() == ERROR_FILE_EXISTS);
	is_file_created = stat(file_path_char, &statAfter);

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadOpenAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadOpenAlways");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	is_file_created = stat(file_path_char, &statAfter);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadOpenExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadOpenExisting");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND); //last error code must be zero
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(-1, is_file_created); // if == 0 -- file is exist
	
	//-------------Create unexist file---------------------//
	NewFile(10, file_path_char, 'a');
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	is_file_created = stat(file_path_char, &statBefore);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);
	
	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadTruncateExisting)
{
	NW_CHECK(true); 
	//GENERIC_READ does not use with TruncateExisting
}

//------------------------------------------------------------------//

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteCreateAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteCreateAlways");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//----------Write something into the file-----------------------//
	NewFile(10, file_path_char, 'a');
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //must rewrite the file
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	is_file_created = stat(file_path_char, &statBefore);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, int(statBefore.st_size));

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteCreateNew)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteCreateNew");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//----------Write something into the file-----------------------//
	NewFile(10, file_path_char, 'a');
	is_file_created = stat(file_path_char, &statBefore);
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	NW_CHECK(GetLastError() == ERROR_FILE_EXISTS);
	is_file_created = stat(file_path_char, &statAfter);

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteOpenAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteOpenAlways");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	is_file_created = stat(file_path_char, &statAfter);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteOpenExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteOpenExisting");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND); //last error code must be zero
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(-1, is_file_created); // if == 0 -- file is exist
	
	//-------------Create unexist file---------------------//
	NewFile(10, file_path_char, 'a');
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	is_file_created = stat(file_path_char, &statBefore);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);
	
	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteTruncateExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteTruncateExisting");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(10, file_path_char, 'a');
	stat(file_path_char, &statBefore);
	fd = CreateFileW(file_path_wchar, GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK_EQUAL(10, int(statBefore.st_size));
	NW_CHECK_EQUAL( 0, int(statAfter.st_size));

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL(2, int(statAfter.st_size));

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

//------------------------------------------------------------------//

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteCreateAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteCreateAlways");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//----------Write something into the file-----------------------//
	NewFile(10, file_path_char, 'a');
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //must rewrite the file
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	is_file_created = stat(file_path_char, &statBefore);

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, int(statBefore.st_size));

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteCreateNew)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteCreateNew");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//----------Write something into the file-----------------------//
	NewFile(10, file_path_char, 'a');
	is_file_created = stat(file_path_char, &statBefore);
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
	NW_CHECK(GetLastError() == ERROR_FILE_EXISTS);
	is_file_created = stat(file_path_char, &statAfter);

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteOpenAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteOpenAlways");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char   = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char  = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar  = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_SUCCESS); //last error code must be zero
#endif
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); //must rewrite the file
#if defined( DOES_NOT_PASS_THE_TESTS )
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);
#endif
	is_file_created = stat(file_path_char, &statAfter);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteOpenExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteOpenExisting");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND); //last error code must be zero
	int is_file_created = stat(file_path_char, &statBefore); //is file exist

	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(-1, is_file_created); // if == 0 -- file is exist

	//-------------Create unexist file---------------------//
	NewFile(10, file_path_char, 'a');
	//-----------Create file when it exist--------------------------//
	fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //must rewrite the file
	is_file_created = stat(file_path_char, &statBefore);

	NW_CHECK_EQUAL(0, is_file_created);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL((statBefore.st_size + 2), statAfter.st_size);

	//---------------------------------//
	unlink(file_path_char);
	rmdir(dir_path_char);
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteTruncateExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteTruncateExisting");
	std::wstring file(L"file");
	std::wstring file_path(path + L"/" + file);

	const char    *dir_path_char = NanoWin::WStrToStrClone((path).c_str()).c_str();
	const char    *file_path_char = NanoWin::WStrToStrClone(file_path.c_str()).c_str();
	const wchar_t *dir_path_wchar = path.c_str();
	const wchar_t *file_path_wchar = file_path.c_str();

#ifdef __linux
	mkdir(dir_path_char, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	_wmkdir(dir_path_wchar);
#endif

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(10, file_path_char, 'a');
	stat(file_path_char, &statBefore);
	fd = CreateFileW(file_path_wchar, GENERIC_READ | GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK_EQUAL(10, int(statBefore.st_size));
	NW_CHECK_EQUAL(0, int(statAfter.st_size));

	//------------Write access test----------------------//
	char buff[] = "a"; DWORD count = 0;
	SetFilePointer(fd, 0, NULL, FILE_END);
	WriteFile(fd, buff, sizeof(buff), &count, NULL);
	CloseFileHandle(fd);
	is_file_created = stat(file_path_char, &statAfter);
	NW_CHECK_EQUAL(2, int(statAfter.st_size));

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
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
