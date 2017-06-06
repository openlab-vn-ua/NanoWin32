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

#include <wchar.h>
#include "NanoWinStrConvert.h"

class NanoWinStringUtils
{
  public:
  // _tcslen
  static int  base_tcslen    (const wchar_t *psz) { return(wcslen(psz)); }
  static int  base_tcslen    (const char    *psz) { return(strlen(psz)); }

  // _tcsncpy_s
  static void base_tcsncpy_s (wchar_t* pchDest, int nDestMax, const wchar_t* pchSrc, int nChars) { wcsncpy_s(pchDest, nDestMax, pchSrc, nChars); }
  static void base_tcsncpy_s (char*    pchDest, int nDestMax, const char*    pchSrc, int nChars) { strncpy_s(pchDest, nDestMax, pchSrc, nChars); }

  // _tcslwr
  static void base_tcslwr    (wchar_t* pchDest) { wcslwr(pchDest); }
  static void base_tcslwr    (char*    pchDest) { strlwr(pchDest); }

  // _tcsups
  static void base_tcsupr    (wchar_t* pchDest) { wcsupr(pchDest); }
  static void base_tcsupr    (char*    pchDest) { strupr(pchDest); }

  // _tcscmp
  static int  base_tcscmp    (const char*    s1, const char*    s2) { return(strcmp(s1, s2)); }
  static int  base_tcscmp    (const wchar_t* s1, const wchar_t* s2) { return(wcscmp(s1, s2)); }
  static int  base_tcsicmp   (const char*    s1, const char*    s2) { return(stricmp(s1, s2)); }
  static int  base_tcsicmp   (const wchar_t* s1, const wchar_t* s2) { return(wcsicmp(s1, s2)); }

  #define VSNPRINTF  vsnprintf
  #define VSPRINTF_S vsprintf_s
  #define XCHAR      char
  #define STDSTRING  std::string

  static int base_vsprintf(STDSTRING &out, const XCHAR *lpszFormat, va_list args)
  {
    // Body invariant to char/wchar
    va_list argsCopy;
    va_copy(argsCopy,args);

    ssize_t result;
    result = VSNPRINTF(NULL, 0, lpszFormat, args);
    if (result < 0) { return(result); }
    out.clear(); // this->Empty();
	if (result == 0) { return(0); } // Nothing to output and out already clear
    out.reserve(result+1); // this->Preallocate(result+1)
    XCHAR *dst = const_cast<XCHAR*>(out.c_str()); // this->LockBuffer()
    error_t cresult;
    cresult = VSPRINTF_S(dst, result + 1, lpszFormat, argsCopy);
    va_end(argsCopy);
    //this->UnlockBuffer();
    if (cresult < 0) { result = -1; } // Catch of unlikely error // Note: Out may be overwritten in this point
	return(result);
  }

  #undef  VSNPRINTF
  #undef  VSPRINTF_S
  #undef  XCHAR
  #undef  STDSTRING

  #define VSNPRINTF  vswprintf
  #define VSPRINTF_S vswprintf_s
  #define XCHAR      wchar_t
  #define STDSTRING  std::wstring

  static int base_vsprintf(STDSTRING &out, const XCHAR *lpszFormat, va_list args)
  {
    // Body invariant to char/wchar
    va_list argsCopy;
    va_copy(argsCopy,args);

    ssize_t result;
    result = VSNPRINTF(NULL, 0, lpszFormat, args);
    if (result < 0) { return(result); }
    out.clear(); // this->Empty();
	if (result == 0) { return(0); } // Nothing to output and out already clear
    out.reserve(result+1); // this->Preallocate(result+1)
    XCHAR *dst = const_cast<XCHAR*>(out.c_str()); // this->LockBuffer()
    error_t cresult;
    cresult = VSPRINTF_S(dst, result + 1, lpszFormat, argsCopy);
    va_end(argsCopy);
    //this->UnlockBuffer();
    if (cresult < 0) { result = -1; } // Catch of unlikely error // Note: Out may be overwritten in this point
	return(result);
  }

  #undef  VSNPRINTF
  #undef  VSPRINTF_S
  #undef  XCHAR
  #undef  STDSTRING
};

// Class to implement CSimpleStringT subset. [Only null-terminated strings supported]
// Note that for MBCS strings, CString still counts, returns, and manipulates strings based on 8-bit characters,
// and your application must interpret MBCS lead and trail bytes itself

template<typename TXCHAR, typename TYCHAR, class TStringBuf>
class CSimpleStringT
{
  // Note: Current (simple) implemenation not use value sharing optimisation, so all strings has its own independent buffer

  #define REQUIRE(f) if (!(f)) { throw "CSimpleStringT has invalid param:" #f; }
  #define CHECKUP(f) if (!(f)) { throw "CSimpleStringT operation failed:" #f; }

  protected:

  typedef TStringBuf                     strBuf_type;
  typedef typename TStringBuf::size_type strBuf_size_type;

  TStringBuf           strBuf;

  public:

  typedef TXCHAR       XCHAR;  // "Native"    char type for string (TCHAR) [char    for CStringA, wchar_t for CStringW]
  typedef TYCHAR       YCHAR;  // "Alternate" char type for string         [wchar_t for CStringA, char    for CStringW]

  typedef XCHAR       *PXSTR;  // TCHAR*
  typedef const XCHAR *PCXSTR; // const TCHAR*
  typedef YCHAR       *PYSTR;  // [Non-native char type] *
  typedef const YCHAR *PCYSTR; // const [Non-native char type] *

  static int  StringLength   (const XCHAR   *psz) { return(NanoWinStringUtils::base_tcslen(psz)); }

  // Need some study (do we need this?)
  // static void CopyChars(XCHAR* pchDest, const XCHAR* pchSrc, int nChars) throw(); // XCHAR=XCHAR here
  // static void CopyCharsOverlapped(XCHAR* pchDest, const XCHAR* pchSrc, int nChars) throw(); // XCHAR=XCHAR here

  public:

  // CSimpleStringT

  // Need some study (do we need this?)
  // void Append(const CSimpleStringT& strSrc);

  void Append(const XCHAR *text)
  {
    REQUIRE(text != NULL);
    strBuf.append(text);
  }

  void Append(const XCHAR *text, int nLength)
  {
    REQUIRE(text != NULL);
    REQUIRE(nLength >= 0);
    strBuf.append(text, nLength);
  }

  void AppendChar(XCHAR item)
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

  // Preallocate/FreeExtra/GetAllocLength // TODO: Clarify what length here mean? XCHAR[Length] or XCHAR[Length+1] 

  protected:

  void FreeExtraSoftHint(void)
  {
    strBuf.reserve(0); // Implemenation-defined hint to free extra mem
  }

  public:

  // Allocates a specific amount of bytes for the CSimpleStringT object.
  // nLength is exact size of the CSimpleStringT character buffer in characters
  // analog of XCHAR Preallocate[nLength]
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
  XCHAR *LockBuffer()
  {
    return(const_cast<XCHAR *>(strBuf.c_str())); // Fairly simple, since we do not implement value sharing optimisation
  }

  // Unlock buffer previously locked by LockBuffer()
  void  UnlockBuffer() // throw()
  {
    // Fairly simple, since we do not implement value sharing optimisation
  }

  // GetBuffer/ReleaseBuffer // TODO: Clarify what length here mean? XCHAR[Length] or XCHAR[Length+1] 

  // Returns a pointer to the internal character buffer for the CSimpleStringT object.
  // nMinBufferLength is a min count the characters buffer can hold. This value does not include space for a null terminator. "XCHAR GetBuffer[nMinBufferLength]"
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

  void SetString(const XCHAR *src)
  {
    REQUIRE(src != NULL);
    if (src == strBuf.c_str()) { return; } // Copy to myself: nothing to do
    strBuf = src;
  }

  // No more then nLength char + checks if we are in the same buffer as the current string
  void SetString(const XCHAR *src, int nLength)
  {
    REQUIRE(src != NULL);
    if (nLength <= 0)          { Empty(); return; }
    int dlen = strBuf.length();
    const XCHAR *dst = strBuf.c_str();
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
      XCHAR *dst = LockBuffer();
      NanoWinStringUtils::base_tcsncpy_s(dst, clen+1, src, clen); // Should create null terminated strings
      UnlockBuffer();
    }
    else
    {
      // overlap
      int clen = nLength;
      if (clen > slen) { clen = slen; }
      Preallocate(clen+1);
      XCHAR *dst = LockBuffer();
      memmove_s(dst,clen*sizeof(*src),src,clen*sizeof(*src));
      dst[clen] = 0; // Null terminate
      UnlockBuffer();
    }
  }

  const XCHAR *GetString()
  const
  {
    return(strBuf.c_str());
  }

  XCHAR GetAt(int iChar)
  const
  {
    REQUIRE(iChar >= 0);
    REQUIRE(iChar < GetLength()); // TODO: Check do we need this check at runtime?
    return(GetString()[iChar]);
  }

  void SetAt(int iChar, XCHAR ch)
  {
    REQUIRE(iChar >= 0);
    REQUIRE(iChar < GetLength()); // TODO: Check do we need this check at runtime?
    const_cast<XCHAR*>(GetString())[iChar] = ch; // Works like this since we do not share values, unlike MFC
  }

  bool IsEmpty()
  const
  {
    //return(strBuf.empty()); // good looking, but slow
    //Faster implementation:
    const XCHAR *src = GetString();
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

  protected:
 
  CSimpleStringT(XCHAR value) // Not good, but allow to init CString with ' ' constant. Bad because CString a = 5 works also. [made protected]
  { 
    if (value != 0) // '\0' cannot be appended to acsiiz string
    {
      strBuf.push_back(value);
    }
  }

  public:
  CSimpleStringT()
  {
    // Do defaults
  }

  CSimpleStringT(const XCHAR *value) : strBuf(value) {}

  #if !defined(NW_CSTRING_DISABLE_NARROW_WIDE_CONVERSION) // _CSTRING_DISABLE_NARROW_WIDE_CONVERSION
  CSimpleStringT(const YCHAR *value)
  {
    strBuf = NanoWin::StrConverter::Convert(value);
  }
  #endif

  // Operators

  void operator= (const XCHAR *value)
  {
    strBuf = value;
  }

  operator PCXSTR ()
  const
  {
    return(strBuf.c_str());
  }

  XCHAR operator[] (int nIndex)
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
template<typename TXCHAR, typename TYCHAR, class TStringBuf>
class CStringT : public CSimpleStringT<TXCHAR, TYCHAR, TStringBuf>
{
  #define REQUIRE(f) if (!(f)) { throw "CString has invalid param:" #f; }
  #define CHECKUP(f) if (!(f)) { throw "CString operation failed:" #f; }

  protected:

  typedef CSimpleStringT<TXCHAR, TYCHAR, TStringBuf> parent;

  typedef typename parent::strBuf_type       strBuf_type;
  typedef typename parent::strBuf_size_type  strBuf_size_type;

  public:

  typedef typename parent::XCHAR  XCHAR;
  typedef typename parent::YCHAR  YCHAR;

  typedef typename parent::PXSTR  PXSTR;
  typedef typename parent::PCXSTR PCXSTR;
  typedef typename parent::PYSTR  PYSTR;
  typedef typename parent::PCYSTR PCYSTR;

  // CString subset

  // Constructors
  CStringT()
  {
    // Do defaults
  }

  CStringT(XCHAR value) : parent(value) {} // Not good, but allow to init CString with ' ' constant. Bad because CString a = 5 works also.
  CStringT(const XCHAR *value) : parent(value) {}

  #if !defined(NW_CSTRING_DISABLE_NARROW_WIDE_CONVERSION) // _CSTRING_DISABLE_NARROW_WIDE_CONVERSION
  CStringT(const YCHAR *value) : parent(value) {}
  #endif

  // Search

  // The zero-based index of the first character in this CString object that matches the requested substring or characters; -1 if the substring or character is not found.
  int Find (PCXSTR sub, int startPos = 0)
  const
  {
    strBuf_size_type result = this->strBuf.find(sub, startPos);
    if (result == strBuf_type::npos)
    {
      return(-1);
    }
    else
    {
      return(result);
    }
  }

  // The zero-based index of the first character in this CString object that matches the requested substring or characters; -1 if the substring or character is not found.
  int Find (XCHAR sch, int startPos = 0)
  const
  {
    strBuf_size_type result = this->strBuf.find(sch, startPos);
    if (result == strBuf_type::npos)
    {
      return(-1);
    }
    else
    {
      return(result);
    }
  }

  // The index of the last character in this CString object that matches the requested character; –1 if the character is not found.
  int ReverseFind(XCHAR sch)
  const
  {
    strBuf_size_type result = this->strBuf.rfind(sch);
    if (result == strBuf_type::npos)
    {
      return(-1);
    }
    else
    {
      return(result);
    }
  }

  // Manipulation

  void FormatV(LPCTSTR lpszFormat, va_list args)
  {
    int result = NanoWinStringUtils::base_vsprintf(this->strBuf, lpszFormat, args);
	CHECKUP(result >= 0);
  }

  protected:

  void FormatF(PCXSTR lpszFormat, ...)
  {
    va_list args;
    va_start (args, lpszFormat);
    FormatV(lpszFormat, args);
    va_end (args);
  }

  public:

  void Format(PCXSTR lpszFormat, ...)
  {
    va_list args;
    va_start (args, lpszFormat);
    FormatV(lpszFormat, args);
    va_end (args);
  }

  // Call gates to support up to 5 CString parameters
  void Format(PCXSTR lpszFormat, PCXSTR arg1) { this->FormatF(lpszFormat, arg1); }
  void Format(PCXSTR lpszFormat, PCXSTR arg1, PCXSTR arg2) { this->FormatF(lpszFormat, arg1, arg2); }
  void Format(PCXSTR lpszFormat, PCXSTR arg1, PCXSTR arg2, PCXSTR arg3) { this->FormatF(lpszFormat, arg1, arg2, arg3); }
  void Format(PCXSTR lpszFormat, PCXSTR arg1, PCXSTR arg2, PCXSTR arg3, PCXSTR arg4) { this->FormatF(lpszFormat, arg1, arg2, arg3, arg4); }
  void Format(PCXSTR lpszFormat, PCXSTR arg1, PCXSTR arg2, PCXSTR arg3, PCXSTR arg4, PCXSTR arg5) { this->FormatF(lpszFormat, arg1, arg2, arg3, arg4, arg5);  }

  // Converts to lowercase
  CStringT& MakeLower()
  {
    if (!this->IsEmpty())
    {
      NanoWinStringUtils::base_tcslwr(const_cast<XCHAR*>(this->strBuf.c_str()));
    }
    return(*this);
  }

  // Converts to Uppercase
  CStringT& MakeUpper()
  {
    if (!this->IsEmpty())
    {
      NanoWinStringUtils::base_tcsupr(const_cast<XCHAR*>(this->strBuf.c_str()));
    }
	return(*this);
  }

  // Reverces characters in a string
  CStringT& MakeReverse()
  {
    strBuf_size_type len     = this->strBuf.length();
    strBuf_size_type halfLen = len / 2;

    for (strBuf_size_type i = 0; i < halfLen; i++)
    {
      XCHAR tmp = this->strBuf[i];

      this->strBuf[i]           = this->strBuf[len - i - 1];
      this->strBuf[len - i - 1] = tmp;
    }

	return(*this);
  }

  // Extracts last (rightmost) nCount characters and returns a copy of the extracted substring.
  // If nCount exceeds the string length, then the entire string is extracted.
  CStringT Right(int nCount)
  const // throw(CMemoryException)
  {
    if (this->IsEmpty())       { return("");    }
    if (nCount <= 0)           { return("");    }
    int len = this->GetLength();
    if (nCount >= len)         { return(*this); }
    const XCHAR *from = this->strBuf.c_str();
    from += (len-nCount);
    return(from);
  }

  CStringT Mid(int nFirst)
  const // throw(CMemoryException)
  {
    if (this->IsEmpty())       { return("");    }
    if (nFirst <= 0)           { return(*this); }
    int len = this->GetLength();
    if (nFirst >= len)         { return("");    }
    const XCHAR *from = this->strBuf.c_str();
    from += nFirst;
    return(from);
  }

  CStringT Mid(int nFirst, int nCount)
  const // throw(CMemoryException)
  {
    if (nCount <= 0)           { return("");    }
    if (this->IsEmpty())       { return("");    }
    int len = this->GetLength();
    if (nFirst >= len)         { return("");    }

    if ((nFirst+nCount) > len)
    {
      nCount = len - nFirst;
    }

    const XCHAR *from = this->strBuf.c_str();
    from += nFirst;

    // Trick: We can do things this way only because we know internals
    // This is may not work fine with MFC CString
    CStringT result;
    result.Preallocate(nCount+1);
    XCHAR *to = result.LockBuffer();
    NanoWinStringUtils::base_tcsncpy_s(to, nCount+1, from, nCount); // Should create null terminated strings
    to[nCount] = 0; // Just in case
    result.UnlockBuffer();
    CHECKUP(to[nCount] == 0);
    return(result);
  }

  // Extracts the first (leftmost) nCount characters and returns a copy of the extracted substring.
  // If nCount exceeds the string length, then the entire string is extracted.
  CStringT Left(int nCount)
  const // throw(CMemoryException)
  {
    return(Mid(0, nCount));
  }

  // Insert src at specified nIndex (zero based). If nIndex >= GetLength(), src will be appended
  // Returns the length of the changed string. 
  int Insert(int nIndex, XCHAR src)
  {
    REQUIRE(src != 0); // Cannot insert null character (we do not support binary strings)
    if (this->IsEmpty())
    {
      this->strBuf.append(1, src);
      return(this->strBuf.length());
    }
    else if (nIndex <= 0)
    {
      this->strBuf.insert(0, 1, src);
      return(this->strBuf.length());
    }
    else
    {
      int dlen = this->strBuf.length();
      if (nIndex >= dlen)
      {
        this->strBuf.append(1, src);
        dlen++;
      }
      else
      {
        this->strBuf.insert(nIndex, 1, src);
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
      return(this->strBuf.length());
    }
    else if (this->IsEmpty())
    {
      this->strBuf.append(src);
      return(this->strBuf.length());
    }
    else if (nIndex <= 0)
    {
      this->strBuf.insert(0, src);
      return(this->strBuf.length());
    }
    else
    {
      int dlen = this->strBuf.length();
      if (nIndex >= dlen)
      {
        this->strBuf.append(src);
      }
      else
      {
        this->strBuf.insert(nIndex, src);
      }
      return(this->strBuf.length());
    }
  }

  // This method deletes a character or characters from a string starting with the character at nIndex (zero based).
  // If nCount is longer than the string, the remainder of the string is removed.
  int Delete(int nIndex, int nCount = 1)
  {
    if (this->IsEmpty())
    {
      return(0); // Nothing to do
    }
    else if (nCount <= 0)
    {
      return(this->strBuf.length()); // Nothing to do
    }
    else if (nIndex <= 0)
    {
      this->strBuf.erase(0, nCount);
      return(this->strBuf.length());
    }
    else
    {
      // would throw an exception if nIndex is out of string range (this is aligned with MFC)
      this->strBuf.erase(nIndex, nCount);
      return(this->strBuf.length());
    }
  }

  int Replace(XCHAR chOld, XCHAR chNew)
  {
    int replaceCount = 0;

    for (strBuf_size_type pos = this->strBuf.find(chOld);
         pos != strBuf_type::npos;
         pos = this->strBuf.find(chOld,pos + 1))
    {
      this->strBuf[pos] = chNew;

      replaceCount++;
    }

    return replaceCount;
  }

  int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
  {
    int    replaceCount = 0;
    size_t oldLen       = this->StringLength(lpszOld);

    if (lpszNew != NULL)
    {
      size_t newLen = this->StringLength(lpszNew);

      for (strBuf_size_type pos = this->strBuf.find(lpszOld);
           pos != strBuf_type::npos;
           pos = this->strBuf.find(lpszOld,pos + newLen))
      {
        this->strBuf.replace(pos,oldLen,lpszNew);

        replaceCount++;
      }
    }
    else
    {
      for (strBuf_size_type pos = this->strBuf.find(lpszOld);
           pos != strBuf_type::npos;
           pos = this->strBuf.find(lpszOld,pos))
      {
        this->strBuf.erase(pos,oldLen);

        replaceCount++;
      }
    }

    return replaceCount;
  }

  protected:

  static bool IsWhitespace(XCHAR src, PCXSTR delims = NULL)
  {
    if (delims == NULL)
    {
      if (src <= 0x20) { return(TRUE); }
      return(FALSE);
    }
    else if (delims[0] == 0)
    {
      return(FALSE);
    }
    else if (delims[1] == 0)
    {
      if (delims[0] == src) { return(TRUE); }
      return(FALSE);
    }
    else // delims contains mutiple delimiters
    {
      while(delims[0] != 0)
      {
        if (delims[0] == src) { return(TRUE); }
        delims++;
      }
      return(FALSE);
    }
  }

  public:

  int Remove(XCHAR ch)
  {
    CStringT sch;
    sch.AppendChar(ch);
    return(this->Replace(sch.GetString(), _T("")));
  }

  void TrimLeft(_In_ PCXSTR delims)
  {
    const XCHAR *src = this->strBuf.c_str();
    int   pos = 0;

    while(src[pos] != 0)
    {
      if (this->IsWhitespace(src[pos], delims))
      {
        pos++;
      }
      else
      {
        break; // found non-ws
      }
    }

    if (pos <= 0) { return; } // nothing to do

    if (src[pos] == 0) { this->Empty(); return; } // whole string is ws

    this->Delete(0, pos); // remove ws
  }

  void TrimLeft(_In_ XCHAR chTarget)
  {
    XCHAR delims[2];
    delims[0] = chTarget; delims[1] = 0;
    TrimLeft(delims);
  }

  void TrimLeft()
  {
    TrimLeft((PCXSTR)NULL);
  }

  void TrimRight(_In_ PCXSTR delims)
  {
    const XCHAR *src = this->strBuf.c_str();
    int   slen = this->strBuf.length();

    if (slen <= 0) { return; } // nothing to do

    int pos = 0;

    while(pos < slen)
    {
      if (this->IsWhitespace(src[slen-1-pos], delims))
      {
        pos++;
      }
      else
      {
        break; // found non-ws
      }
    }

    if (pos <= 0) { return; } // nothing to do

    if (pos >= slen) { this->Empty(); return; } // whole string is ws

    this->Truncate(slen-pos); // remove ws
  }

  void TrimRight(_In_ XCHAR chTarget)
  {
    XCHAR delims[2];
    delims[0] = chTarget; delims[1] = 0;
    TrimRight(delims);
  }

  void TrimRight()
  {
    TrimRight((PCXSTR)NULL);
  }

  void Trim(_In_ PCXSTR delims)
  {
    this->TrimRight(delims);
    this->TrimLeft(delims);
  }

  void Trim(_In_ XCHAR chTarget)
  {
    this->TrimRight(chTarget);
    this->TrimLeft(chTarget);
  }

  void Trim()
  {
    this->TrimRight();
    this->TrimLeft();
  }

  // Other

  int Compare(PCXSTR psz)
  const
  {
    return(NanoWinStringUtils::base_tcscmp(this->GetString(), psz));
  }

  int CompareNoCase(PCXSTR psz)
  const
  {
    return(NanoWinStringUtils::base_tcsicmp(this->GetString(), psz));
  }

  // Operators [bit advanced version]

  const CStringT& operator= (const XCHAR *value)
  {
    this->strBuf = value;
    return(*this);
  }

  const CStringT& operator+= (PCXSTR pszSrc)
  {
    REQUIRE(pszSrc != NULL);
    this->strBuf += pszSrc;
    return(*this);
  }

  const CStringT& operator+= (const CStringT &src)
  {
    this->strBuf += (PCXSTR)src;
    return(*this);
  }

  const CStringT& operator+= (XCHAR src)
  {
    this->AppendChar(src);
    return(*this);
  }

  XCHAR operator[] (int nIndex)
  const
  {
    return(this->GetAt(nIndex));
  }

  #undef CHECKUP
  #undef REQUIRE
};

typedef CStringT<char, wchar_t, std::string>  CStringA;
typedef CStringT<wchar_t, char, std::wstring> CStringW;

// CString 2 operand operators

inline CStringA operator+ (const CStringA &s1, const CStringA &s2)
 {
   CStringA result = s1;
   result += s2;
   return(result);
 }

inline CStringW operator+ (const CStringW &s1, const CStringW &s2)
 {
   CStringW result = s1;
   result += s2;
   return(result);
 }

inline CStringA operator+ (const CStringA &s1, const char s2)
 {
   CStringA result = s1;
   result += s2;
   return(result);
 }

inline CStringW operator+ (const CStringW &s1, const wchar_t s2)
 {
   CStringW result = s1;
   result += s2;
   return(result);
 }

// CString comparison operators

#define CStringCompareOperatorForTypes(CString, OP, arg1_t, arg2_t)  \
inline BOOL operator OP (arg1_t s1, arg2_t s2)                       \
{                                                                    \
  int result = NanoWinStringUtils::base_tcscmp(s1, s2);                  \
  if (result OP 0) { return(TRUE); }                                 \
  return(FALSE);                                                     \
}

#if !defined(NW_CSTRING_DISABLE_NARROW_WIDE_CONVERSION) // _CSTRING_DISABLE_NARROW_WIDE_CONVERSION

#define CStringCompareOperatorForYCHAR(CString, OP)                       \
inline BOOL operator OP (const CString &s1, const CString::YCHAR *s2)     \
{                                                                         \
  CString s2x(s2);                                                        \
  return(s1 OP s2x);                                                      \
}                                                                         \
                                                                          \
inline BOOL operator OP (const CString::YCHAR *s1, const CString &s2)     \
{                                                                         \
	CString s1x(s1);                                                      \
	return(s1x OP s2);                                                    \
}                                                                         

#endif 

#if !defined(CStringCompareOperatorForYCHAR)
#define CStringCompareOperatorForYCHAR(CString, OP) // Nothing
#endif

#define CStringCompareOperator(CString, OP) \
        CStringCompareOperatorForTypes(CString, OP, const CString &, const CString &) \
        CStringCompareOperatorForTypes(CString, OP, const CString &, CString::PCXSTR) \
        CStringCompareOperatorForTypes(CString, OP, CString::PCXSTR, const CString &) \
        CStringCompareOperatorForYCHAR(CString, OP)                                   \

#define CStringCompareAll(CString) \
        CStringCompareOperator(CString, == ) \
        CStringCompareOperator(CString, != ) \
        CStringCompareOperator(CString, > ) \
        CStringCompareOperator(CString, < ) \
        CStringCompareOperator(CString, >= ) \
        CStringCompareOperator(CString, <= ) \

CStringCompareAll(CStringA);
CStringCompareAll(CStringW);

// Some tricks (define back-alias in reverce way to give some code chance to compile)

#if defined(UNICODE) || defined(_UNICODE)
#define CString      CStringW
#else
#define CString      CStringA
#endif

// Functions
// ------------------------------------------

extern BOOL AFXAPI AfxExtractSubString(CString& rString, LPCTSTR lpszFullString, int iSubString, TCHAR chSep = '\n');

// In-Place converters
// ------------------------------------------

#include "NanoWinStrConvert.h"

namespace NanoWin
{
  class WStrToStrCloneInPlace : public WStrToStrClone
  {
    public:
    WStrToStrCloneInPlace (const wchar_t *src) : WStrToStrClone(src)
    {
    }

    operator char* ()
    const // This is not good, but just to make stuff compile where const passed as source
    {
      return(const_cast<char*>(this->c_str()));
    }

	#if defined(__GNUC__)

	// conversion operator from const char* may not work under GCC. This goes not look optimal, but works

	operator const std::string ()
    const
	{
      return(std::string(this->c_str()));
	}

	operator const CStringA ()
    const
	{
      return(CStringA(this->c_str()));
	}

	#endif
  };

  class StrToWStrCloneInPlace : public StrToWStrClone
  {
    public:
    StrToWStrCloneInPlace (const char *src) : StrToWStrClone(src)
    {
    }

    operator wchar_t* ()
    const // This is not good, but just to make stuff compile where const passed as source
    {
      return(const_cast<wchar_t*>(this->c_str()));
    }

	#if defined(__GNUC__)

	// conversion operator from const char* may not work under GCC. This goes not look optimal, but works

	operator const std::wstring ()
    const
	{
      return(std::wstring(this->c_str()));
	}

	operator const CStringW ()
    const
	{
      return(CStringW(this->c_str()));
	}

	#endif
  };

  class WStrToCStrCloneInPlace : public WStrToStrClone
  {
    public:
    WStrToCStrCloneInPlace (const wchar_t *src) : WStrToStrClone(src)
    {
    }

    operator const char* ()
    const
    {
      return(this->c_str());
    }
  };

  class StrToWCStrCloneInPlace : public StrToWStrClone
  {
    public:
    StrToWCStrCloneInPlace (const char *src) : StrToWStrClone(src)
    {
    }

    operator const wchar_t* ()
    const
    {
      return(this->c_str());
    }
  };

  template<typename XT>
  class CXTPtrToXTPtrRef
  {
    protected:

    const XT *src;

    public:
    CXTPtrToXTPtrRef (const XT *src)
    {
      this->src = src;
    }

    operator XT* ()
    const // This is not good, but just to make stuff compile where const passed as source
    {
      return(const_cast<XT*>(this->src));
    }
  };

  template<typename XT>
  class CXTPtrToCXTPtrRef
  {
    protected:

    const XT *src;

    public:
    CXTPtrToCXTPtrRef (const XT *src)
    {
      this->src = src;
    }

    operator const XT* ()
    const
    {
      return(this->src);
    }
  };
}

#define USES_CONVERSION // Nothing to do here all T2W stuff works automatically

#define CW2A     NanoWin::WStrToStrCloneInPlace
#define CA2W     NanoWin::StrToWStrCloneInPlace
#define CW2CA    NanoWin::WStrToCStrCloneInPlace // Not part of ATL/MFC, NW extension
#define CA2CW    NanoWin::StrToWCStrCloneInPlace // Not part of ATL/MFC, NW extension

#if defined(UNICODE) || defined(_UNICODE)
#define CT2A     CW2A
#define CA2T     CA2W
#define CT2CA    CW2CA
#define CA2CT    CA2CW
#define CT2W     NanoWin::CXTPtrToXTPtrRef<wchar_t>
#define CW2T     NanoWin::CXTPtrToXTPtrRef<TCHAR>
#define CT2CW    NanoWin::CXTPtrToCXTPtrRef<wchar_t>
#define CW2CT    NanoWin::CXTPtrToCXTPtrRef<TCHAR>
#else
#define CT2A     NanoWin::CXTPtrToXTPtrRef<char>
#define CA2T     NanoWin::CXTPtrToXTPtrRef<TCHAR>
#define CT2CA    NanoWin::CXTPtrToCXTPtrRef<char>
#define CA2CT    NanoWin::CXTPtrToCXTPtrRef<TCHAR>
#define CT2W     CA2W
#define CW2T     CW2A
#define CT2CW    CA2CW
#define CW2CT    CW2CA
#endif

// Compatibility macroses (without lead C)
// [Obsolete, since name mislead: actually args must be const and result change not propagated back]

#define W2A      CW2A
#define A2W      CA2W
#define W2CA     CW2CA
#define A2CW     CA2CW
#define T2A      CT2A
#define A2T      CA2T
#define T2CA     CT2CA
#define A2CT     CA2CT
#define T2W      CT2W
#define W2T      CW2T
#define T2CW     CT2CW
#define W2CT     CW2CT

#endif // linux
#endif // ...Included
