// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible printf format support
// (removes differences between processing %s,%c in "wide" printf functions between MSVC and GLIBC)
// Include this file in your code if you need ms-compatible wprintf functions
// By default, all headers in this library does not alter operation of wprintf functions

#if !defined(NanoWinMsPrintfIncluded)
#define NanoWinMsPrintfIncluded

#if !defined(NanoWinUseNativePrintfFormat)

#if defined(__GNUC__)

#include <stdarg.h>
#include <stdio.h>

#include "NanoWinTypes.h"

// Format translation
// -----------------------------------------

// Format translation between MS and UNIX need only for wide functions like "wprintf/wscanf", ascci version of "printf/scanf" works fine
// wprintf for strings "s/S" (same rules apply for chars "c/C"):
// FormatSpec  ANSI.arg    MS.arg
// %s          char*       wchar_t*  // reverce! (translated by this module)
// %S          wchar_t*    char*     // reverce! (translated by this module)
// %ls         wchar_t*    wchar_t*  // portable
// %lS         wchar_t*    wchar_t*  // portable
// %ws         (non-std)   wchar_t*  // non-portable
// %wS         (non-std)   wchar_t*  // non-portable
// %hs         (non-std)   char*     // non-portable
// %hS         (non-std)   char*     // non-portable

#include <wchar.h>

// MS2UNIX wprintf format string conversion : number of chars required for srcFormat string to convert to unix
#define          NanoWinMsWPrintfFormatMs2UnixRequiredLength(srcFormat) (wcslen((srcFormat))+1)

// MS2UNIX wprintf format string conversion : convert srcFormat for ms-wprintf to destFormat unix-wprintf
// Note: destFormat must have enough space to store NanoWinMsWPrintfFormatMs2UnixRequiredLength(srcFormat) wide characters
NW_EXTERN_C void NanoWinMsWPrintfFormatMs2Unix(wchar_t *destFormat, const wchar_t *srcFormat);

// Function redefinition
// -----------------------------------------

#include "NanoWinInternal.h"

#ifdef __cplusplus

namespace NanoWin
{
  class WPrintfFormatLine : public PreAllocatedBuffer<wchar_t>
  {
    public:

	WPrintfFormatLine(const wchar_t *srcFormat) : PreAllocatedBuffer<wchar_t>(NanoWinMsWPrintfFormatMs2UnixRequiredLength(srcFormat))
	{
      NanoWinMsWPrintfFormatMs2Unix(data(), srcFormat);
	}
  };
}

#define NW_FORMAT_2_UNIX(format)                      NanoWin::WPrintfFormatLine(format).data()

#else

NW_EXTERN_C const wchar_t *NanoWinMsWPrintfFormatMs2UnixCGate (const wchar_t *srcFormat); // Uses internal buffer

#define NW_FORMAT_2_UNIX(format)                      NanoWinMsWPrintfFormatMs2UnixCGate(format)

#endif

#define swprintf(buffer, count, format, ...)          swprintf(buffer, count, NW_FORMAT_2_UNIX(format), ## __VA_ARGS__)
#define vswprintf(buffer, count, format, va_args)     vswprintf(buffer, count, NW_FORMAT_2_UNIX(format), va_args)
#define wprintf(format, ...)                          wprintf(NW_FORMAT_2_UNIX(format), __VA_ARGS__)
#define vwprintf(format, va_args)                     vwprintf(NW_FORMAT_2_UNIX(format), va_args)
#define fwprintf(stream, format, ...)                 fwprintf(stream, NW_FORMAT_2_UNIX(format), ## __VA_ARGS__)
#define vfwprintf(stream, format, va_args)            vfwprintf(stream, NW_FORMAT_2_UNIX(format), va_args)

#define snwprintf(buffer, count, format, ...)         snwprintf(buffer, count, NW_FORMAT_2_UNIX(format), ## __VA_ARGS__)
#define vsnwprintf(buffer, count, format, va_args)    vsnwprintf(buffer, count, NW_FORMAT_2_UNIX(format), va_args)

#endif // GCC

#endif // NanoWinUseNativePrintfFormat

#endif // ...Included

