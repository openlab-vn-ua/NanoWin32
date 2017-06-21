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

CString CTime::Format(_In_z_ LPCTSTR pszFormat) const
{
  #if defined(UNICODE) || defined(_UNICODE)
  #define format_func wcsftime
  #else
  #define format_func strftime
  #endif
  constexpr int bufferStartSize = 255 + 1;
  CString   formattedTime;
  struct tm brokenTime;

  if (localtime_r(&timeValue, &brokenTime) == &brokenTime)
  {
    TCHAR *buffer = formattedTime.GetBufferSetLength(bufferStartSize);

    if (format_func(buffer, bufferStartSize, pszFormat, &brokenTime) == 0)
    {
      buffer[0] = '\0';

      formattedTime.ReleaseBuffer();

      size_t requiredSize = format_func(NULL,0,pszFormat,&brokenTime) + 1;

      buffer = formattedTime.GetBufferSetLength(requiredSize);

      format_func(buffer,requiredSize,pszFormat,&brokenTime);
    }

    formattedTime.ReleaseBuffer();
  }

  return formattedTime;
}
