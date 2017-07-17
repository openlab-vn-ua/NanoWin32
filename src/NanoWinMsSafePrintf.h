// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe vsprintf_s, sprintf_s extensions

#if !defined(NanoWinMsvcSafePrintfIncluded)
#define NanoWinMsvcSafePrintfIncluded

#if defined(__GNUC__)

#include <stdarg.h>

#include "NanoWinTypes.h"
#include "NanoWinMsSafeString.h" // Types

// String format functions (char)
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

// Format output by format and va_args to dest, dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     vsprintf_s    (char *dest, rsize_t destsz, const char *format, va_list args);

// Format output by format and multiple args to dest, dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     sprintf_s     (char *dest, rsize_t destsz, const char *format, ...);

NW_EXTERN_C_END

#ifdef __cplusplus // safe templates

// Format output by format and va_args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
NW_T_STR_DN_S2Param(int,vsprintf_s,const char *, va_list);

// Format output by format and multiple args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
template<typename TD, size_t sizeD>
int sprintf_s(TD (&dest)[sizeD], const char *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vsprintf_s(dest, sizeD, format, args);
  va_end (args);
  return(result);
};

#endif

// String format functions (wchar_t)
// -----------------------------------------------------
// _nt versions uses "native" format string
// _ms versions uses "ms-compatible" format string

NW_EXTERN_C_BEGIN

// Format output by format and va_args to dest, dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     vswprintf_s_nt (wchar_t *dest, rsize_t destsz, const wchar_t *format, va_list args);
extern int     vswprintf_s_ms (wchar_t *dest, rsize_t destsz, const wchar_t *format, va_list args);

// Format output by format and multiple args to dest, dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     swprintf_s_nt  (wchar_t *dest, rsize_t destsz, const wchar_t *format, ...);
extern int     swprintf_s_ms  (wchar_t *dest, rsize_t destsz, const wchar_t *format, ...);

NW_EXTERN_C_END

#ifdef __cplusplus // safe templates

// Format output by format and va_args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
NW_T_STR_DN_S2Param(int,vswprintf_s_nt,const wchar_t *, va_list);
NW_T_STR_DN_S2Param(int,vswprintf_s_ms,const wchar_t *, va_list);

// Format output by format and multiple args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
template<typename TD, size_t sizeD>
int swprintf_s_nt(TD (&dest)[sizeD], const wchar_t *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vswprintf_s_nt(dest, sizeD, format, args);
  va_end (args);
  return(result);
};

template<typename TD, size_t sizeD>
int swprintf_s_ms(TD (&dest)[sizeD], const wchar_t *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vswprintf_s_ms(dest, sizeD, format, args);
  va_end (args);
  return(result);
};

#endif

#if defined(NW_WPRINTF_FORMAT_REMAPPED)
// NanoWin printf format remapper included before this file, define _ms functions as safe wprintf_s
#define vswprintf_s     vswprintf_s_ms
#define swprintf_s      swprintf_s_ms
#else
// No remapping active (yet) define native functions as safe wprintf_s
#define vswprintf_s     vswprintf_s_nt
#define swprintf_s      swprintf_s_nt
#endif

#endif // GCC

#endif // ...Included

