// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe vsprintf_s, sprintf_s extensions

// Global definitions
// -----------------------------------------------------
// Used by all functions

#include <errno.h>

#include "NanoWinMsSafePrintf.h"

#define invoke_err_handler(etext,earg,errcode) // TODO: call handler here
#define return_after_err_handler(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); return(errcode); }

#define RSIZE_GET_CNT(MAX_MEM,ITEM) (sizeof(ITEM) == 1 ? (MAX_MEM) : ((MAX_MEM) / sizeof(ITEM)))

#define SP ":"

// String format functions (char)
// -----------------------------------------------------

#include <stdio.h> // vsnprintf...

#define sprintf_handle_errcode(errcode) { errno = (errcode); } // Define this as empty if you preffer not to set errno (C11 does not require that)

NW_EXTERN_C_BEGIN

extern int     vsprintf_s    (char *dest, rsize_t destsz, const char *format, va_list args)
{
  #define FN "vsprintf_s"
  #define ITEM char
  #define VSNPRINTF vsnprintf
  // <body> // invaliant for sprintf_s and wsprintf_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); sprintf_handle_errcode(errcode); return(-1); }
  #define return_after_err_FILLDST(etext,earg,errcode) { dest[0] = 0; return_after_err_WMARKER(etext,earg,errcode); }

  if (dest  == NULL)           { return_after_err_WMARKER(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_WMARKER(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_WMARKER(FN SP "destsz too large"            , NULL, ERANGE); }
  // Now we have to fill something at dest, so we clear dest at error
  if (format == NULL)          { return_after_err_FILLDST(FN SP "format is null"              , NULL, EINVAL); }

  int result;
  result = VSNPRINTF(dest, destsz, format, args);

  if (result < 0)              { return_after_err_FILLDST(FN SP "format error detected"       , NULL, EINVAL); }
  // (result >= destsz), string was truncated
  if ((rsize_t)result>=destsz) { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); }

  return(result);

  #undef return_after_err_FILLDST
  #undef return_after_err_WMARKER
  #undef RSIZE_MAX_CNT
  // </body>
  #undef VSNPRINTF
  #undef ITEM
  #undef FN
}

extern int     sprintf_s     (char *dest, rsize_t destsz, const char *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vsprintf_s(dest, destsz, format, args);
  va_end (args);
  return(result);
}

NW_EXTERN_C_END

// String format functions (wchar_t)
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

extern int     vswprintf_s   (wchar_t *dest, rsize_t destsz, const wchar_t *format, va_list args)
{
  #define FN "vwsprintf_s"
  #define ITEM wchar_t
  #define VSNPRINTF vswprintf // strictly speaking vswprintf is not analog of vsnprintf, since vswprintf returns -1 on buffer overflow, but code down here handles that (just another error reported)
  // <body> // invaliant for sprintf_s and wsprintf_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); sprintf_handle_errcode(errcode); return(-1); }
  #define return_after_err_FILLDST(etext,earg,errcode) { dest[0] = 0; return_after_err_WMARKER(etext,earg,errcode); }

  if (dest  == NULL)           { return_after_err_WMARKER(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_WMARKER(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_WMARKER(FN SP "destsz too large"            , NULL, ERANGE); }
  // Now we have to fill something at dest, so we clear dest at error
  if (format == NULL)          { return_after_err_FILLDST(FN SP "format is null"              , NULL, EINVAL); }

  int result;
  result = VSNPRINTF(dest, destsz, format, args);

  if (result < 0)              { return_after_err_FILLDST(FN SP "format error detected"       , NULL, EINVAL); }
  // (result >= destsz), string was truncated
  if ((rsize_t)result>=destsz) { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); }

  return(result);

  #undef return_after_err_FILLDST
  #undef return_after_err_WMARKER
  #undef RSIZE_MAX_CNT
  // </body>
  #undef VSNPRINTF
  #undef ITEM
  #undef FN
}

extern int     swprintf_s    (wchar_t *dest, rsize_t destsz, const wchar_t *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vswprintf_s(dest, destsz, format, args);
  va_end (args);
  return(result);
}

NW_EXTERN_C_END

// Static (compilaton) tests
// -----------------------------------------------------

/*
static int NanoWinMsSafePrintfCompilationTest()
{
  // This should compile
  char  stest[] = "stest";
  char *ptest   =  stest;
  int   ntest   = NanoWinCountOf(stest);
  char  tdest[4];
  char *pdest   = tdest;

  sprintf_s(tdest, _countof(tdest), "just a test %s and here %s %i", ptest, ptest, ntest);
  sprintf_s(tdest, "just a test %s and here %s", ptest, ptest, ntest);

  wchar_t  wstest[] = L"stest";
  wchar_t *wptest   =  wstest;
  int      wntest   = NanoWinCountOf(wstest);
  wchar_t  wtdest[4];
  wchar_t *wpdest   = wtdest;

  swprintf_s(wtdest, _countof(wtdest), L"just a test %s and here %s %i", wptest, wptest, wntest);
  swprintf_s(wtdest, L"just a test %s and here %s", wptest, wptest, wntest);

  return(ntest);
}
*/