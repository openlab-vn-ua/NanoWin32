#include "NWUnitTest.h"

#include <locale.h>
#include <string.h>

#include "NanoWinStrConvert.h"

#ifdef __linux
#else
#include <windows.h>
#endif

#ifdef LINUX // linux-only tests

NW_BEGIN_TEST_GROUP(NanoWinStrConvertTestGroup)
NW_BEGIN_SETUP_TEARDOWN
NW_SETUP_METHOD()
 {
  setlocale(LC_CTYPE,"en_US.UTF-8");
 }
NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinStrConvertTestGroup,TestW2AASCIIOnlyConversion)
 {
  std::string result = NanoWin::StrConverter::Convert(std::wstring(L"hello"));

  NW_CHECK_EQUAL_STRCMP("hello",result.c_str());
 }

NW_TEST(NanoWinStrConvertTestGroup,TestW2AEmptyStringConversion)
 {
  std::string result = NanoWin::StrConverter::Convert(std::wstring(L""));

  NW_CHECK_EQUAL_STRCMP("",result.c_str());
 }

NW_TEST(NanoWinStrConvertTestGroup,TestW2ACyrillicStringConversion)
 {
  std::string result;

  try
   {
    result = NanoWin::StrConverter::Convert(std::wstring(L"привет"));
   }
  catch (...)
   {
   }

  NW_CHECK_EQUAL_STRCMP("привет",result.c_str());
 }

NW_TEST(NanoWinStrConvertTestGroup,TestA2WASCIIOnlyConversion)
 {
  std::wstring result = NanoWin::StrConverter::Convert(std::string("hello"));

  size_t len = wcslen(L"hello");

  NW_CHECK_EQUAL_ULONGS(len,result.length());

  NW_CHECK_EQUAL_MEMCMP(L"hello",result.c_str(),len * sizeof(wchar_t));
 }

NW_TEST(NanoWinStrConvertTestGroup,TestA2WEmptyStringConversion)
 {
  std::wstring result = NanoWin::StrConverter::Convert(std::string(""));

  NW_CHECK_EQUAL_ULONGS(0,result.length());
 }

NW_TEST(NanoWinStrConvertTestGroup,TestA2WCyrillicStringConversion)
 {
  std::wstring result;

  try
   {
    result = NanoWin::StrConverter::Convert(std::string("привет"));
   }
  catch (...)
   {
   }

  size_t len = wcslen(L"привет");

  NW_CHECK_EQUAL_ULONGS(len,result.length());

  NW_CHECK_EQUAL_MEMCMP(L"привет",result.c_str(),len * sizeof(wchar_t));
 }

NW_TEST(NanoWinStrConvertTestGroup,TestA2WCoreanStringConversion)
 {
  std::wstring result;

  try
   {
    result = NanoWin::StrConverter::Convert(std::string("안녕"));
   }
  catch (...)
   {
   }

  size_t len = wcslen(L"안녕");

  NW_CHECK_EQUAL_ULONGS(len,result.length());

  NW_CHECK_EQUAL_MEMCMP(L"안녕",result.c_str(),len * sizeof(wchar_t));
 }

NW_TEST(NanoWinStrConvertTestGroup,TestW2ACoreanStringConversion)
 {
  std::string result;

  try
   {
    result = NanoWin::StrConverter::Convert(std::wstring(L"안녕"));
   }
  catch (...)
   {
   }

  NW_CHECK_EQUAL_STRCMP("안녕",result.c_str());
 }

NW_TEST(NanoWinStrConvertTestGroup,TestA2WHebrewStringConversion)
 {
  std::wstring result;

  try
   {
    result = NanoWin::StrConverter::Convert(std::string("ד'ש"));
   }
  catch (...)
   {
   }

  size_t len = wcslen(L"ד'ש");

  NW_CHECK_EQUAL_ULONGS(len,result.length());

  NW_CHECK_EQUAL_MEMCMP(L"ד'ש",result.c_str(),len * sizeof(wchar_t));
 }

NW_TEST(NanoWinStrConvertTestGroup,TestW2AHebrewStringConversion)
 {
  std::string result;

  try
   {
    result = NanoWin::StrConverter::Convert(std::wstring(L"ד'ש"));
   }
  catch (...)
   {
   }

  NW_CHECK_EQUAL_STRCMP("ד'ש",result.c_str());
 }

NW_END_TEST_GROUP()

#endif // linux only

NW_BEGIN_TEST_GROUP(NanoWinStrConvertWin32APITestGroup)
NW_BEGIN_SETUP_TEARDOWN
NW_SETUP_METHOD()
 {
//  setlocale(LC_CTYPE,"en_US.UTF-8");
 }
NW_END_SETUP_TEARDOWN

NW_TEST(NanoWinStrConvertWin32APITestGroup,MultiByteToWideCharImplicitSize)
 {
  #define LEN (128)

  const char helloStr[] = "hello";
  const wchar_t helloWStr[] = L"hello";

  wchar_t dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = MultiByteToWideChar(CP_ACP,0,helloStr,-1,dest,LEN);

  NW_CHECK_EQUAL_INTS(6,result);
  NW_CHECK_EQUAL_MEMCMP(helloWStr,dest,sizeof(helloWStr));

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,MultiByteToWideCharExplicitSizeWithNullTerminator)
 {
  #define LEN (128)

  const char helloStr[] = "hello";
  const wchar_t helloWStr[] = L"hello";

  wchar_t dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = MultiByteToWideChar(CP_ACP,0,helloStr,6,dest,LEN);

  NW_CHECK_EQUAL_INTS(6,result);
  NW_CHECK_EQUAL_MEMCMP(helloWStr,dest,sizeof(helloWStr));

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,MultiByteToWideCharExplicitSizeNoNullTerminator)
 {
  #define LEN (128)

  const char helloStr[] = "hello";
  const wchar_t helloWStr[] = L"hello";

  wchar_t dest[LEN];
  unsigned char *dest_bytes = (unsigned char*)dest;

  memset(dest,'z',sizeof(dest));

  int result = MultiByteToWideChar(CP_ACP,0,helloStr,2,dest,LEN);

  NW_CHECK_EQUAL_INTS(2,result);
  NW_CHECK_EQUAL_MEMCMP(helloWStr,dest,2 * sizeof(wchar_t));

  NW_CHECK_EQUAL_BYTES('z',dest_bytes[2 * sizeof(wchar_t)]);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,MultiByteToWideCharCbMultiByteIsZeroFails)
 {
  #define LEN (128)

  const char helloStr[] = "hello";

  wchar_t dest[LEN];
  unsigned char *dest_bytes = (unsigned char*)dest;

  memset(dest,'z',sizeof(dest));

  int result = MultiByteToWideChar(CP_ACP,0,helloStr,0,dest,LEN);

  NW_CHECK_EQUAL_INTS(0,result);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[0]);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,MultiByteToWideCharCchWideCharIsZeroSucceeds)
 {
  #define LEN (128)

  const char helloStr[] = "hello";

  wchar_t dest[LEN];
  unsigned char *dest_bytes = (unsigned char*)dest;

  memset(dest,'z',sizeof(dest));

  int result = MultiByteToWideChar(CP_ACP,0,helloStr,-1,dest,0);

  NW_CHECK_EQUAL_INTS(6,result);
  NW_CHECK_EQUAL_BYTES('z',dest_bytes[0]);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,MultiByteToWideCharSmallBufferFails)
 {
  #define LEN (128)

  const char helloStr[] = "hello";

  wchar_t dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = MultiByteToWideChar(CP_ACP,0,helloStr,-1,dest,2);

  NW_CHECK_EQUAL_INTS(0,result);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,WideCharToMultiByteImplicitSize)
 {
  #define LEN (128)

  const char helloStr[] = "hello";
  const wchar_t helloWStr[] = L"hello";

  char dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = WideCharToMultiByte(CP_ACP,0,helloWStr,-1,dest,LEN,NULL,NULL);

  NW_CHECK_EQUAL_INTS(6,result);
  NW_CHECK_EQUAL_STRCMP(helloStr,dest);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,WideCharToMultiByteExplicitSizeWithNullTerminator)
 {
  #define LEN (128)

  const char helloStr[] = "hello";
  const wchar_t helloWStr[] = L"hello";

  char dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = WideCharToMultiByte(CP_ACP,0,helloWStr,6,dest,LEN,NULL,NULL);

  NW_CHECK_EQUAL_INTS(6,result);
  NW_CHECK_EQUAL_STRCMP(helloStr,dest);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,WideCharToMultiByteExplicitSizeNoNullTerminator)
 {
  #define LEN (128)

  const char helloStr[] = "hello";
  const wchar_t helloWStr[] = L"hello";

  char dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = WideCharToMultiByte(CP_ACP,0,helloWStr,2,dest,LEN,NULL,NULL);

  NW_CHECK_EQUAL_INTS(2,result);
  NW_CHECK_EQUAL_MEMCMP(helloStr,dest,2);

  NW_CHECK_EQUAL_BYTES('z',dest[2]);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,WideCharToMultiByteCchWideCharIsZeroFails)
 {
  #define LEN (128)

  const wchar_t helloWStr[] = L"hello";

  char dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = WideCharToMultiByte(CP_ACP,0,helloWStr,0,dest,LEN,NULL,NULL);

  NW_CHECK_EQUAL_INTS(0,result);
  NW_CHECK_EQUAL_BYTES('z',dest[0]);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,WideCharToMultiByteCbMultiByteIsZeroSucceeds)
 {
  #define LEN (128)

  const wchar_t helloWStr[] = L"hello";

  char dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = WideCharToMultiByte(CP_ACP,0,helloWStr,-1,dest,0,NULL,NULL);

  NW_CHECK_EQUAL_INTS(6,result);
  NW_CHECK_EQUAL_BYTES('z',dest[0]);

  #undef LEN
 }

NW_TEST(NanoWinStrConvertWin32APITestGroup,WideCharToMultiByteSmallBufferFails)
 {
  #define LEN (128)

  const wchar_t helloWStr[] = L"hello";

  char dest[LEN];

  memset(dest,'z',sizeof(dest));

  int result = WideCharToMultiByte(CP_ACP,0,helloWStr,-1,dest,2,NULL,NULL);

  NW_CHECK_EQUAL_INTS(0,result);

  #undef LEN
 }

NW_END_TEST_GROUP()

