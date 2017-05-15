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

#if defined(UNICODE) || defined(_UNICODE)
#include "NanoWinStrConvert.h"
#endif

class CString
{
  protected:
  #if defined(UNICODE) || defined(_UNICODE)
  std::wstring str;
  #else
  std::string  str;
  #endif

  public:

  CString(const TCHAR *value) : str(value) {}

  #if defined(UNICODE) || defined(_UNICODE)
  CString(const char *value) {} // TODO: Impement me
  #endif

  int Find (LPCTSTR sub)
  const
  {
    return(-1); // TODO: Impement me
  }

  #if defined(UNICODE) || defined(_UNICODE)
  int Find(const CString &sub)
  const
  {
    return(-1); // TODO: Impement me
  }
  #endif

  const TCHAR *operator= (const TCHAR *value)
  {
    str = value;
    return(value);
  }

  operator LPCTSTR ()
  const
  {
    return(str.c_str());
  }

  #if defined(UNICODE) || defined(_UNICODE)
  const char *operator= (const char *value) {} // TODO: Impement me
  #endif
};

#if defined(UNICODE) || defined(_UNICODE)
#define CT2A(x) NanoWin::WStrToStrClone(x).c_str()
#else
#define CT2A(x) (x)
#endif

#endif // linux
#endif // ...Included
