// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// INTERNAL: wchat_t to char conversion functions

#include "NanoWinStrConvert.h"

#include <string.h>
#include <wchar.h>

namespace NanoWin
{
  const char *StrConverter::Error::what() const
  {
    return "string conversion failed";
  }

  std::string  StrConverter::Convert(const std::wstring &src)
  {
    mbstate_t state;

    memset(&state, 0, sizeof(state));

    const wchar_t *srcChars = src.c_str();

    size_t requiredLen = wcsrtombs(NULL, &srcChars, 0, &state);

    if (requiredLen == (size_t)-1)
    {
      throw Error();
    }

    std::string result;

    result.reserve(requiredLen);

    char charBuffer[MB_CUR_MAX];

    memset(&state, 0, sizeof(state));

    for (std::wstring::const_iterator pos = src.begin(); pos != src.end(); ++pos)
    {
      size_t charCount = wcrtomb(charBuffer, *pos, &state);

      if (charCount == (size_t)-1)
      {
        throw Error();
      }

      result.append(charBuffer, charCount);
    }

    return result;
  }

  std::wstring StrConverter::Convert(const std::string &src)
  {
    mbstate_t state;

    memset(&state, 0, sizeof(state));

    const char *srcChars = src.c_str();

    size_t requiredLen = mbsrtowcs(NULL, &srcChars, 0, &state);

    if (requiredLen == (size_t)-1)
    {
      throw Error();
    }

    std::wstring result;

    result.reserve(requiredLen);

    memset(&state, 0, sizeof(state));

    wchar_t currChar;

    const char *srcPtr = src.c_str();
    size_t      tailLen = src.length() + 1;

    size_t charLen = mbrtowc(&currChar, srcPtr, tailLen, &state);

    while ((ssize_t)charLen > 0)
    {
      result += currChar;

      srcPtr += charLen;
      tailLen -= charLen;

      charLen = mbrtowc(&currChar, srcPtr, tailLen, &state);
    }

    if (charLen != 0)
    {
      throw Error();
    }

    return result;
  }
}

namespace NanoWin
{
  WStrToStrClone::WStrToStrClone(const wchar_t *src)
  {
    buffer = preallocatedBuffer;

    if (src == NULL)
    {
      buffer = NULL;

      return;
    }

    mbstate_t state;

    memset(&state, 0, sizeof(state));

    const wchar_t *srcCopy = src;

    size_t requiredLen = wcsrtombs(NULL, &srcCopy, 0, &state);

    if (requiredLen != (size_t)-1)
    {
      if (requiredLen > PREALLOCATED_STRING_LEN)
      {
        buffer = new char[requiredLen + 1];
      }

      memset(&state, 0, sizeof(state));
      srcCopy = src;

      wcsrtombs(buffer, &srcCopy, requiredLen + 1, &state);
    }
    else
    {
      buffer[0] = '\0';
    }
  }

  WStrToStrClone::~WStrToStrClone()
  {
    if (buffer != preallocatedBuffer && buffer != NULL)
    {
      delete[] buffer;
    }
  }

  const char *WStrToStrClone::c_str() const
  {
    return buffer;
  }
}

namespace NanoWin
{
  StrToWStrClone::StrToWStrClone(const char*src)
  {
    if (src == NULL)
    {
      resultIsNull = true;
      return;
    }

    resultIsNull = false;

    try
    {
      result = StrConverter::Convert(src);
    }
    catch(...)
    {
      result.clear();
    }
  }

  StrToWStrClone::~StrToWStrClone()
  {
    // Auto
  }

  const wchar_t *StrToWStrClone::c_str() const
  {
    if (resultIsNull) { return(NULL); }
    return result.c_str();
  }
}

namespace NanoWin
{
  WStrByStrResultBag::WStrByStrResultBag(size_t nSize, bool bSrcIsNotNull)
  {
    bSrcIsNull = !bSrcIsNotNull;
    nSrcSize = nSize;
    nBagSize = (nSize) * WCHAR_TO_CHAR_MAX;
    if (!bSrcIsNull)
    {
      sBag.reserve(nBagSize+1); // +1 as "guard space"
    }
  }

  WStrByStrResultBag::~WStrByStrResultBag()
  {
    // Auto
  }

  char *WStrByStrResultBag::bag()
  {
    if (bSrcIsNull)
    {
      return(NULL);
    }

    return(const_cast<char *>(sBag.c_str()));
  }

  size_t WStrByStrResultBag::bagSize()
  {
    return(nBagSize);
  }

  bool  WStrByStrResultBag::copyBag(wchar_t *target, size_t nSize)
  const
  {
    if ((target != NULL) && (nSize > 0) && (!bSrcIsNull))
    {
      StrToWStrClone out(sBag.c_str());
      size_t outLen = wcslen(out.c_str());
      if (outLen < nSize)
      {
        wcscpy(target, out.c_str());
        return(true);
      }
      else
      {
        wcsncpy(target, out.c_str(), nSize);
        target[nSize] = 0; // null-terminate the string
        return(false);
      }
    }
    else
    {
      return(true);
    }
  }

  bool WStrByStrResultBag::copyBag(wchar_t *target)
  const
  {
    return(copyBag(target, nSrcSize));
  }
}
