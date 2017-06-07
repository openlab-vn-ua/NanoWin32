// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe string extensions

#if !defined(NanoWinMsvcSafeStringIncluded)
#define NanoWinMsvcSafeStringIncluded

#if defined(__GNUC__)

#if !defined(_countof)
#define _countof(arr)		(sizeof(arr)/sizeof(arr[0]))
#endif

#if !defined(NW_ERRNO_T_DEFINED)
#define NW_ERRNO_T_DEFINED
typedef int errno_t;		// Not defined by GNU C (standard assumed errno is int always)
#endif

#include <string.h>
#include <stdarg.h>

#ifndef NW_EXTERN_C_BEGIN
#ifdef __cplusplus
#define NW_EXTERN_C_BEGIN  extern "C" {
#define NW_EXTERN_C_END    }
#else
#define NW_EXTERN_C_BEGIN  // None
#define NW_EXTERN_C_END    // None
#endif
#endif

#if !defined(RSIZE_MAX)

typedef size_t rsize_t;

#ifndef RSIZE_MAX_MEM
#define RSIZE_MAX_MEM (0x03FFFFFF)      // "reasonable" max for memory operations: 64M
#endif

#ifndef RSIZE_MAX_STR
#define RSIZE_MAX_STR (256*1024)        // "reasonable" max for string operations: 256K
#endif

#define RSIZE_MAX     (((RSIZE_MAX_MEM) > (RSIZE_MAX_STR)) ? (RSIZE_MAX_MEM) : (RSIZE_MAX_STR))

#else

#ifndef RSIZE_MAX_MEM
#define RSIZE_MAX_MEM RSIZE_MAX         // "reasonable" max for memory operations: RSIZE_MAX
#endif

#ifndef RSIZE_MAX_STR
#define RSIZE_MAX_STR (RSIZE_MAX/2)     // "reasonable" max for string operations: RSIZE_MAX/2
#endif

#endif


#ifndef NW_NO_MS_STR_S_TRUNCATE // Support _TRUNCATE semanic a-la MS VisualC [define this to turn support off]
#define NW_STR_S_TRUNCATE     // define this if you need MS-compatible _TRUNCATE semantic to be supported in module body
#define _TRUNCATE (RSIZE_MAX) // Value to pass to strncpy_s and alike to allow return truncated results as-is // (Relay fact that on RSIZE_MAX anyway is quit hight)
#define STRUNCATE (-11111)    // error_t value to return in case of truncation occurs
#endif

// NanoWin add-ons
// -----------------------------------------------------

#ifdef __cplusplus

// Returns count of items in array
template<typename T1, size_t size1>
size_t NanoWinCountOf(T1(&)[size1])
{ 
  return(size1);
};

#endif

// Macroses to convert safe string to safe templates
// -----------------------------------------------------

#ifdef __cplusplus

// Templates for funcs that return specific O_t

#define NW_T_STR_DN_S0Param(O_t,strfunc_s)                           \
template<typename TD, size_t sizeD>                                  \
O_t strfunc_s(TD (&dest)[sizeD])                                     \
{                                                                    \
  return(strfunc_s(dest, sizeD));                                    \
};

#define NW_T_STR_DN_S1Param(O_t,strfunc_s,arg1_t)                    \
template<typename TD, size_t sizeD>                                  \
O_t strfunc_s(TD (&dest)[sizeD], arg1_t arg1)                        \
{                                                                    \
  return(strfunc_s(dest, sizeD, arg1));                              \
};

#define NW_T_STR_DN_S2Param(O_t,strfunc_s,arg1_t,arg2_t)             \
template<typename TD, size_t sizeD>                                  \
O_t strfunc_s(TD (&dest)[sizeD], arg1_t arg1, arg2_t arg2)           \
{                                                                    \
  return(strfunc_s(dest, sizeD, arg1, arg2));                        \
};

#define NW_T_STR_DN_S3Param(O_t,strfunc_s,arg1_t,arg2_t,arg3_t)           \
template<typename TD, size_t sizeD>                                       \
O_t strfunc_s(TD (&dest)[sizeD], arg1_t arg1, arg2_t arg2, arg3_t arg3)   \
{                                                                         \
  return(strfunc_s(dest, sizeD, arg1, arg2, arg3));                       \
};

#define NW_T_STR_DN_S4Param(O_t,strfunc_s,arg1_t,arg2_t,arg3_t,arg4_t)                   \
template<typename TD, size_t sizeD>                                                      \
O_t strfunc_s(TD (&dest)[sizeD], arg1_t arg1, arg2_t arg2, arg3_t arg3, arg4_t arg4)     \
{                                                                                        \
  return(strfunc_s(dest, sizeD, arg1, arg2, arg3, arg4));                                \
};


// Templates for funcs that return errno_t

#define NW_STR_DN0Param(strfunc_s)                                   NW_T_STR_DN_S0Param(errno_t,strfunc_s)
#define NW_STR_DN1Param(strfunc_s,arg1_t)                            NW_T_STR_DN_S1Param(errno_t,strfunc_s,arg1_t)
#define NW_STR_DN2Param(strfunc_s,arg1_t,arg2_t)                     NW_T_STR_DN_S2Param(errno_t,strfunc_s,arg1_t,arg2_t)

#endif

// Memory functions
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

// Fills count items(bytes) with ch(lower byte) to dest, dest size is limited to destsz items(bytes). returns errno_t (0 if ok)
extern errno_t memset_s      (void *dest, rsize_t destsz, int ch, rsize_t count);

// Copy count items(bytes) of src to dest, src and dest must not overlap, dest size is limited to destsz items(bytes). returns errno_t (0 if ok)
extern errno_t memcpy_s      (void *dest, rsize_t destsz, const void *src, rsize_t count);

// Copy count items(wchar_t) of src to dest, src and dest must not overlap, dest size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wmemcpy_s     (wchar_t *dest, rsize_t destsz, const wchar_t *src, rsize_t count);

// Copy count items(bytes) of src to dest, guaranteeing correct behavior for overlapping memeory, dest size is limited to destsz items(bytes). returns errno_t (0 if ok)
extern errno_t memmove_s     (void *dest, rsize_t destsz, const void *src, rsize_t count);

// Copy count items(wchar_t) of src to dest, guaranteeing correct behavior for overlapping memory, dest size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wmemmove_s    (void *dest, rsize_t destsz, const void *src, rsize_t count);

NW_EXTERN_C_END

// String functions
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

// Copy src string to dest, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
extern errno_t strcpy_s      (char *dest, rsize_t destsz, const char *src);

// Copy src string to dest, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wcscpy_s      (wchar_t *dest, rsize_t destsz, const wchar_t *src);

// Append src string to dest, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
extern errno_t strcat_s      (char *dest, rsize_t destsz, const char *src);

// Append src string to dest, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wcscat_s      (wchar_t *dest, rsize_t destsz, const wchar_t *src);

// Copy src string (up to count chars from src) to dest, dest will be null treminated, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
extern errno_t strncpy_s     (char *dest, rsize_t destsz, const char *src, rsize_t count);

// Copy src string (up to count chars from src) to dest, dest will be null treminated, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wcsncpy_s     (wchar_t *dest, rsize_t destsz, const wchar_t *src, rsize_t count);

// Search for tokens in str, acts like strtok_r + quick checks for valid args. Non-C11, non-MS func, but logic extension of strtok_r. Note: Actualy it is how MS strtok_s works
extern char   *strtok_r_s    (char *str, const char *delim, char **context);

// Search for tokens in str, acts like wcstok(_r) + quick checks for valid args. Non-C11, non-MS func, but logic extension of strtok_r. Note: Actualy it is how MS strtok_s works
extern wchar_t*wcstok_r_s    (wchar_t *str, const wchar_t *delim, wchar_t **context);

#ifndef NW_NO_MS_ISO_ALIASES // MS-style aliases for "obsolete/nonconformant" func
#define _strtok_r_s          strtok_r_s // strtok_r_s consformant alias [NanoWin]
#define _wcstok_r_s          wcstok_r_s // wcstok_r_s consformant alias [NanoWin]
#endif

NW_EXTERN_C_END

// Special case for strtok_s/wcstok_s (attempt to achieve C11 compilance via c++ overload)

#if !defined(__cplusplus)

// for MS VC compatibility, we define aliases that takes MS VC style prototype, not C11 prototype

// Search for tokens in str, acts like strtok_r + quick checks for valid args. [MS VC specific, not C11]
#define        strtok_s      strtok_r_s 

// Search for tokens in str, acts like strtok_r + quick checks for valid args. [MS VC specific, not C11]
#define        wcstok_s      wcstok_r_s

#else

// Define line MS VC style prototypes (this will get a chance to someone to overload strtok_s and wcstok_s in a C11 compilant way, so both co-exist)

// Search for tokens in str, acts like strtok_r + quick checks for valid args. [MS VC specific, not C11]
inline char   *strtok_s      (char *str, const char *delim, char **context) { return(strtok_r_s(str, delim, context)); }

// Search for tokens in str, acts like wcstok(_r) + quick checks for valid args. [MS VC specific, not C11]
inline wchar_t*wcstok_s      (wchar_t *str, const wchar_t *delim, wchar_t **context) { return(wcstok_r_s(str, delim, context)); }

#endif

#ifdef __cplusplus // safe templates

// Copy src string to dest[destsz], dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
NW_STR_DN1Param(strcpy_s,const char *);

// Copy src string to dest[destsz], dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
NW_STR_DN1Param(wcscpy_s,const wchar_t *);

// Append src string to dest[destsz], dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
NW_STR_DN1Param(strcat_s,const char *);

// Append src string to dest[destsz], dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
NW_STR_DN1Param(wcscat_s,const wchar_t *);

// Copy src string (up to count chars from src) to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
NW_STR_DN2Param(strncpy_s,const char *, rsize_t);

// Copy src string (up to count chars from src) to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
NW_STR_DN2Param(wcsncpy_s,const wchar_t *, rsize_t);

#endif

// String format functions
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

// Format output by format and va_args to dest, dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     vsprintf_s    (char *dest, rsize_t destsz, const char *format, va_list args);

// Format output by format and va_args to dest, dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     vswprintf_s   (wchar_t *dest, rsize_t destsz, const wchar_t *format, va_list args);

// Format output by format and multiple args to dest, dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     sprintf_s     (char *dest, rsize_t destsz, const char *format, ...);

// Format output by format and multiple args to dest, dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
extern int     swprintf_s    (wchar_t *dest, rsize_t destsz, const wchar_t *format, ...);

NW_EXTERN_C_END

#ifdef __cplusplus // safe templates

// Format output by format and va_args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(chars/bytes)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
NW_T_STR_DN_S2Param(int,vsprintf_s,const char *, va_list);

// Format output by format and va_args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
NW_T_STR_DN_S2Param(int,vswprintf_s,const wchar_t *, va_list);

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

// Format output by format and multiple args to dest[destsz], dest will be null treminated, dest memory size is limited to destsz items(wchar_t)
// [MS VC specific, not C11]. returns -1 on error (target length on ok)
template<typename TD, size_t sizeD>
int swprintf_s(TD (&dest)[sizeD], const wchar_t *format, ...)
{
  int result;
  va_list args;
  va_start (args, format);
  result = vswprintf_s(dest, sizeD, format, args);
  va_end (args);
  return(result);
};

#endif

// MS Extensions
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

// Converts string to uppercase at dest, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
extern errno_t strupr_s      (char *dest, rsize_t destsz);

// Converts string to lowercase at dest, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
extern errno_t strlwr_s      (char *dest, rsize_t destsz);

// Converts string to uppercase at dest, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wcsupr_s      (wchar_t *dest, rsize_t destsz);

// Converts string to lowercase at dest, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern errno_t wcslwr_s      (wchar_t *dest, rsize_t destsz);

#ifndef NW_NO_MS_ISO_ALIASES // MS-style aliases for "obsolete/nonconformant" func
#define _strupr_s            strupr_s
#define _wcsupr_s            wcsupr_s
#define _strlwr_s            strlwr_s
#define _wcslwr_s            wcslwr_s
#endif

NW_EXTERN_C_END

#ifdef __cplusplus // safe templates

// Converts string to uppercase at dest[destsz], dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
NW_STR_DN0Param(strupr_s);

// Converts string to lowercase at dest[destsz], dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
NW_STR_DN0Param(strlwr_s);

// Converts string to uppercase at dest[destsz], dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
NW_STR_DN0Param(wcsupr_s);

// Converts string to lowercase at dest[destsz], dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
NW_STR_DN0Param(wcslwr_s);

#endif

// Char translation MS-safe functions
// -----------------------------------------------------

NW_EXTERN_C_BEGIN

// Converts string of char(s) pointed to by src into multibyte characters stored in dest, dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
extern  errno_t mbstowcs_s
                (
                  size_t        *outCount,
                  wchar_t       *dest,
                  rsize_t        destsz,
                  const char    *src,
                  rsize_t        count
                );

// Converts string of wchar_t(s) pointed to by src into multibyte characters stored in dest, dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
extern  errno_t wcstombs_s
                (
                  size_t        *outCount,
                  char          *dest,
                  rsize_t        destsz,
                  const wchar_t *src,
                  rsize_t        count
                );

NW_EXTERN_C_END

#ifdef __cplusplus // safe templates

// Converts string of char(s) pointed to by src into multibyte characters stored in dest[destsz], dest memory size is limited to destsz items(wchar_t). returns errno_t (0 if ok)
template<typename TD, size_t sizeD>
errno_t mbstowcs_s(size_t *outCount, TD (&dest)[sizeD], const char *src, rsize_t count)
{
  return(mbstowcs_s(outCount, dest, sizeD, src, count));
};

// Converts string of wchar_t(s) pointed to by src into multibyte characters stored in dest[destsz], dest memory size is limited to destsz items(chars/bytes). returns errno_t (0 if ok)
template<typename TD, size_t sizeD>
errno_t wcstombs_s(size_t *outCount, TD (&dest)[sizeD], const wchar_t *src, rsize_t count)
{
  return(wcstombs_s(outCount, dest, sizeD, src, count));
};

#endif

#endif // GCC
#endif // ...Included
