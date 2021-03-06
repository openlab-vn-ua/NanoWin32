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

#if defined(LINUX)
#define SKIP_LINUX
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMSExtraTestGroup)

#define SETUP_S_TEST()    NW_SETUP_ERR_HANDLER_FOR_TEST() // For compatibility with old tests

NW_TEST(NanoWinMSExtraTestGroup, Path_fullpath_Test)
{
    #define ITEM      char
    #define GETCWD    _getcwd
    #define STRLEN    strlen
    #define EMPTS     ""
    #define _FULLPATH _fullpath
    #define STRCPY_S  strcpy_s
    #define STRCAT_S  strcat_s
	// <body> // invaliant for Path_FULLPATH_Test and Path_wfullpath_Test

	SETUP_S_TEST();

	// prepare

	ITEM     *result;
	ITEM      cwd[_MAX_PATH];
	result = GETCWD(cwd, _countof(cwd));
	NW_CHECK_TRUE(result == cwd);

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _FULLPATH(NULL, NULL, 0); // NULL as srcrelpath mean get current directory
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, EMPTS, 0);
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, EMPTS, 1);
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, EMPTS, _MAX_PATH);
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	ITEM      out[_MAX_PATH];

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, _MAX_PATH);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(cwd, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	#ifndef SKIP_MS // Exact fit fail with MS for some reason in _wfullpath (_fullpath works OK)
	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, STRLEN(cwd)+1);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(cwd, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
	#endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, STRLEN(cwd)+20); // works in all cases
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(cwd, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, STRLEN(cwd)); // no buff space
	NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_STR_EMPTY(out); // Works this way under MS
    #endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, 0);
	NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, 1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_STR_EMPTY(out); // Works this way under MS 
    #endif

	#ifndef SKIP_MS // RSIZE_MAX is not checked under MS
	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, RSIZE_MAX_STR+1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    #endif

	ITEM     fexp1[_MAX_PATH];
	STRCPY_S(fexp1, cwd);
	STRCAT_S(fexp1, EMPTS DSEPS);
	STRCAT_S(fexp1, EMPTS "expected1");
	const ITEM *rexp1= EMPTS "expected1";

	ITEM     fexp2[_MAX_PATH];
	STRCPY_S(fexp2, cwd);
	STRCAT_S(fexp2, EMPTS DSEPS);
	STRCAT_S(fexp2, EMPTS "expected1");
	STRCAT_S(fexp2, EMPTS DSEPS);
	STRCAT_S(fexp2, EMPTS "expected2");
	const ITEM *rexp2= EMPTS "expected1" DSEPS "expected2";

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _FULLPATH(NULL, rexp1, 0);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, rexp1, 1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, rexp2, STRLEN(fexp2)+1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp1, 0);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	#if defined(SKIP_MS)
	NW_RESET_ERR_HANDLER_COUNT(); // The handler may/may not be fired under MS (against the standard) // not fired VS2019
	#else
	NW_CHECK_ERR_HANDLER_FIRED();
	#endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp1, 1);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp1, _MAX_PATH);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(fexp1, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, STRLEN(fexp2)+1);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, 1); // buffer too small
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_VAR_FILL_5A(out); // Work this way under MS // ???
    #endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, STRLEN(fexp2)); // buffer too small
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_VAR_FILL_5A(out); // Work this way under MS // ???
    #endif

	// <body> // invaliant for Path_FULLPATH_Test and Path_wfullpath_Test
    #undef  STRCAT_S
    #undef  STRCPY_S
    #undef  _FULLPATH
    #undef  EMPTS
    #undef  STRLEN
    #undef  GETCWD
    #undef  ITEM
}

NW_TEST(NanoWinMSExtraTestGroup, Path_wfullpath_Test)
{
    #define ITEM      wchar_t
    #define GETCWD    _wgetcwd
    #define STRLEN    wcslen
    #define EMPTS     L""
    #define _FULLPATH _wfullpath
    #define STRCPY_S  wcscpy_s
    #define STRCAT_S  wcscat_s
	// <body> // invaliant for Path_FULLPATH_Test and Path_wfullpath_Test

	SETUP_S_TEST();

	// prepare

	ITEM     *result;
	ITEM      cwd[_MAX_PATH];
	result = GETCWD(cwd, _countof(cwd));
	NW_CHECK_TRUE(result == cwd);

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _FULLPATH(NULL, NULL, 0); // NULL as srcrelpath mean get current directory
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, EMPTS, 0);
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, EMPTS, 1);
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, EMPTS, _MAX_PATH);
    NW_CHECK_EQUAL_STRCMP(cwd, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	ITEM      out[_MAX_PATH];

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, _MAX_PATH);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(cwd, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	#ifndef SKIP_MS // Exact fit fail with MS for some reason in _wfullpath (_fullpath works OK)
	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, STRLEN(cwd)+1);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(cwd, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
	#endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, STRLEN(cwd)+20); // works in all cases
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(cwd, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, STRLEN(cwd)); // no buff space
	NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_STR_EMPTY(out); // Works this way under MS
    #endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, 0);
	NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, 1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_STR_EMPTY(out); // Works this way under MS
    #endif

	#ifndef SKIP_MS // RSIZE_MAX is not checked under MS
	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, EMPTS, RSIZE_MAX_STR+1);
	NW_CHECK_TRUE(result == NULL); // will work fail unless we are at root folder in LINUX (hardly unlikely)
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    #endif

	ITEM     fexp1[_MAX_PATH];
	STRCPY_S(fexp1, cwd);
	STRCAT_S(fexp1, EMPTS DSEPS);
	STRCAT_S(fexp1, EMPTS "expected1");
	const ITEM *rexp1= EMPTS "expected1";

	ITEM     fexp2[_MAX_PATH];
	STRCPY_S(fexp2, cwd);
	STRCAT_S(fexp2, EMPTS DSEPS);
	STRCAT_S(fexp2, EMPTS "expected1");
	STRCAT_S(fexp2, EMPTS DSEPS);
	STRCAT_S(fexp2, EMPTS "expected2");
	const ITEM *rexp2= EMPTS "expected1" DSEPS "expected2";

	// dynamic buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	result = _FULLPATH(NULL, rexp1, 0);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, rexp1, 1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	result = _FULLPATH(NULL, rexp2, STRLEN(fexp2)+1);
    NW_CHECK_TRUE(result != NULL);
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
    NW_CHECK_ERRNO_OK();
	free(result);

	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	// static buffer

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp1, 0);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	#if defined(SKIP_MS)
	NW_RESET_ERR_HANDLER_COUNT(); // The handler may/may not be fired under MS (against the standard) // not fired VS2019
	#else
	NW_CHECK_ERR_HANDLER_FIRED();
	#endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp1, 1);
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp1, _MAX_PATH);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(fexp1, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, _MAX_PATH);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, STRLEN(fexp2)+1);
    NW_CHECK_TRUE(result == out);
    NW_CHECK_ERRNO_OK();
    NW_CHECK_EQUAL_STRCMP(fexp2, result);
	NW_CHECK_ERR_HANDLER_NOT_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, 1); // buffer too small
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_VAR_FILL_5A(out); // Work this way under MS // ???
    #endif

	NW_RESET_VAR_FILL_5A(out);
	result = _FULLPATH(out, rexp2, STRLEN(fexp2)); // buffer too small
    NW_CHECK_TRUE(result == NULL);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    #ifndef SKIP_LINUX
	// NW_CHECK_VAR_FILL_5A(out); // Work this way under MS // ???
    #endif

	// <body> // invaliant for Path_FULLPATH_Test and Path_wfullpath_Test
    #undef  STRCAT_S
    #undef  STRCPY_S
    #undef  _FULLPATH
    #undef  EMPTS
    #undef  STRLEN
    #undef  GETCWD
    #undef  ITEM
}

NW_TEST(NanoWinMSExtraTestGroup, Path_makepath_s_Test)
{
    #define ITEM         char
    #define TS(X)        "" X
    #define _MAKEPATH_S _makepath_s
    #define STRLEN      strlen
	// <body> // invaliant for Path_makepath_s_Test and Path_wmakepath_s_Test

	SETUP_S_TEST();

	ITEM         out[_MAX_PATH];
	const ITEM  *exp;

	errno_t rc;

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), TS("ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), NULL);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), TS(""));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS(""));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS("ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS("..ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("..ext"), out);

	// path

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path"), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "path2"), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "path2" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "path2" DSEPS "name.ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	// abs path

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS "path1" DSEPS "path2"), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS "path1" DSEPS "path2" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS "path1" DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS "path1" DSEPS "path2" DSEPS "name.ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	// 2 dots not expanded

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(".." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(".." DSEPS "path2" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS ".." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS ".." DSEPS "path2" DSEPS "name.ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS ".." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS ".." DSEPS "path2" DSEPS "name.ext"), out);

	// single dot not removed

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "." DSEPS "path2" DSEPS "name.ext"), out);

	// drive

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS(""), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS(""), TS("path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("" "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	// no buff tests

	exp = TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext");

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(exp, out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, STRLEN(exp)+1, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(exp, out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, STRLEN(exp), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
	NW_CHECK_STR_EMPTY(out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(NULL, _countof(out), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, 0, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, 1, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
	NW_CHECK_STR_EMPTY(out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, 5, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
	NW_CHECK_STR_EMPTY(out);

	#ifndef SKIP_LINUX // Drive support is intentionaly limited on LINUX (MS Gived correct result)
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C:::"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);
	#endif

	#ifndef SKIP_LINUX // Drive support is intentionaly limited on LINUX (MS Gived correct result)
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);
	#endif

	#ifndef SKIP_LINUX // Drive support is intentionaly limited on LINUX (MS Gived correct result)
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("COUT"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);
	#endif

	// </body>
    #undef  STRLEN
    #undef  _MAKEPATH_S
    #undef  TS
    #undef  ITEM
}

NW_TEST(NanoWinMSExtraTestGroup, Path_wmakepath_s_Test)
{
    #define ITEM         wchar_t
    #define TS(X)        L"" X
    #define _MAKEPATH_S _wmakepath_s
    #define STRLEN      wcslen
	// <body> // invaliant for Path_makepath_s_Test and Path_wmakepath_s_Test

	SETUP_S_TEST();

	ITEM        out[_MAX_PATH];
	const ITEM *exp;

	errno_t rc;

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), TS("ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), NULL);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, TS("name"), TS(""));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("name"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS(""));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS("ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, NULL, NULL, TS("..ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("..ext"), out);

	// path

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path"), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "path2"), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "path2" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "path2" DSEPS "name.ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	// abs path

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS "path1" DSEPS "path2"), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS "path1" DSEPS "path2" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS "path1" DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS "path1" DSEPS "path2" DSEPS "name.ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	// 2 dots not expanded

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(".." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(".." DSEPS "path2" DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS ".." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS ".." DSEPS "path2" DSEPS "name.ext"), out);

	// kinda ugly, but works this way
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS(DSEPS ".." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(DSEPS ".." DSEPS "path2" DSEPS "name.ext"), out);

	// single dot not removed

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), NULL, TS("path1" DSEPS "." DSEPS "path2" DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("path1" DSEPS "." DSEPS "path2" DSEPS "name.ext"), out);

	// drive

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS(""), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS(""), TS("path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("" "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);

	// no buff tests

	exp = TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext");

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(exp, out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, STRLEN(exp)+1, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(exp, out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, STRLEN(exp), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
	NW_CHECK_STR_EMPTY(out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(NULL, _countof(out), TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, 0, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, 1, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
	NW_CHECK_STR_EMPTY(out);

	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, 5, TS("C:"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
	NW_CHECK_STR_EMPTY(out);

	#ifndef SKIP_LINUX // Drive support is intentionaly limited on LINUX (MS Gived correct result)
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C:::"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);
	#endif

	#ifndef SKIP_LINUX // Drive support is intentionaly limited on LINUX (MS Gived correct result)
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("C"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);
	#endif

	#ifndef SKIP_LINUX // Drive support is intentionaly limited on LINUX (MS Gived correct result)
	NW_RESET_VAR_FILL_5A(out);
	rc = _MAKEPATH_S(out, _countof(out), TS("COUT"), TS(DSEPS "path1" DSEPS "path2" DSEPS DSEPS), TS("name"), TS(".ext"));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:" DSEPS "path1" DSEPS "path2" DSEPS DSEPS "name.ext"), out);
	#endif

	// </body>
    #undef  STRLEN
    #undef  _MAKEPATH_S
    #undef  TS
    #undef  ITEM
}

NW_TEST(NanoWinMSExtraTestGroup, Path_splitpath_s_Test)
{
    #define ITEM         char
    #define TS(X)        "" X
    #define STRLEN       strlen
    #define _SPLITPATH_S _splitpath_s
	// <body> // invaliant for Path_splitpath_s_Test and Path_wsplitpath_s_Test
    #define PAIR(arr)    arr, _countof(arr)

	SETUP_S_TEST();

	ITEM      outdrive[_MAX_DRIVE];
	ITEM      outdir  [_MAX_DIR];
	ITEM      outfname[_MAX_FNAME];
	ITEM      outext  [_MAX_EXT];

	errno_t rc;

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	rc = _SPLITPATH_S(TS("name"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("name.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:name.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("name.aux2.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:name.aux2.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/name.aux2.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("path2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("/path1/path2"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("path2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("path1/path2"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("path1/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("path2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("./"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("./"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("C:./"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("./"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("."), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS("."), outext);

	// bit ugly, but work this way
	rc = _SPLITPATH_S(TS(".."), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("."), outfname);
    NW_CHECK_EQUAL_STRCMP(TS("."), outext);

	rc = _SPLITPATH_S(TS("../"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("../"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	// buffer errors

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), NULL,0, PAIR(outdir), PAIR(outfname), PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), NULL,0, PAIR(outfname), PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), NULL,0, PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), NULL,0);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), NULL,1);
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    NW_CHECK_STR_EMPTY(outdrive);
    NW_CHECK_STR_EMPTY(outdir);
    NW_CHECK_STR_EMPTY(outfname);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), outext,0);
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    NW_CHECK_STR_EMPTY(outdrive);
    NW_CHECK_STR_EMPTY(outdir);
    NW_CHECK_STR_EMPTY(outfname);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), outext,1); // no space for ext
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
    #ifdef SKIP_MS
    NW_CHECK_ERR_HANDLER_NOT_FIRED(); // Under MS works this way // strange and non-consitent ???
    #else
    NW_CHECK_ERR_HANDLER_FIRED(); // Much more consistent
    #endif
    NW_CHECK_STR_EMPTY(outdrive);
    NW_CHECK_STR_EMPTY(outdir);
    NW_CHECK_STR_EMPTY(outfname);
    NW_CHECK_STR_EMPTY(outext);

    #undef  PAIR
	// </body>
    #undef  STRLEN
    #undef  _SPLITPATH_S
    #undef  TS
    #undef  ITEM
}


NW_TEST(NanoWinMSExtraTestGroup, Path_wsplitpath_s_Test)
{
    #define ITEM         wchar_t
    #define TS(X)        L"" X
    #define STRLEN       wcslen
    #define _SPLITPATH_S _wsplitpath_s
	// <body> // invaliant for Path_splitpath_s_Test and Path_wsplitpath_s_Test
    #define PAIR(arr)    arr, _countof(arr)

	SETUP_S_TEST();

	ITEM      outdrive[_MAX_DRIVE];
	ITEM      outdir  [_MAX_DIR];
	ITEM      outfname[_MAX_FNAME];
	ITEM      outext  [_MAX_EXT];

	errno_t rc;

	NW_RESET_ERRNO();
	NW_RESET_ERR_HANDLER_COUNT();

	rc = _SPLITPATH_S(TS("name"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("name.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:name.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("name.aux2.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:name.aux2.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/name.aux2.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.ext"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("path2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("/path1/path2"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("path2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("path1/path2"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("path1/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("path2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("./"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("./"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("C:./"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("./"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	rc = _SPLITPATH_S(TS("."), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS("."), outext);

	// bit ugly, but work this way
	rc = _SPLITPATH_S(TS(".."), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS(""), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("."), outfname);
    NW_CHECK_EQUAL_STRCMP(TS("."), outext);

	rc = _SPLITPATH_S(TS("../"), outdrive, outdir, outfname, outext);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS(""), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("../"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(""), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(""), outext);

	// buffer errors

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), NULL,0, PAIR(outdir), PAIR(outfname), PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), NULL,0, PAIR(outfname), PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), NULL,0, PAIR(outext));
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS(".ext"), outext);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), NULL,0);
    NW_CHECK_RC_OK(rc);
    NW_CHECK_ERRNO_OK();
	NW_CHECK_ERR_HANDLER_NOT_FIRED();
    NW_CHECK_EQUAL_STRCMP(TS("C:"), outdrive);
    NW_CHECK_EQUAL_STRCMP(TS("/path1/path2/"), outdir);
    NW_CHECK_EQUAL_STRCMP(TS("name.aux2"), outfname);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), NULL,1);
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    NW_CHECK_STR_EMPTY(outdrive);
    NW_CHECK_STR_EMPTY(outdir);
    NW_CHECK_STR_EMPTY(outfname);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), outext,0);
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
	NW_CHECK_ERR_HANDLER_FIRED();
    NW_CHECK_STR_EMPTY(outdrive);
    NW_CHECK_STR_EMPTY(outdir);
    NW_CHECK_STR_EMPTY(outfname);

	rc = _SPLITPATH_S(TS("C:/path1/path2/name.aux2.ext"), PAIR(outdrive), PAIR(outdir), PAIR(outfname), outext,1); // no space for ext
    NW_CHECK_RC_ERR(rc);
    NW_CHECK_ERRNO_ERR();
    #ifdef SKIP_MS
    NW_CHECK_ERR_HANDLER_NOT_FIRED(); // Under MS works this way // strange and non-consitent ???
    #else
    NW_CHECK_ERR_HANDLER_FIRED(); // Much more consistent
    #endif
    NW_CHECK_STR_EMPTY(outdrive);
    NW_CHECK_STR_EMPTY(outdir);
    NW_CHECK_STR_EMPTY(outfname);
    NW_CHECK_STR_EMPTY(outext);

    #undef  PAIR
	// </body>
    #undef  STRLEN
    #undef  _SPLITPATH_S
    #undef  TS
    #undef  ITEM
}

NW_END_TEST_GROUP()

static int vsnwprintf_test_helper(wchar_t *buffer, size_t count, const wchar_t *format,...)
{
  va_list args;

  va_start(args,format);

  #if defined(_MSC_VER)
  int result = _vsnwprintf(buffer,count,format,args);
  #else
  int result = vsnwprintf(buffer, count, format, args);
  #endif

  va_end(args);

  return result;
}

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMSExtraVSNWPrintFTestGroup)

NW_TEST(NanoWinMSExtraTestGroup, VSNWPrintFLargeBufferTest)
{
  constexpr size_t BUFFER_SIZE = 256;
  wchar_t          buffer[BUFFER_SIZE];
  int              char_count;

  char_count = vsnwprintf_test_helper(buffer,sizeof(buffer) / sizeof(wchar_t),L"TEST");

  const wchar_t *expected_str = L"TEST";

  NW_CHECK_EQUAL_INTS(4,char_count);
  NW_CHECK_EQUAL_MEMCMP(expected_str,buffer,wcslen(expected_str)+1);

  char_count = vsnwprintf_test_helper(buffer,sizeof(buffer) / sizeof(wchar_t),L"%d",34);

  expected_str = L"34";

  NW_CHECK_EQUAL_INTS(2,char_count);
  NW_CHECK_EQUAL_MEMCMP(expected_str,buffer,wcslen(expected_str)+1);
}
#if !defined(_MSC_VER)
NW_TEST(NanoWinMSExtraTestGroup, VSNWPrintFSmallBufferTest)
{
  constexpr size_t BUFFER_SIZE = 2;
  wchar_t          buffer[BUFFER_SIZE];
  int              char_count;

  char_count = vsnwprintf_test_helper(buffer, sizeof(buffer) / sizeof(wchar_t), L"TEST");

  NW_CHECK_EQUAL_INTS(4, char_count);
}
#endif
NW_TEST(NanoWinMSExtraTestGroup, VSNWPrintFNoBufferTest)
{
  NW_CHECK_EQUAL_INTS(4,vsnwprintf_test_helper(NULL,0,L"TEST"));
  NW_CHECK_EQUAL_INTS(2,vsnwprintf_test_helper(NULL,0,L"%d",34));
}

NW_TEST(NanoWinMSExtraTestGroup, VSNWPrintFFormatNoBufferTest)
{
  NW_CHECK_EQUAL_INTS(5, vsnwprintf_test_helper(NULL, 0, L"%05d", 34));
  NW_CHECK_EQUAL_INTS(8, vsnwprintf_test_helper(NULL, 0, L"%f", 4.56));
  NW_CHECK_EQUAL_INTS(4, vsnwprintf_test_helper(NULL, 0, L"%.2f", 4.56789));
  NW_CHECK_EQUAL_INTS(2, vsnwprintf_test_helper(NULL, 0, L"%ld", 34L));
}
#ifndef __GNUC__
NW_TEST(NanoWinMSExtraTestGroup, VSNWPrintFFormatNoBufferStrTest)
{
  // MS format policy
  NW_CHECK_EQUAL_INTS(10, vsnwprintf_test_helper(NULL, 0, L"%10s", "abcde"));
  NW_CHECK_EQUAL_INTS(10, vsnwprintf_test_helper(NULL, 0, L"%ld %.1f %s", 34L, 1.234, L"def"));
}
#endif
NW_TEST(NanoWinMSExtraTestGroup, VSNWPrintFLargeStringsTest)
{
  constexpr size_t BUFFER_SIZE = 1024;
  wchar_t          format[BUFFER_SIZE];

  for (size_t i = 0; i < BUFFER_SIZE; i++)
  {
    format[i] = L'Z';
  }

  format[BUFFER_SIZE - 1] = L'\0';

  int char_count;

  char_count = vsnwprintf_test_helper(NULL,0,format);

  NW_CHECK_EQUAL_INTS(BUFFER_SIZE - 1,char_count);
}

NW_END_TEST_GROUP()
