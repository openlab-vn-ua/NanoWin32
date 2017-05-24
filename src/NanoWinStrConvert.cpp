// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// INTERNAL: wchat_t to char conversion functions

#include "NanoWinStrConvert.h"

#ifdef LINUX
 #include "NanoWinError.h"
 #include "NanoWinMsSafeString.h"
#endif

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

    #ifdef __linux
    char charBuffer[MB_CUR_MAX];
    #else
    char charBuffer[6];
    #endif

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

    while (charLen > 0 && charLen != (size_t)-1  && charLen != (size_t)-2)
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

#ifdef LINUX

NW_EXTERN_C_BEGIN

extern int MultiByteToWideChar(_In_      UINT   CodePage,
                               _In_      DWORD  dwFlags,
                               _In_      LPCSTR lpMultiByteStr,
                               _In_      int    cbMultiByte,
                               _Out_opt_ LPWSTR lpWideCharStr,
                               _In_      int    cchWideChar)
{
  if (CodePage != CP_ACP) // other codepages not supported
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return 0;
  }

  if (dwFlags != 0) // flags are not supported
  {
    SetLastError(ERROR_INVALID_FLAGS);
    return 0;
  }

  if (cbMultiByte == 0)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return 0;
  }

  if ((void*)lpMultiByteStr == (void*)lpWideCharStr)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return 0;
  }

  mbstate_t state;

  memset(&state, 0, sizeof(state));

  const char *srcPtr  = lpMultiByteStr;
  wchar_t    *dstPtr  = cchWideChar > 0 ? lpWideCharStr : NULL;
  size_t      tailLen = cbMultiByte == -1 ? strlen(lpMultiByteStr) + 1 : cbMultiByte;
  bool        interrupted = false;
  int         wideCharCounter = 0;

  while (!interrupted && tailLen > 0 && (cchWideChar == 0 || wideCharCounter < cchWideChar))
  {
    size_t charLen = mbrtowc(dstPtr,srcPtr,tailLen,&state);

    if (charLen > 0 && charLen <= tailLen)
    {
      tailLen -= charLen;
      srcPtr  += charLen;

      if (dstPtr != NULL) dstPtr++;

      wideCharCounter++;
    }
    else
    {
      interrupted = true;

      if (charLen == 0)
      {
        tailLen--;
        wideCharCounter++;
      }
    }
  }

  if (tailLen > 0)
  {
    SetLastError(ERROR_INSUFFICIENT_BUFFER);
    return 0;
  }
  else
  {
    return wideCharCounter;
  }
}

extern int WideCharToMultiByte(_In_      UINT    CodePage,
                               _In_      DWORD   dwFlags,
                               _In_      LPCWSTR lpWideCharStr,
                               _In_      int     cchWideChar,
                               _Out_opt_ LPSTR   lpMultiByteStr,
                               _In_      int     cbMultiByte,
                               _In_opt_  LPCSTR  lpDefaultChar,
                               _Out_opt_ LPBOOL  lpUsedDefaultChar)
{
  if (CodePage != CP_ACP) // other codepages not supported
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return 0;
  }

  if (dwFlags != 0) // flags are not supported
  {
    SetLastError(ERROR_INVALID_FLAGS);
    return 0;
  }

  if (cchWideChar == 0)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return 0;
  }

  if ((void*)lpMultiByteStr == (void*)lpWideCharStr)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return 0;
  }

  mbstate_t state;

  memset(&state, 0, sizeof(state));

  const wchar_t *srcPtr  = lpWideCharStr;
  char          *dstPtr  = cbMultiByte > 0 ? lpMultiByteStr : NULL;
  size_t         tailLen = cchWideChar == -1 ? wcslen(lpWideCharStr) + 1 : cchWideChar;
  int            byteCounter = 0;

  #ifdef __linux
  char charBuffer[MB_CUR_MAX];
  #else
  char charBuffer[6];
  #endif

  bool ok = true;

  while (ok && tailLen > 0)
  {
    size_t byteCount = wcrtomb(charBuffer,*srcPtr++,&state);

    if (byteCount != (size_t)-1)
    {
      if (dstPtr != NULL)
      {
        size_t offset = 0;

        while (offset < byteCount && (cbMultiByte == 0 || byteCounter < cbMultiByte))
        {
          *dstPtr++ = charBuffer[offset++];

          byteCounter++;
        }

        if (offset < byteCount)
        {
          ok = false;
          SetLastError(ERROR_INSUFFICIENT_BUFFER);
        }
      }
      else
      {
        byteCounter += byteCount;
      }

      tailLen--;
    }
    else
    {
      ok = false;
      SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    }
  }

  if (!ok)
  {
    byteCounter = 0;
  }

  return byteCounter;
}

NW_EXTERN_C_END

#endif  // LINUX
