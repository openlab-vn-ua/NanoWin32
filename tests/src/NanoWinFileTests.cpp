#include "NWUnitTest.h"

#ifdef __linux
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include "NanoWinFile.h"
#include "NanoWinError.h"
#include "NanoWinStrConvert.h"
#else
#error "unsupported platform"
// #include <windows.h>
#endif

static void NewFile(const int size, const char *fileName, const char symbol)
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

static void CreateTestDirForNanoWinFileCreateFileW()
{
	mkdir(NanoWin::WStrToStrClone(L"TestDirForNanoWinFileCreateFileW").c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}

static void RemoveTestDirForNanoWinFileCreateFileW()
{
	std::wstring root_path = L"TestDirForNanoWinFileCreateFileW";

	int n_test_path = 15;
	std::wstring test_path[n_test_path] =
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

	int n_file_names = 1;
	std::wstring file[n_file_names] =
	{
		L"/file",
	};
	
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
	RemoveTestDirForNanoWinFileCreateFileW();
}

NW_END_SETUP_TEARDOWN

/* dwDesiredAccess can be only: GENERIC_READ GENERIC_WRITE */
/* dwShareMode isn't used */
/* lpSecurityAttributes isn't used */
/* dwCreationDisposition can be only: CREATE_ALWAYS || CREATE_NEW || OPEN_ALWAYS || OPEN_EXISTING || TRUNCATE_EXISTING */
/* dwFlagsAndAttributes can be only: FILE_ATTRIBUTE_NORMAL */
/* hTemplateFile isn't used */

//#define CHECK_ERROR_SUCCESS
NW_TEST(NanoWinFileCreateFileW, CreateFileWReadCreateAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadCreateAlways");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	#ifdef CHECK_ERROR_SUCCESS
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
	#endif

	CloseFileHandle(fd);

	//----------Write something into the file-----------------------//
	NewFile(10, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	//--------------------------------------------------------------//
	fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	//must don't rewrite the file
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statAfter); //check new files stat

	NW_CHECK_EQUAL(statBefore.st_size, statAfter.st_size); // if that are not equal -- func work wrong, values must be same
	NW_CHECK_EQUAL(ERROR_ALREADY_EXISTS, GetLastError());
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	
	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadCreateNew)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadCreateNew");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE)
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//-------- Create file when it exist--------------//
	fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(ERROR_FILE_EXISTS, GetLastError());

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadOpenAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadOpenAlways");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
    #ifdef CHECK_ERROR_SUCCESS
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
	#endif

	CloseFileHandle(fd);

	//-------- Create file when it exist--------------//
	fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore); //is file exist
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadOpenExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadOpenExisting");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(1, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadTruncateExisting)
{
	NW_CHECK(true); /* GENERIC_READ don't use with TruncateExisting*/
}

//------------------------------------------------------------------//

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteCreateAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteCreateAlways");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW( (path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	#ifdef CHECK_ERROR_SUCCESS
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
	#endif

	CloseFileHandle(fd);

	//----------Write something into the file-----------------------//
	NewFile(10, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	//--------------------------------------------------------------//
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//must rewrite the file
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statAfter); //check new files stat

	NW_CHECK_EQUAL(0, statAfter.st_size); // if that are not equal -- func work wrong
	NW_CHECK_EQUAL(ERROR_ALREADY_EXISTS, GetLastError());
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteCreateNew)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteCreateNew");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//-------- Create file when it exist--------------//
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(ERROR_FILE_EXISTS, GetLastError());

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteOpenAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteOpenAlways");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	#ifdef CHECK_ERROR_SUCCESS
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
	#endif

	CloseFileHandle(fd);

	//-------- Create file when it exist--------------//
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore); //is file exist
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
		NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteOpenExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteOpenExisting");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(1, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWWriteTruncateExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/WriteTruncateExisting");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(10, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = 
		stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statAfter);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK_EQUAL(10, statBefore.st_size);
	NW_CHECK_EQUAL( 0, statAfter.st_size);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

//------------------------------------------------------------------//

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteCreateAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteCreateAlways");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore); //is file exist

	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	#ifdef CHECK_ERROR_SUCCESS
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
	#endif

	CloseFileHandle(fd);

	//----------Write something into the file-----------------------//
	NewFile(10, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);

	//--------------------------------------------------------------//
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//must rewrite the file
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statAfter); //check new files stat

	NW_CHECK_EQUAL(0, statAfter.st_size); // if that are not equal -- func work wrong
	NW_CHECK_EQUAL(ERROR_ALREADY_EXISTS, GetLastError());
	NW_CHECK(fd != INVALID_HANDLE_VALUE);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteCreateNew)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteCreateNew");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//-------- Create file when it exist--------------//
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(ERROR_FILE_EXISTS, GetLastError());

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteOpenAlways)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteOpenAlways");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	#ifdef CHECK_ERROR_SUCCESS
	NW_CHECK(GetLastError() == ERROR_SUCCESS);
	#endif

	CloseFileHandle(fd);

	//-------- Create file when it exist--------------//
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore); //is file exist
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK(GetLastError() == ERROR_ALREADY_EXISTS);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteOpenExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteOpenExisting");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(1, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created = stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_TEST(NanoWinFileCreateFileW, CreateFileWReadWriteTruncateExisting)
{
	std::wstring path(L"TestDirForNanoWinFileCreateFileW/ReadWriteTruncateExisting");
	std::wstring file(L"file");
	mkdir(NanoWin::WStrToStrClone(path.c_str()).c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	struct stat statBefore;
	struct stat statAfter;

	//-------- Create file when it does not exist--------------//
	HANDLE fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NW_CHECK(fd == INVALID_HANDLE_VALUE);
	NW_CHECK(GetLastError() == ERROR_FILE_NOT_FOUND);

	//-------- Create file when it exist--------------//
	NewFile(10, NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), 'a');
	stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statBefore);
	fd = CreateFileW((path + L"/" + file).c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int is_file_created =
		stat(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str(), &statAfter);
	NW_CHECK(fd != INVALID_HANDLE_VALUE);
	NW_CHECK_EQUAL(0, is_file_created); // if == 0 -- file is exist
	NW_CHECK_EQUAL(10, statBefore.st_size);
	NW_CHECK_EQUAL(0, statAfter.st_size);

	CloseFileHandle(fd);

	//---------------------------------//
	unlink(NanoWin::WStrToStrClone((path + L"/" + file).c_str()).c_str());
	rmdir(NanoWin::WStrToStrClone(path.c_str()).c_str());
}

NW_END_TEST_GROUP()
