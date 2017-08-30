// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCTime functions

#include "NanoWinMFCAfxTime.h"

#define NanoWinCTimeYearBase (1900)

CTime::CTime() throw()
{
  timeValue = 0;
}

CTime::CTime(const SYSTEMTIME& st, int nDST) throw()
{
  //NOTE: only date range which can be represented by UNIX time_t value is supported
  struct tm brokenTime;

  brokenTime.tm_sec   = st.wSecond;
  brokenTime.tm_min   = st.wMinute;
  brokenTime.tm_hour  = st.wHour;
  brokenTime.tm_mday  = st.wDay;
  brokenTime.tm_mon   = st.wMonth - 1;
  brokenTime.tm_year  = st.wYear - NanoWinCTimeYearBase;
  brokenTime.tm_isdst = nDST;

  timeValue = mktime(&brokenTime);
}

CTime CTime::GetCurrentTime() throw()
{
  CTime currTime;

  currTime.timeValue = time(NULL);

  return currTime;
}

template<typename S, typename C, int initialBufferSize>
S CTimeFormatHelperFunc (time_t timeValue, const C *pszFormat, size_t (formatFunc) (C*,size_t,const C*, const struct tm*))
{
  S         formattedTime;
  struct tm brokenTime;
  C         initialBuffer[initialBufferSize];

  if (localtime_r(&timeValue, &brokenTime) == &brokenTime)
  {
    if (formatFunc(initialBuffer, initialBufferSize, pszFormat, &brokenTime) == 0)
    {
      size_t requiredSize = formatFunc(NULL,0,pszFormat,&brokenTime) + 1;

      C *buffer = (C*)malloc(sizeof(C) * requiredSize);

      if (buffer == NULL)
      {
        throw new CMemoryException();
      }

      formatFunc(buffer,requiredSize,pszFormat,&brokenTime);

      try
      {
        formattedTime = buffer;
      }
      catch (...)
      {
        free(buffer);

        throw;
      }

      free(buffer);
    }
    else
    {
      formattedTime = initialBuffer;
    }
  }

  return formattedTime;
}

CStringA CTime::Format(_In_z_ LPCSTR pszFormat) const
{
 return CTimeFormatHelperFunc<CStringA,CHAR,255 + 1>(timeValue,pszFormat,strftime);
}

CStringW CTime::Format(_In_z_ LPCWSTR pszFormat) const
{
 return CTimeFormatHelperFunc<CStringW,WCHAR,255 + 1>(timeValue,pszFormat,wcsftime);
}

int CTime::GetYear() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_year + NanoWinCTimeYearBase;
}

int CTime::GetMonth() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_mon + 1;
}

int CTime::GetDay() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_mday;
}

int CTime::GetHour() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_hour;
}

int CTime::GetMinute() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_min;
}

int CTime::GetSecond() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_sec;
}

int CTime::GetDayOfWeek() const throw()
{
  struct tm brokenTime;

  GetLocalTm(&brokenTime);

  return brokenTime.tm_wday + 1;
}

struct tm* CTime::GetLocalTm(struct tm* ptm) const
{
  return localtime_r(&timeValue,ptm);
}
