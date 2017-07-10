// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCTime functions

#include "NanoWinMFCAfxTime.h"

CTime::CTime() throw()
{
  timeValue = 0;
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
