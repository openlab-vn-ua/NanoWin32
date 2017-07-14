// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible wprintf/wcsanf format support
// (removes differences between processing %s,%c in "wide" printf functions between MSVC and GLIBC)
// Include this file in your code if you need ms-compatible wprintf/wscanf functions
// By default, all headers of NanoWin library does not alter operation of wprintf/wscanf functions
// Note: The remapping is not "free" and has some restrictions / edge cases
// 1. You cannot get addr of func anymore
// 2. You cannot name any object with these names (say, you cannot name your method as swprintf)

#if !defined(NanoWinMsPrintfIncluded)
#define NanoWinMsPrintfIncluded

#if !defined(NanoWinForceNativePrintfFormat) // Force avoid remapping of printf functions

#if defined(__GNUC__)

#include "NanoWinTypes.h"
#include "NanoWinMsWFormatProc.h"

#include <wchar.h>

#define NW_WPRINTF_FORMAT_REMAPPED // Flag that remapping is active

#define swprintf(buffer, count, format, ...)          swprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__)
#define vswprintf(buffer, count, format, va_args)     vswprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), va_args)
#define wprintf(format, ...)                          wprintf(NW_FORMAT_2_UNIX(format), __VA_ARGS__)
#define vwprintf(format, va_args)                     vwprintf(NW_FORMAT_2_UNIX(format), va_args)
#define fwprintf(stream, format, ...)                 fwprintf(stream, NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__)
#define vfwprintf(stream, format, va_args)            vfwprintf(stream, NW_WPRINTF_FORMAT_2_UNIX(format), va_args)

#define snwprintf(buffer, count, format, ...)         snwprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__)
#define vsnwprintf(buffer, count, format, va_args)    vsnwprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), va_args)

#endif // GCC

#endif // NanoWinForceNativePrintfFormat

#endif // ...Included

