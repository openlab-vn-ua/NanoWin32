#include "NWUnitTest.h"

#include <stdio.h>

#ifdef __linux
#include <unistd.h>
#include <string.h>

#include "NanoWinMsSafeScanf.h"

#else
#include <windows.h>
#endif

#define NanoWinMsSafeScanfGenericStrMaxLen (255)

typedef char NanoWinMsSafeScanfGenericStr[NanoWinMsSafeScanfGenericStrMaxLen + 1];

namespace
{
  class ScanfTestFile
  {
  public:

    ScanfTestFile(const char *testStr)
    {
      stream = fopen(fileName, "w+t");

      if (stream != NULL)
      {
        fputs(testStr, stream);
        fseek(stream, 0, SEEK_SET);
      }
    }

    ~ScanfTestFile()
    {
      if (stream != NULL)
      {
        fclose(stream);
      }

      unlink(fileName);
    }

    FILE *getStream()
    {
      return stream;
    }

    bool isOk()
    {
      return stream != NULL;
    }

  private:

    FILE *stream;
   
    static constexpr const char *fileName = "fscanf_s_test.txt";
  };

  class ScanfTestFileW
  {
  public:

    ScanfTestFileW(const wchar_t *testStr)
    {
      #ifndef __GNUC__
      stream = _wfopen(fileNameW, L"w+t");
      #else
      stream = fopen(fileName, "w+t");
      #endif

      if (stream != NULL)
      {
        fputws(testStr, stream);
        fseek(stream, 0, SEEK_SET);
      }
    }

    ~ScanfTestFileW()
    {
      if (stream != NULL)
      {
        fclose(stream);
      }
      #ifndef __GNUC__
      _wunlink(fileNameW);
      #else
      unlink(fileName);
      #endif
    }

    FILE *getStream()
    {
      return stream;
    }

    bool isOk()
    {
      return stream != NULL;
    }

  private:

    FILE *stream;

    static constexpr const wchar_t *fileNameW = L"fscanf_s_test.txt";
    static constexpr const char    *fileName  = "fscanf_s_test.txt";
  };
}


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsSafeSScanfSTestGroup)

#define NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(type,expected_value,specifier,testname)  \
NW_TEST(NanoWinMsSafeSScanfSTestGroup, testname)                                        \
{                                                                                   \
	const char  input[] = #expected_value;                                            \
	type                  actual_value;                                               \
                                                                                    \
	sscanf_s(input, specifier, &actual_value);                                        \
                                                                                    \
	NW_CHECK_EQUAL((type)expected_value, (type)actual_value);                         \
}

NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(int,          123,   "%d",  SScanfSimpleIntTest)
NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(int,          0123,  "%od", SScanfSimpleOctIntTest)
NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(unsigned int, 0x123, "%xd", SScanfSimpleHexIntTest)

NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(float, 1.23, "%f", SScanfSimpleFloatFTest)
NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(float, 1.23, "%e", SScanfSimpleFloatETest)
NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(float, 1.23, "%g", SScanfSimpleFloatGTest)

NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(long int,           345, "%ld",  SScanfSimpleLongTest)
NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(unsigned long,      345, "%lu",  SScanfSimpleLongUIntTest)

// CppUtest can't compare unsigned long long, so this test is disabled for gcc
#ifndef __GNUC__
NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(unsigned long long, 345, "%llu", SScanfSimpleLongLongUIntTest)
#endif

NW_DEFINE_SIMPLE_TYPED_SSCANF_TEST(short int, 456, "%hi", SScanfSimpleShortTest)

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfFileEmptyTest)
{
  const char    input[] = "";
  int           intNum;

  int count = sscanf_s(input, "%d", &intNum);

  NW_CHECK_EQUAL_INTS(-1, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSimpleLongLongIntTest)
{
  const char  input[] = "345";
  long long   longNum;

  int count = sscanf_s(input, "%lld", &longNum);

  NW_CHECK_EQUAL_INTS (1,   count);
  NW_CHECK_EQUAL_LONGS(345, longNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSimpleShortUIntTest)
{
  const char  input[] = "34";
  unsigned short   intNum;

  int count = sscanf_s(input, "%hu", &intNum);

  NW_CHECK_EQUAL_INTS (1,  count);
  NW_CHECK_EQUAL_LONGS(34, intNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfInvalidValueIntTest)
{
	const char  input[] = "abc";
	int   intNum;

	int count = sscanf_s(input, "%d", &intNum);

	NW_CHECK_EQUAL_INTS(0, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfIntWithCounterTest)
{
	const char  input[] = "-123";
	int   intNum;

	int count = sscanf_s(input, "%3d", &intNum);

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_INTS(-12, intNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfIntTest)
{
	const char  input[] = "0230  -0045 123abc";
	int   intNum1;
	int   intNum2;
	int   intNum3;

	int count = sscanf_s(input, "%10d%d%d", &intNum1, &intNum2, &intNum3);

	NW_CHECK_EQUAL_INTS(3, count);

	NW_CHECK_EQUAL_INTS(230, intNum1);
	NW_CHECK_EQUAL_INTS(-45,  intNum2);
	NW_CHECK_EQUAL_INTS(123, intNum3);

	count = sscanf_s(input, "%d%2d", &intNum1, &intNum2);

	NW_CHECK_EQUAL_INTS(2, count);

	NW_CHECK_EQUAL_INTS(230, intNum1);
	NW_CHECK_EQUAL_INTS(0, intNum2);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfInvalidValueOctIntTest)
{
	const char  input[] = "abc";
	int   octNum;

	int count = sscanf_s(input, "%od", &octNum);

	NW_CHECK_EQUAL_INTS(0, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfOctIntWithCounterTest)
{
	const char  input[] = "-123";
	int   octNum;

	int count = sscanf_s(input, "%3od", &octNum);

	NW_CHECK_EQUAL_INTS(1,    count);
	NW_CHECK_EQUAL_INTS(-012, octNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfOctIntTest)
{
	const char  input[] = "0230  -00459 123abc";
	int   octNum1;
	int   octNum2;
	int   octNum3;

	int count = sscanf_s(input, "%10od%od%od", &octNum1, &octNum2, &octNum3);

	NW_CHECK_EQUAL_INTS(1, count);

	NW_CHECK_EQUAL_INTS(0230, octNum1);

	count = sscanf_s(input, "%od%5od", &octNum1, &octNum2);

	NW_CHECK_EQUAL_INTS(1, count);

	NW_CHECK_EQUAL_INTS(0230, octNum1);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfInvalidValueHexIntTest)
{
	const char   input[] = "www";
	unsigned int hexNum;

	int count = sscanf_s(input, "%xd", &hexNum);

	NW_CHECK_EQUAL_INTS(0x0, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfHexIntWithCounterTest)
{
	const char   input[] = "12AB";
	unsigned int hexNum;

	int count = sscanf_s(input, "%3xd", &hexNum);

	NW_CHECK_EQUAL_INTS(0x1, count);
	NW_CHECK_EQUAL_INTS(0x12A, hexNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfHexIntTest)
{
	const char   input[] = "0230  -0045 123ABX";
	unsigned int hexNum1;
	unsigned int hexNum2;
	unsigned int hexNum3;

	int count = sscanf_s(input, "%10xd%xd%xd", &hexNum1, &hexNum2, &hexNum3);

	NW_CHECK_EQUAL_INTS(1, count);

	NW_CHECK_EQUAL_INTS(0x230,   hexNum1);

	count = sscanf_s(input, "%xd%2xd", &hexNum1, &hexNum2);

	NW_CHECK_EQUAL_INTS(1, count);

	NW_CHECK_EQUAL_INTS(0x230, hexNum1);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfInvalidValueShortTest)
{
	const char  input[] = "abc";
	short       shortNum;

	int count = sscanf_s(input, "%hd", &shortNum);

	NW_CHECK_EQUAL_INTS(0, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfShortWithCounterTest)
{
	const char  input[] = "-123";
	short       shortNum;

	int count = sscanf_s(input, "%3hd", &shortNum);

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_INTS(-12, shortNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfShortTest)
{
	const char  input[] = "0230  -0045 123abc";
	short       shortNum1;
	short       shortNum2;
	short       shortNum3;

	int count = sscanf_s(input, "%10hd%hd%hd", &shortNum1, &shortNum2, &shortNum3);

	NW_CHECK_EQUAL_INTS(3, count);

	NW_CHECK_EQUAL_INTS(230, shortNum1);
	NW_CHECK_EQUAL_INTS(-45, shortNum2);
	NW_CHECK_EQUAL_INTS(123, shortNum3);

	count = sscanf_s(input, "%hd%2hd", &shortNum1, &shortNum2);

	NW_CHECK_EQUAL_INTS(2, count);

	NW_CHECK_EQUAL_INTS(230, shortNum1);
	NW_CHECK_EQUAL_INTS(0, shortNum2);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfInvalidValueLongTest)
{
	const char  input[] = "abc";
	long int    longNum;

	int count = sscanf_s(input, "%ld", &longNum);

	NW_CHECK_EQUAL_INTS(0, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfLongWithCounterTest)
{
	const char  input[] = "123";
	long int    longNum;

	int count = sscanf_s(input, "%2ld", &longNum);

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_LONGS(12, longNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfLongTest)
{
	const char  input[] = "012 -00045  12.3abc";
	long int    longNum1;
	long int    longNum2;
	long int    longNum3;

	int count = sscanf_s(input, "%5ld%ld%ld", &longNum1, &longNum2, &longNum3);

	NW_CHECK_EQUAL_INTS(3, count);

	NW_CHECK_EQUAL_LONGS(12, longNum1);
	NW_CHECK_EQUAL_LONGS(-45, longNum2);
	NW_CHECK_EQUAL_LONGS(12, longNum3);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfInvalidValueFloatTest)
{
	const char  input[] = "abc";
	float floatNum;

	int count = sscanf_s(input, "%f", &floatNum);

	NW_CHECK_EQUAL_INTS(0, count);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfFloatWithCounterTest)
{
	const char  input[] = "1.23";
	float floatNum;

	int count = sscanf_s(input, "%3f", &floatNum);

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL(1.2f, floatNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfFloatTest)
{
	const char  input[] = "1.230045 abc12.3abc";
	float floatNum1;
	float floatNum2;
	float floatNum3;

	int count = sscanf_s(input, "%4f%6f%f", &floatNum1, &floatNum2, &floatNum3);

	NW_CHECK_EQUAL_INTS(2, count);

	NW_CHECK_EQUAL(1.23f, floatNum1);
	NW_CHECK_EQUAL(45.0f, floatNum2);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfIntAndFloatTest)
{
	const char  input[] = "123.45";
	int   intNum1;
	float floatNum1;

	int count = sscanf_s(input, "%2d%6f", &intNum1, &floatNum1);

	NW_CHECK_EQUAL_INTS(2, count);

	NW_CHECK_EQUAL_INTS(12,    intNum1);
	NW_CHECK_EQUAL_INTS(3.45f, floatNum1);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfStrSimpleTest)
{
	const char  input[] = "123abc";
	char str[8];

	int count = sscanf_s(input, "%s", str, sizeof(str));

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_STRCMP("123abc", str);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfStrWithCounterTest)
{
	const char  input[] = "123abc	 def 456";
	char str[8];

	int count = sscanf_s(input, "%3s", str, sizeof(str));

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_STRCMP("123", str);

	count = sscanf_s(input, "%10s", str, sizeof(str));

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_STRCMP("123abc", str);
}
/*
NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfStrSmallBuffTest)
{
	const char  input[] = "123abc";
	char str[3];

	int count = sscanf_s(input, "%s", str, sizeof(str));

	NW_CHECK_EQUAL_INTS(0, count);
	NW_CHECK_EQUAL_STRCMP("", str);
}
*/

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfPercentCharInFormatTest)
{
  const char input[] = "%37";
  int        value;

	int count = sscanf_s(input, "%%%d", &value);

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_INTS(37, value);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfStrTest)
{
	const char  input[] = "123  abc		4def5";
	
	char str1[8];
	char str2[8];
	char str3[8];

	int count = sscanf_s(input, "%s%s%s", str1, sizeof(str1), str2, sizeof(str2), str3, sizeof(str3));

	NW_CHECK_EQUAL_INTS(3, count);

	NW_CHECK_EQUAL_STRCMP("123", str1);
	NW_CHECK_EQUAL_STRCMP("abc", str2);
	NW_CHECK_EQUAL_STRCMP("4def5", str3);

	count = sscanf_s(input, "%2s%s%s", str1, sizeof(str1), str2, sizeof(str2), str3, sizeof(str3));

	NW_CHECK_EQUAL_INTS(3, count);

	NW_CHECK_EQUAL_STRCMP("12", str1);
	NW_CHECK_EQUAL_STRCMP("3", str2);
	NW_CHECK_EQUAL_STRCMP("abc", str3);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfCharSimpleTest)
{
	const char  input[] = "abc";
	char symb;

	int count = sscanf_s(input, "%c", &symb, 1);

	NW_CHECK_EQUAL_INTS(1, count);
	NW_CHECK_EQUAL_INTS('a', symb);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfCharWithCounterTest)
{
	const char  input[] = "abc";
	char symb[4];

	int count = sscanf_s(input, "%3c", symb, 3);

	NW_CHECK_EQUAL_INTS(1, count);

	NW_CHECK_EQUAL('a', symb[0]);
	NW_CHECK_EQUAL('b', symb[1]);
	NW_CHECK_EQUAL('c', symb[2]);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfCharTest)
{
	const char  input[] = "abc de";
	char symb;
	char str[5];

	int count = sscanf_s(input, "%c%3c", &symb, 1, str, 3);

	NW_CHECK_EQUAL_INTS(2, count);

	NW_CHECK_EQUAL('a', symb);
	NW_CHECK_EQUAL('b', str[0]);
	NW_CHECK_EQUAL('c', str[1]);
	NW_CHECK_EQUAL(' ', str[2]);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfCharAndIntTest)
{
	const char  input[] = "a12";
	char symb;
	int  intNum;

	int count = sscanf_s(input, "%c%d", &symb, 1, &intNum);

	NW_CHECK_EQUAL_INTS(2, count);

	NW_CHECK_EQUAL('a', symb);
	NW_CHECK_EQUAL_INTS(12, intNum);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfCommonTest)
{
	const char  input[] = "abc123 4def5.6.7 nnn56.78";
	
	char  str1[10];
	char  str2[10];
	char  str3[10];
	
  int   intNum1;
	float floatNum1;
	
	int count = sscanf_s(input, "%s%d%s%s", str1, sizeof(str1), &intNum1, str2, sizeof(str2), str3, sizeof(str3));

	NW_CHECK_EQUAL_INTS(4, count);

	NW_CHECK_EQUAL_STRCMP("abc123", str1);
	NW_CHECK_EQUAL_INTS  (4,     intNum1);
	NW_CHECK_EQUAL_STRCMP("def5.6.7", str2);
	NW_CHECK_EQUAL_STRCMP("nnn56.78", str3);

	count = sscanf_s(input, "%3s%d%4s%f", str1, sizeof(str1), &intNum1, str2, sizeof(str2), &floatNum1);

	NW_CHECK_EQUAL_INTS(4, count);

	NW_CHECK_EQUAL_STRCMP("abc", str1);
	NW_CHECK_EQUAL_INTS  (123, intNum1);
	NW_CHECK_EQUAL_STRCMP("4def", str2);
	NW_CHECK_EQUAL       (5.6f, floatNum1);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSTrivialTest)
{
  int                          parsedFieldCount;
  int                          intValue;
  float                        floatValue;

  parsedFieldCount = sscanf_s("abc", "abc");

  // it looks like implementation-specific behavior
  // under MS Windows function returns either 0 or -1 on different installations
  /*
  #ifdef __GNUC__
  NW_CHECK_EQUAL(0, parsedFieldCount);
  #else
  NW_CHECK_EQUAL(-1, parsedFieldCount);
  #endif
  */

  parsedFieldCount = sscanf_s("3", "%d", &intValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(3, intValue);

  parsedFieldCount = sscanf_s("3.14", "%f", &floatValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(3.14f, floatValue);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSTrivialIntTest)
{
  int                          parsedFieldCount;
  int                          intValue;
  unsigned int                 unsignedIntValue;

  parsedFieldCount = sscanf_s("3", "%d", &intValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(3, intValue);

  parsedFieldCount = sscanf_s("8", "%i", &intValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(8, intValue);

  parsedFieldCount = sscanf_s("010", "%i", &intValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(8, intValue);

  parsedFieldCount = sscanf_s("0x10", "%i", &intValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(16, intValue);

  parsedFieldCount = sscanf_s("10", "%x", &unsignedIntValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(16u, unsignedIntValue);

  parsedFieldCount = sscanf_s("12", "%X", &unsignedIntValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(18u, unsignedIntValue);

  parsedFieldCount = sscanf_s("10", "%o", &unsignedIntValue);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(8u, unsignedIntValue);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSTrivialCharTest)
{
  int                          parsedFieldCount;
  char                         charBuffer[250];

  parsedFieldCount = sscanf_s("Z", "%c", charBuffer, 1);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL_INTS('Z', charBuffer[0]);

  parsedFieldCount = sscanf_s("ABC", "%3c", charBuffer, sizeof(charBuffer));

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL_INTS('A', charBuffer[0]);
  NW_CHECK_EQUAL_INTS('B', charBuffer[1]);
  NW_CHECK_EQUAL_INTS('C', charBuffer[2]);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSTrivialMultiFieldTest)
{
  int                          parsedFieldCount;
  int                          intValue1;
  int                          intValue2;
  float                        floatValue;

  parsedFieldCount = sscanf_s("5 -3", "%d %d", &intValue1, &intValue2);

  NW_CHECK_EQUAL(2, parsedFieldCount);
  NW_CHECK_EQUAL(5, intValue1);
  NW_CHECK_EQUAL(-3, intValue2);

  parsedFieldCount = sscanf_s("-10 3.14", "%d %f", &intValue1, &floatValue);

  NW_CHECK_EQUAL(2, parsedFieldCount);
  NW_CHECK_EQUAL(-10, intValue1);
  NW_CHECK_EQUAL(3.14f, floatValue);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSStringSimpleTest)
{
  int                          parsedFieldCount;
  NanoWinMsSafeScanfGenericStr strValue;

  parsedFieldCount = sscanf_s("hello", "%s", strValue, sizeof(strValue));

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL_STRCMP("hello", strValue);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSStringAndOthersTest)
{
  int                          parsedFieldCount;
  int                          intValue;
  NanoWinMsSafeScanfGenericStr strValue;

  parsedFieldCount = sscanf_s("hello 11", "%s %d", strValue, sizeof(strValue), &intValue);

  NW_CHECK_EQUAL(2, parsedFieldCount);
  NW_CHECK_EQUAL_STRCMP("hello", strValue);
  NW_CHECK_EQUAL(11, intValue);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSStringAndFormatWhitespacesTest)
{
  int                          parsedFieldCount;
  NanoWinMsSafeScanfGenericStr str1Value;
  NanoWinMsSafeScanfGenericStr str2Value;

  parsedFieldCount = sscanf_s("   hello    sscanf_s  ", "%s \t %s",
    str1Value, sizeof(str1Value),
    str2Value, sizeof(str2Value));

  NW_CHECK_EQUAL(2, parsedFieldCount);
  NW_CHECK_EQUAL_STRCMP("hello", str1Value);
  NW_CHECK_EQUAL_STRCMP("sscanf_s", str2Value);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSAssignmentSuppressionTest)
{
  int                          parsedFieldCount;
  int                          int1Value;
  int                          int2Value;

  parsedFieldCount = sscanf_s("10 20 30", "%d %*d %d", &int1Value, &int2Value);

  NW_CHECK_EQUAL(2, parsedFieldCount);
  NW_CHECK_EQUAL(10, int1Value);
  NW_CHECK_EQUAL(30, int2Value);

  parsedFieldCount = sscanf_s("answer: 42", "%*s %d", &int1Value);

  NW_CHECK_EQUAL(1, parsedFieldCount);
  NW_CHECK_EQUAL(42, int1Value);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfAbsentValueTest)
{
  int                          parsedFieldCount;
  int                          int1Value;
  int                          int2Value;

  parsedFieldCount = sscanf_s("1", "%d%d", &int1Value, &int2Value);

  NW_CHECK_EQUAL(1, parsedFieldCount);

  NW_CHECK_EQUAL(1, int1Value);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSWidthSpecifierTest)
{
  int                          parsedFieldCount;
  int                          int1Value;
  int                          int2Value;
  float                        floatValue;

  parsedFieldCount = sscanf_s("1020", "%2d", &int1Value);

  NW_CHECK_EQUAL(1, parsedFieldCount);

  NW_CHECK_EQUAL(10, int1Value);

  parsedFieldCount = sscanf_s("1321", "%2d%1d", &int1Value, &int2Value);

  NW_CHECK_EQUAL(2, parsedFieldCount);

  NW_CHECK_EQUAL(13, int1Value);
  NW_CHECK_EQUAL(2, int2Value);

  parsedFieldCount = sscanf_s("14 21.1", "%16d %f", &int1Value, &floatValue);

  NW_CHECK_EQUAL(2, parsedFieldCount);

  NW_CHECK_EQUAL(14, int1Value);
  NW_CHECK_EQUAL(21.1f, floatValue);
}

NW_TEST(NanoWinMsSafeSScanfSTestGroup, SScanfSBadFormatTest)
{
  int                          parsedFieldCount;
  int                          intValue;

  parsedFieldCount = sscanf_s("abc", "%d", &intValue);

  NW_CHECK_EQUAL(0, parsedFieldCount);

  parsedFieldCount = sscanf_s("abc", "%2d", &intValue);

  NW_CHECK_EQUAL(0, parsedFieldCount);
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinSafeFScanfTestGroup)

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfFileEmptyTest)
{
  ScanfTestFile input("");
  int           intNum;

  int count = fscanf_s(input.getStream(), "%d", &intNum);

  NW_CHECK_EQUAL_INTS(-1, count);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfIntTest)
{
  ScanfTestFile input("123");
  int           intNum;

  int count = fscanf_s(input.getStream(), "%d", &intNum);

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_INTS(123, intNum);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfIntTest1)
{
  ScanfTestFile input("123");
  int           intNum1;
  int           intNum2;

  int count = fscanf_s(input.getStream(), "%d%d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_INTS(123, intNum1);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfIntTest2)
{
  ScanfTestFile input("123      ");
  int           intNum1;
  int           intNum2;

  int count = fscanf_s(input.getStream(), "%d%d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_INTS(123, intNum1);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfIntTest3)
{
  ScanfTestFile input("123");
  int           intNum1;
  int           intNum2;

  int count = fscanf_s(input.getStream(), "%2d%3d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(2,  count);
  NW_CHECK_EQUAL_INTS(12, intNum1);
  NW_CHECK_EQUAL_INTS(3,  intNum2);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfStrTest1)
{
  ScanfTestFile input("123");
  char          str1[8];
  char          str2[8];

  int count = fscanf_s(input.getStream(), "%s%s", str1, sizeof(str1), str2, sizeof(str2));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_STRCMP("123", str1);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FScanfStrTest2)
{
  ScanfTestFile input("123      ");
  char          str1[8];
  char          str2[8];

  int count = fscanf_s(input.getStream(), "%4s%s", str1, sizeof(str1), str2, sizeof(str2));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_STRCMP("123", str1);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FWScanfFileEmptyTest)
{
  ScanfTestFileW input(L"");
  int            intNum;

  int count = fwscanf_s(input.getStream(), L"%d", &intNum);

  NW_CHECK_EQUAL_INTS(-1, count);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FWScanfIntTest1)
{
  ScanfTestFileW input(L"123");
  int            intNum1;
  int            intNum2;

  int count = fwscanf_s(input.getStream(), L"%d%d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_INTS(123, intNum1);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FWScanfIntTest2)
{
  ScanfTestFileW input(L"123      ");
  int            intNum1;
  int            intNum2;

  int count = fwscanf_s(input.getStream(), L"%d%d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_INTS(123, intNum1);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FWScanfIntTest3)
{
  ScanfTestFileW input(L"123");
  int            intNum1;
  int            intNum2;

  int count = fwscanf_s(input.getStream(), L"%2d%3d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(2, count);
  NW_CHECK_EQUAL_INTS(12, intNum1);
  NW_CHECK_EQUAL_INTS(3, intNum2);
}

NW_TEST(NanoWinSafeFScanfTestGroup, FWScanfStrTest1)
{
  ScanfTestFileW input(L"123");
  wchar_t        str1[18];
  wchar_t        str2[18];

  int count = fwscanf_s(input.getStream(), L"%s%s", 
                        str1, (unsigned)(sizeof(str1) / sizeof(wchar_t)), 
                        str2, (unsigned)(sizeof(str2) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_MEMCMP(L"123", str1, 3 * sizeof(wchar_t));
}

NW_TEST(NanoWinSafeFScanfTestGroup, FWScanfStrTest2)
{
  ScanfTestFileW input(L"123      ");
  wchar_t        str1[18];
  wchar_t        str2[18];

  int count = fwscanf_s(input.getStream(), L"%4s%s", 
                       str1, (unsigned)(sizeof(str1) / sizeof(wchar_t)),
                       str2, (unsigned)(sizeof(str2) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_MEMCMP(L"123", str1, 3 * sizeof(wchar_t));
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsSafeSWScanfSTestGroup)

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfFileEmptyTest)
{
  const wchar_t    input[] = L"";
  int              intNum;

  int count = swscanf_s(input, L"%d", &intNum);

  NW_CHECK_EQUAL_INTS(-1, count);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfIntTest)
{
  const wchar_t input[] = L"0230  -0045 123abc";
  int   intNum1;
  int   intNum2;
  int   intNum3;

  int count = swscanf_s(input, L"%10d%d%d", &intNum1, &intNum2, &intNum3);

  NW_CHECK_EQUAL_INTS(3, count);

  NW_CHECK_EQUAL_INTS(230, intNum1);
  NW_CHECK_EQUAL_INTS(-45, intNum2);
  NW_CHECK_EQUAL_INTS(123, intNum3);

  count = swscanf_s(input, L"%d%2d", &intNum1, &intNum2);

  NW_CHECK_EQUAL_INTS(2, count);

  NW_CHECK_EQUAL_INTS(230, intNum1);
  NW_CHECK_EQUAL_INTS(0, intNum2);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfOctIntTest)
{
  const wchar_t  input[] = L"0230  -00459 123abc";
  int   octNum1;
  int   octNum2;
  int   octNum3;

  int count = swscanf_s(input, L"%10od%od%od", &octNum1, &octNum2, &octNum3);

  NW_CHECK_EQUAL_INTS(1, count);

  NW_CHECK_EQUAL_INTS(0230, octNum1);

  count = swscanf_s(input, L"%od%5od", &octNum1, &octNum2);

  NW_CHECK_EQUAL_INTS(1, count);

  NW_CHECK_EQUAL_INTS(0230, octNum1);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfHexIntTest)
{
  const wchar_t   input[] = L"0230  -0045 123ABX";
  unsigned int hexNum1;
  unsigned int hexNum2;
  unsigned int hexNum3;

  int count = swscanf_s(input, L"%10xd%xd%xd", &hexNum1, &hexNum2, &hexNum3);

  NW_CHECK_EQUAL_INTS(1, count);

  NW_CHECK_EQUAL_INTS(0x230, hexNum1);

  count = swscanf_s(input, L"%xd%2xd", &hexNum1, &hexNum2);

  NW_CHECK_EQUAL_INTS(1, count);

  NW_CHECK_EQUAL_INTS(0x230, hexNum1);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfShortTest)
{
  const wchar_t  input[] = L"0230  -0045 123abc";
  short       shortNum1;
  short       shortNum2;
  short       shortNum3;

  int count = swscanf_s(input, L"%10hd%hd%hd", &shortNum1, &shortNum2, &shortNum3);

  NW_CHECK_EQUAL_INTS(3, count);

  NW_CHECK_EQUAL_INTS(230, shortNum1);
  NW_CHECK_EQUAL_INTS(-45, shortNum2);
  NW_CHECK_EQUAL_INTS(123, shortNum3);

  count = swscanf_s(input, L"%hd%2hd", &shortNum1, &shortNum2);

  NW_CHECK_EQUAL_INTS(2, count);

  NW_CHECK_EQUAL_INTS(230, shortNum1);
  NW_CHECK_EQUAL_INTS(0, shortNum2);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfLongTest)
{
  const wchar_t  input[] = L"012 -00045  12.3abc";
  long int    longNum1;
  long int    longNum2;
  long int    longNum3;

  int count = swscanf_s(input, L"%5ld%ld%ld", &longNum1, &longNum2, &longNum3);

  NW_CHECK_EQUAL_INTS(3, count);

  NW_CHECK_EQUAL_LONGS(12, longNum1);
  NW_CHECK_EQUAL_LONGS(-45, longNum2);
  NW_CHECK_EQUAL_LONGS(12, longNum3);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfFloatTest)
{
  const wchar_t  input[] = L"1.230045 abc12.3abc";
  float floatNum1;
  float floatNum2;
  float floatNum3;

  int count = swscanf_s(input, L"%4f%6f%f", &floatNum1, &floatNum2, &floatNum3);

  NW_CHECK_EQUAL_INTS(2, count);

  NW_CHECK_EQUAL(1.23f, floatNum1);
  NW_CHECK_EQUAL(45.0f, floatNum2);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfIntAndFloatTest)
{
  const wchar_t  input[] = L"123.45";
  int   intNum1;
  float floatNum1;

  int count = swscanf_s(input, L"%2d%6f", &intNum1, &floatNum1);

  NW_CHECK_EQUAL_INTS(2, count);

  NW_CHECK_EQUAL_INTS(12, intNum1);
  NW_CHECK_EQUAL_INTS(3.45f, floatNum1);
}


NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfStrSimpleTest)
{
  const wchar_t  input[] = L"123abc";
  wchar_t str[18];

  int count = swscanf_s(input, L"%s", str, (unsigned)(sizeof(str) /sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_MEMCMP(L"123abc", str, sizeof(input));
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfStrWithCounterTest)
{
  const wchar_t  input[] = L"123abc	 def 456";
  wchar_t str[8];

  int count = swscanf_s(input, L"%3s", str, (unsigned)(sizeof(str) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_MEMCMP(L"123", str, 3 * sizeof(wchar_t));

  count = swscanf_s(input, L"%10s", str, (unsigned)(sizeof(str) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_MEMCMP(L"123abc", str, 6 * sizeof(wchar_t));
}


NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfStrTest)
{
  const wchar_t  input[] = L"123  abc		4def5";

  wchar_t str1[8];
  wchar_t str2[8];
  wchar_t str3[8];

  int count = swscanf_s(input, L"%s%s%s", 
                        str1, (unsigned)(sizeof(str1) / sizeof(wchar_t)), 
                        str2, (unsigned)(sizeof(str2) / sizeof(wchar_t)), 
                        str3, (unsigned)(sizeof(str3) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(3, count);

  NW_CHECK_EQUAL_MEMCMP(L"123",   str1, 3 * sizeof(wchar_t));
  NW_CHECK_EQUAL_MEMCMP(L"abc",   str2, 3 * sizeof(wchar_t));
  NW_CHECK_EQUAL_MEMCMP(L"4def5", str3, 5 * sizeof(wchar_t));

  count = swscanf_s(input, L"%2s%s%s",
                    str1, (unsigned)(sizeof(str1) / sizeof(wchar_t)),
                    str2, (unsigned)(sizeof(str2) / sizeof(wchar_t)),
                    str3, (unsigned)(sizeof(str3) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(3, count);

  NW_CHECK_EQUAL_MEMCMP(L"12", str1,  2 * sizeof(wchar_t));
  NW_CHECK_EQUAL_MEMCMP(L"3", str2,   1 * sizeof(wchar_t));
  NW_CHECK_EQUAL_MEMCMP(L"abc", str3, 3 * sizeof(wchar_t));
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfCharSimpleTest)
{
  const wchar_t  input[] = L"abc";
  wchar_t symb;

  int count = swscanf_s(input, L"%c", &symb, 1);

  NW_CHECK_EQUAL_INTS(1, count);
  NW_CHECK_EQUAL_INTS(L'a', symb);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfCharWithCounterTest)
{
  const wchar_t  input[] = L"abc";
  wchar_t symb[4];

  int count = swscanf_s(input, L"%3c", symb, (unsigned)(sizeof(symb) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(1, count);

  NW_CHECK_EQUAL(L'a', symb[0]);
  NW_CHECK_EQUAL(L'b', symb[1]);
  NW_CHECK_EQUAL(L'c', symb[2]);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfCharTest)
{
  const wchar_t  input[] = L"abc de";
  wchar_t symb;
  wchar_t str[5];

  int count = swscanf_s(input, L"%c%3c", 
                        &symb, (unsigned)(sizeof(symb) / sizeof(wchar_t)),
                        str, (unsigned)(sizeof(str) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(2, count);

  NW_CHECK_EQUAL(L'a', symb);
  NW_CHECK_EQUAL(L'b', str[0]);
  NW_CHECK_EQUAL(L'c', str[1]);
  NW_CHECK_EQUAL(L' ', str[2]);
}

NW_TEST(NanoWinMsSafeSWScanfSTestGroup, SWScanfCommonTest)
{
  const wchar_t  input[] = L"abc123 4def5.6.7 nnn56.78";

  wchar_t  str1[10];
  wchar_t  str2[10];
  wchar_t  str3[10];

  int   intNum1;
  float floatNum1;

  int count = swscanf_s(input, L"%s%d%s%s",
                        str1, (unsigned)(sizeof(str1) / sizeof(wchar_t)),
                        &intNum1, 
                        str2, (unsigned)(sizeof(str2) / sizeof(wchar_t)),
                        str3, (unsigned)(sizeof(str3) / sizeof(wchar_t)));

  NW_CHECK_EQUAL_INTS(4, count);

  NW_CHECK_EQUAL_MEMCMP(L"abc123", str1, 6 * sizeof(wchar_t));
  NW_CHECK_EQUAL_INTS(4, intNum1);
  NW_CHECK_EQUAL_MEMCMP(L"def5.6.7", str2, 8 * sizeof(wchar_t));
  NW_CHECK_EQUAL_MEMCMP(L"nnn56.78", str3, 8 * sizeof(wchar_t));

  count = swscanf_s(input, L"%3s%d%4s%f", 
                    str1, (unsigned)(sizeof(str1) / sizeof(wchar_t)),
                    &intNum1, 
                    str2, (unsigned)(sizeof(str2) / sizeof(wchar_t)),
                    &floatNum1);

  NW_CHECK_EQUAL_INTS(4, count);

  NW_CHECK_EQUAL_MEMCMP(L"abc", str1, 3 * sizeof(wchar_t));
  NW_CHECK_EQUAL_INTS(123, intNum1);
  NW_CHECK_EQUAL_MEMCMP(L"4def", str2, 4 * sizeof(wchar_t));
  NW_CHECK_EQUAL(5.6f, floatNum1);
}

NW_END_TEST_GROUP()