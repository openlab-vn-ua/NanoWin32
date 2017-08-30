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
#include "NanoWinSystem.h"

//NOTE: only date range which can be represented by UNIX time_t value is supported
//      (for 32-bit Linux system time_t is 32-bit value and will overflow in 2038)
class CTime
{
  public :

                   CTime          () throw();
                   CTime          (const SYSTEMTIME& st, int nDST = - 1) throw();

  static CTime     GetCurrentTime () throw();

  CStringA         Format         (_In_z_ LPCSTR pszFormat) const;
  CStringW         Format         (_In_z_ LPCWSTR pszFormat) const;

  int              GetYear        () const throw();
  int              GetMonth       () const throw();
  int              GetDay         () const throw();
  int              GetHour        () const throw();
  int              GetMinute      () const throw();
  int              GetSecond      () const throw();
  int              GetDayOfWeek   () const throw();

  struct tm*       GetLocalTm     (struct tm* ptm) const;

  private :

  time_t           timeValue;
};

#endif // linux
#endif // ...Included
