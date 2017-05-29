#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinEnv.h"
#else
 #include <windows.h>
#endif

#define MAX_LEN  (1024)

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinEnvTestGroup)

NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableATest)
{
	DWORD count = -1;

	count = GetEnvironmentVariableA("PATH", NULL, 0);

	NW_CHECK(count > 0);
}

NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableASetVarTest)
{
	char buff[MAX_LEN];
	DWORD count = -1;
	
	BOOL res = SetEnvironmentVariableA("NW_TEST_ENV_1", "test");

	NW_CHECK_TRUE(res);

	res = GetEnvironmentVariableA("NW_TEST_ENV_1", buff, sizeof(buff));

	NW_CHECK(res > 0);
}
/*
NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableASmallBufTest)
{
	char buff[MAX_LEN];
	DWORD count = -1;

	count = GetEnvironmentVariableA("NW_TEST_ENV_1", buff, 1);

	NW_CHECK_EQUAL_LONGS(5, count);
}
*/
NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableARemoveVarTest)
{
	char buff[MAX_LEN];
	DWORD count = -1;

	BOOL res = SetEnvironmentVariableA("NW_TEST_ENV_1", NULL);

	NW_CHECK_TRUE(res);

	count = GetEnvironmentVariableA("NW_TEST_ENV_1", buff, sizeof(buff));

	NW_CHECK_EQUAL_LONGS(0, count);
}
/*
NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableWTest)
{
	DWORD count = -1;

	count = GetEnvironmentVariableW(L"PATH", NULL, 0);

	NW_CHECK(count > 0);
}
*/
NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableWSetVarTest)
{
	wchar_t buff[MAX_LEN];
	DWORD count = -1;

	BOOL res = SetEnvironmentVariableW(L"NW_TEST_ENV_2", L"test");

	NW_CHECK_TRUE(res);

	res = GetEnvironmentVariableW(L"NW_TEST_ENV_2", buff, sizeof(buff));

	NW_CHECK(res > 0);
}
/*
NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableWSmallBufTest)
{
	wchar_t buff[MAX_LEN];
	DWORD count = -1;

	count = GetEnvironmentVariableW(L"NW_TEST_ENV_2", buff, 1);

	NW_CHECK_EQUAL_LONGS(5, count);
}
*/
NW_TEST(NanoWinEnvTestGroup, GetEnvironmentVariableWRemoveVarTest)
{
	wchar_t buff[MAX_LEN];
	DWORD count = -1;

	BOOL res = SetEnvironmentVariableW(L"NW_TEST_ENV_2", NULL);

	NW_CHECK_TRUE(res);

	count = GetEnvironmentVariableW(L"NW_TEST_ENV_2", buff, sizeof(buff));

	NW_CHECK_EQUAL_LONGS(0, count);
}

NW_END_TEST_GROUP()