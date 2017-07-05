#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMsPrintf.h"
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsPrintfTestGroup)

NW_TEST(NanoWinMsPrintfTestGroup, PrintfWFormatMs2UnixSimpleTest)
{
  const wchar_t *srcS  = L"%s";
  wchar_t       destS[256];

  NanoWinMsPrintfWFormatMs2Unix(destS, srcS);
  NW_CHECK_EQUAL_MEMCMP(L"%S", destS, 3 * sizeof(wchar_t));

  const wchar_t *srcC = L"%c";
  wchar_t       destC[256];

  NanoWinMsPrintfWFormatMs2Unix(destC, srcC);
  NW_CHECK_EQUAL_MEMCMP(L"%C", destC, 3 * sizeof(wchar_t));
  
  const wchar_t *srcSs = L"%S";
  wchar_t       destSs[256];

  NanoWinMsPrintfWFormatMs2Unix(destSs, srcSs);
  NW_CHECK_EQUAL_MEMCMP(L"%s", destSs, 3 * sizeof(wchar_t));

  const wchar_t *srcCs = L"%C";
  wchar_t       destCs[256];

  NanoWinMsPrintfWFormatMs2Unix(destCs, srcCs);
  NW_CHECK_EQUAL_MEMCMP(L"%c", destCs, 3 * sizeof(wchar_t));

  const wchar_t *srcLS = L"%ls";
  wchar_t       destLS[256];

  NanoWinMsPrintfWFormatMs2Unix(destLS, srcLS);
  NW_CHECK_EQUAL_MEMCMP(L"%ls", destLS, 4 * sizeof(wchar_t));

  const wchar_t *srcLC = L"%lc";
  wchar_t       destLC[256];

  NanoWinMsPrintfWFormatMs2Unix(destLC, srcLC);
  NW_CHECK_EQUAL_MEMCMP(L"%lc", destLC, 4 * sizeof(wchar_t));

  const wchar_t *srcWS = L"%ws";
  wchar_t       destWS[256];

  NanoWinMsPrintfWFormatMs2Unix(destWS, srcWS);
  NW_CHECK_EQUAL_MEMCMP(L"%ws", destWS, 4 * sizeof(wchar_t));

  const wchar_t *srcWC = L"%wc";
  wchar_t       destWC[256];

  NanoWinMsPrintfWFormatMs2Unix(destWC, srcWC);
  NW_CHECK_EQUAL_MEMCMP(L"%wc", destWC, 4 * sizeof(wchar_t));
}

NW_TEST(NanoWinMsPrintfTestGroup, PrintfWFormatMs2UnixTest)
{
  const wchar_t *srcS = L"%5s %#20S";
  wchar_t       destS[256];

  NanoWinMsPrintfWFormatMs2Unix(destS, srcS);
  NW_CHECK_EQUAL_MEMCMP(L"%5S %#20s", destS, 10 * sizeof(wchar_t));

  const wchar_t *srcF = L"%+0*f  %4.2f";
  wchar_t       destF[256];

  NanoWinMsPrintfWFormatMs2Unix(destF, srcF);
  NW_CHECK_EQUAL_MEMCMP(L"%+0*f  %4.2f", destF, 13 * sizeof(wchar_t));

  const wchar_t *srcD = L"%+05lld   %-I64i %02I32u %#x%#X %#3o";
  wchar_t       destD[256];

  NanoWinMsPrintfWFormatMs2Unix(destD, srcD);
  NW_CHECK_EQUAL_MEMCMP(L"%+05lld   %-I64i %02I32u %#x%#X %#3o", destD, 37 * sizeof(wchar_t));

  const wchar_t *srcC = L"%-c %5C %wc";
  wchar_t       destC[256];

  NanoWinMsPrintfWFormatMs2Unix(destC, srcC);
  NW_CHECK_EQUAL_MEMCMP(L"%-C %5c %wc", destC, 12 * sizeof(wchar_t));

  const wchar_t *srcG = L"%-3lg %+5LG";
  wchar_t       destG[256];

  NanoWinMsPrintfWFormatMs2Unix(destG, srcG);
  NW_CHECK_EQUAL_MEMCMP(L"%-3lg %+5LG", destG, 12 * sizeof(wchar_t));

  const wchar_t *srcA = L"%-*3.a   %+5#A";
  wchar_t       destA[256];

  NanoWinMsPrintfWFormatMs2Unix(destA, srcA);
  NW_CHECK_EQUAL_MEMCMP(L"%-*3.a   %+5#A", destA, 15 * sizeof(wchar_t));
}

NW_TEST(NanoWinMsPrintfTestGroup, NanoWinMsSWPrintfSimpleTest)
{
  wchar_t str[256];

  int count1 = NanoWinMsSWPrintf(str, 4, L"%s", L"abc");
  NW_CHECK_EQUAL(count1, 3);
  NW_CHECK_EQUAL_MEMCMP(L"abc", str, 4 * sizeof(wchar_t));
  #ifndef __GNUC__
  int count2 = NanoWinMsSWPrintf(str, 4, L"%ws", L"def");
  NW_CHECK_EQUAL(count2, 3);
  NW_CHECK_EQUAL_MEMCMP(L"def", str, 4 * sizeof(wchar_t));

  int count3 = NanoWinMsSWPrintf(str, 4, L"%wc", L'g');
  NW_CHECK_EQUAL(count3, 1);
  NW_CHECK_EQUAL_MEMCMP(L"g", str, 2 * sizeof(wchar_t));
  #endif
  int count4 = NanoWinMsSWPrintf(str, 4, L"%hd", -12);
  NW_CHECK_EQUAL(count4, 3);
  NW_CHECK_EQUAL_MEMCMP(L"-12", str, 4 * sizeof(wchar_t));

  int count5 = NanoWinMsSWPrintf(str, 4, L"%lu", 34);
  NW_CHECK_EQUAL(count5, 2);
  NW_CHECK_EQUAL_MEMCMP(L"34", str, 3 * sizeof(wchar_t));

  int count6 = NanoWinMsSWPrintf(str, 4, L"%c", L'd');
  NW_CHECK_EQUAL(count6, 1);
  NW_CHECK_EQUAL_MEMCMP(L"d", str, 2 * sizeof(wchar_t));

  int count7 = NanoWinMsSWPrintf(str, 5, L"%#x", 0xFF);
  NW_CHECK_EQUAL(count7, 4);
  NW_CHECK_EQUAL_MEMCMP(L"0xff", str, 5 * sizeof(wchar_t));

  int count8 = NanoWinMsSWPrintf(str, 5, L"%#X", 0xff);
  NW_CHECK_EQUAL(count8, 4);
  NW_CHECK_EQUAL_MEMCMP(L"0XFF", str, 5 * sizeof(wchar_t));

  int count9 = NanoWinMsSWPrintf(str, 5, L"%o", 056);
  NW_CHECK_EQUAL(count9, 2);
  NW_CHECK_EQUAL_MEMCMP(L"56", str, 3 * sizeof(wchar_t));

  int count10 = NanoWinMsSWPrintf(str, 10, L"%f", 5.678);
  NW_CHECK_EQUAL(count10, 8);
  NW_CHECK_EQUAL_MEMCMP(L"5.678000", str, 9 * sizeof(wchar_t));
}

NW_TEST(NanoWinMsPrintfTestGroup, NanoWinMsSWPrintfTest)
{
  wchar_t str[256];  
  #ifndef __GNUC__
  int count1 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%ls ww %#2s%ws", L"abc", L"f", L"zz");
  NW_CHECK_EQUAL(count1, 11);
  NW_CHECK_EQUAL_MEMCMP(L"abc ww  fzz", str, 12 * sizeof(wchar_t));

  int count2 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%c %wc %lc", L'd', L'e', L'f');
  NW_CHECK_EQUAL(count2, 5);
  NW_CHECK_EQUAL_MEMCMP(L"d e f", str, 6 * sizeof(wchar_t));
  #endif
  
  int count3 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%hd %+i  bb %#llu", -56, 123, 10);
  NW_CHECK_EQUAL(count3, 15);
  NW_CHECK_EQUAL_MEMCMP(L"-56 +123  bb 10", str, 16 * sizeof(wchar_t));

  int count4 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%#x  %#X %x", 11, 22, 33);
  NW_CHECK_EQUAL(count4, 12);
  NW_CHECK_EQUAL_MEMCMP(L"0xb  0X16 21", str, 13 * sizeof(wchar_t));

  int count5 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%o  %#o", 056, 24);
  NW_CHECK_EQUAL(count5, 7);
  NW_CHECK_EQUAL_MEMCMP(L"56  030", str, 8 * sizeof(wchar_t));

  int count6 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%o  %#o", 056, 24);
  NW_CHECK_EQUAL(count6, 7);
  NW_CHECK_EQUAL_MEMCMP(L"56  030", str, 8 * sizeof(wchar_t));

  int count7 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%.*f %.3f", 3, 0.1312, 1.22222);
  NW_CHECK_EQUAL(count7, 11);
  NW_CHECK_EQUAL_MEMCMP(L"0.131 1.222", str, 12 * sizeof(wchar_t));
}

NW_END_TEST_GROUP()