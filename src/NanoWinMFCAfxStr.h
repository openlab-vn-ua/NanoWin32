// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCStr functions

#if !defined(NanoWinMFCAfxStrIncluded)
#define NanoWinMFCAfxStrIncluded

#include "NanoWinTypes.h"

#if defined LINUX

// MFC subset (CString and related classes)

#include <string>
#include <string.h> // strlen
#include <stdarg.h> // va_arg

#include "NanoWinMsSafeString.h"
#include "NanoWinMsExtra.h"

#include "NanoWinTCHAR.h"
#include "NanoWinMFCAfx.h"

#if defined(UNICODE) || defined(_UNICODE)
#include <wchar.h>
#include "NanoWinStrConvert.h"
#endif

// Class to implement CSimpleString subset. [Only null-terminated strings supported]
// Note that for MBCS strings, CString still counts, returns, and manipulates strings based on 8-bit characters,
// and your application must interpret MBCS lead and trail bytes itself
class CSimpleString
{
  // Note: Current (simple) implemenation not use value sharing optimisation, so all strings has its own independent buffer

  #define REQUIRE(f) if (!(f)) { throw "CSimpleString has invalid param:" #f; }
  #define CHECKUP(f) if (!(f)) { throw "CSimpleString operation failed:" #f; }

  protected:

  #if defined(UNICODE) || defined(_UNICODE)
  std::wstring strBuf;
  #else
  std::string  strBuf;
  #endif

  public:

  #if defined(UNICODE) || defined(_UNICODE)
  typedef wchar_t      XCHAR;  // "Native"    char type for string (TCHAR) [wchar_t]
  typedef char         YCHAR;  // "Alternate" char type for string [char]
  #else
  typedef char         XCHAR;  // "Native"    char type for string (TCHAR) [char]
  typedef wchar_t      YCHAR;  // "Alternate" char type for string [wchar_t]
  #endif

  typedef XCHAR       *PXSTR;  // TCHAR*
  typedef const XCHAR *PCXSTR; // const TCHAR*
  typedef YCHAR       *PYSTR;  // [Non-native char type] *
  typedef const YCHAR *PCYSTR; // const [Non-native char type] *

  static int StringLength(const TCHAR *psz) // throw()
  {
    return(_tcslen(psz));
  }

  // Need some study (do we need this?)
  // static void CopyChars(XCHAR* pchDest, const XCHAR* pchSrc, int nChars) throw(); // XCHAR=TCHAR here
  // static void CopyCharsOverlapped(XCHAR* pchDest, const XCHAR* pchSrc, int nChars) throw(); // XCHAR=TCHAR here

  public:

  // CSimpleString

  // Need some study (do we need this?)
  // void Append(const CSimpleStringT& strSrc);

  void Append(const TCHAR *text)
  {
    REQUIRE(text != NULL);
    strBuf.append(text);
  }

  void Append(const TCHAR *text, int nLength)
  {
    REQUIRE(text != NULL);
    REQUIRE(nLength >= 0);
    strBuf.append(text, nLength);
  }

  void AppendChar(TCHAR item)
  {
    REQUIRE(item != 0); // TODO: Check assumption
    strBuf.append(1, item);
  }

  void Empty(void)
  {
    strBuf.clear();
    strBuf.shrink_to_fit();
  }

  int  GetLength()
  const
  {
    return(strBuf.length());
  }

  // Preallocate/FreeExtra/GetAllocLength // TODO: Clarify what length here mean? TCHAR[Length] or TCHAR[Length+1] 

  protected:

  void FreeExtraSoftHint(void)
  {
    strBuf.reserve(0); // Implemenation-defined hint to free extra mem
  }

  public:

  // Allocates a specific amount of bytes for the CSimpleStringT object.
  // nLength is exact size of the CSimpleStringT character buffer in characters
  // analog of TCHAR Preallocate[nLength]
  void Preallocate(int nLength)
  {
    // Actually, some guard +1 space is here (thanks to std::string). BTW CAfxStringMgr::Allocate look like do the same :)

    REQUIRE(nLength >= 0);
    strBuf.reserve(nLength); 
  }

  void FreeExtra(void)
  {
    strBuf.shrink_to_fit();
  }

  int GetAllocLength()
  const
  {
    return(strBuf.capacity());
  }

  // LockBuffer/UnlockBuffer

  // Make this strings buffer unique (that is, not {potentially} shared with other strings with same value)
  // Note: It is good idea to Preallocate(nLength) space in buffer first!
  TCHAR *LockBuffer()
  {
    return(const_cast<TCHAR *>(strBuf.c_str())); // Fairly simple, since we do not implement value sharing optimisation
  }

  // Unlock buffer previously locked by LockBuffer()
  void  UnlockBuffer() // throw()
  {
    // Fairly simple, since we do not implement value sharing optimisation
  }

  // GetBuffer/ReleaseBuffer // TODO: Clarify what length here mean? TCHAR[Length] or TCHAR[Length+1] 

  // Returns a pointer to the internal character buffer for the CSimpleStringT object.
  // nMinBufferLength is a min count the characters buffer can hold. This value does not include space for a null terminator. "TCHAR GetBuffer[nMinBufferLength]"
  // The buffer would be made unique (that is, not {potentially} shared with other strings with same value)
  // TODO: Semantic is not clear here. Why do we need LockBuffer in that case? Is initial content preserved (guess no)?
  PXSTR GetBuffer(int nMinBufferLength)
  {
    Preallocate(nMinBufferLength);
    return(LockBuffer());
  }

  // Returns a pointer to the internal character buffer for the CSimpleStringT object.
  // The buffer would be made unique (that is, not {potentially} shared with other strings with same value)
  // Note: It is good idea to Preallocate(nLength) space in buffer first ???
  // TODO: Semantic is not clear here. What length result buffer would have? Why do we need LockBuffer in that case? Is content preserved (guess no)?
  PXSTR GetBuffer()
  {
    return(LockBuffer());
  }

  // Much a same as GetBuffer(int nLength), but preserves string content?
  // The buffer would be made unique (that is, not {potentially} shared with other strings with same value)
  PXSTR GetBufferSetLength(int nLength)
  {
    int len = GetLength();

    if (len > nLength)
    {
      Truncate(nLength); FreeExtra();
    }
    else
    {
      Preallocate(nLength);
    }

    return(LockBuffer());
  }

  void  ReleaseBuffer(int nNewLength = -1)
  {
    if (nNewLength <= 0)
    {
      FreeExtraSoftHint(); // Adjust to actual capacity, if want to
    }
    else
    {
      Preallocate(nNewLength);
    }
  }

  void  ReleaseBufferSetLength(int nNewLength)
  {
    Preallocate(nNewLength);
  }

  // Check to we need this (hope no)
  // IAtlStringMgr* GetManager() const throw();
  // void SetManager(IAtlStringMgr* pStringMgr);

  // Copy string to this CString

  void SetString(const TCHAR *src)
  {
    REQUIRE(src != NULL);
    if (src == strBuf.c_str()) { return; } // Copy to myself: nothing to do
    strBuf = src;
  }

  // No more then nLength char + checks if we are in the same buffer as the current string
  void SetString(const TCHAR *src, int nLength)
  {
    REQUIRE(src != NULL);
    if (nLength <= 0)          { Empty(); return; }
    int dlen = strBuf.length();
    const TCHAR *dst = strBuf.c_str();
    if (src == dst)
    {
      // Copy to myself, but at most first nLength chars only
      if (dlen > nLength)
      {
        Truncate(nLength);
      }
      return;
    }

    int slen = StringLength(src);

    if (slen <= 0)            { Empty(); return; }

    // check for overlap
    if (
        ((src < dst) && ((src + slen) < dst)) ||
        ((dst < src) && ((dst + dlen) < src))
       )
    {
      // No overlap
      int clen = nLength;
      if (clen > slen) { clen = slen; }
      Preallocate(clen+1);
      TCHAR *dst = LockBuffer();
      _tcsncpy_s(dst, clen+1, src, clen); // Should create null terminated strings
      UnlockBuffer();
    }
    else
    {
      // overlap
      int clen = nLength;
      if (clen > slen) { clen = slen; }
      Preallocate(clen+1);
      TCHAR *dst = LockBuffer();
      memmove_s(dst,clen*sizeof(*src),src,clen*sizeof(*src));
      dst[clen] = 0; // Null terminate
      UnlockBuffer();
    }
  }

  const TCHAR *GetString()
  const
  {
    return(strBuf.c_str());
  }

  TCHAR GetAt(int iChar)
  const
  {
    REQUIRE(iChar >= 0);
    REQUIRE(iChar < GetLength()); // TODO: Check do we need this check at runtime?
    return(GetString()[iChar]);
  }

  void SetAt(int iChar, TCHAR ch)
  {
    REQUIRE(iChar >= 0);
    REQUIRE(iChar < GetLength()); // TODO: Check do we need this check at runtime?
    const_cast<TCHAR*>(GetString())[iChar] = ch; // Works like this since we do not share values, unlike MFC
  }

  bool IsEmpty()
  const
  {
    //return(strBuf.empty()); // good looking, but slow
    //Faster implementation:
    const TCHAR *src = GetString();
    if (src[0] == 0) { return(true); }
    return(false);
  }

  // Truncates the string to the new length. // TODO: Check the case when string is already lesser length then nNewLength
  // This does not affect the allocated length of the buffer. To decrease or increase the current buffer, see FreeExtra and Preallocate.
  void Truncate(int nNewLength)
  {
    REQUIRE(nNewLength >= 0);
    if (GetLength() > nNewLength)
    {
      strBuf.resize(nNewLength);
    }
  }

  // Constructors

  CSimpleString()
  {
    // Do defaults
  }

  CSimpleString(const TCHAR *value) : strBuf(value) {}

  #if defined(UNICODE) || defined(_UNICODE)
  CSimpleString(const char *value)
  {
    strBuf = NanoWin::StrConverter::Convert(value);
  }
  #endif

  // Operators

  void operator= (const TCHAR *value)
  {
    strBuf = value;
  }

  operator LPCTSTR ()
  const
  {
    return(strBuf.c_str());
  }

  TCHAR operator[] (int nIndex)
  const
  {
    return(GetAt(nIndex));
  }

  void operator+= (PCXSTR pszSrc)
  {
    REQUIRE(pszSrc != NULL);
    strBuf += pszSrc;
  }

  #undef CHECKUP
  #undef REQUIRE
};

// Class to implement CString subset. [Only null-terminated strings supported]
// Note that for MBCS strings, CString still counts, returns, and manipulates strings based on 8-bit characters,
// and your application must interpret MBCS lead and trail bytes itself
class CString : public CSimpleString
{
  #define REQUIRE(f) if (!(f)) { throw "CString has invalid param:" #f; }
  #define CHECKUP(f) if (!(f)) { throw "CString operation failed:" #f; }

  public:

  // CString subset

  // Constructors
  CString()
  {
    // Do defaults
  }

  CString(const TCHAR *value) : CSimpleString(value) {}

  #if defined(UNICODE) || defined(_UNICODE)
  CString(const char *value)  : CSimpleString(value) {}
  #endif

  // Search

  // The zero-based index of the first character in this CString object that matches the requested substring or characters; -1 if the substring or character is not found.
  int Find (LPCTSTR sub, int startPos = 0)
  const
  {
    std::string::size_type result = strBuf.find(sub, startPos);
    if (result == strBuf.npos)
    {
      return(-1);
    }
    else
    {
      return(result);
    }
  }

  // The zero-based index of the first character in this CString object that matches the requested substring or characters; -1 if the substring or character is not found.
  int Find (TCHAR sch, int startPos = 0)
  const
  {
    std::string::size_type result = strBuf.find(sch, startPos);
    if (result == strBuf.npos)
    {
      return(-1);
    }
    else
    {
      return(result);
    }
  }

  // The index of the last character in this CString object that matches the requested character; –1 if the character is not found.
  int ReverseFind(TCHAR sch)
  const
  {
    std::string::size_type result = strBuf.rfind(sch);
    if (result == strBuf.npos)
    {
      return(-1);
    }
    else
    {
      return(result);
    }
  }

  #if defined(AFX_NW_STR_EXTRA)
  #if defined(UNICODE) || defined(_UNICODE)
  int Find(const CString &sub, int startPos = 0)
  const
  {
    return(Find(sub.GetString(), startPos));
  }
  #endif
  #endif

  // Manipulation

  void FormatV(LPCTSTR lpszFormat, va_list args)
  {
    #if defined(UNICODE) || defined(_UNICODE)
    #define VSNPRINTF  vswprintf // vswnprintf
    #define VSPRINTF_S vswprintf_s
    #else
    #define VSNPRINTF  vsnprintf
    #define VSPRINTF_S vsprintf_s
    #endif

    ssize_t result;
    result = VSNPRINTF(NULL, 0, lpszFormat, args);
    CHECKUP(result >= 0);
    Empty();
    Preallocate(result+1);
    TCHAR *dst = LockBuffer();
    error_t cresult;
    cresult = VSPRINTF_S(dst, result, lpszFormat, args);
    UnlockBuffer();
    CHECKUP(cresult == 0);

    #undef  VSNPRINTF
    #undef  VSPRINTF_S
  }

  void Format(LPCTSTR lpszFormat, ...)
  {
    va_list args;
    va_start (args, lpszFormat);
    FormatV(lpszFormat, args);
    va_end (args);
  }

  // Converts to lowercase
  CString& MakeLower()
  {
    if (!IsEmpty())
    {
      _tcslwr(const_cast<TCHAR*>(strBuf.c_str()));
    }
    return(*this);
  }

  // Converts to Uppercase
  CString& MakeUpper()
  {
    if (!IsEmpty())
    {
      _tcsupr(const_cast<TCHAR*>(strBuf.c_str()));
    }
	return(*this);
  }

  // Reverces characters in a string
  CString& MakeReverse()
  {
    strBuf.reserve();
	return(*this);
  }

  // Extracts last (rightmost) nCount characters and returns a copy of the extracted substring.
  // If nCount exceeds the string length, then the entire string is extracted.
  CString Right(int nCount)
  const // throw(CMemoryException)
  {
    if (IsEmpty())             { return("");    }
    if (nCount <= 0)           { return("");    }
    int len = GetLength();
    if (nCount >= len)         { return(*this); }
    const TCHAR *from = strBuf.c_str();
    from += (len-nCount);
    return(from);
  }

  CString Mid(int nFirst)
  const // throw(CMemoryException)
  {
    if (IsEmpty())             { return("");    }
    if (nFirst <= 0)           { return(*this); }
    int len = GetLength();
    if (nFirst >= len)         { return("");    }
    const TCHAR *from = strBuf.c_str();
    from += nFirst;
    return(from);
  }

  CString Mid(int nFirst, int nCount)
  const // throw(CMemoryException)
  {
    if (nCount <= 0)           { return("");    }
    if (IsEmpty())             { return("");    }
    int len = GetLength();
    if (nFirst >= len)         { return("");    }

    if ((nFirst+nCount) > len)
    {
      nCount = len - nFirst;
    }

    const TCHAR *from = strBuf.c_str();
    from += nFirst;

    // Trick: We can do things this way only because we know internals
    // This is may not work fine with MFC CString
    CString result;
    result.Preallocate(nCount+1);
    TCHAR *to = result.LockBuffer();
    _tcsncpy_s(to, nCount+1, from, nCount); // Should create null terminated strings
    to[nCount] = 0; // Just in case
    result.UnlockBuffer();
    CHECKUP(to[nCount] == 0);
    return(result);
  }

  // Extracts the first (leftmost) nCount characters and returns a copy of the extracted substring.
  // If nCount exceeds the string length, then the entire string is extracted.
  CString Left(int nCount)
  const // throw(CMemoryException)
  {
    return(Mid(0, nCount));
  }

  // Insert src at specified nIndex (zero based). If nIndex >= GetLength(), src will be appended
  // Returns the length of the changed string. 
  int Insert(int nIndex, TCHAR src)
  {
    REQUIRE(src != 0); // Cannot insert null character (we do not support binary strings)
    if (IsEmpty())
    {
      strBuf.append(1, src);
      return(strBuf.length());
    }
    else if (nIndex <= 0)
    {
      strBuf.insert(0, 1, src);
      return(strBuf.length());
    }
    else
    {
      int dlen = strBuf.length();
      if (nIndex >= dlen)
      {
        strBuf.append(1, src);
        dlen++;
      }
      else
      {
        strBuf.insert(nIndex, 1, src);
        dlen++;
      }
      return(dlen);
    }
  }

  // Insert src at specified nIndex (zero based). If nIndex >= GetLength(), src will be appended
  // Returns the length of the changed string. 
  int Insert(int nIndex, LPCTSTR src)
  {
    REQUIRE(src != NULL);
    if (src[0] == 0)
    {
      // src is empty - nothing to do
      return(strBuf.length());
    }
    else if (IsEmpty())
    {
      strBuf.append(src);
      return(strBuf.length());
    }
    else if (nIndex <= 0)
    {
      strBuf.insert(0, src);
      return(strBuf.length());
    }
    else
    {
      int dlen = strBuf.length();
      if (nIndex >= dlen)
      {
        strBuf.append(src);
      }
      else
      {
        strBuf.insert(nIndex, src);
      }
      return(strBuf.length());
    }
  }

  // This method deletes a character or characters from a string starting with the character at nIndex (zero based).
  // If nCount is longer than the string, the remainder of the string is removed.
  int Delete(int nIndex, int nCount = 1)
  {
    if (IsEmpty())
    {
      return(0); // Nothing to do
    }
    else if (nCount <= 0)
    {
      return(strBuf.length()); // Nothing to do
    }
    else if (nIndex <= 0)
    {
      strBuf.erase(0, nCount);
      return(strBuf.length());
    }
    else
    {
      // would throw an exception if nIndex is out of string range (this is aligned with MFC)
      strBuf.erase(nIndex, nCount);
      return(strBuf.length());
    }
  }

  int Replace(TCHAR chOld, TCHAR chNew)
  {
    #if defined(UNICODE) || defined(_UNICODE)
      #define cstring_str_t std::wstring
    #else
      #define cstring_str_t std::string
    #endif

    int replaceCount = 0;

    for (cstring_str_t::size_type pos = strBuf.find(chOld);
         pos != cstring_str_t::npos;
         pos = strBuf.find(chOld,pos + 1))
    {
      strBuf[pos] = chNew;

      replaceCount++;
    }

    #undef cstring_str_t

    return replaceCount;
  }

  int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
  {
    #if defined(UNICODE) || defined(_UNICODE)
      #define cstring_str_t std::wstring
    #else
      #define cstring_str_t std::string
    #endif

    int    replaceCount = 0;
    size_t oldLen       = _tcslen(lpszOld);

    if (lpszNew != NULL)
    {
      size_t newLen = _tcslen(lpszNew);

      for (cstring_str_t::size_type pos = strBuf.find(lpszOld);
           pos != cstring_str_t::npos;
           pos = strBuf.find(lpszOld,pos + newLen))
      {
        strBuf.replace(pos,oldLen,lpszNew);

        replaceCount++;
      }
    }
    else
    {
      for (cstring_str_t::size_type pos = strBuf.find(lpszOld);
           pos != cstring_str_t::npos;
           pos = strBuf.find(lpszOld,pos))
      {
        strBuf.erase(pos,oldLen);

        replaceCount++;
      }
    }

    #undef cstring_str_t

    return replaceCount;
  }

  protected:

  bool IsWhitespace(TCHAR src) { if (src < 0x20) { return(TRUE); } return(FALSE); }

  public:

  CString& TrimLeft()
  {
    const TCHAR *src = strBuf.c_str();
    int   pos = 0;

    while(src[pos] != 0)
    {
      if (IsWhitespace(src[pos]))
      {
        pos++;
      }
      else
      {
        break; // found non-ws
      }
    }

    if (pos <= 0) { return(*this); } // nothing to do

    if (src[pos] == 0) { Empty(); return(*this); } // whole string is ws

    Delete(0, pos); // remove ws
    return(*this);
  }

  CString& TrimRight()
  {
    const TCHAR *src = strBuf.c_str();
    int   slen = strBuf.length();

    if (slen <= 0) { return(*this); } // nothing to do

    int pos = 0;

    while(pos < slen)
    {
      if (IsWhitespace(src[slen-1-pos]))
      {
        pos++;
      }
      else
      {
        break; // found non-ws
      }
    }

    if (pos <= 0) { return(*this); } // nothing to do

    if (pos >= slen) { Empty(); return(*this); } // whole string is ws

    Truncate(slen-pos); // remove ws
    return(*this);
  }

  CString& Trim()
  {
    TrimRight();
    TrimLeft();
    return(*this);
  }

  // Other

  int Compare(PCXSTR psz)
  const
  {
    return(_tcscmp(GetString(), psz));
  }

  int CompareNoCase(PCXSTR psz)
  const
  {
    return(_tcsicmp(GetString(), psz));
  }

  // Operators [bit advanced version]

  const CString& operator= (const TCHAR *value)
  {
    strBuf = value;
    return(*this);
  }

  const CString &operator+= (PCXSTR pszSrc)
  {
    REQUIRE(pszSrc != NULL);
    strBuf += pszSrc;
    return(*this);
  }

  const CString &operator+= (const CString &src)
  {
    strBuf += (PCXSTR)src;
    return(*this);
  }

  const CString& operator+= (TCHAR src)
  {
    AppendChar(src);
    return(*this);
  }

  #undef CHECKUP
  #undef REQUIRE
};

// CString 2 operand operators

inline CString operator+ (const CString &s1, const CString &s2)
{
  CString result = s1;
  result += s2;
  return(result);
}

// CString comparison operators

#define CStringCompareOperatorForTypes(OP, arg1_t, arg2_t)     \
inline BOOL operator OP (arg1_t s1, arg2_t s2)                 \
{                                                              \
  int result = _tcscmp(s1, s2);                                \
  if (result OP 0) { return(TRUE); }                           \
  return(FALSE);                                               \
}

#define CStringCompareOperator(OP) \
        CStringCompareOperatorForTypes(OP, const CString &, const CString &) \
        CStringCompareOperatorForTypes(OP, const CString &, CString::PCXSTR) \
        CStringCompareOperatorForTypes(OP, CString::PCXSTR, const CString &) \

CStringCompareOperator( == )
CStringCompareOperator( != )
CStringCompareOperator( > )
CStringCompareOperator( < )
CStringCompareOperator( >= )
CStringCompareOperator( <= )

// Functions
// ------------------------------------------

extern BOOL AFXAPI AfxExtractSubString(CString& rString, LPCTSTR lpszFullString, int iSubString, TCHAR chSep = '\n');

#if defined(UNICODE) || defined(_UNICODE)
#define CT2A(x) NanoWin::WStrToStrClone(x).c_str()
#define CA2T(x) NanoWin::StrToWStrClone(x).c_str()
#else
#define CT2A(x) (x)
#define CA2T(x) (x)
#endif

#endif // linux
#endif // ...Included
