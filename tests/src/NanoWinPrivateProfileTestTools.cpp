#include <stdio.h>

#ifdef __linux
#include <unistd.h>
#endif

#include "NanoWinPrivateProfileTestTools.h"
#include "NanoWinStrConvert.h"

#ifdef __linux
char    TestINIFileName[]  = "test.ini"; // foreward slash replace test as well
wchar_t TestINIFileNameW[] = L"тест.ini"; // foreward slash replace test as well
#else
char    TestINIFileName[]  = "d:\\test.ini";
wchar_t TestINIFileNameW[] = L"d:\\тест.ini";
#endif

bool TestINIFileCreateBinary(const char *src)
 {
  bool  ok = false;
  FILE *f  = fopen(TestINIFileName,"wb");

  if (f == NULL)
   {
      // int e = errno;
      // printf("fopen(%s)=NULL,errno:%d\n", TestINIFileName, e);
      ok = false;
   }
  else
   {
    ok = fputs(src,f) > 0;

    if (!ok)
    {
      int e = errno;

      fprintf(f, "%d\n", e);
    }

    fclose(f);
   }

  return ok;
 }

bool TestINIFileCreateBinaryW(const wchar_t *src)
 {
  bool  ok =false;

  try
   {
    FILE *f  = fopen(NanoWin::StrConverter::Convert(TestINIFileNameW).c_str(),"wb");

    if (f != NULL)
     {
      ok = fputs(NanoWin::StrConverter::Convert(src).c_str(),f) > 0;

      fclose(f);
     }
   }
  catch (.../*NanoWin::StrConverter::ConversionError*/)
   {
    // conversion error is equal to file creation error
   }

  return ok;
 }

std::string TestINIFileReadBinary   (void)
 {
  std::string result;
  char        buffer[4096 + 1];

  FILE *f = fopen(TestINIFileName,"rb");

  if (f != NULL)
   {
    size_t read_size;

    do
     {
      read_size = fread(buffer,1,sizeof(buffer) - 1,f);

      if (read_size > 0)
       {
        buffer[read_size] = '\0';

        result += buffer;
       }
     } while (read_size > 0);

    fclose(f);
   }

  return result;
 }

std::wstring TestINIFileReadBinaryW   (void)
 {
  std::string result;
  char        buffer[4096 + 1];

  try
   {
    FILE *f = fopen(NanoWin::StrConverter::Convert(TestINIFileNameW).c_str(),"rb");

    if (f != NULL)
     {
      size_t read_size;

      do
       {
        read_size = fread(buffer,1,sizeof(buffer) - 1,f);

        if (read_size > 0)
         {
          buffer[read_size] = '\0';

          result += buffer;
         }
       } while (read_size > 0);

      fclose(f);
     }

    return NanoWin::StrConverter::Convert(result);
   }
  catch (.../*NanoWin::StrConverter::ConversionError*/)
   {
    // conversion error is similar to file open/read error

    return std::wstring();
   }
 }

void TestINIFileRemove (void)
 {
#ifdef __linux
  unlink(TestINIFileName);
#else
  _unlink(TestINIFileName);
#endif
 }

void TestINIFileRemoveW (void)
 {
  try
   {
    #ifdef __linux
     unlink(NanoWin::StrConverter::Convert(TestINIFileNameW).c_str());
    #else
     _unlink(NanoWin::StrConverter::Convert(TestINIFileNameW).c_str());
    #endif
   }
  catch (.../*NanoWin::StrConverter::ConversionError*/)
   {
    // ignore conversion errors, inability to remove file is
    // not critical in our tests
   }
 }
