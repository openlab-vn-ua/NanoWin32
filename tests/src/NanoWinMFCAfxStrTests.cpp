#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxStr.h"
#else
 //#include <windows.h>
 #include <afx.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxStrTestGroup)

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceSimpleTCHARTest)
{
	CString cString(_T("abc abc abc"));

	int n = cString.Replace('a', 'q');

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, _T("qbc qbc qbc"));

	n = cString.Replace('w', 'q');

	NW_CHECK_EQUAL(0, n);
	NW_CHECK_EQUAL_STRCMP(cString, _T("qbc qbc qbc"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceSimpleLPCTSTRTest)
{
	CString cString(_T("abc abc abc"));

	LPCTSTR strOld = _T("a");
	LPCTSTR strNew = _T("q");

	int n = cString.Replace(strOld, strNew);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, _T("qbc qbc qbc"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceNullLPCTSTRTest)
{
	CString cString(_T("abc abc abc"));

	LPCTSTR strOld = _T("ab");
	LPCTSTR strNew = NULL;

	int n = cString.Replace(strOld, strNew);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, _T("c c c"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReplaceDifferentLengthLPCTSTRTest)
{
	CString cString(_T("abc abc abc qwe"));

	LPCTSTR strOld = _T("abc");
	LPCTSTR strNew = _T("de");

	int n = cString.Replace(strOld, strNew);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(cString, _T("de de de qwe"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindSimpleTCHARTest)
{
	CString cString(_T("abcdef"));
	int n = cString.Find('b');

	NW_CHECK_EQUAL(1, n);

	n = cString.Find('q');
	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindWithStartPosTCHARTest)
{
	CString cString(_T("abcdef abcdef"));

	int n = cString.Find('b', 4);

	NW_CHECK_EQUAL(8, n);

	n = cString.Find('q', 5);

	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindSimpleLPCTSTRTest)
{
	CString cString(_T("abcdef"));
	LPCTSTR sub = _T("b");

	int n = cString.Find(sub);
	
	NW_CHECK_EQUAL(1, n);

	sub = _T("def");
	n = cString.Find(sub);

	NW_CHECK_EQUAL(3, n);

	sub = _T("qqq");
	n = cString.Find(sub);

	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFindWithStartPosLPCTSTRTest)
{
	CString cString(_T("abcdef abcdef"));
	LPCTSTR sub = _T("b");

	int n = cString.Find(sub, 4);

	NW_CHECK_EQUAL(8, n);

	sub = _T("def");
	n = cString.Find(sub, 5);

	NW_CHECK_EQUAL(10, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringReverseFindSimpleTCHARTest)
{
	CString cString(_T("abc abc abc"));

	int n = cString.ReverseFind('a');

	NW_CHECK_EQUAL(8, n);

	n = cString.ReverseFind('q');

	NW_CHECK_EQUAL(-1, n);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMakeLowerTest)
{
	CString cString(_T("aBcDeFZzZ"));

	cString.MakeLower();

	NW_CHECK_EQUAL_STRCMP(cString, _T("abcdefzzz"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMakeUpperTest)
{
	CString cString(_T("aBcDeFZzZ"));

	cString.MakeUpper();

	NW_CHECK_EQUAL_STRCMP(cString, _T("ABCDEFZZZ"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMakeReverseTest)
{
	CString cString(_T("abcdef"));

	cString.MakeReverse();

	NW_CHECK_EQUAL_STRCMP(cString, _T("fedcba"));
}


NW_TEST(NanoWinMFCAfxStrTestGroup, CStringRightTest)
{
	CString cString(_T("abcdef"));

	CString resStr = cString.Right(3);

	NW_CHECK_EQUAL_STRCMP(resStr, _T("def"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMidTest)
{
	CString cString(_T("abcdef"));

	CString resStr = cString.Mid(2);

	NW_CHECK_EQUAL_STRCMP(resStr, _T("cdef"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringMidWithCounterTest)
{
	CString cString(_T("abcdefabcdef"));

	CString resStr = cString.Mid(2, 5);

	NW_CHECK_EQUAL_STRCMP(_T("cdefa"), resStr);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringLeftTest)
{
	CString cString(_T("abcdef"));

	CString resStr = cString.Left(3);

	NW_CHECK_EQUAL_STRCMP(_T("abc"), resStr);
}


NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertTCHARTest)
{
	CString cString(_T("abcdef"));

	int n = cString.Insert(3, 'z');

	NW_CHECK_EQUAL(7, n);
	NW_CHECK_EQUAL_STRCMP(_T("abczdef"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertIndexHigherThanLengthTCHARTest)
{
	CString cString(_T("abcdef"));

	int n = cString.Insert(100, 'z');

	NW_CHECK_EQUAL(7, n);
	NW_CHECK_EQUAL_STRCMP(_T("abcdefz"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringInsertLPCTSTRTest)
{
	CString cString(_T("abcdef"));

	int n = cString.Insert(0, _T("zzz"));

	NW_CHECK_EQUAL(9, n);
	NW_CHECK_EQUAL_STRCMP(_T("zzzabcdef"), cString);

  n = cString.Insert(100, _T("zzz"));

  NW_CHECK_EQUAL(12, n);
  NW_CHECK_EQUAL_STRCMP(_T("zzzabcdefzzz"), cString);

  n = cString.Insert(-1, _T("a"));

  NW_CHECK_EQUAL(13, n);
  NW_CHECK_EQUAL_STRCMP(_T("azzzabcdefzzz"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringDeleteTest)
{
	CString cString(_T("abcdefabc"));

	int n = cString.Delete(3, 3);

	NW_CHECK_EQUAL(6, n);
	NW_CHECK_EQUAL_STRCMP(_T("abcabc"), cString);

	n = cString.Delete(3, 100);

	NW_CHECK_EQUAL(3, n);
	NW_CHECK_EQUAL_STRCMP(_T("abc"), cString);

  n = cString.Delete(10, 1);

  NW_CHECK_EQUAL(3, n);
  NW_CHECK_EQUAL_STRCMP(_T("abc"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringRemoveTest)
{
	CString cString(_T("abcdefabc"));

	int n = cString.Remove('a');

	NW_CHECK_EQUAL(2, n);
	NW_CHECK_EQUAL_STRCMP(_T("bcdefbc"), cString);

	n = cString.Remove('q');

	NW_CHECK_EQUAL(0, n);
	NW_CHECK_EQUAL_STRCMP(_T("bcdefbc"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringTrimLeftTest)
{
	CString cString;

	cString = "abcdefabc";
	cString.TrimLeft();
	NW_CHECK_EQUAL_STRCMP(_T("abcdefabc"), cString);

	cString = " abcdefabc ";
	cString.TrimLeft();
	NW_CHECK_EQUAL_STRCMP(_T("abcdefabc "), cString);

	cString = _T("  \t \n  abcdefabc ");
	cString.TrimLeft();
	NW_CHECK_EQUAL_STRCMP(_T("abcdefabc "), cString);

	cString = _T(" zz z \t \n  abcdefabc zz");
	cString.TrimLeft('z');
	NW_CHECK_EQUAL_STRCMP(_T(" zz z \t \n  abcdefabc zz"), cString);

	cString = _T("zz z \t \n  abcdefabc zz");
	cString.TrimLeft('z');
	NW_CHECK_EQUAL_STRCMP(_T(" z \t \n  abcdefabc zz"), cString);

	cString = _T("zqd qdz \t \n  abcdefabc zz");
	cString.TrimLeft(_T("zqdka"));
	NW_CHECK_EQUAL_STRCMP(_T(" qdz \t \n  abcdefabc zz"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringTrimRightTest)
{
	CString cString;
	
	cString = _T("abcdefabc");
	cString.TrimRight();
	NW_CHECK_EQUAL_STRCMP(_T("abcdefabc"), cString);

	cString = _T(" abcdefabc ");
	cString.TrimRight();
	NW_CHECK_EQUAL_STRCMP(_T(" abcdefabc"), cString);

	cString = _T(" abcdefabc \t   \n   ");
	cString.TrimRight();
	NW_CHECK_EQUAL_STRCMP(_T(" abcdefabc"), cString);

	cString = _T(" abcdefabc \t   \n   ");
	cString.TrimRight();
	NW_CHECK_EQUAL_STRCMP(_T(" abcdefabc"), cString);

	cString = _T(" abcdefabc \t   \n z  ");
	cString.TrimRight('z');
	NW_CHECK_EQUAL_STRCMP(_T(" abcdefabc \t   \n z  "), cString);

	cString = _T(" abcdefabc \t   \n z  zzzz");
	cString.TrimRight('z');
	NW_CHECK_EQUAL_STRCMP(_T(" abcdefabc \t   \n z  "), cString);

	cString = _T(" abcdefabc \t   \n z  zqw zwwwq");
	cString.TrimRight(_T("zqwka"));
	NW_CHECK_EQUAL_STRCMP(_T(" abcdefabc \t   \n z  zqw "), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringTrimTest)
{
	CString cString;

	cString = _T("  \t \n abcdefabc  \t \n ");
	cString.Trim();
	NW_CHECK_EQUAL_STRCMP(_T("abcdefabc"), cString);

	cString = _T("zz z  \t abcdefabc  \t z zzzz");
	cString.Trim('z');
	NW_CHECK_EQUAL_STRCMP(_T(" z  \t abcdefabc  \t z "), cString);

	cString = _T("qqqwzz z  \t abcdefabc  \t z zwwrrzzz");
	cString.Trim(_T("zqwrac"));
	NW_CHECK_EQUAL_STRCMP(_T(" z  \t abcdefabc  \t z "), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCompareTest)
{
	CString cString(_T("abcdef"));

	int res = cString.Compare(_T("abcdef"));
	NW_CHECK_EQUAL(0, res);

	res = cString.Compare(_T("A"));
	NW_CHECK(res > 0);

	res = cString.Compare(_T("abcdefabc"));
	NW_CHECK(res < 0);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCompareNoCaseTest)
{
	CString cString(_T("abcdef"));

	int res = cString.CompareNoCase(_T("ABcdEF"));
	NW_CHECK_EQUAL(0, res);

	res = cString.CompareNoCase(_T("ABcdE"));
	NW_CHECK(res > 0);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFormatSimpleTest)
{
  CString str;
  CString str2(_T("abc"));

  str.Format(_T("%s"), str2);
  NW_CHECK_EQUAL_STRCMP(_T("abc"), str);

  str.Format(_T("%d"), 123);
  NW_CHECK_EQUAL_STRCMP(_T("123"), str);

  str.Format(_T("%f"), 1.23f);
  NW_CHECK_EQUAL_STRCMP(_T("1.230000"), str);

  str.Format(_T("%x"), 0xFFF);
  NW_CHECK_EQUAL_STRCMP(_T("fff"), str);

  str.Format(_T("%c"), 'a');
  NW_CHECK_EQUAL_STRCMP(_T("a"), str);

  str.Format(_T("%lu"), 123);
  NW_CHECK_EQUAL_STRCMP(_T("123"), str);

  str.Format(_T("%hd"), 123);
  NW_CHECK_EQUAL_STRCMP(_T("123"), str);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFormatTest)
{
	CString str;

	str.Format(_T("%d%s%d%s%d"), 2, _T(" * "), 2, _T(" equal "), 4);

	NW_CHECK_EQUAL_STRCMP(_T("2 * 2 equal 4"), str);

	str.Format(_T(""), 2, 3, 4, 5); // format string is empty, args does not matter

	NW_CHECK_EQUAL_STRCMP(_T(""), str);

  CString str2(_T("abc"));

  str.Format(_T("%s"), str2);
  NW_CHECK_EQUAL_STRCMP(_T("abc"), str);

  str.Format(_T("%s%s"), str2, _T("123"));
  NW_CHECK_EQUAL_STRCMP(_T("abc123"), str);

  str.Format(_T("%s%d"), str2, 123);
  NW_CHECK_EQUAL_STRCMP(_T("abc123"), str)

  str.Format(_T("%s %d %4f"), str2, 123, 5.67f);
  NW_CHECK_EQUAL_STRCMP(_T("abc 123 5.670000"), str);

  str.Format(_T("%s %x"), str2, 0xFF123);
  NW_CHECK_EQUAL_STRCMP(_T("abc ff123"), str);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringFormatCStringTest)
{
	// This works under Win32 only...
	CString str;
	CString one;
	CString two;
	CString three;

	one   = _T("one");
	two   = _T("two");
	three = _T("three");

	str.Format(_T("just a [%s] test"), one);
	NW_CHECK_EQUAL_STRCMP(_T("just a [one] test"), str);

	str.Format(_T("just a [%s] test"), two);
	NW_CHECK_EQUAL_STRCMP(_T("just a [two] test"), str);

	str.Format(_T("just a [%s] [%s] test"), one, two);
	NW_CHECK_EQUAL_STRCMP(_T("just a [one] [two] test"), str);

	str.Format(_T("just a [%s] [%s] [%s] test"), one, two, three);
	NW_CHECK_EQUAL_STRCMP(_T("just a [one] [two] [three] test"), str);

	str.Format(_T("just a [%s] [%s] [%s] %d test"), one, two, three, 4);
	NW_CHECK_EQUAL_STRCMP(_T("just a [one] [two] [three] 4 test"), str);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringOperatorAssignTest)
{
	CString cString(_T("zzz"));

	cString = _T("a");

	NW_CHECK_EQUAL_STRCMP(_T("a"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringOperatorPlusAssignTest)
{
	CString cString(_T("zzz"));

	cString += _T("www");

	NW_CHECK_EQUAL_STRCMP(_T("zzzwww"), cString);

	CString strAdd = _T("aaa");

	cString += strAdd;

	NW_CHECK_EQUAL_STRCMP(_T("zzzwwwaaa"), cString);

	cString += 'b';

	NW_CHECK_EQUAL_STRCMP(_T("zzzwwwaaab"), cString);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringOperatorPlusTest)
{
	CString str1(_T("zzz"));
	CString str2(_T("aaa"));

	CString resStr = str1 + str2;

	NW_CHECK_EQUAL_STRCMP(_T("zzzaaa"), resStr);
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCompareOperatorsTest)
{
	NW_CHECK_TRUE(CString(_T("abc")) == CString("abc"));
	NW_CHECK_TRUE(CString(_T("abc")) == "abc");
	NW_CHECK_FALSE(CString(_T("abc")) == CString("abcde"));
	NW_CHECK_FALSE(_T("abcde") == CString("abc"));

	NW_CHECK_TRUE(CString(_T("abc")) != CString("abcde"));
	NW_CHECK_TRUE(CString(_T("abc")) != "abcde");
	NW_CHECK_FALSE(CString(_T("abc")) != CString("abc"));
	NW_CHECK_FALSE(_T("abc") != CString("abc"));

	NW_CHECK_TRUE(CString(_T("abc")) > CString("ab"));
	NW_CHECK_TRUE(CString(_T("abc")) > "aaa");
	NW_CHECK_FALSE(_T("aaa") > CString("abc"));

	NW_CHECK_TRUE(CString(_T("ab")) < CString("abc"));
	NW_CHECK_TRUE(CString(_T("aaa")) < "abc");
	NW_CHECK_FALSE(_T("abc") < CString("aaa"));

	NW_CHECK_TRUE(CString(_T("abc")) >= CString("abc"));
	NW_CHECK_TRUE(CString(_T("abc")) >= "aaa");
	NW_CHECK_FALSE(_T("abc") >= CString("abcd"));

	NW_CHECK_TRUE(CString(_T("abc")) <= CString("abc"));
	NW_CHECK_TRUE(CString(_T("aaa")) <= "abc");
	NW_CHECK_FALSE(_T("abcd") <= CString("abc"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, C2CMacroTest)
{
	const char *ctest;

	CT2A c11(_T("abc"));
	ctest = c11;
	NW_CHECK_EQUAL_STRCMP("abc", ctest);
	NW_CHECK_EQUAL_STRCMP("abc", CT2A(_T("abc")));

	CT2CA c12(_T("abc1"));
	ctest = c12;
	NW_CHECK_EQUAL_STRCMP("abc1", ctest);
	NW_CHECK_EQUAL_STRCMP("abc1", CT2CA(_T("abc1")));

	CW2A c13(L"abd");
	ctest = c13;
	NW_CHECK_EQUAL_STRCMP("abd", ctest);
	NW_CHECK_EQUAL_STRCMP("abd", CW2A(L"abd"));

	// NW_CHECK_EQUAL_STRCMP("abd1", CW2CA(L"abd1")); // CW2CA is available only at NanoWin

	char *test;
	
	CT2A c21(_T("abr"));
	test = c21;
	NW_CHECK_EQUAL_STRCMP("abr", test);
	NW_CHECK_EQUAL_STRCMP("abr", CT2A(_T("abr")));

	CW2A c23(L"abrbalg");
	test = c23;
	NW_CHECK_EQUAL_STRCMP("abrbalg", test);
	NW_CHECK_EQUAL_STRCMP("abrbalg", CW2A(L"abrbalg"));

	const TCHAR *ttest;

	CA2CT c31("abc");
	ttest = c31;
	NW_CHECK_EQUAL(0,_tcscmp(_T("abc"), ttest));
	NW_CHECK_EQUAL(0,_tcscmp(_T("abc"), CA2CT("abc")));

	CW2CT c32(L"abc");
	ttest = c32;
	NW_CHECK_EQUAL(0,_tcscmp(_T("abc"), ttest));
	NW_CHECK_EQUAL(0,_tcscmp(_T("abc"), CW2CT(L"abc")));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, ReleaseBufferTest)
{
  constexpr int bufferStartSize = 255 + 1;
  CString   str;

  TCHAR *buffer = str.GetBufferSetLength(bufferStartSize);

  _tcscpy(buffer, _T("2017"));

  str.ReleaseBuffer();

  NW_CHECK_EQUAL_STRCMP(str.GetString(), _T("2017"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringSetStringTest)
{
  CString str;
  bool exceptionCatched = false;

  str.SetString(_T("abcd"), 3);
  NW_CHECK_EQUAL_STRCMP(_T("abc"), str.GetString());

  str.SetString(_T("efgh"), 10);
  NW_CHECK_EQUAL_STRCMP(_T("efgh"), str.GetString());

  /* trying to pass -1 as nLength parameter throws exception in MS implementation,
     NanoWin implementation clears string */
  /*
  try
  {
    str.SetString("abcd", -1);
  }
  catch (...)
  {
    exceptionCatched = true;
  }

  NW_CHECK_TRUE(exceptionCatched);
  */
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringAddSymbolTest)
{
  CString str;

  for (int i = 0; i < 128; i++)
  {
    str.AppendChar('a');
  }

  NW_CHECK_EQUAL(128, str.GetLength());
  NW_CHECK_EQUAL_STRCMP(_T("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", str.GetString());
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCW2ATest)
{
  NW_CHECK_EQUAL_STRCMP("hello",(const char*)CW2A(L"hello"));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCA2WTest)
{
  NW_CHECK_EQUAL_MEMCMP(L"hello", (const wchar_t*)CA2W("hello"), 6 * sizeof(wchar_t));
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCW2AFormatTest)
{
  CStringA str;

  str.Format("%s", (const char*)CW2A(L"hello"));
  NW_CHECK_EQUAL_STRCMP("hello", str.GetString());

  str.Format("%d%s%d", 123, (const char*)CW2A(L"abc"), 456);
  NW_CHECK_EQUAL_STRCMP("123abc456", str.GetString());
}

NW_TEST(NanoWinMFCAfxStrTestGroup, CStringCA2WFormatTest)
{
  CStringW str;

  str.Format(L"%s", (const wchar_t*)CA2W("hello"));
  NW_CHECK_EQUAL_MEMCMP(L"hello", str.GetString(), 6 * sizeof(wchar_t));

  str.Format(L"%d%s%d", 123, (const wchar_t*)CA2W("abc"), 456);
  NW_CHECK_EQUAL_MEMCMP(L"123abc456", str.GetString(), 10 * sizeof(wchar_t));
}
#ifndef __GNUC__
NW_TEST(NanoWinMFCAfxStrTestGroup, CStringExtractSubStringTest)
{
  CStringA str;
  bool res;

  res = AfxExtractSubString(str, "ab cd ef", 1, ' ');

  NW_CHECK_TRUE(res);
  NW_CHECK_EQUAL_STRCMP("cd", str.GetString());

  res = AfxExtractSubString(str, "ab cd ef", 10, ' ');

  NW_CHECK_FALSE(res);
}
#endif
NW_END_TEST_GROUP()