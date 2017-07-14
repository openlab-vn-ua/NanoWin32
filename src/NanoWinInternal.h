// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Internal functions

#if !defined(NanoWinInternalIncluded)
#define NanoWinInternalIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <time.h>
#include <stdio.h>

NW_EXTERN_C_BEGIN

#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

extern bool   NanoWinFileNameMatchMaskA(const char *name, const char *mask);
extern bool   NanoWinFileNameMatchMaskW(const wchar_t *name, const wchar_t *mask);

extern void   NanoWinTimeSpecAddTimeoutInMillis (struct timespec *ts, DWORD millis);

extern FILE*  NanoWinFileHandleAsStdioFILE (HANDLE fileHandle);
extern HANDLE NanoWinStdioFileAsFileHandle (FILE *fileStream);

NW_EXTERN_C_END

#ifdef __cplusplus

namespace NanoWin
{
  template<typename T, size_t init_size = 256>
  class PreAllocatedBuffer
  {
    public:

    PreAllocatedBuffer (size_t size)
    {
      init(size);
    }

    void reset (size_t newsize)
    {
      done();
      init(newsize);
    }

    ~PreAllocatedBuffer()
    {
      done();
    }

    T       *data()       { return buffer; }
    const T *data() const { return buffer; }

    private:

    void init(size_t size)
    {
      if (size <= init_size)
      {
        buffer = fixed_buffer;
      }
      else
      {
        buffer = new T[size];
      }
    }

    void done()
    {
      if (buffer != fixed_buffer)
      {
        delete[] buffer;
      }
    }

    T  fixed_buffer[init_size];
    T *buffer;

    private:
    PreAllocatedBuffer (const PreAllocatedBuffer &src) = delete;
    PreAllocatedBuffer (PreAllocatedBuffer &&src) = delete;

    private:
    PreAllocatedBuffer& operator = (const PreAllocatedBuffer &src) = delete;
    PreAllocatedBuffer& operator = (PreAllocatedBuffer &&src) = delete;
  };
}

#endif

#endif // linux
#endif // ...Included
