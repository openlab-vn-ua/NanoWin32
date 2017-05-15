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

#include "NanoWinTCHAR.h"
#include "NanoWinMFCAfx.h"

#if defined(UNICODE) || defined(_UNICODE)
#include <wchar.h>
#include "NanoWinStrConvert.h"
#endif

class CString
{
  #define REQUIRE(f) if (!(f)) { return; }

  protected:
  #if defined(UNICODE) || defined(_UNICODE)
  std::wstring strBuf;
  #else
  std::string  strBuf;
  #endif

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

  // Need some study (do we need this?)
  // static void CopyChars(XCHAR* pchDest, const XCHAR* pchSrc, int nChars) throw(); // XCHAR=TCHAR here
  // static void CopyCharsOverlapped(XCHAR* pchDest, const XCHAR* pchSrc, int nChars) throw(); // XCHAR=TCHAR here

  void Empty(void)
  {
    strBuf.clear();
    strBuf.shrink_to_fit();
  }

  int GetAllocLength()
  const
  {
    return(strBuf.capacity());
  }

  int  GetLength()
  const
  {
    return(strBuf.length());
  }

  // Need some study (do we need this?)
  // PXSTR GetBuffer(int nMinBufferLength);
  // PXSTR GetBufferSetLength(int nLength);
  // PXSTR GetBuffer();
  // void  ReleaseBuffer(int nNewLength = -1);
  // void  ReleaseBufferSetLength(int nNewLength);  
  // PXSTR LockBuffer();
  // void  UnlockBuffer() throw();
  // IAtlStringMgr* GetManager() const throw();
  // void SetManager(IAtlStringMgr* pStringMgr);

  void SetString(const TCHAR *src)
  {
	REQUIRE(src != NULL);
    strBuf = src;
  }

  // Need some study (do we need this?)
  // No more then nLength char + checks if we are in the same buffer as the current string
  // void SetString(PCXSTR pszSrc, int nLength);

  // Need some study (do we need this?)
  //static int StringLength(const TCHAR psz) throw()
  //{
  //  #if defined(UNICODE) || defined(_UNICODE)
  //  return(wsclen(psz));
  //  #else
  //  return(strlen(psz));
  //  #endif
  //}

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
	const_cast<TCHAR*>(GetString())[iChar] = ch;
  }

  bool IsEmpty()
  const
  {
    return(strBuf.empty());
  }

  void Truncate(int nNewLength)
  {
	REQUIRE(nNewLength >= 0);
	if (GetLength() > nNewLength)
	{
	  strBuf.resize(nNewLength);
	}
  }

  void Preallocate(int nLength)
  {
	REQUIRE(nLength >= 0);
	strBuf.reserve(nLength);
  }

  void FreeExtra(void)
  {
    strBuf.shrink_to_fit();
  }

  // CString subset
  // Constructors

  CString(const TCHAR *value) : strBuf(value) {}

  #if defined(UNICODE) || defined(_UNICODE)
  CString(const char *value)
  {
	strBuf = NanoWin::StrConverter::Convert(value);
  }
  #endif

  int Find (LPCTSTR sub, int startPos = 0)
  const
  {
    int result = strBuf.find(sub, startPos);
	if (result == strBuf.npos)
	{
      return(-1);
	}
	else
	{
	  return(result);
	}
  }

  int Find (TCHAR sch, int startPos = 0)
  const
  {
    int result = strBuf.find(sch, startPos);
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

  const CString& operator= (const TCHAR *value)
  {
    strBuf = value;
    return(*this);
  }

  operator LPCTSTR ()
  const
  {
    return(strBuf.c_str());
  }

  TCHAR operator [](int nIndex)
  const
  {
    return(GetAt(nIndex));
  }

  #undef REQUIRE
};

#if defined(UNICODE) || defined(_UNICODE)
#define CT2A(x) NanoWin::WStrToStrClone(x).c_str()
#define CA2T(x) NanoWin::StrToWStrClone(x).c_str()
#else
#define CT2A(x) (x)
#define CA2T(x) (x)
#endif

#endif // linux
#endif // ...Included
