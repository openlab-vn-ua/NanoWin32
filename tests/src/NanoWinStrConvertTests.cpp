#include "NWUnitTest.h"

#include <locale.h>

#ifdef __linux
#include "NanoWinStrConvert.h"
#else
#include <windows.h>
#endif

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
