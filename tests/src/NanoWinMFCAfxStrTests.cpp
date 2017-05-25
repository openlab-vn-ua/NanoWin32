#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxStr.h"
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxStrTestGroup)

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceSimpleTCHARTest)
{
	CString cString("abc abc abc");

	int n = cString.Replace('a', 'q');

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, "qbc qbc qbc");

	n = cString.Replace('w', 'q');

	NW_CHECK_EQUAL(0, n);
	NW_CHECK_EQUAL_STRCMP(cString, "qbc qbc qbc");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceDifferentLengthTCHARTest)
{
	CString cString("abc abc abc qwe");

	int n = cString.Replace("abc", "de");

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, "de de de qwe");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceSimpleLPCTSTRTest)
{
	CString cString("abc abc abc");

	LPCTSTR strOld = "a";
	LPCTSTR strNew = "q";

	int n = cString.Replace(strOld, strNew);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, "qbc qbc qbc");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceNullLPCTSTRTest)
{
	CString cString("abc abc abc");

	LPCTSTR strOld = "ab";
	LPCTSTR strNew = NULL;

	int n = cString.Replace(strOld, strNew);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, "c c c");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceDifferentLengthLPCTSTRTest)
{
	CString cString("abc abc abc qwe");

	LPCTSTR strOld = "abc";
	LPCTSTR strNew = "de";

	int n = cString.Replace(strOld, strNew);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, "de de de qwe");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindSimpleTCHARTest)
{
	CString cString("abcdef");
	int n = cString.Find('b');

	NW_CHECK_EQUAL(1, n);

	n = cString.Find("def");
	NW_CHECK_EQUAL(3, n);

	n = cString.Find("qqq");
	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindWithStartPosTCHARTest)
{
	CString cString("abcdef abcdef");

	int n = cString.Find('b', 4);

	NW_CHECK_EQUAL(8, n);

	n = cString.Find("def", 5);

	NW_CHECK_EQUAL(10, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindSimpleLPCTSTRTest)
{
	CString cString("abcdef");
	LPCTSTR sub = "b";

	int n = cString.Find(sub);
	
	NW_CHECK_EQUAL(1, n);

	sub = "def";
	n = cString.Find(sub);

	NW_CHECK_EQUAL(3, n);

	sub = "qqq";
	n = cString.Find(sub);

	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindWithStartPosLPCTSTRTest)
{
	CString cString("abcdef abcdef");
	LPCTSTR sub = "b";

	int n = cString.Find(sub, 4);

	NW_CHECK_EQUAL(8, n);

	sub = "def";
	n = cString.Find(sub, 5);

	NW_CHECK_EQUAL(10, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReverseFindSimpleTCHARTest)
{
	CString cString("abc abc abc");

	int n = cString.ReverseFind('a');

	NW_CHECK_EQUAL(8, n);

	n = cString.ReverseFind('q');

	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMakeLowerTest)
{
	CString cString("aBcDeFZzZ");

	cString.MakeLower();

	NW_CHECK_EQUAL_STRCMP(cString, "abcdefzzz");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMakeUpperTest)
{
	CString cString("aBcDeFZzZ");

	cString.MakeUpper();

	NW_CHECK_EQUAL_STRCMP(cString, "ABCDEFZZZ");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMakeReverseTest)
{
	CString cString("abcdef");

	cString.MakeReverse();

	NW_CHECK_EQUAL_STRCMP(cString, "fedcba");
}


NW_TEST(NanoWinMFCAfxStrTestGroup, CStringRightTest)
{
	CString cString("abcdef");

	CString resStr = cString.Right(3);

	NW_CHECK_EQUAL_STRCMP(resStr, "def");
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMidTest)
{
	CString cString("abcdef");

	CString resStr = cString.Mid(2);

	NW_CHECK_EQUAL_STRCMP(resStr, "cdef");
}
/*
NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMidWithCounterTest)
{
	CString cString("abcdefabcdef");

	CString resStr = cString.Mid(2, 5);

	NW_CHECK_EQUAL_STRCMP("cdefa", resStr);
}
*/
/*
NW_TEST(NanoWinMFCAfxStrTestGroup, CStringLeftTest)
{
	CString cString("abcdef");

	CString resStr = cString.Left(3);

	NW_CHECK_EQUAL_STRCMP("abc", resStr);
}
*/

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertTest)
{
	CString cString("abcdef");

	CString resStr = cString.Mid(2);

	NW_CHECK_EQUAL_STRCMP(resStr, "cdef");
}

NW_END_TEST_GROUP()

// format