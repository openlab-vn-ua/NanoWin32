#ifndef __PRIVATEPROFILETOOLS_H__
#define __PRIVATEPROFILETOOLS_H__

#include <string.h>
#include <string>

#define TestINIFileValueStringMaxLength (255)

//typedef char TestINIFileValueString[TestINIFileValueStringMaxLength + 1];

class TestINIFileValueString
 {
  public           :

                   TestINIFileValueString ()
   {
    memset(buffer,'Z',sizeof(buffer));

    buffer[sizeof(buffer) - 1] = '\0';
   }

  operator char * ()
   {
    return buffer;
   }

  char          get_char (size_t index)
   {
    if (index < sizeof(buffer))
     {
      return buffer[index];
     }
    else
     {
      return 'Z';
     }
   }

  private          :

  char             buffer[TestINIFileValueStringMaxLength + 1];
 };

class TestINIFileValueStringW
 {
  public           :

                   TestINIFileValueStringW ()
   {
    for (size_t i = 0; i < sizeof(buffer) - 1; i++)
     {
      buffer[i] = L'Z';
     }

    buffer[sizeof(buffer) - 1] = L'\0';
   }

  operator wchar_t * ()
   {
    return buffer;
   }

  wchar_t           get_char (size_t index)
   {
    if (index < sizeof(buffer))
     {
      return buffer[index];
     }
    else
     {
      return L'Z';
     }
   }

  private          :

  wchar_t          buffer[TestINIFileValueStringMaxLength + 1];
 };


extern char    TestINIFileName[];
extern wchar_t TestINIFileNameW[];

extern bool         TestINIFileCreateBinary (const char *src);
extern std::string  TestINIFileReadBinary   (void);
extern void         TestINIFileRemove       (void);
extern bool         TestINIFileCreateBinaryW(const wchar_t *src);
extern std::wstring TestINIFileReadBinaryW  (void);
extern void         TestINIFileRemoveW      (void);

#endif

