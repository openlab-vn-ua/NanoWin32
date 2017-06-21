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
  constexpr int initialBufferSize = 255 + 1;
  CString   formattedTime;
  struct tm brokenTime;
  TCHAR     initialBuffer[initialBufferSize];

  if (localtime_r(&timeValue, &brokenTime) == &brokenTime)
  {
    if (format_func(initialBuffer, initialBufferSize, pszFormat, &brokenTime) == 0)
    {
      size_t requiredSize = format_func(NULL,0,pszFormat,&brokenTime) + 1;

      TCHAR *buffer = (TCHAR*)malloc(sizeof(TCHAR) * requiredSize);

      if (buffer == NULL)
      {
        throw new CMemoryException();
      }

      format_func(buffer,requiredSize,pszFormat,&brokenTime);

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
