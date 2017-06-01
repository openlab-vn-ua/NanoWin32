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

	n = cString.Find('q');
	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindWithStartPosTCHARTest)
{
	CString cString("abcdef abcdef");

	int n = cString.Find('b', 4);

	NW_CHECK_EQUAL(8, n);

	n = cString.Find('q', 5);

	NW_CHECK_EQUAL(-1, n);
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

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMidWithCounterTest)
{
	CString cString("abcdefabcdef");

	CString resStr = cString.Mid(2, 5);

	NW_CHECK_EQUAL_STRCMP("cdefa", resStr);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringLeftTest)
{
	CString cString("abcdef");

	CString resStr = cString.Left(3);

	NW_CHECK_EQUAL_STRCMP("abc", resStr);
}


NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertTCHARTest)
{
	CString cString("abcdef");

	int n = cString.Insert(3, 'z');

	NW_CHECK_EQUAL(7, n);
	NW_CHECK_EQUAL_STRCMP("abczdef", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertIndexHigherThanLengthTCHARTest)
{
	CString cString("abcdef");

	int n = cString.Insert(100, 'z');

	NW_CHECK_EQUAL(7, n);
	NW_CHECK_EQUAL_STRCMP("abcdefz", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertLPCTSTRTest)
{
	CString cString("abcdef");

	int n = cString.Insert(0, "zzz");

	NW_CHECK_EQUAL(9, n);
	NW_CHECK_EQUAL_STRCMP("zzzabcdef", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringDeleteTest)
{
	CString cString("abcdefabc");

	int n = cString.Delete(3, 3);

	NW_CHECK_EQUAL(6, n);
	NW_CHECK_EQUAL_STRCMP("abcabc", cString);

	n = cString.Delete(3, 100);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP("abc", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringRemoveTest)
{
	CString cString("abcdefabc");

	int n = cString.Remove('a');

	NW_CHECK_EQUAL(2, n);
	NW_CHECK_EQUAL_STRCMP("bcdefbc", cString);

	n = cString.Remove('q');

	NW_CHECK_EQUAL(0, n);
	NW_CHECK_EQUAL_STRCMP("bcdefbc", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringTrimLeftTest)
{
	CString cString("	  abcdefabc ");

	cString.TrimLeft();

	NW_CHECK_EQUAL_STRCMP("abcdefabc ", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringTrimRightTest)
{
	CString cString(" abcdefabc		  ");

	cString.TrimRight();

	NW_CHECK_EQUAL_STRCMP(" abcdefabc", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringTrimTest)
{
	CString cString("	  abcdefabc		  ");

	cString.Trim();

	NW_CHECK_EQUAL_STRCMP("abcdefabc", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCompareTest)
{
	CString cString("abcdef");

	int res = cString.Compare("abcdef");
	NW_CHECK_EQUAL(0, res);

	res = cString.Compare("A");
	NW_CHECK(res > 0);

	res = cString.Compare("abcdefabc");
	NW_CHECK(res < 0);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCompareNoCaseTest)
{
	CString cString("abcdef");

	int res = cString.CompareNoCase("ABcdEF");
	NW_CHECK_EQUAL(0, res);

	res = cString.CompareNoCase("ABcdE");
	NW_CHECK(res > 0);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFormatTest)
{
	CString str;

	str.Format("%d%s%d%s%d", 2, " * ", 2, " equal ", 4);

	NW_CHECK_EQUAL_STRCMP("2 * 2 equal 4", str);

	str.Format("", 2, 3, 4, 5); // format string is empty, args does not matter

	NW_CHECK_EQUAL_STRCMP("", str);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringOperatorAssignTest)
{
	CString cString("zzz");

	cString = "a";

	NW_CHECK_EQUAL_STRCMP("a", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringOperatorPlusAssignTest)
{
	CString cString("zzz");

	cString += "www";

	NW_CHECK_EQUAL_STRCMP("zzzwww", cString);

	CString strAdd = "aaa";

	cString += strAdd;

	NW_CHECK_EQUAL_STRCMP("zzzwwwaaa", cString);

	cString += 'b';

	NW_CHECK_EQUAL_STRCMP("zzzwwwaaab", cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringOperatorPlusTest)
{
	CString str1("zzz");
	CString str2("aaa");

	CString resStr = str1 + str2;

	NW_CHECK_EQUAL_STRCMP("zzzaaa", resStr);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCompareOperatorsTest)
{
	NW_CHECK_TRUE(CString("abc") == CString("abc"));
	NW_CHECK_TRUE(CString("abc") == "abc");
	NW_CHECK_FALSE(CString("abc") == CString("abcde"));
	NW_CHECK_FALSE("abcde" == CString("abc"));

	NW_CHECK_TRUE(CString("abc") != CString("abcde"));
	NW_CHECK_TRUE(CString("abc") != "abcde");
	NW_CHECK_FALSE(CString("abc") != CString("abc"));
	NW_CHECK_FALSE("abc" != CString("abc"));

	NW_CHECK_TRUE(CString("abc") > CString("ab"));
	NW_CHECK_TRUE(CString("abc") > "aaa");
	NW_CHECK_FALSE("aaa" > CString("abc"));

	NW_CHECK_TRUE(CString("ab") < CString("abc"));
	NW_CHECK_TRUE(CString("aaa") < "abc");
	NW_CHECK_FALSE("abc" < CString("aaa"));

	NW_CHECK_TRUE(CString("abc") >= CString("abc"));
	NW_CHECK_TRUE(CString("abc") >= "aaa");
	NW_CHECK_FALSE("abc" >= CString("abcd"));

	NW_CHECK_TRUE(CString("abc") <= CString("abc"));
	NW_CHECK_TRUE(CString("aaa") <= "abc");
	NW_CHECK_FALSE("abcd" <= CString("abc"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, C2CMacroTest)
{
	const char *ctest;

	ctest = CT2A("abc");
	NW_CHECK_EQUAL_STRCMP("abc", ctest);

	ctest = CT2CA("abc1");
	NW_CHECK_EQUAL_STRCMP("abc1", ctest);

	ctest = CW2A(L"abd");
	NW_CHECK_EQUAL_STRCMP("abd", ctest);

	ctest = CW2CA(L"abd1");
	NW_CHECK_EQUAL_STRCMP("abd1", ctest);
	
	char *test;
	
	test = CT2A("abr");
	NW_CHECK_EQUAL_STRCMP("abr", test);

	test = CW2A(L"abrbalg");
	NW_CHECK_EQUAL_STRCMP("abrbalg", test);
}

NW_END_TEST_GROUP()