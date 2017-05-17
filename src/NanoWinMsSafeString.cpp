// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe string extensions

#include "NanoWinMsSafeString.h"

#include <string.h>
#include <wchar.h>
#include <errno.h>

#if !defined(EOK)
#define  EOK  (0)
#endif

#define invoke_err_handler(etext,earg,errcode)
#define return_after_err_handler(etext,earg,errcode) return(errcode)

#define SP ":"
typedef unsigned char XBYTE;

NW_EXTERN_C_BEGIN

// Memory functions
// -----------------------------------------------------

#define RSIZE_GET_CNT(MAX_MEM,ITEM) (sizeof(ITEM) == 1 ? (MAX_MEM) : ((MAX_MEM) / sizeof(ITEM)))
#define MEM_GET_SIZE(count,ITEM)    (sizeof(ITEM) == 1 ? (count) : ((count) * sizeof(ITEM)))

// helper: returns non-zero if dest and src overlaps, zero if not
static int     is_overlap    (void *dest, const void *src, rsize_t count, rsize_t itemsz)
{
  if (count  <= 0) { return(0); }
  if (itemsz <= 0) { return(0); }

  rsize_t csize = count;

  // try to avoid multiplication
  if (csize > 1)
  {
	csize *= itemsz;
  }

  if (src == dest)
  {
    return(1); // equal pointers threated as overlapped
  }
  if (src > dest)
  {
    XBYTE *dest_l = ((XBYTE*)dest)+(csize-1); // last byte of dest
	if (src <= dest_l) { return 1; }
  }
  else if (src < dest)
  {
    XBYTE *src_l = ((XBYTE*)src)+(csize-1); // last byte of src
	if (src_l >= dest) { return -1; }
  }

  return(0);
}

extern errno_t memset_s      (void *dest, rsize_t destsz, int ch, rsize_t count)
{
  #define FN "memset_s"
  #define ITEM XBYTE
  // <body>
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_MEM, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); memset(dest, ch, MEM_GET_SIZE(destsz, ITEM)); return(errcode); }

  if (count  <= 0)             { return(EOK); } // Nothing to do
  // count > 0, something have to be done
  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz less then count"      , NULL, ERANGE); } // core check

  // Action!
  memset(dest, ch, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef ITEM
  #undef FN
}

extern errno_t memcpy_s      (void *dest, rsize_t destsz, const void *src, rsize_t count)
{
  #define FN "memcpy_s"
  #define ITEM XBYTE
  // <body> // invaliant for memcpy_s and wmemcpy_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_MEM, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); memset(dest, 0, MEM_GET_SIZE(destsz, ITEM)); return(errcode); }

  if (count  <= 0)             { return(EOK); } // Nothing to do
  // count > 0, something have to be done
  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz less then count"      , NULL, ERANGE); } // core check

  if (src == dest)             { return(EOK); } // Done :)

  if (is_overlap(dest,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest and src overlap"        , NULL, EINVAL); }

  // Action!
  memcpy(dest, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef ITEM
  #undef FN
}

extern errno_t wmemcpy_s     (wchar_t *dest, rsize_t destsz, const wchar_t *src, rsize_t count)
{
  #define FN "wmemcpy_s"
  #define ITEM wchar_t
  // <body> // invaliant for memcpy_s and wmemcpy_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_MEM, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); memset(dest, 0, MEM_GET_SIZE(destsz, ITEM)); return(errcode); }

  if (count  <= 0)             { return(EOK); } // Nothing to do
  // count > 0, something have to be done
  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz less then count"      , NULL, ERANGE); } // core check

  if (src == dest)             { return(EOK); } // Done :)

  if (is_overlap(dest,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest and src overlap"        , NULL, EINVAL); }

  // Action!
  memcpy(dest, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef ITEM
  #undef FN
}

extern errno_t memmove_s     (void *dest, rsize_t destsz, const void *src, rsize_t count)
{
  #define FN "memmove_s"
  #define ITEM XBYTE
  // <body> // invaliant for memmove_s and wmemmove_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_MEM, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); memset(dest, 0, MEM_GET_SIZE(destsz, ITEM)); return(errcode); }

  if (count  <= 0)             { return(EOK); } // Nothing to do
  // count > 0, something have to be done
  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz less then count"      , NULL, ERANGE); } // core check

  if (src == dest)             { return(EOK); } // Done :)

  // Action!
  memmove(dest, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef ITEM
  #undef FN
}

extern errno_t wmemmove_s    (void *dest, rsize_t destsz, const void *src, rsize_t count)
{
  #define FN "wmemmove_s"
  #define ITEM wchar_t
  // <body> // invaliant for memmove_s and wmemmove_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_MEM, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); memset(dest, 0, MEM_GET_SIZE(destsz, ITEM)); return(errcode); }

  if (count  <= 0)             { return(EOK); } // Nothing to do
  // count > 0, something have to be done
  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz less then count"      , NULL, ERANGE); } // core check

  if (src == dest)             { return(EOK); } // Done :)

  // Action!
  memmove(dest, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef ITEM
  #undef FN
}

// String functions
// -----------------------------------------------------

extern errno_t strcpy_s      (char *dest, rsize_t destsz, const char *src)
{
  #define FN "strcpy_s"
  #define ITEM char
  #define STRNLEN              strnlen
  // <body> // invaliant for strcpy_s and wcscpy_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); dest[0] = 0; return(errcode); }

  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  rsize_t slen = STRNLEN(src, destsz);
  if (slen   <  0)             { return_after_err_FILLDST(FN SP "src len invalid"             , src , EINVAL); }
  if (slen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "src len invalid."            , src , EINVAL); }

  rsize_t count = (slen+1);

  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); } // core check

  if (is_overlap(dest,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest and src overlap"        , NULL, EINVAL); }

  // Action!
  // Use memcpy here, since we already have a count (=len+1) here
  memcpy(dest, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNLEN
  #undef ITEM
  #undef FN
}

extern errno_t wcscpy_s      (wchar_t *dest, rsize_t destsz, const wchar_t *src)
{
  #define FN "wcscpy_s"
  #define ITEM wchar_t
  #define STRNLEN              wcsnlen
  // <body> // invaliant for strcpy_s and wcscpy_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); dest[0] = 0; return(errcode); }

  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  rsize_t slen = STRNLEN(src, destsz);
  if (slen   <  0)             { return_after_err_FILLDST(FN SP "src len invalid"             , src , EINVAL); }
  if (slen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "src len invalid."            , src , EINVAL); }

  rsize_t count = (slen+1);

  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); } // core check

  if (is_overlap(dest,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest and src overlap"        , NULL, EINVAL); }

  // Action!
  // Use memcpy here, since we already have a count (=len+1) here
  memcpy(dest, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNLEN
  #undef ITEM
  #undef FN
}

extern errno_t strcat_s      (char *dest, rsize_t destsz, const char *src)
{
  #define FN "strcat_s"
  #define ITEM char
  #define STRNLEN              strnlen
  // <body> // invaliant for strcat_s and wcscat_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); dest[0] = 0; return(errcode); }

  // MSDN says it would not fill dest if it is not null-terminated, but it looks like typo

  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  rsize_t dlen = STRNLEN(src, destsz);
  if (dlen   <  0)             { return_after_err_FILLDST(FN SP "dest len invalid"            , dest, EINVAL); }
  if (dlen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "dest len invalid."           , dest, EINVAL); }

  if (destsz <  (dlen+1))      { return_after_err_FILLDST(FN SP "dest is not null terminated" , dest, EINVAL); } // core check

  rsize_t slen = STRNLEN(src, destsz);
  if (slen   <  0)             { return_after_err_FILLDST(FN SP "src len invalid"             , src , ERANGE); }
  if (slen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "src len invalid."            , src , ERANGE); }

  if (destsz <  (dlen+slen+1)) { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); } // core check

  rsize_t count = (slen+1);

  if (is_overlap(dest+dlen,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest+ and src overlap"       , NULL, EINVAL); }

  // Action!
  // Use memcpy here, since we already have a count (=len+1) here
  memcpy(dest+dlen, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNLEN
  #undef ITEM
  #undef FN
}

extern errno_t wcscat_s      (wchar_t *dest, rsize_t destsz, const wchar_t *src)
{
  #define FN "wcscat_s"
  #define ITEM char
  #define STRNLEN              wcsnlen
  // <body> // invaliant for strcat_s and wcscat_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); dest[0] = 0; return(errcode); }

  // MSDN says it would not fill dest if it is not null-terminated, but it looks like typo

  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  rsize_t dlen = STRNLEN(src, destsz);
  if (dlen   <  0)             { return_after_err_FILLDST(FN SP "dest len invalid"            , dest, EINVAL); }
  if (dlen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "dest len invalid."           , dest, EINVAL); }

  if (destsz <  (dlen+1))      { return_after_err_FILLDST(FN SP "dest is not null terminated" , dest, EINVAL); } // core check

  rsize_t slen = STRNLEN(src, destsz);
  if (slen   <  0)             { return_after_err_FILLDST(FN SP "src len invalid"             , src , ERANGE); }
  if (slen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "src len invalid."            , src , ERANGE); }

  if (destsz <  (dlen+slen+1)) { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); } // core check

  rsize_t count = (slen+1);

  if (is_overlap(dest+dlen,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest+ and src overlap"       , NULL, EINVAL); }

  // Action!
  // Use memcpy here, since we already have a count (=len+1) here
  memcpy(dest+dlen, src, MEM_GET_SIZE(count, ITEM));
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNLEN
  #undef ITEM
  #undef FN
}

extern errno_t strncpy_s     (char *dest, rsize_t destsz, const char *src, rsize_t count)
{
  #define FN "strncpy_s"
  #define ITEM char
  #define STRNLEN              strnlen
  // <body> // invaliant for strcpy_s and wcscpy_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); dest[0] = 0; return(errcode); }

  #if defined(NW_STR_S_TRUNCATE)
  bool allowtruncate = false;
  bool setnullatcount = false;
  if (count == _TRUNCATE) { allowtruncate = true; count = destsz; }
  #endif

  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (count  <= 0)             { return(EOK); } // Nothing to do // C11 says this is error
  // count > 0, something have to be done
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  rsize_t slen = STRNLEN(src, count);
  if (slen   <  0)             { return_after_err_FILLDST(FN SP "src len invalid"             , src , ERANGE); }
  if (slen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "src len invalid."            , src , ERANGE); }

  if (slen < count)
  {
	// source is null terminated
    // adjust count to copy only slen+1
    count = slen+1;
  }
  else
  {
    // source is not null-terminated and lasts more then count
    // We may copy count items, add set null-terminator at count char (overwrite last byte at target) to truncate
    #if defined(NW_STR_S_TRUNCATE)
    if (allowtruncate)         { setnullatcount = true; }
    else                       { return_after_err_FILLDST(FN SP "src truncate would ocurr"    , src , ERANGE); }
    #else
	// truncate is a C11 error
                               { return_after_err_FILLDST(FN SP "src truncate would ocurr"    , src , ERANGE); }
    #endif
  }

  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); } // core check

  if (is_overlap(dest,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest and src overlap"        , NULL, EINVAL); }

  // Action!
  // Use memcpy here, since we already have a count (=len+1) here
  memcpy(dest, src, MEM_GET_SIZE(count, ITEM));
  #if defined(NW_STR_S_TRUNCATE)
  if (setnullatcount) { dest[count] = 0; return(STRUNCATE); } // set null at count to truncate result
  #endif
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNLEN
  #undef ITEM
  #undef FN
}

extern errno_t wcsncpy_s     (wchar_t *dest, rsize_t destsz, const wchar_t *src, rsize_t count)
{
  #define FN "wscncpy_s"
  #define ITEM wchar_t
  #define STRNLEN              wcsnlen
  // <body> // invaliant for strcpy_s and wcscpy_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); dest[0] = 0; return(errcode); }

  #if defined(NW_STR_S_TRUNCATE)
  bool allowtruncate = false;
  bool setnullatcount = false;
  if (count == _TRUNCATE) { allowtruncate = true; count = destsz; }
  #endif

  if (dest   == NULL)          { return_after_err_handler(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_handler(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_handler(FN SP "destsz too large"            , NULL, ERANGE); }
  // dest valid, now we have to fill dest in case of fail
  if (count  <= 0)             { return(EOK); } // Nothing to do // C11 says this is error
  // count > 0, something have to be done
  if (count  >  RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "count too large"             , NULL, EINVAL); }
  if (src    == NULL)          { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  rsize_t slen = STRNLEN(src, count);
  if (slen   <  0)             { return_after_err_FILLDST(FN SP "src len invalid"             , src , ERANGE); }
  if (slen   >= RSIZE_MAX_CNT) { return_after_err_FILLDST(FN SP "src len invalid."            , src , ERANGE); }

  if (slen < count)
  {
	// source is null terminated
    // adjust count to copy only slen+1
    count = slen+1;
  }
  else
  {
    // source is not null-terminated and lasts more then count
    // We may copy count items, add set null-terminator at count char (overwrite last byte at target) to truncate
    #if defined(NW_STR_S_TRUNCATE)
    if (allowtruncate)         { setnullatcount = true; }
    else                       { return_after_err_FILLDST(FN SP "src truncate would ocurr"    , src , ERANGE); }
    #else
	// truncate is a C11 error
                               { return_after_err_FILLDST(FN SP "src truncate would ocurr"    , src , ERANGE); }
    #endif
  }

  if (destsz <  count)         { return_after_err_FILLDST(FN SP "destsz too small for result" , NULL, ERANGE); } // core check

  if (is_overlap(dest,src,count,sizeof(ITEM)))
                               { return_after_err_FILLDST(FN SP "dest and src overlap"        , NULL, EINVAL); }

  // Action!
  // Use memcpy here, since we already have a count (=len+1) here
  memcpy(dest, src, MEM_GET_SIZE(count, ITEM));
  #if defined(NW_STR_S_TRUNCATE)
  if (setnullatcount) { dest[count] = 0; return(STRUNCATE); } // set null at count to truncate result
  #endif
  return(EOK);

  #undef return_after_err_FILLDST
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNLEN
  #undef ITEM
  #undef FN
}

#define strtok_handle_errcode(errcode) { errno = (errcode); } // Define this as empty if you preffer not to set errno (C11 does not require that)

// Search for tokens in str, acts like strtok_r + quick checks for valid args. Non-C11, non-MS func, but logic extension of strtok_r. Note: Actualy it is how MS strtok_s works
extern char   *strtok_r_s    (char *str, const char *delim, char **context)
{
  #define FN "strtok_r_s"
  #define ITEM char
  #define STRTOK_R strtok_r
  // <body> // invaliant for strtok_r_s and wcstok_r_s
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); strtok_handle_errcode(errcode); return(NULL); }

  if (delim  == NULL)          { return_after_err_WMARKER(FN SP "delim is null"               , NULL, EINVAL); }
  if (context == NULL)         { return_after_err_WMARKER(FN SP "context is null"             , NULL, EINVAL); }

  if (str == NULL)
  {
    // not a first call, (*context) should be valid
    if ((*context) == NULL)    { return_after_err_WMARKER(FN SP "context is invalid"          , NULL, EINVAL); }
  }

  return(STRTOK_R(str, delim, context));

  #undef return_after_err_WMARKER
  // </body>
  #undef STRTOK_R
  #undef ITEM
  #undef FN
}

// Search for tokens in str, acts like wcstok_r + quick checks for valid args. Non-C11, non-MS func, but logic extension of strtok_r. Note: Actualy it is how MS strtok_s works
extern wchar_t*wcstok_r_s    (wchar_t *str, const wchar_t *delim, wchar_t **context)
{
  #define FN "wcstok_r_s"
  #define ITEM char
  #define STRTOK_R wcstok // for wide chars, wcstok is initially acts as reenterant (strtok_r) so there is no wcstok_r
  // <body> // invaliant for strtok_r_s and wcstok_r_s
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); strtok_handle_errcode(errcode); return(NULL); }

  if (delim  == NULL)          { return_after_err_WMARKER(FN SP "delim is null"               , NULL, EINVAL); }
  if (context == NULL)         { return_after_err_WMARKER(FN SP "context is null"             , NULL, EINVAL); }

  if (str == NULL)
  {
    // not a first call, (*context) should be valid
    if ((*context) == NULL)    { return_after_err_WMARKER(FN SP "context is invalid"          , NULL, EINVAL); }
  }

  return(STRTOK_R(str, delim, context));

  #undef return_after_err_WMARKER
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRTOK_R
  #undef ITEM
  #undef FN
}

// String format functions
// -----------------------------------------------------

#include <stdio.h>

#define sprintf_handle_errcode(errcode) { errno = (errcode); } // Define this as empty if you preffer not to set errno (C11 does not require that)

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
  // </body>
  #undef VSNPRINTF
  #undef ITEM
  #undef FN
}

extern int     vwsprintf_s   (wchar_t *dest, rsize_t destsz, const wchar_t *format, va_list args)
{
  #define FN "vwsprintf_s"
  #define ITEM wchar_t
  #define VSNPRINTF vswprintf
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

extern int     wsprintf_s    (wchar_t *dest, rsize_t destsz, const wchar_t *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vwsprintf_s(dest, destsz, format, args);
  va_end (args);
  return(result);
}

NW_EXTERN_C_END

/*
int NanoWinMsSafeStringCompilationTest()
{
  // This should compile
  char  stest[] = "stest";
  char *ptest   =  stest;
  int   ntest   = NanoWinCountOf(stest);
  char  tdest[4];
  char *pdest   = tdest;

  strcpy_s(tdest, stest);
  strcpy_s(tdest, 2, stest);
  strcpy_s(pdest, 2, ptest);

  strcpy_s(tdest, "");
  strcat_s(tdest, stest);
  strcat_s(tdest, 2, stest);
  strcat_s(pdest, 2, ptest);

  strncpy_s(tdest, stest, 1);
  strncpy_s(tdest, 2, stest, 1);
  strncpy_s(pdest, 2, ptest, 1);

  sprintf_s(tdest, _countof(tdest), "just a test %s and here %s", ptest, ptest);
  sprintf_s(tdest, "just a test %s and here %s", ptest, ptest);

  return(ntest);
}
*/
