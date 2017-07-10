// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCTime functions

#if !defined(NanoWinMFCAfxTimeIncluded)
#define NanoWinMFCAfxTimeIncluded

#include <time.h>

#include "NanoWinTypes.h"

#if defined LINUX

#include "NanoWinMFCAfxStr.h"

class CTime
{
  public :

  CTime () throw();

  static CTime WINAPI GetCurrentTime () throw();

  CStringA Format(_In_z_ LPCSTR pszFormat) const;
  CStringW Format(_In_z_ LPCWSTR pszFormat) const;

  private :

  //NOTE: on 32-bit Linux systems time_t is 32-bit signed integer and will overflow in 2038,
  //      it is safe to use it in current implementation for the next 20 years since we only provide
  //      ability to get current time. if additional methods to be implemented (for example constructor
  //      with year,month etc), it will require to change internal date representation

  time_t  timeValue;
};

#endif // linux
#endif // ...Included
