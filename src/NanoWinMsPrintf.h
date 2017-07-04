// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible printf format support (removes differences between processing
// %s,%c in "wide" printf functions between MSVC and GLIBC)

#if !defined(NanoWinMsPrintfIncluded)
#define NanoWinMsPrintfIncluded

#if defined(__GNUC__)

#include "NanoWinTypes.h"

// destFormat must have enough space to store wcslen(srcFormat) + 1 wide characters
NW_EXTERN_C void NanoWinMsPrintfWFormatMs2Unix (wchar_t *destFormat, const wchar_t *srcFormat);

#endif // GCC

#endif // ...Included

