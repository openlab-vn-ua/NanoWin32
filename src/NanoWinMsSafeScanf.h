// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe sscanf_s, fscanf_s extensions

#if !defined(NanoWinMsvcSafeScanfIncluded)
#define NanoWinMsvcSafeScanfIncluded

#if defined(__GNUC__)

#include <NanoWinTypes.h>

NW_EXTERN_C  int sscanf_s(const char *buffer, const char *format, ...);
NW_EXTERN_C  int fscanf_s(FILE       *stream, const char *format, ...);

#endif // GCC

#endif // ...Included

