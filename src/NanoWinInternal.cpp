// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Internal functions

#include "NanoWinInternal.h"

#define NanoWinFileNameMatchMaskMaxRecLevel    (32)
#define NanoWinFileNameMatchMaskSingleWildChar ('?')
#define NanoWinFileNameMatchMaskMultiWildChar  ('*')

#define NanoWinMilliSecondsInSecond     (1000)
#define NanoWinNanoSecondsInMilliSecond (1000000L)
#define NanoWinNanoSecondsInSecond      (NanoWinMilliSecondsInSecond * NanoWinNanoSecondsInMilliSecond)

template<typename C>
static bool NanoWinFileNameMatchMaskPartA(const C *name, const C *mask, unsigned int *recDepth)
{
  (*recDepth)++;

  if (*recDepth > NanoWinFileNameMatchMaskMaxRecLevel)
  {
    (*recDepth)--;

    return false; // Max recursion level reached
  }

  bool matchFlag = false;
  bool checkNextCharFlag = true; // is used to iterate through non-'*' mask characters without recursion

  while (checkNextCharFlag)
  {
    C nameChar = name[0];
    C maskChar = mask[0];

    checkNextCharFlag = false;

    if (maskChar == (C)('\0'))
    {
      matchFlag = nameChar == (C)('\0');
    }
    else if (maskChar == (C)(NanoWinFileNameMatchMaskMultiWildChar))
    {
      int offset = -1;

      do // try every possible offset until either match detection or end of string reached
      {
        offset++;

        matchFlag = NanoWinFileNameMatchMaskPartA(name + offset, mask + 1, recDepth);
      } while (!matchFlag && name[offset] != (C)('\0'));
    }
    else
    {
      if ((maskChar == nameChar) ||
        (maskChar == (C)(NanoWinFileNameMatchMaskSingleWildChar) && nameChar != (C)('\0')))
      {
        checkNextCharFlag = true; // check next chars without recursion

        name++;
        mask++;
      }
    }
  }

  (*recDepth)--;

  return matchFlag;
}

NW_EXTERN_C_BEGIN

extern bool NanoWinFileNameMatchMaskA(const char *name, const char *mask)
{
  unsigned int recDepth = 0;

  return NanoWinFileNameMatchMaskPartA(name, mask, &recDepth);
}

extern bool NanoWinFileNameMatchMaskW(const wchar_t *name, const wchar_t *mask)
{
  unsigned int recDepth = 0;

  return NanoWinFileNameMatchMaskPartA(name, mask, &recDepth);
}

extern void NanoWinTimeSpecAddTimeoutInMillis(struct timespec *ts, DWORD millis)
{
  long seconds     = millis / NanoWinMilliSecondsInSecond;
  long nanoSeconds = ((long)(millis % NanoWinMilliSecondsInSecond)) * NanoWinNanoSecondsInMilliSecond;

  ts->tv_sec  += seconds;
  ts->tv_nsec += nanoSeconds;

  if (ts->tv_nsec >= NanoWinNanoSecondsInSecond)
  {
    ts->tv_sec  += ts->tv_nsec / NanoWinNanoSecondsInSecond;
    ts->tv_nsec %= NanoWinNanoSecondsInSecond;
  }
}

extern FILE*  NanoWinFileHandleAsStdioFILE (HANDLE fileHandle)
{
  return fileHandle == INVALID_HANDLE_VALUE ? NULL : (FILE*)fileHandle;
}

extern HANDLE NanoWinStdioFileAsFileHandle (FILE *fileStream)
{
  return fileStream == NULL ? INVALID_HANDLE_VALUE : (HANDLE)fileStream;
}

NW_EXTERN_C_END
