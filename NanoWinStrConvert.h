// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// INTERNAL: wchat_t to char conversion functions

#ifndef __NANOWINSTRCONVERT_H__
#define __NANOWINSTRCONVERT_H__

#include <string>

namespace NanoWin
{
  class StrConverter
  {
    public:

    class Error
    {
      public:
      virtual const char *what() const; // a-la std::exception
    };

    static std::string  Convert(const std::wstring &src); // throw(Error);
    static std::wstring Convert(const std::string  &src); // throw(Error);
  };
}

namespace NanoWin
{
  class WStrToStrClone
  {
    public:

    WStrToStrClone (const wchar_t *src);
    ~WStrToStrClone();

    const char *c_str() const; // To look like std::string

    private:

    enum { PREALLOCATED_STRING_LEN = 256 };

    char  preallocatedBuffer[PREALLOCATED_STRING_LEN + 1];
    char *buffer;
  };
}

namespace NanoWin
{
  class StrToWStrClone
  {
    public:

    StrToWStrClone (const char *src);
    ~StrToWStrClone();

    const wchar_t *c_str() const; // To look like std::string

    private:

    // TODO: Optimize me like WStrToStrClone (uncritical)
    std::wstring       result;
    bool               resultIsNull;
  };
}

namespace NanoWin
{
  class WStrByStrResultBag
  {
    public:

    // Allocate at least nSize wchars for result [nSize counts '\0' byte, so if nSize is 0 no write allowed]
    // Use case:
    // WStrByStrResultBag sResult(nResultSize, lpResult);
    // GetCurrentDirectoryA(sResult.bag(), sResult.bagSize());
    // sResult.copyBag(lpResult);
    WStrByStrResultBag (size_t nSize, bool bSrcIsNotNull = true);
    ~WStrByStrResultBag();

    // provides room with at least nSize*WCHAR_TO_CHAR_MAX chars to hold result [nSize counts '\0' byte] [will be null is bSrcIsNotNull is false]
    char              *bag();
    size_t             bagSize();

    // copy result back as WStr [trune if size fits]
    bool               copyBag(wchar_t *target, size_t nSize) const;
    bool               copyBag(wchar_t *target) const; // Assume *target has at least nSize bytes

    private:

    enum { WCHAR_TO_CHAR_MAX = 4 }; // Expected that WCHAR can be represented by 4 CHARs

    std::string        sBag;
    size_t             nBagSize;
    size_t             nSrcSize;
	bool               bSrcIsNull;
  };
}

#endif

