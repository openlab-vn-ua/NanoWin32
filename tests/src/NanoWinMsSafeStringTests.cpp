// Unit tests inspired by Safe C library (https://sourceforge.net/projects/safeclib/)

#include "NWUnitTest.h"

#ifdef __linux
 #include "errno.h"
 #include "NanoWinMsSafeString.h"
#else
 #include <windows.h>
#endif

#if !defined(EOK)
 #define EOK (0)
#endif

#if !defined(RSIZE_MAX_STR)
#define RSIZE_MAX_STR RSIZE_MAX
#endif

#if !defined(RSIZE_MAX_MEM)
#define RSIZE_MAX_MEM RSIZE_MAX
#endif

#define NW_CHECK_RC_OK(rc)        NW_CHECK_EQUAL_INTS(EOK,rc)
#define NW_CHECK_RC_ERR(rc)       NW_CHECK_TRUE(rc != EOK)
#define NW_CHECK_STR_EMPTY(s)     NW_CHECK_TRUE((s)[0] == '\0')
#define NW_CHECK_STR_NOT_EMPTY(s) NW_CHECK_TRUE((s)[0] != '\0')

// Error handler setup
// ---------------------------------------------

#if defined(_MSC_VER)

static void invalid_parameter_report_and_continue
			(
				const wchar_t * expression,  
				const wchar_t * function,   
				const wchar_t * file,   
				unsigned int line,  
				uintptr_t pReserved  
			)
{
	#if 0
	wprintf(L"Invalid parameter detected in function %s."
			L" File: %s Line: %d\n", function, file, line);
	wprintf(L"Expression: %s\n", expression);
	//abort();
	#endif
	return;
}

class  SET_FAIL_FOR_TEST
{
	protected:
	bool                       old_app_param_defined;
    _invalid_parameter_handler old_app_invalid_parameter_handler;
	int                        old_app_crtdbg_report_mode;

	public:
	SET_FAIL_FOR_TEST()
	{
		old_app_param_defined = false;
	}

	void set_fail_as_report_and_continue()
	{
		if (!old_app_param_defined)
		{
			// First call - save app-default handler
			old_app_crtdbg_report_mode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_REPORT_MODE);
			old_app_invalid_parameter_handler = _set_invalid_parameter_handler(invalid_parameter_report_and_continue);
		}
		else
		{
			// App handler already saved, just update our handler
			_set_invalid_parameter_handler(invalid_parameter_report_and_continue);
		}

		_CrtSetReportMode(_CRT_ASSERT, 0);
		old_app_param_defined = true;
	}

	void set_fail_by_restore_default()
	{
	  if (old_app_param_defined)
	  {
		// app-wide old info available, restore it
		_set_invalid_parameter_handler(old_app_invalid_parameter_handler);
		_CrtSetReportMode(_CRT_ASSERT, old_app_crtdbg_report_mode);
	  }
	}

};

static SET_FAIL_FOR_TEST set_fail_for_test_handler;

#define SET_FAIL_AS_REPORT_AND_CONTINUE() set_fail_for_test_handler.set_fail_as_report_and_continue()
#define SET_FAIL_AS_DEFAULT()		      set_fail_for_test_handler.set_fail_by_restore_default()

#else

#define SET_FAIL_AS_REPORT_AND_CONTINUE() // nothing yet
#define SET_FAIL_AS_DEFAULT() // nothing yet

#endif

class SET_FAIL_FOR_TEST_AUTO_CLASS
{
	public:
	SET_FAIL_FOR_TEST_AUTO_CLASS()  { SET_FAIL_AS_REPORT_AND_CONTINUE(); }
	~SET_FAIL_FOR_TEST_AUTO_CLASS() { SET_FAIL_AS_DEFAULT(); }
};

#define SETUP_S_TEST() SET_FAIL_FOR_TEST_AUTO_CLASS __set_fail_auto_init_done_object;

// Some aux tools
// ---------------------------------------------

#if defined(_MSC_VER)
#define SKIP_MS
#endif

// Tests
// ---------------------------------------------

NW_BEGIN_TEST_GROUP(NanoWinMsSafeStringTestGroup)

NW_BEGIN_SETUP_TEARDOWN
NW_SETUP_METHOD()
 {
	// SET_FAIL_AS_REPORT_AND_CONTINUE(); // Does not work for some reason
 }
NW_TEARDOWN_METHOD()
 {
	// SET_FAIL_AS_DEFAULT(); // Does not work for some reason
 }
NW_END_SETUP_TEARDOWN


NW_TEST(NanoWinMsSafeStringTestGroup, StrNCpySTest)
{
	SETUP_S_TEST();

	#define MAX   ( 128 )
	#define LEN   ( 128 )

	char   str1[LEN];
	char   str2[LEN];
	char   dest[LEN];

	errno_t rc;
	rsize_t nlen;
	int32_t ind;

	nlen = 5;
	rc = strncpy_s(NULL, LEN, str2, nlen);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	nlen = 5;
	rc = strncpy_s(str1, 5, NULL, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	nlen = 5;
	rc = strncpy_s(str1, 0, str2, nlen);

	NW_CHECK_RC_ERR(rc);
	// dest not modified
	NW_CHECK_STR_NOT_EMPTY(str1);
	NW_CHECK_TRUE(str1[0]=='a');

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	rc = strncpy_s(str1, (RSIZE_MAX_STR + 1), str2, nlen);

	NW_CHECK_RC_ERR(rc);
	// dest not modified
	NW_CHECK_STR_NOT_EMPTY(str1);
	NW_CHECK_TRUE(str1[0]=='a');
	#endif

	//--------------------------------------------------//
	//--------------------------------------------------//

	#ifndef SKIP_MS
	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';

	rc = strncpy_s(str1, 5, str2, 0);

	NW_CHECK_RC_ERR(rc); // Under MS rc==EOK, that is against the standard
	NW_CHECK_STR_EMPTY(str1); // Will be "" MS, that is ok
	#endif

	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	rc = strncpy_s(str1, 5, str2, (RSIZE_MAX_STR + 1));

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1); // count too big (unreasonable) target should be clear

	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';
	nlen = 5;

	rc = strncpy_s(&str1[0], LEN / 2, &str2[0], nlen);

	NW_CHECK_RC_OK(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	nlen = 5;

	// test overlap //
	rc = strncpy_s(str1, LEN, str1, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	nlen = 18;

	rc = strncpy_s(&str1[0], LEN, &str1[5], nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);
	#endif

	//--------------------------------------------------//

	strcpy(str1, "keep it simple");
	str2[0] = '\0';

	nlen = 10;
	rc = strncpy_s(str1, LEN, str2, nlen);

	NW_CHECK_RC_OK(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	nlen = 20;
	rc = strncpy_s(str1, LEN, str2, nlen);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(str1, str2);
	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	nlen = 32;
	rc = strncpy_s(str1, LEN, str2, nlen);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//
	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strncpy_s(str1, 1, str2, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strncpy_s(str1, 2, str2, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//
	// TR example //

	strcpy(dest, "                            ");
	strcpy(str1, "hello");

	rc = strncpy_s(dest, 6, str1, 100);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(dest, str1);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//
	// TR example //

	strcpy(dest, "                            ");
	strcpy(str2, "goodbye");

	rc = strncpy_s(dest, 5, str2, 7);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//
	// TR example //

	strcpy(dest, "                            ");
	strcpy(str2, "goodbye");

	rc = strncpy_s(dest, 5, str2, 4);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //

	ind = strcmp(dest, "good");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(dest, "                            ");
	strcpy(str2, "good");

	//   strnlen("good") < 5
	rc = strncpy_s(dest, 5, str2, 8);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(dest, "good");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "qq12345weqeqeqeq");
	strcpy(str2, "it");

	nlen = 10;
	rc = strncpy_s(str1, 10, str2, nlen);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	// **** Truncate tests ****

    #if (defined(_TRUNCATE) && defined(STRUNCATE))

	//--------------------------------------------------//

	strcpy(str1, "qwertyuiopasdfghjklzxcvbnm");
	strcpy(str2, "1234567");

	rc = strncpy_s(str1, 5, str2, _TRUNCATE);

	NW_CHECK_EQUAL_INTS(rc, STRUNCATE);

	// be sure the results are the same as strcmp //
	NW_CHECK_EQUAL(strcmp(str1, "1234"), 0);

	//--------------------------------------------------//

	strcpy(str1, "qwertyuiopasdfghjklzxcvbnm");
	strcpy(str2, "1234567");

	rc = strncpy_s(str1, 1, str2, _TRUNCATE);

	NW_CHECK_EQUAL_INTS(rc, STRUNCATE);

	// be sure the results are the same as strcmp //
	NW_CHECK_EQUAL(strcmp(str1, ""), 0);

	//--------------------------------------------------//

	#endif

	#undef MAX
	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, WcsNCpySTest)
{
	SETUP_S_TEST();

	#define MAX   ( 128 )
	#define LEN   ( 128 )

	wchar_t   str1[LEN];
	wchar_t   str2[LEN];
	wchar_t   dest[LEN];

	errno_t rc;
	rsize_t nlen;
	int32_t ind;

	nlen = 5;
	rc = wcsncpy_s(NULL, LEN, str2, nlen);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	nlen = 5;
	rc = wcsncpy_s(str1, 5, NULL, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	wcscpy(str2, L"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	nlen = 5;
	rc = wcsncpy_s(str1, 0, str2, nlen);

	NW_CHECK_RC_ERR(rc);
	// dest not modified
	NW_CHECK_STR_NOT_EMPTY(str1);
	NW_CHECK_TRUE(str1[0]=='a');

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	wcscpy(str2, L"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	rc = wcsncpy_s(str1, (RSIZE_MAX_STR + 1), str2, nlen);

	NW_CHECK_RC_ERR(rc);
	// dest not modified
	NW_CHECK_STR_NOT_EMPTY(str1);
	NW_CHECK_TRUE(str1[0]=='a');
	#endif

	//--------------------------------------------------//
	//--------------------------------------------------//

	#ifndef SKIP_MS
	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';

	rc = wcsncpy_s(str1, 5, str2, 0);

	NW_CHECK_RC_ERR(rc); // Under MS rc==EOK, that is against the standard
	NW_CHECK_STR_EMPTY(str1); // Will be "" MS, that is ok
	#endif

	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	wcscpy(str2, L"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

	rc = wcsncpy_s(str1, 5, str2, (RSIZE_MAX_STR + 1));

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1); // count too big (unreasonable) target should be clear

	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';
	nlen = 5;

	rc = wcsncpy_s(&str1[0], LEN / 2, &str2[0], nlen);

	NW_CHECK_RC_OK(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	nlen = 5;

	// test overlap //
	rc = wcsncpy_s(str1, LEN, str1, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	nlen = 18;

	rc = wcsncpy_s(&str1[0], LEN, &str1[5], nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);
	#endif

	//--------------------------------------------------//

	wcscpy(str1, L"keep it simple");
	str2[0] = '\0';

	nlen = 10;
	rc = wcsncpy_s(str1, LEN, str2, nlen);

	NW_CHECK_RC_OK(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	str1[0] = '\0';
	wcscpy(str2, L"keep it simple");

	nlen = 20;
	rc = wcsncpy_s(str1, LEN, str2, nlen);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(str1, str2);
	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"keep it simple");

	nlen = 32;
	rc = wcsncpy_s(str1, LEN, str2, nlen);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//
	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"keep it simple");

	rc = wcsncpy_s(str1, 1, str2, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"keep it simple");

	rc = wcsncpy_s(str1, 2, str2, nlen);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_STR_EMPTY(str1);

	//--------------------------------------------------//
	// TR example //

	wcscpy(dest, L"                            ");
	wcscpy(str1, L"hello");

	rc = wcsncpy_s(dest, 6, str1, 100);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(dest, str1);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//
	// TR example //

	wcscpy(dest, L"                            ");
	wcscpy(str2, L"goodbye");

	rc = wcsncpy_s(dest, 5, str2, 7);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//
	// TR example //

	wcscpy(dest, L"                            ");
	wcscpy(str2, L"goodbye");

	rc = wcsncpy_s(dest, 5, str2, 4);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //

	ind = wcscmp(dest, L"good");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(dest, L"                            ");
	wcscpy(str2, L"good");

	//   strnlen("good") < 5
	rc = wcsncpy_s(dest, 5, str2, 8);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(dest, L"good");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"qq12345weqeqeqeq");
	wcscpy(str2, L"it");

	nlen = 10;
	rc = wcsncpy_s(str1, 10, str2, nlen);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	// **** Truncate tests ****

    #if (defined(_TRUNCATE) && defined(STRUNCATE))

	//--------------------------------------------------//

	wcscpy(str1, L"qwertyuiopasdfghjklzxcvbnm");
	wcscpy(str2, L"1234567");

	rc = wcsncpy_s(str1, 5, str2, _TRUNCATE);

	NW_CHECK_EQUAL_INTS(rc, STRUNCATE);

	// be sure the results are the same as strcmp //
	NW_CHECK_EQUAL(wcscmp(str1, L"1234"), 0);

	//--------------------------------------------------//

	wcscpy(str1, L"qwertyuiopasdfghjklzxcvbnm");
	wcscpy(str2, L"1234567");

	rc = wcsncpy_s(str1, 1, str2, _TRUNCATE);

	NW_CHECK_EQUAL_INTS(rc, STRUNCATE);

	// be sure the results are the same as strcmp //
	NW_CHECK_EQUAL(wcscmp(str1, L""), 0);

	//--------------------------------------------------//

	#endif

	#undef MAX
	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, MemCpySTest)
{
	SETUP_S_TEST();

	#define LEN   ( 1024 )

	uint8_t  mem1[LEN + 2];
	uint8_t  mem2[LEN + 2];


	errno_t rc;
	uint32_t i;
	rsize_t len;

	//--------------------------------------------------//

	rc = memcpy_s(NULL, LEN, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	//--------------------------------------------------//

	rc = memcpy_s(mem1, 0, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	rc = memcpy_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = memcpy_s(mem1, LEN, NULL, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	rc = memcpy_s(mem1, 10, mem2, 0);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = memcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	for (i = 0; i < LEN + 1; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(mem1[i], mem2[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[i]);

	//--------------------------------------------------//

	for (i = 0; i < LEN + 1; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, (len + 1));

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[i]);

	//--------------------------------------------------//

	for (i = 0; i < LEN + 2; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN / 2;
	rc = memcpy_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[len]);

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	for (i = 0; i < LEN + 2; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, 0);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_FALSE(mem1[len] == 0);

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 55; }

	// same ptr - no move //
	rc = memcpy_s(mem1, LEN, mem1, LEN);
	NW_CHECK_RC_OK(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	for (i = 0; i < LEN; i++) { mem1[i] = 55; }
	for (i = 0; i < LEN; i++) { mem2[i] = 65; }

	// overlap //
	len = 100;
	rc = memcpy_s(&mem1[0], len, &mem1[10], len);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_FALSE(mem1[len] == 0);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	for (i = 0; i < LEN; i++) { mem1[i] = 55; }
	for (i = 0; i < LEN; i++) { mem2[i] = 65; }

	// overlap //
	len = 100;
	rc = memcpy_s(&mem1[10], len, &mem1[0], len);

	NW_CHECK_RC_ERR(rc);

	for (i = 10; i < len + 10; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 35; }
	for (i = 0; i < LEN; i++) { mem2[i] = 55; }

	len = 5;
	rc = memcpy_s(mem1, len, mem2, len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(55, mem1[i]);
	}

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 35; }
	for (i = 0; i < LEN; i++) { mem2[i] = 55; }

	rc = memcpy_s(mem1, LEN, mem2, LEN / 2);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < LEN / 2; i++)
	{
		NW_CHECK_EQUAL_BYTES(55, mem1[i]);
	}

	//--------------------------------------------------//

	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, WMemCpySTest)
{
	SETUP_S_TEST();

	#define LEN   ( 1024 )

	wchar_t  mem1[LEN + 2];
	wchar_t  mem2[LEN + 2];

	errno_t rc;
	uint32_t i;
	rsize_t len;

	//--------------------------------------------------//

	rc = wmemcpy_s(NULL, LEN, mem2, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = wmemcpy_s(mem1, 0, mem2, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	rc = wmemcpy_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = wmemcpy_s(mem1, LEN, NULL, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	rc = wmemcpy_s(mem1, 10, mem2, 0);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = wmemcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	for (i = 0; i < LEN + 1; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = wmemcpy_s(mem1, len, mem2, len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(mem1[i], mem2[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[i]);

	//--------------------------------------------------//

	for (i = 0; i < LEN + 1; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = wmemcpy_s(mem1, len, mem2, (len + 1));

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[i]);

	//--------------------------------------------------//

	for (i = 0; i < LEN + 2; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN / 2;
	rc = wmemcpy_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[len]);

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	for (i = 0; i < LEN + 2; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = wmemcpy_s(mem1, len, mem2, 0);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = wmemcpy_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_FALSE(mem1[len] == 0);

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 55; }

	// same ptr - no move //
	rc = wmemcpy_s(mem1, LEN, mem1, LEN);
	NW_CHECK_RC_OK(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	for (i = 0; i < LEN; i++) { mem1[i] = 55; }
	for (i = 0; i < LEN; i++) { mem2[i] = 65; }

	// overlap //
	len = 100;
	rc = wmemcpy_s(&mem1[0], len, &mem1[10], len);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_FALSE(mem1[len] == 0);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	for (i = 0; i < LEN; i++) { mem1[i] = 55; }
	for (i = 0; i < LEN; i++) { mem2[i] = 65; }

	// overlap //
	len = 100;
	rc = wmemcpy_s(&mem1[10], len, &mem1[0], len);

	NW_CHECK_RC_ERR(rc);

	for (i = 10; i < len + 10; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 35; }
	for (i = 0; i < LEN; i++) { mem2[i] = 55; }

	len = 5;
	rc = wmemcpy_s(mem1, len, mem2, len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(55, mem1[i]);
	}

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 35; }
	for (i = 0; i < LEN; i++) { mem2[i] = 55; }

	rc = wmemcpy_s(mem1, LEN, mem2, LEN / 2);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < LEN / 2; i++)
	{
		NW_CHECK_EQUAL_BYTES(55, mem1[i]);
	}

	//--------------------------------------------------//

	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, MemMoveSTest)
{
	SETUP_S_TEST();

	#define LEN   ( 1024 )

	uint8_t  mem1[LEN];
	uint8_t  mem2[LEN];


	errno_t rc;
	uint32_t i;
	uint32_t len;

	//--------------------------------------------------//

	rc = memmove_s(NULL, LEN, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	//--------------------------------------------------//

	rc = memmove_s(mem1, 0, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	rc = memmove_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = memmove_s(mem1, LEN, NULL, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//  it is not clear why original test expects error

	rc = memmove_s(mem1, 10, mem2, 0);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = memmove_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
	NW_CHECK_RC_ERR(rc);


	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// a valid move //
	len = LEN;
	rc = memmove_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_OK(rc);
	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(mem2[i], mem1[i]);
	}

	//--------------------------------------------------//

	#ifndef SKIP_MS // F
	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// length error //
	len = LEN / 2;
	rc = memmove_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memmove_s(mem1, len, mem2, 0);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // F
	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// invalid length - zero dest //
	len = LEN;
	rc = memmove_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);


	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// same ptr - no move //
	rc = memmove_s(mem1, LEN, mem1, LEN);

	NW_CHECK_RC_OK(rc);


	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem1[i] = 35; }

	// overlap move //
	len = 20;
	rc = memmove_s(&mem1[0], len, &mem1[10], len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(35, mem1[i]);
	}

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem1[i] = 35; }

	// overlap move //
	len = 20;
	rc = memmove_s(&mem1[10], len, &mem1[0], len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < LEN; i++) { mem2[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem2[i] = 35; }


	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(25, mem1[i]);
	}

	//--------------------------------------------------//
	//--------------------------------------------------//

	#undef LEN
}

///////////////////////////////////////////////////////////////////
NW_TEST(NanoWinMsSafeStringTestGroup, WMemMoveSTest)
{
	SETUP_S_TEST();

	#define LEN   ( 1024 )

	wchar_t  mem1[LEN];
	wchar_t  mem2[LEN];

	errno_t rc;
	uint32_t i;
	uint32_t len;

	//--------------------------------------------------//

	rc = wmemmove_s(NULL, LEN, mem2, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = wmemmove_s(mem1, 0, mem2, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	rc = wmemmove_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = wmemmove_s(mem1, LEN, NULL, LEN);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//  it is not clear why original test expects error

	rc = memmove_s(mem1, 10, mem2, 0);
	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	rc = wmemmove_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// a valid move //
	len = LEN;
	rc = wmemmove_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_OK(rc);
	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(mem2[i], mem1[i]);
	}

	//--------------------------------------------------//

	#ifndef SKIP_MS // F
	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// length error //
	len = LEN / 2;
	rc = wmemmove_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = wmemmove_s(mem1, len, mem2, 0);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // F
	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// invalid length - zero dest //
	len = LEN;
	rc = wmemmove_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);


	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	// same ptr - no move //
	rc = wmemmove_s(mem1, LEN, mem1, LEN);

	NW_CHECK_RC_OK(rc);


	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem1[i] = 35; }

	// overlap move //
	len = 20;
	rc = wmemmove_s(&mem1[0], len, &mem1[10], len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(35, mem1[i]);
	}

	//--------------------------------------------------//

	for (i = 0; i < LEN; i++) { mem1[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem1[i] = 35; }

	// overlap move //
	len = 20;
	rc = wmemmove_s(&mem1[10], len, &mem1[0], len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < LEN; i++) { mem2[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem2[i] = 35; }


	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(25, mem1[i]);
	}

	//--------------------------------------------------//
	//--------------------------------------------------//

	#undef LEN
}
///////////////////////////////////////////////////////////////////

NW_TEST(NanoWinMsSafeStringTestGroup, StrCatSTest)
{
	SETUP_S_TEST();

	#define LEN   ( 128 )

	char   str1[LEN];
	char   str2[LEN];

	errno_t rc;
	int32_t  ind;
	int32_t  len1;
	int32_t  len2;
	int32_t  len3;

	//--------------------------------------------------//

	rc = strcat_s(NULL, LEN, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = strcat_s(str1, LEN, NULL);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = strcat_s(str1, 0, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP
	rc = strcat_s(str1, (RSIZE_MAX_STR + 1), str2);

	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//
	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaa");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 1, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaa");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 2, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	strcpy(&str1[0], "aaaaaaaaaa");
	strcpy(&str2[0], "keep it simple");

	len1 = strlen(str1);
	len2 = strlen(str2);

	rc = strcat_s(str1, 50, str2);

	NW_CHECK_RC_OK(rc);

	len3 = strlen(str1);

	NW_CHECK_EQUAL(len1 + len2, len3);

	//--------------------------------------------------//
	//--------------------------------------------------//

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 1, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 2, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 20, str2);

	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//
	//--------------------------------------------------//

	str1[0] = '\0';
	str2[0] = '\0';

	rc = strcat_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "qqweqq");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, "qqweqqkeep it simple");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "1234");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 12, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	strcpy(str1, "1234");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 52, str2);

	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, "1234keep it simple");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "12345678901234567890");

	rc = strcat_s(str1, 8, &str1[7]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	strcpy(str1, "123456789");

	rc = strcat_s(str1, 9, &str1[8]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	strcpy(str2, "123");
	strcpy(str1, "keep it simple");

	rc = strcat_s(str2, 31, &str1[0]);

	NW_CHECK_RC_OK(rc);

	ind = strcmp(str2, "123keep it simple");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str2, "1234");
	strcpy(str1, "56789");

	rc = strcat_s(str2, 10, str1);

	NW_CHECK_RC_OK(rc);

	ind = strcmp(str2, "123456789");

	NW_CHECK_EQUAL(0, ind);

	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, WcsCatSTest)
{
	SETUP_S_TEST();

	#define LEN   ( 128 )

	wchar_t   str1[LEN];
	wchar_t   str2[LEN];

	errno_t rc;
	int32_t  ind;
	int32_t  len1;
	int32_t  len2;
	int32_t  len3;

	//--------------------------------------------------//

	rc = wcscat_s(NULL, LEN, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = wcscat_s(str1, LEN, NULL);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = wcscat_s(str1, 0, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP
	rc = wcscat_s(str1, (RSIZE_MAX_STR + 1), str2);

	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//
	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaa");
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 1, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaa");
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 2, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	wcscpy(&str1[0], L"aaaaaaaaaa");
	wcscpy(&str2[0], L"keep it simple");

	len1 = wcslen(str1);
	len2 = wcslen(str2);

	rc = wcscat_s(str1, 50, str2);

	NW_CHECK_RC_OK(rc);

	len3 = wcslen(str1);

	NW_CHECK_EQUAL(len1 + len2, len3);

	//--------------------------------------------------//
	//--------------------------------------------------//

	str1[0] = '\0';
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 1, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 2, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 20, str2);

	NW_CHECK_RC_OK(rc);

	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//
	//--------------------------------------------------//

	str1[0] = '\0';
	str2[0] = '\0';

	rc = wcscat_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqq");
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	ind = wcscmp(str1, L"qqweqqkeep it simple");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"1234");
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 12, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	wcscpy(str1, L"1234");
	wcscpy(str2, L"keep it simple");

	rc = wcscat_s(str1, 52, str2);

	NW_CHECK_RC_OK(rc);

	ind = wcscmp(str1, L"1234keep it simple");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"12345678901234567890");

	rc = wcscat_s(str1, 8, &str1[7]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	wcscpy(str1, L"123456789");

	rc = wcscat_s(str1, 9, &str1[8]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	wcscpy(str2, L"123");
	wcscpy(str1, L"keep it simple");

	rc = wcscat_s(str2, 31, &str1[0]);

	NW_CHECK_RC_OK(rc);

	ind = wcscmp(str2, L"123keep it simple");

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str2, L"1234");
	wcscpy(str1, L"56789");

	rc = wcscat_s(str2, 10, str1);

	NW_CHECK_RC_OK(rc);

	ind = wcscmp(str2, L"123456789");

	NW_CHECK_EQUAL(0, ind);

	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, StrCpySTest)
{
	SETUP_S_TEST();

	#define MAX   ( 128 )
	#define LEN   ( 128 )

	char   str1[LEN];
	char   str2[LEN];

	errno_t rc;
	int32_t  ind;

	//--------------------------------------------------//

	rc = strcpy_s(NULL, LEN, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	rc = strcpy_s(str1, 5, NULL);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	rc = strcpy_s(str1, 0, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP
	rc = strcpy_s(str1, (RSIZE_MAX_STR + 1), str2);

	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//
	//--------------------------------------------------//

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';

	rc = strcpy_s(str1, LEN / 2, str2);

	NW_CHECK_RC_OK(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	#if 0  // TODO: check correct behavior [verify TEST expectation]
	//  test is uncorrect, standard says that result of this operation will be error
	//  to check if src==dest should this be considered as "overlap" ?
	//  memcpy does not assume that (src==dest is ok for memecpy)
	//  should we just check that length assumption is match and go on?

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	rc = strcpy_s(str1, LEN, str1);

	NW_CHECK_RC_OK(rc);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	strcpy(&str1[0], "keep it simple");

	rc = strcpy_s(&str1[0], LEN, &str1[5]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');
	#endif

	//--------------------------------------------------//

	strcpy(&str1[0], "keep it simple");
	str2[0] = '\0';

	rc = strcpy_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	NW_CHECK(*str1 == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	strcpy(&str2[0], "keep it simple");

	rc = strcpy_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strcpy_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strcpy_s(str1, 1, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(*str1 == '\0');

	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strcpy_s(str1, 2, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(*str1 == '\0');

	//--------------------------------------------------//

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "it");

	rc = strcpy_s(str1, 3, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	strcpy(str1, "qq12345weqeqeqeq");
	strcpy(str2, "it");

	rc = strcpy_s(str1, 10, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcpy //
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	#undef MAX
	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, WcsCpySTest)
{
	SETUP_S_TEST();

	#define MAX   ( 128 )
	#define LEN   ( 128 )

	wchar_t   str1[LEN];
	wchar_t   str2[LEN];

	errno_t rc;
	int32_t  ind;

	//--------------------------------------------------//

	rc = wcscpy_s(NULL, LEN, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	rc = wcscpy_s(str1, 5, NULL);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	rc = wcscpy_s(str1, 0, str2);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP
	rc = wcscpy_s(str1, (RSIZE_MAX_STR + 1), str2);

	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//
	//--------------------------------------------------//

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';

	rc = wcscpy_s(str1, LEN / 2, str2);

	NW_CHECK_RC_OK(rc);

	NW_CHECK(str1[0] == '\0');

	//--------------------------------------------------//

	#if 0  // TODO: check correct behavior [verify TEST expectation]
	//  test is uncorrect, standard says that result of this operation will be error
	//  to check if src==dest should this be considered as "overlap" ?
	//  memcpy does not assume that (src==dest is ok for memecpy)
	//  should we just check that length assumption is match and go on?

	wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	rc = wcscpy_s(str1, LEN, str1);

	NW_CHECK_RC_OK(rc);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // Assert
	wcscpy(&str1[0], L"keep it simple");

	rc = wcscpy_s(&str1[0], LEN, &str1[5]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');
	#endif

	//--------------------------------------------------//

	wcscpy(&str1[0], L"keep it simple");
	str2[0] = '\0';

	rc = wcscpy_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	NW_CHECK(*str1 == '\0');

	//--------------------------------------------------//

	str1[0] = '\0';
	wcscpy(&str2[0], L"keep it simple");

	rc = wcscpy_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"keep it simple");

	rc = wcscpy_s(str1, LEN, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"keep it simple");

	rc = wcscpy_s(str1, 1, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(*str1 == '\0');

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"keep it simple");

	rc = wcscpy_s(str1, 2, str2);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(*str1 == '\0');

	//--------------------------------------------------//

	wcscpy(str1, L"qqweqeqeqeq");
	wcscpy(str2, L"it");

	rc = wcscpy_s(str1, 3, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcmp //
	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	//--------------------------------------------------//

	wcscpy(str1, L"qq12345weqeqeqeq");
	wcscpy(str2, L"it");

	rc = wcscpy_s(str1, 10, str2);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcpy //
	ind = wcscmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	#undef MAX
	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, StrUprSTest)
{
    #define STRS     "aqweAQWEaqweAQWE12345!@#$%aqweAQWE" // known source
    #define STRD     "AQWEAQWEAQWEAQWE12345!@#$%AQWEAQWE" // Known good answer
    #define STRCPY   strcpy
    #define STRCMP   strcmp
    #define STRFUN   _strupr_s

	SETUP_S_TEST();

	#define MAX   ( 128 )
	#define LEN   ( 128 )

	char   str1[LEN];
	char   str2[LEN];

	errno_t rc;

	//--------------------------------------------------//

	rc = STRFUN(NULL, LEN);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = STRFUN(NULL, 0);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	rc = STRFUN(str1, 0);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP
	memset(str1,0,sizeof(str1)); // null-terminated

	rc = STRFUN(str1, RSIZE_MAX_STR + 1);

	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	#ifndef SKIP_MS // TRP
	STRCPY(str2,STRS);
	memset(str1,str2[0],sizeof(str1)); // NOT null-terminated

	rc = STRFUN(str1, RSIZE_MAX_STR + 1);

	NW_CHECK_RC_ERR(rc);
	#endif

	//--------------------------------------------------//

	STRCPY(str2,STRS);
	memset(str1,str2[0],sizeof(str1)); // NOT null-terminated

	rc = STRFUN(str1, LEN);

	NW_CHECK_RC_ERR(rc);

	//--------------------------------------------------//

	STRCPY(str1,STRS);
	STRCPY(str2,STRD);
	rc = STRFUN(str1, LEN);

	NW_CHECK_RC_OK(rc);

	// be sure the results are the same as strcpy //
	NW_CHECK_EQUAL(0, STRCMP(str1, str2));

	//--------------------------------------------------//

	#undef MAX
	#undef LEN

    #undef STRS
    #undef STRD
    #undef STRCPY
    #undef STRCMP
    #undef STRFUN
}

NW_TEST(NanoWinMsSafeStringTestGroup, MbsToWcsSTest)
{
  #define MEMSET   memset

  SETUP_S_TEST();

  #define LEN   ( 128 )
  #define INVALID_COUNT ( 0xABCD )
  #define MAGIC         ((wchar_t)-1)

  #define NW_CHECK_GUARDS() \
   NW_CHECK_EQUAL_ULONGS(MAGIC,dest_with_guards[0]); \
   NW_CHECK_EQUAL_ULONGS(MAGIC,dest_with_guards[LEN + 1]);

  wchar_t   dest_with_guards[LEN + 2];
  wchar_t  *dest = &dest_with_guards[1];
  const unsigned char *dest_bytes = (unsigned char*)dest;

  dest_with_guards[0]       = MAGIC;
  dest_with_guards[LEN + 1] = MAGIC;

  errno_t rc;
  size_t  outc;
  char helloStr[] = "hello";
  wchar_t helloWStr[] = L"hello";

  //--------------------------------------------------//
  // simple conversion must succeed

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,LEN,helloStr,LEN - 1);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_MEMCMP(helloWStr,dest,sizeof(helloWStr));
  NW_CHECK_EQUAL_ULONGS(6,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if src == NULL

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,LEN,NULL,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_EQUAL_ULONGS(0,dest[0]);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dst == NULL and dstsz > 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,NULL,LEN,helloStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_LONGS(INVALID_COUNT,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dst != NULL and dstsz == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,0,helloStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_LONGS(INVALID_COUNT,outc);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[0]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[1]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[2]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[3]);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if retval == NULL

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(NULL,dest,LEN,helloStr,LEN - 1);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_MEMCMP(helloWStr,dest,sizeof(helloWStr));
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dest == NULL, destsz == 0, count == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,NULL,0,helloStr,0);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_ULONGS(6,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dest == NULL, destsz == 0, count > 0 && count < len(src)
  // (ms implementation doesn't check count argument and just returns full length

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,NULL,0,helloStr,2);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_ULONGS(6,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok len(src) > count

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,LEN,helloStr,2);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_MEMCMP(helloWStr,dest,2 * sizeof(wchar_t));
  NW_CHECK_EQUAL_ULONGS(0,dest[2]);
  NW_CHECK_EQUAL_ULONGS(3,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check error if dstsz == len(src)

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,5,helloStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_ULONGS(0,dest[0]);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check error if dstsz < len(src)

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,2,helloStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_ULONGS(0,dest[0]);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check error if dstsz == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,0,helloStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[0]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[1]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[2]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[3]);
  NW_CHECK_EQUAL_ULONGS(INVALID_COUNT,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dstsz > 0, count == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,LEN,helloStr,0);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_ULONGS(0,dest[0]);
  NW_CHECK_EQUAL_ULONGS(1,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dstsz < count and count < len(src)

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,2,helloStr,3);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_ULONGS(L'\0',dest[0]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[4]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[5]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[6]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[7]);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dstsz == 0 and count < len(src) (also ensures that
  // writes outside dest happens

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,dest,0,helloStr,3);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[0]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[1]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[2]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[3]);
  NW_CHECK_EQUAL_ULONGS(INVALID_COUNT,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dstsz == 0 and count == 0 with empty string

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = mbstowcs_s(&outc,NULL,0,"",0);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[0]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[1]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[2]);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[3]);
  NW_CHECK_EQUAL_ULONGS(1,outc);
  NW_CHECK_GUARDS();

  #undef NW_CHECK_GUARDS
  #undef MAGIC
  #undef INVALID_COUNT
  #undef LEN
 
  #undef MEMSET
}

NW_TEST(NanoWinMsSafeStringTestGroup, WcsToMbsSTest)
{
  #define MEMSET   memset

  SETUP_S_TEST();

  #define LEN   ( 128 )
  #define INVALID_COUNT (0xABCD)
  #define MAGIC         ((char)0xEF)

  #define NW_CHECK_GUARDS() \
   NW_CHECK_EQUAL_BYTES(MAGIC,dest_with_guards[0]); \
   NW_CHECK_EQUAL_BYTES(MAGIC,dest_with_guards[LEN + 1]);

  char      dest_with_guards[LEN + 2];
  char     *dest = &dest_with_guards[1];

  dest_with_guards[0]       = MAGIC;
  dest_with_guards[LEN + 1] = MAGIC;

  errno_t rc;
  size_t  outc;
  char helloStr[] = "hello";
  wchar_t helloWStr[] = L"hello";

  //--------------------------------------------------//
  // simple conversion must succeed

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,LEN,helloWStr,LEN - 1);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_STRCMP(helloStr,dest);
  NW_CHECK_EQUAL_ULONGS(6,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if src == NULL

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,LEN,NULL,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_EQUAL_BYTES(0,dest[0]);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dst == NULL and dstsz > 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,NULL,LEN,helloWStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_LONGS(INVALID_COUNT,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dst != NULL and dstsz == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,0,helloWStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_LONGS(INVALID_COUNT,outc);
  NW_CHECK_EQUAL_BYTES('z',dest[0]);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if retval == NULL

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(NULL,dest,LEN,helloWStr,LEN - 1);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_STRCMP(helloStr,dest);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dest == NULL, destsz == 0, count == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,NULL,0,helloWStr,0);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_ULONGS(6,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dest == NULL, destsz == 0, count > 0 && count < len(src)
  // (ms implementation doesn't check count argument and just returns full length

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,NULL,0,helloWStr,2);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_ULONGS(6,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok len(src) > count

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,LEN,helloWStr,2);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_MEMCMP(helloStr,dest,2);
  NW_CHECK_EQUAL_BYTES(0,dest[2]);
  NW_CHECK_EQUAL_ULONGS(3,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check error if dstsz == len(src)

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,5,helloWStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES(0,dest[0]);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check error if dstsz < len(src)

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,2,helloWStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES(0,dest[0]);
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check error if dstsz == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,0,helloWStr,LEN - 1);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES('z',dest[0]);
  NW_CHECK_EQUAL_ULONGS(INVALID_COUNT,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dstsz > 0, count == 0

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,LEN,helloWStr,0);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_BYTES(0,dest[0]);
  NW_CHECK_EQUAL_ULONGS(1,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dstsz < count and count < len(src)

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,2,helloWStr,3);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES('\0',dest[0]);
///////  NW_CHECK_EQUAL_BYTES('z',dest[1]); // MS changes this (this conforms to standard)
  NW_CHECK_EQUAL_ULONGS(0,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check fails if dstsz == 0 and count < len(src) (also ensures that
  // writes outside dest happens

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,dest,0,helloWStr,3);

  NW_CHECK_RC_ERR(rc);
  NW_CHECK_EQUAL_BYTES('z',dest[0]);
  NW_CHECK_EQUAL_ULONGS(INVALID_COUNT,outc);
  NW_CHECK_GUARDS();

  //--------------------------------------------------//
  // check ok if dstsz == 0 and count == 0 with empty string

  outc = INVALID_COUNT;
  MEMSET(dest,'z',LEN);

  rc = wcstombs_s(&outc,NULL,0,L"",0);

  NW_CHECK_RC_OK(rc);
  NW_CHECK_EQUAL_BYTES('z',dest[0]);
  NW_CHECK_EQUAL_ULONGS(1,outc);
  NW_CHECK_GUARDS();

  #undef NW_CHECK_GUARDS
  #undef MAGIC
  #undef INVALID_COUNT
  #undef LEN
 
  #undef MEMSET
}

NW_TEST(NanoWinMsSafeStringTestGroup, StrTokSTest)
{
	SETUP_S_TEST();

	#define LEN   ( 128 )

	char        str1[LEN];
	const char *delm;
	char       *state;
	char       *token;
	char        str2[LEN]; // just placeholder to be non-null

	//--------------------------------------------------//

	strcpy(str1, "Quick brown\tfox");
	delm = " \t";
	state = str2; // No matter where

	token = strtok_s(str1, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(strcmp(token,"Quick"),0);
	token = strtok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(strcmp(token,"brown"),0);
	token = strtok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(strcmp(token,"fox"),0);
	token = strtok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token == NULL);

	//--------------------------------------------------//

	strcpy(str1, "Test Pattern");
	delm = " ";
	state = str2; // No matter where, just non-null

	token = strtok_s(NULL, NULL, &state);
	NW_CHECK_TRUE(token == NULL);
	token = strtok_s(str1, NULL, &state);
	NW_CHECK_TRUE(token == NULL);

	state = NULL;
	token = strtok_s(NULL, delm, &state); // cannot continue with undefined state
	NW_CHECK_TRUE(token == NULL);

	token = strtok_s(NULL, NULL, &state);
	NW_CHECK_TRUE(token == NULL);
	token = strtok_s(str1, NULL, &state);
	NW_CHECK_TRUE(token == NULL);

	token = strtok_s(str1, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(strcmp(token,"Test"),0);
	token = strtok_s(NULL, NULL, &state);
	NW_CHECK_TRUE(token == NULL); // invalid delim, state should be unaffected
	token = strtok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(strcmp(token,"Pattern"),0);
	token = strtok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token == NULL);

	//--------------------------------------------------//

	#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, WcsTokSTest)
{
	SETUP_S_TEST();

	#define LEN   ( 128 )

	wchar_t        str1[LEN];
	const wchar_t *delm;
	wchar_t       *state;
	wchar_t       *token;
	wchar_t        str2[LEN]; // just placeholder to be non-null

	//--------------------------------------------------//

	wcscpy(str1, L"Quick brown\tfox");
	delm = L" \t";
	state = str2; // No matter where

	token = wcstok_s(str1, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(wcscmp(token,L"Quick"),0);
	token = wcstok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(wcscmp(token,L"brown"),0);
	token = wcstok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(wcscmp(token,L"fox"),0);
	token = wcstok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token == NULL);

	//--------------------------------------------------//

	wcscpy(str1, L"Test Pattern");
	delm = L" ";
	state = str2; // No matter where, just non-null

	token = wcstok_s(NULL, NULL, &state);
	NW_CHECK_TRUE(token == NULL);
	token = wcstok_s(str1, NULL, &state);
	NW_CHECK_TRUE(token == NULL);

	state = NULL;
	token = wcstok_s(NULL, delm, &state); // cannot continue with undefined state
	NW_CHECK_TRUE(token == NULL);

	token = wcstok_s(NULL, NULL, &state);
	NW_CHECK_TRUE(token == NULL);
	token = wcstok_s(str1, NULL, &state);
	NW_CHECK_TRUE(token == NULL);

	token = wcstok_s(str1, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(wcscmp(token,L"Test"),0);
	token = wcstok_s(NULL, NULL, &state);
	NW_CHECK_TRUE(token == NULL); // invalid delim, state should be unaffected
	token = wcstok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token != NULL);
	NW_CHECK_EQUAL(wcscmp(token,L"Pattern"),0);
	token = wcstok_s(NULL, delm, &state);
	NW_CHECK_TRUE(token == NULL);

	//--------------------------------------------------//

	#undef LEN
}

NW_END_TEST_GROUP()
