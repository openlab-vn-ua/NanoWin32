// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe sscanf_s, fscanf_s extensions

#if !defined(NanoWinMsvcSafeScanfIncluded)
#define NanoWinMsvcSafeScanfIncluded

#if defined(__GNUC__)

#include <stdarg.h>

#include <NanoWinTypes.h>

NW_EXTERN_C  int sscanf_s  (const char    *buffer, const char    *format, ...);
NW_EXTERN_C  int vsscanf_s (const char    *buffer, const char    *format, va_list argptr);
NW_EXTERN_C  int fscanf_s  (FILE          *stream, const char    *format, ...);
NW_EXTERN_C  int vfscanf_s (FILE          *stream, const char    *format, va_list argptr);

NW_EXTERN_C  int swscanf_s (const wchar_t *buffer, const wchar_t *format, ...);
NW_EXTERN_C  int vswscanf_s(const wchar_t *buffer, const wchar_t *format, va_list argptr);
NW_EXTERN_C  int fwscanf_s (FILE          *stream, const wchar_t *format, ...);
NW_EXTERN_C  int vfwscanf_s(FILE          *stream, const wchar_t *format, va_list argptr);

#endif // GCC

#endif // ...Included

