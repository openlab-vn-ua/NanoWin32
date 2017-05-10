// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// INTERNAL: wchat_t to char conversion functions

#ifndef __NANOWINSTRCONVERT_H__
#define __NANOWINSTRCONVERT_H__

#include <string.h>
#include <wchar.h>
#include <string>

namespace NanoWin
{
  class StrConverter
  {
    public:

    class Error
    {
      public:
      virtual const char *what() const;
    };

    static std::string  Convert(const std::wstring &src);
    static std::wstring Convert(const std::string &src);
  };
}

#endif

