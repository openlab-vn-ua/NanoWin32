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

#define NW_STR_S_TRUNCATE // define this if you need MS-compatible _TRUNCATE semantic to be supported

#if defined(NW_STR_S_TRUNCATE)
// Support _TRUNCATE semanic a-la MS VisualC
#define _TRUNCATE (RSIZE_MAX) // RSIZE_MAX anyway is quit hight
#define STRUNCATE (-11111)    // error_t value to return in case of truncation occurs
#endif

NW_EXTERN_C_BEGIN

// Memory functions
// -----------------------------------------------------

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

// String functions
// -----------------------------------------------------

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

// Search for tokens in str, acts like wcstok_r + quick checks for valid args. Non-C11, non-MS func, but logic extension of strtok_r. Note: Actualy it is how MS strtok_s works
extern wchar_t*wcstok_r_s    (wchar_t *str, const wchar_t *delim, wchar_t **context);

NW_EXTERN_C_END

#if !defined(__cplusplus)
#define        strtok_s      strtok_r_s // for ms-compatibility
#define        wsctok_s      wsctok_r_s // for ms-compatibility
#else
// get a chance to someone to overload strtok_s and wcstok_s in a C11 compilant way
inline char   *strtok_s      (char *str, const char *delim, char **context) { return(strtok_s(str, delim, context)); }
inline wchar_t*wcstok_s      (wchar_t *str, const wchar_t *delim, wchar_t **context) { return(wcstok_s(str, delim, context)); }
#endif

// C++ gates
// -----------------------------------------------------

#ifdef __cplusplus

// Returns count of items in array
template<typename T1, size_t size1>
size_t NanoWinCountOf(T1(&)[size1])
{ 
  return(size1);
};

#define NW_STR_DN1Param(strfunc_s,arg1type)                          \
template<typename TD, size_t sizeD>                                  \
errno_t strfunc_s(TD (&dest)[sizeD], arg1type arg1)                  \
{                                                                    \
  return(strfunc_s(dest, sizeD, arg1));                              \
};

#define NW_STR_DN2Param(strfunc_s,arg1type,arg2type)                 \
template<typename TD, size_t sizeD>                                  \
errno_t strfunc_s(TD (&dest)[sizeD], arg1type arg1, arg2type arg2)   \
{                                                                    \
  return(strfunc_s(dest, sizeD, arg1, arg2));                        \
};

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
NW_STR_DN2Param(wcsncpy_s,const char *, rsize_t);

#endif

#endif // GCC
#endif // ...Included
