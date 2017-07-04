// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible printf format support (removes differences between processing
// %s,%c in "wide" printf functions between MSVC and GLIBC)

#if !defined(NanoWinMsPrintfIncluded)
#define NanoWinMsPrintfIncluded

#if !defined(NanoWinUseNativePrintfFormat)

#if defined(__GNUC__)

#include <stdarg.h>
#include <stdio.h>

#include "NanoWinTypes.h"

// destFormat must have enough space to store wcslen(srcFormat) + 1 wide characters
NW_EXTERN_C void NanoWinMsPrintfWFormatMs2Unix (wchar_t *destFormat, const wchar_t *srcFormat);

NW_EXTERN_C int NanoWinMsSWPrintf  (wchar_t *buffer, size_t count, const wchar_t *format, ...);
NW_EXTERN_C int NanoWinMsVSWPrintf (wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr);

NW_EXTERN_C int NanoWinMsWPrintf   (const wchar_t *format, ...);
NW_EXTERN_C int NanoWinMsVWPrintf  (const wchar_t *format, va_list argptr);

NW_EXTERN_C int NanoWinMsFWPrintf  (FILE *stream, const wchar_t *format, ...);
NW_EXTERN_C int NanoWinMsVFWPrintf (FILE *stream, const wchar_t *format, va_list argptr);

#define swprintf  NanoWinMsSWPrintf
#define vswprintf NanoWinMsVSWPrintf
#define wprintf   NanoWinMsWPrintf
#define vwprintf  NanoWinMsVWPrintf
#define fwprintf  NanoWinMsFWPrintf
#define vfwprintf NanoWinMsVFWPrintf

#endif // GCC

#endif // NanoWinUseNativePrintfFormat

#endif // ...Included

