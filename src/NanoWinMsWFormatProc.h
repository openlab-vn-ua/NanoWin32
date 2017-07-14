// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible wprintf/wscanf format processor support
// (fixes differences between processing %s,%c in "wide" printf functions between MSVC and GLIBC)
// This file defines proccessor to update format line (it does not remap functions)

#if !defined(NanoWinMsWFormatProcIncluded)
#define NanoWinMsWFormatProcIncluded

#if defined(__GNUC__)

// Format translation proccessor
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

#include "NanoWinTypes.h"
#include <wchar.h>

// Internal use only

// MS2UNIX wprintf/wscanf format string conversion : number of chars required for srcFormat string to convert to unix
#define          NanoWinMsWFormatProcMs2UnixRequiredLength(srcFormat) (wcslen((srcFormat))+1)

// MS2UNIX wprintf/wscanf format string conversion : convert srcFormat for ms-wprintf to destFormat unix-wprintf
// Note: destFormat must have enough space to store NanoWinMsWFormatProcMs2UnixRequiredLength(srcFormat) wide characters
NW_EXTERN_C void NanoWinMsWFormatProcMs2Unix(wchar_t *destFormat, const wchar_t *srcFormat);

// Format proc functions (now, macros)

#define          NanoWinMsWFormatPrintfProcMs2UnixRequiredLength     NanoWinMsWFormatProcMs2UnixRequiredLength
#define          NanoWinMsWFormatPrintfProcMs2Unix                   NanoWinMsWFormatProcMs2Unix
#define          NanoWinMsWFormatScanfProcMs2UnixRequiredLength      NanoWinMsWFormatProcMs2UnixRequiredLength
#define          NanoWinMsWFormatScanfProcMs2Unix                    NanoWinMsWFormatProcMs2Unix

// Function redefinition tools
// -----------------------------------------
// Used by function remapping modules

#ifdef __cplusplus

#include "NanoWinInternal.h"

namespace NanoWin
{
  class WFormatPrintfLine : public PreAllocatedBuffer<wchar_t>
  {
    public:

    WFormatPrintfLine(const wchar_t *srcFormat) : PreAllocatedBuffer<wchar_t>(NanoWinMsWFormatPrintfProcMs2UnixRequiredLength(srcFormat))
    {
      NanoWinMsWFormatPrintfProcMs2Unix(data(), srcFormat);
    }
  };

  class WFormatScanfLine : public PreAllocatedBuffer<wchar_t>
  {
    public:

    WFormatScanfLine(const wchar_t *srcFormat) : PreAllocatedBuffer<wchar_t>(NanoWinMsWFormatScanfProcMs2UnixRequiredLength(srcFormat))
    {
      NanoWinMsWFormatScanfProcMs2Unix(data(), srcFormat);
    }
  };
}

#endif

// Gates for C compilation

NW_EXTERN_C const wchar_t *NanoWinMsWFormatPrintfProcMs2UnixCFormatGate (const wchar_t *src_format); // Translates src_format using internal buffer, returns pointer to internal buffer
NW_EXTERN_C intptr_t       NanoWinMsWFormatPrintfProcMs2UnixCResultFree (intptr_t       the_result); // Returns the_result and frees internal buffer for ...CFormatGate
NW_EXTERN_C const wchar_t *NanoWinMsWFormatScanfProcMs2UnixCFormatGate  (const wchar_t *src_format); // Translates src_format using internal buffer, returns pointer to internal buffer
NW_EXTERN_C intptr_t       NanoWinMsWFormatScanfProcMs2UnixCResultFree  (intptr_t       the_result); // Returns the_result and frees internal buffer for ...CFormatGate


#ifdef __cplusplus

#define NW_WPRINTF_FORMAT_2_UNIX(format)         NanoWin::WFormatPrintfLine(format).data()
#define NW_WPRINTF_RESULT_2_UNIX(type,r)         r
#define NW_WSCANF_FORMAT_2_UNIX(format)          NanoWin::WFormatScanfLine(format).data()
#define NW_WSCANF_RESULT_2_UNIX(type,r)          r

#else // C

#define NW_WPRINTF_FORMAT_2_UNIX(format)         NanoWinMsWFormatPrintfProcMs2UnixCFormatGate(format)
#define NW_WPRINTF_RESULT_2_UNIX(type,r)         ((type)(NanoWinMsWFormatPrintfProcMs2UnixCResultFree(r)))
#define NW_WSCANF_FORMAT_2_UNIX(format)          NanoWinMsWFormatScanfProcMs2UnixCFormatGate(format)
#define NW_WSCANF_RESULT_2_UNIX(type,r)          ((type)(NanoWinMsWFormatScanfProcMs2UnixCResultFree(r)))

#endif

#endif // GCC

#endif // ...Included
