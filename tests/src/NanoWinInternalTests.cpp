#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinInternal.h"
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinInternalFileNameMatchMaskTestGroup)

NW_TEST(NanoWinInternalFileNameMatchMaskTestGroup, FileNameMatchMaskNoWildCardsTest)
{
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("",""));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","a"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test","test"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("","a"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("a",""));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("test","tes"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("test","tesf"));
}

NW_TEST(NanoWinInternalFileNameMatchMaskTestGroup, FileNameMatchMaskSingleCharWildCardsTest)
{
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","?"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("","?"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","??"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","??"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","???"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","a??"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","?b?"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","??c"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","ab?"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","?bc"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("ab","?"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("a","??"));
}

NW_TEST(NanoWinInternalFileNameMatchMaskTestGroup, FileNameMatchMaskMultiCharWildCardsTest)
{
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("","*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","a*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","*b"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("ab","b*"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("ab","*a"));
}

NW_TEST(NanoWinInternalFileNameMatchMaskTestGroup, FileNameMatchMaskComplexWildCardsTest)
{
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("","**"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","**"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","**"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","?*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","*?"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("a","*?*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","*?*?"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","?**?"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("ab","*??*"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("a","??*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","??*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("abc","?b*"));
}

NW_TEST(NanoWinInternalFileNameMatchMaskTestGroup, FileNameMatchMaskFileNameBasedTest)
{
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.txt","*.*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.txt","*.txt"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("test.txt","*.csv"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.txt","test.*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.csv","test.*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.csv","test.c*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.csv","test.c??"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskA("test.csv","test.c??"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskA("test.txt","*.c??"));
}

NW_TEST(NanoWinInternalFileNameMatchMaskTestGroup, FileNameMatchMaskFileNameBasedUnicodeTest)
{
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.txt",L"*.*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.txt",L"*.txt"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskW(L"test.txt",L"*.csv"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.txt",L"test.*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.csv",L"test.*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.csv",L"test.c*"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.csv",L"test.c??"));
  NW_CHECK_TRUE(NanoWinFileNameMatchMaskW(L"test.csv",L"test.c??"));
  NW_CHECK_FALSE(NanoWinFileNameMatchMaskW(L"test.txt",L"*.c??"));
}


NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinInternalTimeSpecArithmeticTestGroup)

NW_TEST(NanoWinInternalTimeSpecArithmeticTestGroup, TimeSpecAddTimeoutInMillisSimpleTest)
{
	struct timespec time;

	time.tv_sec = 1;
	time.tv_nsec = 100;

	NanoWinTimeSpecAddTimeoutInMillis(&time, 100);

	NW_CHECK_EQUAL_LONGS(100000100, time.tv_nsec);
	NW_CHECK_EQUAL(1, time.tv_sec);
}

NW_TEST(NanoWinInternalTimeSpecArithmeticTestGroup, TimeSpecNanoSecondfOverflowTest)
{
	struct timespec time;

	time.tv_sec = 1;
	time.tv_nsec = 1000000;

	NanoWinTimeSpecAddTimeoutInMillis(&time, 1000);

	NW_CHECK_EQUAL_LONGS(1000000, time.tv_nsec);
	NW_CHECK_EQUAL(2, time.tv_sec);
}

NW_TEST(NanoWinInternalTimeSpecArithmeticTestGroup, TimeSpecSecondsOverflowTest)
{
	struct timespec time;

	time.tv_sec = 1;
	time.tv_nsec = 1000000;

	NanoWinTimeSpecAddTimeoutInMillis(&time, 10011);

	NW_CHECK_EQUAL_LONGS(12000000, time.tv_nsec);
	NW_CHECK_EQUAL(11, time.tv_sec);
}

NW_END_TEST_GROUP()