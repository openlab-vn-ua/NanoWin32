#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMsPrintf.h"
 #include <unistd.h>
#else
 #include <windows.h>
#endif

#define NanoWinMsSWPrintf   swprintf
#define NanoWinMsVSWPrintf  vswprintf
#define NanoWinMsWPrintf    wprintf
#define NanoWinMsVWPrintf   vwprintf
#define NanoWinMsFWPrintf   fwprintf
#define NanoWinMsVFWPrintf  vfwprintf

#define FILE_NAME     ("testFile.txt")
#define FILE_NAME_W   (L"testFile.txt")

static FILE *GetFile()
{
  #ifndef __GNUC__ 
  FILE *stream = _wfopen(FILE_NAME_W, L"w+t");
  #else
  FILE *stream = fopen(FILE_NAME, "w+t");
  #endif

  if (stream != NULL)
  {
    return stream;
  }
  else return NULL;
}

static void CloseFile(FILE *stream)
{
  if (stream != NULL)
  {
    fclose(stream);
  }
  #ifndef __GNUC__  
  _wunlink(FILE_NAME_W);
  #else
  unlink(FILE_NAME);
  #endif
}

static void ReadFile(FILE *stream, wchar_t *buff)
{
  fseek(stream, 0, SEEK_SET);

  if (stream != NULL)
  {
    fgetws(buff, 256, stream);
  }
}

NW_BEGIN_TEST_GROUP_SIMPLE(PrintfWFormatMs2UnixTestGroup)

NW_TEST(PrintfWFormatMs2UnixTestGroup, PrintfWFormatMs2UnixSimpleTest)
{
  const wchar_t *srcS  = L"%s";
  wchar_t       destS[256];

  NanoWinMsWFormatProcMs2Unix(destS, srcS);
  NW_CHECK_EQUAL_MEMCMP(L"%S", destS, 3 * sizeof(wchar_t));

  const wchar_t *srcC = L"%c";
  wchar_t       destC[256];

  NanoWinMsWFormatProcMs2Unix(destC, srcC);
  NW_CHECK_EQUAL_MEMCMP(L"%C", destC, 3 * sizeof(wchar_t));
  
  const wchar_t *srcSs = L"%S";
  wchar_t       destSs[256];

  NanoWinMsWFormatProcMs2Unix(destSs, srcSs);
  NW_CHECK_EQUAL_MEMCMP(L"%s", destSs, 3 * sizeof(wchar_t));

  const wchar_t *srcCs = L"%C";
  wchar_t       destCs[256];

  NanoWinMsWFormatProcMs2Unix(destCs, srcCs);
  NW_CHECK_EQUAL_MEMCMP(L"%c", destCs, 3 * sizeof(wchar_t));

  const wchar_t *srcLS = L"%ls";
  wchar_t       destLS[256];

  NanoWinMsWFormatProcMs2Unix(destLS, srcLS);
  NW_CHECK_EQUAL_MEMCMP(L"%ls", destLS, 4 * sizeof(wchar_t));

  const wchar_t *srcLC = L"%lc";
  wchar_t       destLC[256];

  NanoWinMsWFormatProcMs2Unix(destLC, srcLC);
  NW_CHECK_EQUAL_MEMCMP(L"%lc", destLC, 4 * sizeof(wchar_t));

  const wchar_t *srcWS = L"%ws";
  wchar_t       destWS[256];

  NanoWinMsWFormatProcMs2Unix(destWS, srcWS);
  NW_CHECK_EQUAL_MEMCMP(L"%ls", destWS, 4 * sizeof(wchar_t));

  const wchar_t *srcWC = L"%wc";
  wchar_t       destWC[256];

  NanoWinMsWFormatProcMs2Unix(destWC, srcWC);
  NW_CHECK_EQUAL_MEMCMP(L"%lc", destWC, 4 * sizeof(wchar_t));
}

NW_TEST(PrintfWFormatMs2UnixTestGroup, PrintfWFormatMs2UnixTest)
{
  const wchar_t *srcS = L"%5s %#20S";
  wchar_t       destS[256];

  NanoWinMsWFormatProcMs2Unix(destS, srcS);
  NW_CHECK_EQUAL_MEMCMP(L"%5S %#20s", destS, 10 * sizeof(wchar_t));

  const wchar_t *srcF = L"%+0*f  %4.2f";
  wchar_t       destF[256];

  NanoWinMsWFormatProcMs2Unix(destF, srcF);
  NW_CHECK_EQUAL_MEMCMP(L"%+0*f  %4.2f", destF, 13 * sizeof(wchar_t));

  const wchar_t *srcD = L"%+05lld   %-I64i %02I32u %#x%#X %#3o";
  wchar_t       destD[256];

  NanoWinMsWFormatProcMs2Unix(destD, srcD);
  NW_CHECK_EQUAL_MEMCMP(L"%+05lld   %-I64i %02I32u %#x%#X %#3o", destD, 37 * sizeof(wchar_t));

  const wchar_t *srcC = L"%-c %5C %wc";
  wchar_t       destC[256];

  NanoWinMsWFormatProcMs2Unix(destC, srcC);
  NW_CHECK_EQUAL_MEMCMP(L"%-C %5c %lc", destC, 12 * sizeof(wchar_t));

  const wchar_t *srcG = L"%-3lg %+5LG";
  wchar_t       destG[256];

  NanoWinMsWFormatProcMs2Unix(destG, srcG);
  NW_CHECK_EQUAL_MEMCMP(L"%-3lg %+5LG", destG, 12 * sizeof(wchar_t));

  const wchar_t *srcA = L"%-*3.a   %+5#A";
  wchar_t       destA[256];

  NanoWinMsWFormatProcMs2Unix(destA, srcA);
  NW_CHECK_EQUAL_MEMCMP(L"%-*3.a   %+5#A", destA, 15 * sizeof(wchar_t));
}

NW_END_TEST_GROUP()

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsSWPrintfTestGroup)

NW_TEST(NanoWinMsSWPrintfTestGroup, NanoWinMsSWPrintfSimpleTest)
{
  wchar_t str[256];

  int count0 = NanoWinMsSWPrintf(str, 5, L"test");
  NW_CHECK_EQUAL(4, count0);
  NW_CHECK_EQUAL_MEMCMP(L"test", str, 5 * sizeof(wchar_t));

  int count1 = NanoWinMsSWPrintf(str, 4, L"%s", L"abc");
  NW_CHECK_EQUAL(3, count1);
  NW_CHECK_EQUAL_MEMCMP(L"abc", str, 4 * sizeof(wchar_t));

  int count2 = NanoWinMsSWPrintf(str, 4, L"%ws", L"def");
  NW_CHECK_EQUAL(3, count2);
  NW_CHECK_EQUAL_MEMCMP(L"def", str, 4 * sizeof(wchar_t));

  int count3 = NanoWinMsSWPrintf(str, 4, L"%wc", L'g');
  NW_CHECK_EQUAL(1, count3);
  NW_CHECK_EQUAL_MEMCMP(L"g", str, 2 * sizeof(wchar_t));

  int count4 = NanoWinMsSWPrintf(str, 4, L"%hd", (short int)-12);
  NW_CHECK_EQUAL(3, count4);
  NW_CHECK_EQUAL_MEMCMP(L"-12", str, 4 * sizeof(wchar_t));

  int count5 = NanoWinMsSWPrintf(str, 4, L"%lu", (unsigned long int)34);
  NW_CHECK_EQUAL(2, count5);
  NW_CHECK_EQUAL_MEMCMP(L"34", str, 3 * sizeof(wchar_t));

  int count6 = NanoWinMsSWPrintf(str, 4, L"%c", L'd');
  NW_CHECK_EQUAL(1, count6);
  NW_CHECK_EQUAL_MEMCMP(L"d", str, 2 * sizeof(wchar_t));

  int count7 = NanoWinMsSWPrintf(str, 5, L"%#x", 0xFF);
  NW_CHECK_EQUAL(4, count7);
  NW_CHECK_EQUAL_MEMCMP(L"0xff", str, 5 * sizeof(wchar_t));

  int count8 = NanoWinMsSWPrintf(str, 5, L"%#X", 0xff);
  NW_CHECK_EQUAL(4, count8);
  NW_CHECK_EQUAL_MEMCMP(L"0XFF", str, 5 * sizeof(wchar_t));

  int count9 = NanoWinMsSWPrintf(str, 5, L"%o", 056);
  NW_CHECK_EQUAL(2, count9);
  NW_CHECK_EQUAL_MEMCMP(L"56", str, 3 * sizeof(wchar_t));

  int count10 = NanoWinMsSWPrintf(str, 10, L"%f", 5.678f);
  NW_CHECK_EQUAL(8, count10);
  NW_CHECK_EQUAL_MEMCMP(L"5.678000", str, 9 * sizeof(wchar_t));

  // 'h'-prefix in char and string fields checks

  int count = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%hc",'A');
  NW_CHECK_EQUAL(1,count);
  NW_CHECK_EQUAL_STRCMP(L"A",str);

  count = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%hs","multibyte string");
  NW_CHECK_EQUAL(16,count);
  NW_CHECK_EQUAL_STRCMP(L"multibyte string",str);
}

NW_TEST(NanoWinMsSWPrintfTestGroup, NanoWinMsSWPrintfTest)
{
  wchar_t str[256];  

  int count1 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%ls ww %#2s%ws", L"abc", L"f", L"zz");
  NW_CHECK_EQUAL(11, count1);
  NW_CHECK_EQUAL_MEMCMP(L"abc ww  fzz", str, 12 * sizeof(wchar_t));

  int count2 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%c %wc %lc", L'd', L'e', L'f');
  NW_CHECK_EQUAL(5, count2);
  NW_CHECK_EQUAL_MEMCMP(L"d e f", str, 6 * sizeof(wchar_t));
  
  int count3 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%hd %+i  bb %#llu", (short int)-56, 123, (long long unsigned int)10);
  NW_CHECK_EQUAL(15, count3);
  NW_CHECK_EQUAL_MEMCMP(L"-56 +123  bb 10", str, 16 * sizeof(wchar_t));

  int count4 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%#x  %#X %x", 11, 22, 33);
  NW_CHECK_EQUAL(12, count4);
  NW_CHECK_EQUAL_MEMCMP(L"0xb  0X16 21", str, 13 * sizeof(wchar_t));

  int count5 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%o  %#o", 056, 24);
  NW_CHECK_EQUAL(7, count5);
  NW_CHECK_EQUAL_MEMCMP(L"56  030", str, 8 * sizeof(wchar_t));

  int count6 = NanoWinMsSWPrintf(str, sizeof(str) / sizeof(wchar_t), L"%.*f %.3f", 3, 0.1312f, 1.22222f);
  NW_CHECK_EQUAL(11, count6);
  NW_CHECK_EQUAL_MEMCMP(L"0.131 1.222", str, 12 * sizeof(wchar_t));
}

NW_END_TEST_GROUP()

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsFWPrintfTestGroup)
#ifndef __GNUC__
NW_TEST(NanoWinMsFWPrintfTestGroup, NanoWinMsFWPrintfSTest)
{
  wchar_t str[256];

  FILE *file = GetFile();
  if (file != NULL)
  {
    int count = NanoWinMsFWPrintf(file, L"%ls ww %#2s%ws", L"abc", L"f", L"zz");
    NW_CHECK_EQUAL(11, count);
  
    ReadFile(file, str);
    NW_CHECK_EQUAL_MEMCMP(L"abc ww  fzz", str, 12 * sizeof(wchar_t));
   
    CloseFile(file);
  }
}

NW_TEST(NanoWinMsFWPrintfTestGroup, NanoWinMsFWPrintfCTest)
{
  wchar_t str[256];

  FILE *file = GetFile();
  
  if (file != NULL)
  {
    int count = NanoWinMsFWPrintf(file, L"%c %wc %lc", L'd', L'e', L'f');
    NW_CHECK_EQUAL(5, count);

    ReadFile(file, str);
    NW_CHECK_EQUAL_MEMCMP(L"d e f", str, 6 * sizeof(wchar_t));

    CloseFile(file);
  }
}
#endif
NW_TEST(NanoWinMsFWPrintfTestGroup, NanoWinMsFWPrintfDTest)
{
  wchar_t str[256];

  FILE *file = GetFile();

  if (file != NULL)
  {
    int count = NanoWinMsFWPrintf(file, L"%hd %+i  bb %#llu", (short int)-56, 123, (long long unsigned int)10);
    NW_CHECK_EQUAL(15, count);

    ReadFile(file, str);
    NW_CHECK_EQUAL_MEMCMP(L"-56 +123  bb 10", str, 16 * sizeof(wchar_t));

    CloseFile(file);
  }
}

NW_TEST(NanoWinMsFWPrintfTestGroup, NanoWinMsFWPrintfXTest)
{
  wchar_t str[256];

  FILE *file = GetFile();

  if (file != NULL)
  {
    int count = NanoWinMsFWPrintf(file, L"%#x  %#X %x", 11, 22, 33);
    NW_CHECK_EQUAL(12, count);

    ReadFile(file, str);
    NW_CHECK_EQUAL_MEMCMP(L"0xb  0X16 21", str, 13 * sizeof(wchar_t));

    CloseFile(file);
  }
}

NW_TEST(NanoWinMsFWPrintfTestGroup, NanoWinMsFWPrintfOTest)
{
  wchar_t str[256];

  FILE *file = GetFile();

  if (file != NULL)
  {
    int count = NanoWinMsFWPrintf(file, L"%o  %#o", 056, 24);
    NW_CHECK_EQUAL(7, count);

    ReadFile(file, str);
    NW_CHECK_EQUAL_MEMCMP(L"56  030", str, 8 * sizeof(wchar_t));

    CloseFile(file);
  }
}

NW_TEST(NanoWinMsFWPrintfTestGroup, NanoWinMsFWPrintfFTest)
{
  wchar_t str[256];

  FILE *file = GetFile();

  if (file != NULL)
  {
    int count = NanoWinMsFWPrintf(file, L"%.*f %.3f", 3, 0.1312f, 1.22222f);
    NW_CHECK_EQUAL(11, count);

    ReadFile(file, str);
    NW_CHECK_EQUAL_MEMCMP(L"0.131 1.222", str, 12 * sizeof(wchar_t));

    CloseFile(file);
  }
}

NW_END_TEST_GROUP()
