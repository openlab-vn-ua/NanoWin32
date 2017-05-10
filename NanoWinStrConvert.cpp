#include "NanoWinStrConvert.h"

namespace NanoWin {

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
