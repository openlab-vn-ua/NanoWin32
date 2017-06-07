#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinFileSys.h"
 #include "NanoWinMsExtra.h"
 #include <unistd.h>
#else
 #include <windows.h>
 #include <direct.h>
 #include <stdlib.h>
 #include <stdio.h>
#endif

#include "NWUnitTestTools.h"

#ifdef __linux
 #define  DSEPS "/"
#else
 #define  DSEPS "\\"
#endif

#if defined(_MSC_VER)
#define SKIP_MS
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMSExtraTestGroup)

#define SETUP_S_TEST()    NW_SETUP_ERR_HANDLER_FOR_TEST() // For compatibility with old tests

NW_TEST(NanoWinMSExtraTestGroup, Path_fullpath_Test)
{
	SETUP_S_TEST();

	// prepare

	char     *result;
	char      cwd[_MAX_PATH];
	result = _getcwd(cwd, _countof(cwd));
	NW_CHECK_TRUE(result == cwd);

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _fullpath(NULL, NULL, 0); // NULL as srcrelpath mean get current directory
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _fullpath(NULL, "", 0);
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _fullpath(NULL, "", 1);
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _fullpath(NULL, "", _MAX_PATH);
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	char      tmp[_MAX_PATH];

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, "", _MAX_PATH);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, cwd);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, "", 0);
	NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, "", 1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	#ifndef SKIP_MS // RSIZE_MAX is not checked under MS
	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, "", RSIZE_MAX_STR+1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    #endif

	char     fexp1[_MAX_PATH];
	strcpy_s(fexp1, cwd);
	strcat_s(fexp1, DSEPS);
	strcat_s(fexp1, "expected1");
	char    *rexp1= "expected1";

	char     fexp2[_MAX_PATH];
	strcpy_s(fexp2, cwd);
	strcat_s(fexp2, DSEPS);
	strcat_s(fexp2, "expected1");
	strcat_s(fexp2, DSEPS);
	strcat_s(fexp2, "expected2");
	char    *rexp2= "expected1" DSEPS "expected2";

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _fullpath(NULL, rexp1, 0);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _fullpath(NULL, rexp1, 1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _fullpath(NULL, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _fullpath(NULL, rexp2, strlen(fexp2)+1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, rexp1, 0);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, rexp1, 1);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, rexp1, _MAX_PATH);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, fexp1);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, rexp2, strlen(fexp2)+1);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _fullpath(tmp, rexp2, strlen(fexp2)); // buffer too small
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
}

NW_TEST(NanoWinMSExtraTestGroup, Path_wfullpath_Test)
{
	SETUP_S_TEST();

	// prepare

	wchar_t   *result;
	wchar_t    cwd[_MAX_PATH];
	result = _wgetcwd(cwd, _countof(cwd));
	NW_CHECK_TRUE(result == cwd);

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _wfullpath(NULL, NULL, 0); // NULL as srcrelpath mean get current directory
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _wfullpath(NULL, L"", 0);
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _wfullpath(NULL, L"", 1);
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _wfullpath(NULL, L"", _MAX_PATH);
    NW_CHECK_EQUAL_STRCMP(result, cwd);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	wchar_t   tmp[_MAX_PATH];

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, L"", _MAX_PATH);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, cwd);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, L"", 0);
	NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, L"", 1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	#ifndef SKIP_MS // RSIZE_MAX is not checked under MS
	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, L"", RSIZE_MAX_STR+1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    #endif

	wchar_t  fexp1[_MAX_PATH];
	wcscpy_s(fexp1, cwd);
	wcscat_s(fexp1, L"" DSEPS);
	wcscat_s(fexp1, L"" "expected1");
	wchar_t *rexp1= L"" "expected1";

	wchar_t  fexp2[_MAX_PATH];
	wcscpy_s(fexp2, cwd);
	wcscat_s(fexp2, L"" DSEPS);
	wcscat_s(fexp2, L"" "expected1");
	wcscat_s(fexp2, L"" DSEPS);
	wcscat_s(fexp2, L"" "expected2");
	wchar_t *rexp2= L"" "expected1" DSEPS "expected2";

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _wfullpath(NULL, rexp1, 0);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _wfullpath(NULL, rexp1, 1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _wfullpath(NULL, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _wfullpath(NULL, rexp2, wcslen(fexp2)+1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, rexp1, 0);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, rexp1, 1);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, rexp1, _MAX_PATH);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, fexp1);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, rexp2, wcslen(fexp2)+1);
    NW_CHECK_TRUE(result == tmp);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(result, fexp2);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_FILL_VAR_WITH_5A(tmp);
	result = _wfullpath(tmp, rexp2, wcslen(fexp2)); // buffer too small
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
}

NW_END_TEST_GROUP()