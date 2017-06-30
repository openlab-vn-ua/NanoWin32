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
  static size_t base_tcslen  (const wchar_t *psz) { return(wcslen(psz)); }
  static size_t base_tcslen  (const char    *psz) { return(strlen(psz)); }

  // _tcsncpy_s
  static void base_tcsncpy_s (wchar_t* pchDest, int nDestMax, const wchar_t* pchSrc, rsize_t nChars) { wcsncpy_s(pchDest, nDestMax, pchSrc, nChars); }
  static void base_tcsncpy_s (char*    pchDest, int nDestMax, const char*    pchSrc, rsize_t nChars) { strncpy_s(pchDest, nDestMax, pchSrc, nChars); }

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

  template<typename XCHAR, typename XSTRING>
  static int base_vsprintf(XSTRING &out, const XCHAR *lpszFormat, va_list args)
  {
    va_list argsCopy;
    va_copy(argsCopy,args);

    int result; // ssize_t result looks logicaly OK, but vsnprintf returns int for some reason
    result = vsnprintf_overload(NULL, 0, lpszFormat, args);
    if (result < 0) { return(result); }
    out.Empty(); // this->Empty();
  	if (result == 0) { return(0); } // Nothing to output and out already clear
    out.Preallocate(result+1); // this->Preallocate(result+1)
    XCHAR *dst = out.LockBuffer(); // this->LockBuffer()
    error_t cresult;
    cresult = vsprintf_s_overload(dst, result + 1, lpszFormat, argsCopy);
    va_end(argsCopy);
    out.UnlockBuffer();
    if (cresult < 0) { result = -1; } // Catch of unlikely error // Note: Out may be overwritten in this point
    return(result);
  }

  static const char    *empty_str;
  static const wchar_t *empty_wstr;

  private :

  static int vsnprintf_overload(char *buffer, size_t count, const char *format, va_list argptr)
  {
    return vsnprintf(buffer,count,format,argptr);
  }

  static int vsnprintf_overload(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr)
  {
    return vsnwprintf(buffer,count,format,argptr);
  }

  static int vsprintf_s_overload(char *buffer, size_t numberOfElements, const char *format, va_list argptr)
  {
    return vsprintf_s(buffer,numberOfElements,format,argptr);
  }

  static int vsprintf_s_overload(wchar_t *buffer, size_t numberOfElements, const wchar_t *format, va_list argptr )
  {
    return vsprintf_s_overload(buffer,numberOfElements,format,argptr);
  }
};

template <typename XCHAR>
struct CStringEmptyStrData
{
  static XCHAR *GetEmptyStr () { return XCHAR::EmptyStr; }
};

template<>
struct CStringEmptyStrData<char>
{
  static const char *GetEmptyStr () { return NanoWinStringUtils::empty_str; }
};

template<>
struct CStringEmptyStrData<wchar_t>
{
  static const wchar_t *GetEmptyStr () { return NanoWinStringUtils::empty_wstr; }
};

template<typename XCHAR>
struct TStringContainer
{
  size_t capacity;
  XCHAR  buffer[0];

  static XCHAR *Allocate (size_t initialCapacity)
  {
    if (initialCapacity > 0)
    {
      TStringContainer<XCHAR> *container = (TStringContainer<XCHAR>*)malloc(sizeof(TStringContainer<XCHAR>) + initialCapacity * sizeof(XCHAR));

      if (container == NULL) throw new CMemoryException();

      container->capacity = initialCapacity;

      return (XCHAR*)&container->buffer;
    }
    else
    {
      return (XCHAR*)GetEmptyStr();
    }
  }

  static XCHAR *AllocateEmpty()
  {
    return Allocate(0);
  }

  static void Free(XCHAR *buffer)
  {
    if (!IsEmptyStr(buffer))
    {
      free(GetContainerPtr(buffer));
    }
  }

  static void ExtendIfNeeded(XCHAR **buffer, size_t newCapacity)
  {
    if (!IsEmptyStr(*buffer))
    {
      TStringContainer<XCHAR> *container = GetContainerPtr(*buffer);

      if (container->capacity < newCapacity)
      {
        ReAllocate(buffer,CalcOptimalCapacity(container->capacity,newCapacity));
      }
    }
    else
    {
      ReAllocate(buffer,CalcOptimalCapacity(0,newCapacity));
    }
  }

  static void Shrink (XCHAR **buffer, size_t newCapacity)
  {
    ReAllocate(buffer,newCapacity);
  }

  static void ShrinkHint(XCHAR **buffer, size_t newCapacity)
  {
    ReAllocate(buffer,newCapacity);
  }

  static void SetToEmptyStr (XCHAR **buffer)
  {
    *buffer = (XCHAR*)GetEmptyStr();
  }

  static void ResetToEmptyStr (XCHAR **buffer)
  {
    Free(*buffer);

    *buffer = (XCHAR*)GetEmptyStr();
  }

  static size_t GetCapacity (const XCHAR *buffer)
  {
    return IsEmptyStr(buffer) ? 0 : GetContainerPtr(buffer)->capacity;
  }

  static void Insert(XCHAR *buffer, size_t startPos, size_t count, const XCHAR *data, size_t tailSize)
  {
    memmove(&buffer[startPos + count],&buffer[startPos],tailSize * sizeof(XCHAR));
    memcpy(&buffer[startPos],data,count * sizeof(XCHAR));
  }

  static void Move (XCHAR *buffer, size_t dstPos, size_t srcPos, size_t count)
  {
    memmove(&buffer[dstPos],&buffer[srcPos],count * sizeof(XCHAR));
  }

  private :

  static size_t GetAlignedBlockSize(size_t blockSize, size_t alignment)
  {
    return ((blockSize + alignment - 1) / alignment) * alignment;
  }

  static size_t CalcOptimalCapacity(size_t oldCapacity, size_t newCapacity)
  {
    if      (newCapacity < oldCapacity)
    {
      if (newCapacity > 0)
      {
        return GetAlignedBlockSize(newCapacity,OPTIMAL_ALLOCATION_BLOCK_SIZE);
      }
      else
      {
        return 0;
      }
    }
    else if (newCapacity > oldCapacity)
    {
      return GetAlignedBlockSize(newCapacity,OPTIMAL_ALLOCATION_BLOCK_SIZE);
    }
    else //if (newCapacity == oldCapacity)
    {
      return newCapacity;
    }
  }

  static TStringContainer<XCHAR> *GetContainerPtr(const XCHAR *buffer)
  {
    return (TStringContainer<XCHAR>*)((char*)buffer - sizeof(TStringContainer<XCHAR>));
  }

  static void ReAllocate(XCHAR **buffer, size_t newCapacity)
  {
    if (!IsEmptyStr(*buffer))
    {
      TStringContainer<XCHAR> *oldContainer = GetContainerPtr(*buffer);

      if (newCapacity > 0)
      {
        if (newCapacity != oldContainer->capacity)
        {
          TStringContainer<XCHAR> *newContainer = (TStringContainer<XCHAR>*)realloc(oldContainer,sizeof(TStringContainer<XCHAR>) + newCapacity * sizeof(XCHAR));

          if (newContainer != NULL)
          {
            newContainer->capacity = newCapacity;

            *buffer = (XCHAR*)&newContainer->buffer;
          }
          else
          {
            throw new CMemoryException();
          }
        }
      }
      else
      {
        free(oldContainer);

        *buffer = (XCHAR*)GetEmptyStr();
      }
    }
    else
    {
      *buffer = Allocate(newCapacity);
    }
  }

  static bool IsEmptyStr (const XCHAR *buffer)
  {
    return buffer == GetEmptyStr();
  }

  static const XCHAR *GetEmptyStr()
  {
    return CStringEmptyStrData<XCHAR>::GetEmptyStr();
  }

  static constexpr size_t OPTIMAL_ALLOCATION_BLOCK_SIZE = 32;
};

// Class to implement CSimpleStringT subset. [Only null-terminated strings supported]
// Note that for MBCS strings, CString still counts, returns, and manipulates strings based on 8-bit characters,
// and your application must interpret MBCS lead and trail bytes itself

// Note:
// Original CString class has all external length/pos interfaces as int
// So, using it does not require any types like size_t.
// On other hand since int is 32 bit on most 64 platforms,
// it leads to 2G implicit limit on string (rande for positive vales for signed int)

template<typename TXCHAR, typename TYCHAR>
class CSimpleStringT
{
  // Note: Current (simple) implemenation not use value sharing optimisation, so all strings has its own independent buffer

  #define REQUIRE(f) if (!(f)) { throw "CSimpleStringT has invalid param:" #f; }
  #define CHECKUP(f) if (!(f)) { throw "CSimpleStringT operation failed:" #f; }

  protected:

  typedef TStringContainer<TXCHAR>       container_type;

  TXCHAR              *buffer;

  public:

  typedef TXCHAR       XCHAR;  // "Native"    char type for string (TCHAR) [char    for CStringA, wchar_t for CStringW]
  typedef TYCHAR       YCHAR;  // "Alternate" char type for string         [wchar_t for CStringA, char    for CStringW]

  typedef XCHAR       *PXSTR;  // TCHAR*
  typedef const XCHAR *PCXSTR; // const TCHAR*
  typedef YCHAR       *PYSTR;  // [Non-native char type] *
  typedef const YCHAR *PCYSTR; // const [Non-native char type] *

  static int  StringLength   (const XCHAR   *psz) { return(static_cast<int>(NanoWinStringUtils::base_tcslen(psz))); }

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

    size_t thisLen = NanoWinStringUtils::base_tcslen(buffer);
    size_t srcLen  = NanoWinStringUtils::base_tcslen(text);

    size_t requiredSize = thisLen + srcLen + 1;

    container_type::ExtendIfNeeded(&buffer,requiredSize);

    memcpy(&buffer[thisLen],text,sizeof(XCHAR) * (srcLen + 1));
   }

  void Append(const XCHAR *text, int nLength)
  {
    REQUIRE(text != NULL);
    REQUIRE(nLength >= 0);

    size_t srcLen = NanoWinStringUtils::base_tcslen(text);

    if (nLength >= srcLen)
    {
      Append(text);
    }
    else
    {
      size_t thisLen      = NanoWinStringUtils::base_tcslen(buffer);
      size_t requiredSize = thisLen + nLength + 1;

      container_type::ExtendIfNeeded(&buffer,requiredSize);

      memcpy(&buffer[thisLen],text,sizeof(XCHAR) * nLength);
      buffer[requiredSize - 1] = '\0';
    }
  }

  void AppendChar(XCHAR item)
  {
    REQUIRE(item != 0); // TODO: Check assumption

    size_t thisLen      = NanoWinStringUtils::base_tcslen(buffer);
    size_t requiredSize = thisLen + 1 + 1;

    container_type::ExtendIfNeeded(&buffer,requiredSize);

    buffer[thisLen]     = item;
    buffer[thisLen + 1] = '\0';
  }

  void Empty(void)
  {
    container_type::ResetToEmptyStr(&buffer);
  }

  const XCHAR *GetString()
  const
  {
    return(buffer);
  }

  int  GetLength()
  const
  {
    return(static_cast<int>(NanoWinStringUtils::base_tcslen(buffer)));
  }

  // Preallocate/FreeExtra/GetAllocLength // TODO: Clarify what length here mean? XCHAR[Length] or XCHAR[Length+1] 

  protected:

  void FreeExtraSoftHint(void)
  {
    size_t thisLen = NanoWinStringUtils::base_tcslen(buffer);

    if (thisLen > 0)
    {
      container_type::ShrinkHint(&buffer,thisLen + 1);
    }
    else
    {
      container_type::ResetToEmptyStr(&buffer);
    }
  }

  public:

  // Allocates a specific amount of bytes for the CSimpleStringT object.
  // nLength is exact size of the CSimpleStringT character buffer in characters
  // analog of XCHAR Preallocate[nLength]
  void Preallocate(int nLength)
  {
    // Actually, some guard +1 space is here (thanks to std::string). BTW CAfxStringMgr::Allocate look like do the same :)

    REQUIRE(nLength >= 0);

    size_t requiredSize = nLength + 1; //FIXME: not sure if +1 is required here

    container_type::ExtendIfNeeded(&buffer,requiredSize);
  }

  void FreeExtra(void)
  {
    size_t thisLen = NanoWinStringUtils::base_tcslen(buffer);

    if (thisLen > 0)
    {
      container_type::Shrink(&buffer,thisLen + 1);
    }
    else
    {
      container_type::ResetToEmptyStr(&buffer);
    }
  }

  int GetAllocLength()
  const
  {
    return(static_cast<int>(container_type::GetCapacity(buffer)));
  }

  // LockBuffer/UnlockBuffer

  // Make this strings buffer unique (that is, not {potentially} shared with other strings with same value)
  // Note: It is good idea to Preallocate(nLength) space in buffer first!
  XCHAR *LockBuffer()
  {
    return(buffer); // Fairly simple, since we do not implement value sharing optimisation
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
    if (src == buffer) { return; } // Copy to myself: nothing to do

    size_t srcLen = NanoWinStringUtils::base_tcslen(src);

    if (srcLen > 0)
    {
      container_type::ExtendIfNeeded(&buffer,srcLen + 1);

      memcpy(buffer,src,sizeof(XCHAR) * (srcLen + 1));
    }
    else
    {
      container_type::ResetToEmptyStr(&buffer);
    }
  }

  // No more then nLength char + checks if we are in the same buffer as the current string
  void SetString(const XCHAR *src, int nLength)
  {
    REQUIRE(src != NULL);
    if (nLength <= 0)          { Empty(); return; }
    int dlen = GetLength();
    const XCHAR *dst = GetString();
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
      buffer[nNewLength] = '\0';
    }
  }

  // Constructors

  protected:
 
  CSimpleStringT(XCHAR value) // Not good, but allow to init CString with ' ' constant. Bad because CString a = 5 works also. [made protected]
  { 
    if (value != 0) // '\0' cannot be appended to acsiiz string
    {
      buffer = container_type::Allocate(2);
      buffer[0] = value;
      buffer[1] = '\0';
    }
    else
    {
      buffer = container_type::AllocateEmpty();
    }
  }

  public:
  CSimpleStringT()
  {
    // Do defaults

    buffer = container_type::AllocateEmpty();
  }

  CSimpleStringT(const XCHAR *value)
  {
    size_t valueLen = NanoWinStringUtils::base_tcslen(value);

    if (valueLen > 0)
    {
      buffer = container_type::Allocate(valueLen + 1);

      memcpy(buffer,value,sizeof(XCHAR) * (valueLen + 1));
    }
    else
    {
      buffer = container_type::AllocateEmpty();
    }
  }

  #if !defined(NW_CSTRING_DISABLE_NARROW_WIDE_CONVERSION) // _CSTRING_DISABLE_NARROW_WIDE_CONVERSION
  CSimpleStringT(const YCHAR *value)
  {
    std::basic_string<XCHAR> tempStr = NanoWin::StrConverter::Convert(value);

    size_t valueLen = tempStr.length();

    if (valueLen > 0)
    {
      buffer = container_type::Allocate(valueLen + 1);

      memcpy(buffer,tempStr.c_str(),sizeof(XCHAR) * (valueLen + 1));
    }
    else
    {
      container_type::AllocateEmpty();
    }
  }
  #endif

  CSimpleStringT(const CSimpleStringT &src)
  {
    size_t srcCapacity = container_type::GetCapacity(src.buffer);

    if (srcCapacity > 0)
    {
      buffer = container_type::Allocate(srcCapacity);

      memcpy(buffer,src.buffer,sizeof(XCHAR) * srcCapacity);
    }
    else
    {
      buffer = container_type::AllocateEmpty();
    }
  }

  // Operators

  void operator= (const XCHAR *value)
  {
    if (value == buffer) return;

    size_t valueLen = NanoWinStringUtils::base_tcslen(value);

    if (valueLen > 0)
    {
      container_type::ExtendIfNeeded(&buffer,valueLen + 1);

      memcpy(buffer,value,sizeof(XCHAR) * (valueLen + 1));
    }
    else
    {
      container_type::ResetToEmptyStr(&buffer);
    }
  }

  void operator= (const CSimpleStringT &src)
  {
    if (&src == this) return;

    size_t srcCapacity = container_type::GetCapacity(src.buffer);

    if (srcCapacity > 0)
    {
      container_type::ExtendIfNeeded(&buffer,srcCapacity);

      memcpy(buffer,src.buffer,sizeof(XCHAR) * srcCapacity);
    }
    else
    {
      container_type::ResetToEmptyStr(&buffer);
    }
  }

  operator PCXSTR ()
  const
  {
    return(GetString());
  }

  XCHAR operator[] (int nIndex)
  const
  {
    return(GetAt(nIndex));
  }

  void operator+= (PCXSTR pszSrc)
  {
    Append(pszSrc);
  }

  #undef CHECKUP
  #undef REQUIRE
};

template <typename TXCHAR>
class CStringCanProvideStrOf
{
  public:
  const TXCHAR *get_c_str()
  const;
};

// Class to implement CString subset. [Only null-terminated strings supported]
// Note that for MBCS strings, CString still counts, returns, and manipulates strings based on 8-bit characters,
// and your application must interpret MBCS lead and trail bytes itself
template<typename TXCHAR, typename TYCHAR>
class CStringT : public CSimpleStringT<TXCHAR, TYCHAR>
{
  #define REQUIRE(f) if (!(f)) { throw "CString has invalid param:" #f; }
  #define CHECKUP(f) if (!(f)) { throw "CString operation failed:" #f; }

  protected:

  typedef CSimpleStringT<TXCHAR, TYCHAR> parent;

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
    size_t currPos = static_cast<size_t>(startPos);
    size_t thisLen = this->GetLength();

    if (currPos >= thisLen) { return -1; }

    bool found = false;

    while (!found && this->buffer[currPos] != '\0')
    {
      size_t offset;

      for (offset = 0; this->buffer[currPos + offset] == sub[offset] && sub[offset] != '\0'; offset++)
      {
      }

      found = sub[offset] == '\0';

      if (!found)
      {
        currPos++;
      }
    }

    if (found)
    {
      return(static_cast<int>(currPos));
    }
    else
    {
      return -1;
    }
  }

  // The zero-based index of the first character in this CString object that matches the requested substring or characters; -1 if the substring or character is not found.
  int Find (XCHAR sch, int startPos = 0)
  const
  {
    size_t currPos = static_cast<size_t>(startPos);
    size_t thisLen = this->GetLength();

    if (currPos >= thisLen) { return -1; }

    bool found = false;

    while (!found && this->buffer[currPos] != '\0')
    {
      found = this->buffer[currPos] == sch;

      currPos++;
    }

    if (found)
    {
      return(static_cast<int>(currPos - 1));
    }
    else
    {
      return -1;
    }
  }

  // The index of the last character in this CString object that matches the requested character; –1 if the character is not found.
  int ReverseFind(XCHAR sch)
  const
  {
    int foundPos = -1;

    for (size_t offset = 0; this->buffer[offset] != '\0'; offset++)
    {
      if (this->buffer[offset] == sch)
      {
        foundPos = static_cast<int>(offset);
      }
    }

    return foundPos;
  }

  // Manipulation

  void FormatV(PCXSTR lpszFormat, va_list args)
  {
    int result = NanoWinStringUtils::base_vsprintf<XCHAR,parent>(*this,lpszFormat,args);

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

  // Format is defined as a template in order to support CString parameters
  // This is not the best solution, but this gived old code chance to work w/o modification
  // Note: according to MSDN you have to cast CString to (LPCTSTR) when passing it to printf-like functions
 
  template<typename T>
  static inline T CStringArgAsFormatArg (T v, typename std::enable_if<!(std::is_base_of<CStringCanProvideStrOf<XCHAR>, T>::value), CStringCanProvideStrOf<XCHAR>>::type *tt = NULL)
  {
    return v;
  }

  template<typename T>
  static inline const XCHAR *CStringArgAsFormatArg(const T &v, typename std::enable_if<std::is_base_of<CStringCanProvideStrOf<XCHAR>, T>::value, CStringCanProvideStrOf<XCHAR>>::type *tt = NULL)
  {
    const CStringCanProvideStrOf<XCHAR> *t = &v; 
    return t->get_c_str();
  }

  static inline const XCHAR *CStringArgAsFormatArg(const CStringT<XCHAR,YCHAR> &v) { return v.GetString(); }
  static inline const XCHAR *CStringArgAsFormatArg(const XCHAR *v) { return v; }

  template<typename... Args>
  inline void Format(PCXSTR lpszFormat, Args&&... args)
  {
    FormatF(lpszFormat,CStringArgAsFormatArg(args)...);
  }

  // Converts to lowercase
  CStringT& MakeLower()
  {
    if (!this->IsEmpty())
    {
      NanoWinStringUtils::base_tcslwr(this->buffer);
    }

    return(*this);
  }

  // Converts to Uppercase
  CStringT& MakeUpper()
  {
    if (!this->IsEmpty())
    {
      NanoWinStringUtils::base_tcsupr(this->buffer);
    }

  	return(*this);
  }

  // Reverces characters in a string
  CStringT& MakeReverse()
  {
    size_t len     = NanoWinStringUtils::base_tcslen(this->buffer);
    size_t halfLen = len / 2;

    for (size_t i = 0; i < halfLen; i++)
    {
      XCHAR tmp = this->buffer[i];

      this->buffer[i]           = this->buffer[len - i - 1];
      this->buffer[len - i - 1] = tmp;
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
    const XCHAR *from = this->buffer;
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
    const XCHAR *from = this->buffer;
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

    const XCHAR *from = this->buffer;
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

    int thisLen = this->GetLength();
    size_t requiredSize = thisLen + 1 + 1;

    parent::container_type::ExtendIfNeeded(&this->buffer,requiredSize);

    if (thisLen == 0)
    {
      this->buffer[0] = src;
      this->buffer[1] = '\0';
    }
    else if (nIndex <= 0)
    {
      parent::container_type::Insert(this->buffer,0,1,&src,thisLen + 1);
    }
    else
    {
      if (nIndex >= thisLen)
      {
        this->buffer[thisLen]     = src;
        this->buffer[thisLen + 1] = '\0';
      }
      else
      {
        parent::container_type::Insert(this->buffer,nIndex,1,&src,thisLen - nIndex + 1);
      }
    }

    return(thisLen + 1);
  }

  // Insert src at specified nIndex (zero based). If nIndex >= GetLength(), src will be appended
  // Returns the length of the changed string. 
  int Insert(int nIndex, LPCTSTR src)
  {
    REQUIRE(src != NULL);
    if (src[0] == 0)
    {
      // src is empty - nothing to do
      return(this->GetLength());
    }
    else if (this->IsEmpty())
    {
      this->SetString(src);
      return(this->GetLength());
    }
    else
    {
      int    thisLen      = this->GetLength();
      int    srcLen       = NanoWinStringUtils::base_tcslen(src);
      size_t requiredSize = thisLen + srcLen + 1;

      parent::container_type::ExtendIfNeeded(&this->buffer,requiredSize);

      if (nIndex <= 0)
      {
        parent::container_type::Insert(this->buffer,0,srcLen,src,thisLen + 1);
      }
      else
      {
        if (nIndex >= thisLen)
        {
          this->Append(src);
        }
        else
        {
          parent::container_type::Insert(this->buffer,nIndex,srcLen,src,thisLen - nIndex + 1);
        }
      }

      return(thisLen + srcLen);
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
      return(this->GetLength()); // Nothing to do
    }
    else if (nIndex <= 0)
    {
      int thisLen = this->GetLength();

      if (nCount > thisLen)
      {
        nCount = thisLen;
      }

      parent::container_type::Move(this->buffer,0,nCount,thisLen - nCount + 1);

      return(thisLen - nCount);
    }
    else
    {
      int thisLen = this->GetLength();

      if (nIndex < thisLen)
      {
        if (nIndex + nCount > thisLen)
        {
          nCount = thisLen - nIndex;
        }

        parent::container_type::Move(this->buffer,nIndex,nIndex + nCount,thisLen - (nIndex + nCount) + 1);
      }
      else
      {
        nCount = 0; // ignore call - startIndex is outside of the string
      }

      return(thisLen - nCount);
    }
  }

  int Replace(XCHAR chOld, XCHAR chNew)
  {
    int replaceCount = 0;

    for (XCHAR *ptr = this->buffer; *ptr != '\0'; ptr++)
    {
      if (*ptr == chOld)
      {
        *ptr = chNew;

        replaceCount++;
      }
    }

    return replaceCount;
  }

  int Replace(PCXSTR lpszOld, PCXSTR lpszNew)
  {
    int    replaceCount = 0;
    size_t oldLen       = this->StringLength(lpszOld);

    if (lpszNew != NULL)
    {
      size_t newLen = this->StringLength(lpszNew);

      if (newLen > oldLen)
      {
        size_t deltaLen = newLen - oldLen;
        size_t thisLen  = this->GetLength();

        for (int currPos = this->Find(lpszOld); currPos >= 0; currPos = this->Find(lpszOld,currPos))
        {
          parent::container_type::ExtendIfNeeded(&this->buffer,thisLen + deltaLen + 1);

          parent::container_type::Move(this->buffer,currPos + oldLen + deltaLen,currPos + oldLen,thisLen - (currPos + oldLen) + 1);

          memcpy(&this->buffer[currPos],lpszNew,newLen * sizeof(XCHAR));

          thisLen += deltaLen;
          currPos += newLen;

          replaceCount++;
        }
      }
      else
      {
        size_t deltaLen = oldLen - newLen;
        size_t thisLen  = this->GetLength();

        for (int currPos = this->Find(lpszOld); currPos >= 0; currPos = this->Find(lpszOld,currPos))
        {
          parent::container_type::Move(this->buffer,currPos + newLen ,currPos + oldLen,thisLen - (currPos + oldLen) + 1);

          memcpy(&this->buffer[currPos],lpszNew,newLen * sizeof(XCHAR));

          thisLen -= deltaLen;
          currPos += newLen;

          replaceCount++;
        }
      }
    }
    else
    {
      for (int currPos = this->Find(lpszOld); currPos >= 0; currPos = this->Find(lpszOld,currPos))
      {
        this->Delete(currPos,oldLen);

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
    const XCHAR *src = this->GetString();
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
    const XCHAR *src = this->GetString();
    int   slen = this->GetLength();

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
    this->SetString(value);
    return(*this);
  }

  const CStringT& operator+= (PCXSTR pszSrc)
  {
    REQUIRE(pszSrc != NULL);
    this->Append(pszSrc);
    return(*this);
  }

  const CStringT& operator+= (const CStringT &src)
  {
    this->Append(src.buffer);
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

typedef CStringT<char, wchar_t> CStringA;
typedef CStringT<wchar_t, char> CStringW;

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

// #define   NW_CX2Y_MACROS_CONST_ONLY // CX2Y macros yields "const X *" output (this is work better, since non-const ptrs from CX2Y are non-writable anyway, but may prevent old code compile)

namespace NanoWin
{
  #if defined(__GNUC__) // conversion operator from non-const char pointer to string types may not work under GCC. This goes not look optimal, but works

  #define DeclConversionToCStringA() \
    operator const std::string ()   const { return(std::string(this->c_str()));  } \
    operator const CStringA ()      const { return(CStringA(this->c_str()));     } \

  #define DeclConversionToCStringW() \
    operator const std::wstring ()  const { return(std::wstring(this->c_str())); } \
    operator const CStringW ()      const { return(CStringW(this->c_str()));     } \

  #else // Good compilator may resolve this via call to constructor with character pointer as argument

  #define DeclConversionToCStringA()
  #define DeclConversionToCStringW()

  #endif

  #define DeclCStringCanProvideStrOf(ctype) \
    const ctype *get_c_cstr()       const { return(this->c_str()); } // CStringCanProvideStrOf<ctype>

  #if defined(NW_CX2Y_MACROS_CONST_ONLY)

  // Non-const output classes are unavailable

  #else

  class WStrToStrCloneInPlace : public WStrToStrClone, public CStringCanProvideStrOf<char>
  {
    public:

    WStrToStrCloneInPlace (const wchar_t *src) : WStrToStrClone(src)  {   }

    // Convert to non-const pointer to target character type. location may not be writable.
    // Note: This is not good, but may be required by some legacy code. 
    operator char* ()               const { return(const_cast<char*>(this->c_str())); }

    DeclCStringCanProvideStrOf(char)
    DeclConversionToCStringA()
  };

  class StrToWStrCloneInPlace : public StrToWStrClone, public CStringCanProvideStrOf<wchar_t>
  {
    public:

    StrToWStrCloneInPlace (const char *src) : StrToWStrClone(src)  {   }

    // Convert to non-const pointer to target character type. location may not be writable.
    // Note: This is not good, but may be required by some legacy code. 
    operator wchar_t* ()            const { return(const_cast<wchar_t*>(this->c_str())); }

    DeclCStringCanProvideStrOf(wchar_t)
    DeclConversionToCStringW()
  };

  #endif

  class WStrToCStrCloneInPlace : public WStrToStrClone, public CStringCanProvideStrOf<char>
  {
    public:

    WStrToCStrCloneInPlace (const wchar_t *src) : WStrToStrClone(src) {   }

    operator const char* ()         const { return(this->c_str()); }

    DeclCStringCanProvideStrOf(char)
    //DeclConversionToCStringA() // not need, since const character ptr works fine
  };

  class StrToWCStrCloneInPlace : public StrToWStrClone, public CStringCanProvideStrOf<wchar_t>
  {
    public:

    StrToWCStrCloneInPlace (const char *src) : StrToWStrClone(src) {   }

    operator const wchar_t* ()      const { return(this->c_str()); }

    DeclCStringCanProvideStrOf(wchar_t)
    //DeclConversionToCStringW() // not need, since const character ptr works fine
  };

  #if defined(NW_CX2Y_MACROS_CONST_ONLY)
  // CXTPtrToXTPtrRef not available
  #else

  template<typename XT>
  class CXTPtrToXTPtrRef : public CStringCanProvideStrOf<XT>
  {
    protected:

    const XT *src;

    public:
    CXTPtrToXTPtrRef (const XT *src)
    {
      this->src = src;
    }

    // Convert to non-const pointer to target character type. location may not be writable.
    // Note: This is not good, but may be required by some legacy code. 
    operator XT* ()        const { return(const_cast<XT*>(this->src)); }

    DeclCStringCanProvideStrOf(XT)
  };

  #endif

  template<typename XT>
  class CXTPtrToCXTPtrRef : public CStringCanProvideStrOf<XT>
  {
    protected:

    const XT *src;

    public:
    CXTPtrToCXTPtrRef (const XT *src)
    {
      this->src = src;
    }

    operator const XT* ()  const { return(this->src);  }

    DeclCStringCanProvideStrOf(XT)
  };
}

#define USES_CONVERSION // Nothing to do here all T2W stuff works automatically

#if defined(NW_CX2Y_MACROS_CONST_ONLY)
#define CW2A     NanoWin::WStrToCStrCloneInPlace
#define CA2W     NanoWin::StrToWCStrCloneInPlace
#else
#define CW2A     NanoWin::WStrToStrCloneInPlace
#define CA2W     NanoWin::StrToWStrCloneInPlace
#endif

#define CW2CA    NanoWin::WStrToCStrCloneInPlace // Not part of ATL/MFC, NW extension
#define CA2CW    NanoWin::StrToWCStrCloneInPlace // Not part of ATL/MFC, NW extension

#if defined(UNICODE) || defined(_UNICODE)
#define CT2A     CW2A
#define CA2T     CA2W
#define CT2CA    CW2CA
#define CA2CT    CA2CW
#if defined(NW_CX2Y_MACROS_CONST_ONLY)
#define CT2W     NanoWin::CXTPtrToCXTPtrRef<wchar_t>
#define CW2T     NanoWin::CXTPtrToCXTPtrRef<TCHAR>
#else
#define CT2W     NanoWin::CXTPtrToXTPtrRef<wchar_t>
#define CW2T     NanoWin::CXTPtrToXTPtrRef<TCHAR>
#endif
#define CT2CW    NanoWin::CXTPtrToCXTPtrRef<wchar_t>
#define CW2CT    NanoWin::CXTPtrToCXTPtrRef<TCHAR>
#else
#if defined(NW_CX2Y_MACROS_CONST_ONLY)
#define CT2A     NanoWin::CXTPtrToCXTPtrRef<char>
#define CA2T     NanoWin::CXTPtrToCXTPtrRef<TCHAR>
#else
#define CT2A     NanoWin::CXTPtrToXTPtrRef<char>
#define CA2T     NanoWin::CXTPtrToXTPtrRef<TCHAR>
#endif
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
