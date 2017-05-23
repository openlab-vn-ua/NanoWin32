// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft extra functions defined, but missing under linux/unix/GCC

#if !defined(NanoWinMsvcExtraIncluded)
#define NanoWinMsvcExtraIncluded

#if defined(__GNUC__)

// MS Dll Export / Import
// ---------------------------------------------
// Just to allow stuff to compile
// So you can write __declspec(dllexport) and __declspec(dllimport)
// TODO: Finlize me (and adjust to link stuff)

#ifndef NW_NO_MS_DECLSPEC

#ifndef __declspec
#define __declspec(spec)     // None
#endif

#ifndef dllexport
#define dllexport            // None
#endif

#ifndef dllimport
#define dllimport            // None
#endif

#endif

#include "NanoWinMsSafeString.h"

// MS extensions and tricks
// ---------------------------------------------
#include <stdio.h>

NW_EXTERN_C_BEGIN

extern FILE  *wfopen(const wchar_t *name, const wchar_t *mode);

#ifndef NW_NO_MS_ISO_ALIASES // MS-style aliases for "obsolete/nonconformant" func
#define _wfopen              wfopen // alias to wfopen
#endif

extern int    _mkdir(const char *dirname);
extern int    _wmkdir(const wchar_t *dirname);   

extern errno_t _dupenv_s(char **buffer, size_t *numberOfElements, const char *varname);
// extern errno_t _wdupenv_s(wchar_t **buffer, size_t *numberOfElements, const wchar_t *varname);

// share.h subset

#define SH_DENYNO           (0x40) // The only supported value so far
#define fsopen(fn,m,sc)     (fopen((fn),(m)))  // TODO: Refine me (incomplete implemenation)
#define wfsopen(fn,m,sc)    (wfopen((fn),(m))) // TODO: Refine me (incomplete implemenation)

#ifndef NW_NO_MS_ISO_ALIASES // MS-style aliases for "obsolete/nonconformant" func
#define _SH_DENYNO          SH_DENYNO
#define _fsopen             fsopen
#define _wfsopen            wfsopen
#endif

// Errno add-ons

#define set_errno(v)        ((errno = (v)),0)
#define get_errno(pv)       ((*(pv) = errno),0)

#ifndef NW_NO_MS_ISO_ALIASES // MS-style aliases for "obsolete/nonconformant" func
#define _set_errno          set_errno
#define _get_errno          get_errno
#endif

// MS more safe functions
// ---------------------------------------------

#define fopen_s(f,fn,m)      ((*(f) = fopen(fn,m)) == NULL ? errno : 0)
#define wfopen_s(f,fn,m)     ((*(f) = wfopen(fn,m)) == NULL ? errno : 0) // TODO: Align better: there is only _wfopen_s under win32 (check does wfopen_s ever existed?)
#define localtime_s(pti,pt)  (((*(pti)) = *(localtime(pt))),0)

#ifndef NW_NO_MS_ISO_ALIASES // MS aliases for "obsolete" func
#define _wfopen_s             wfopen_s // alias to wfopen_s
#endif

// for *printf_s functions work alike to *printf but just add format string check
// assume it the same, we may will implement version with check later

#define printf_s             printf
#define wprintf_s            wprintf
#define fprintf_s            fprintf
#define fwprintf_s           fwprintf
#define vfprintf_s           vfprintf
#define vfwprintf_s          vfwprintf

// MS string functions
// ---------------------------------------------

#define strnicmp(s1,s2,n)    strncasecmp(s1,s2,n)
#define stricmp(s1,s2)       strcasecmp(s1,s2)
#define wcsnicmp(s1,s2,n)    wcsncasecmp(s1,s2,n)
#define wcsicmp(s1,s2)       wcscasecmp(s1,s2)

#define atoi64(a)            atoll(a)

extern  char    *strlwr      (char    *s);
extern  char    *strupr      (char    *s);
extern  wchar_t *wsclwr      (wchar_t *s);
extern  wchar_t *wscupr      (wchar_t *s);

#ifndef NW_NO_MS_ISO_ALIASES // MS aliases for "obsolete" func
#define _strnicmp            strnicmp
#define _stricmp             stricmp
#define _wcsnicmp            wcsnicmp
#define _wcsicmp             wcsicmp
#define _atoi64              atoi64
// Other
#define _strupr              strupr
#define _wcsupr              wcsupr
#define _strlwr              strlwr
#define _wcslwr              wcslwr
#endif

// Aliases for other functions
// ---------------------------------------------

#ifndef NW_NO_MS_ISO_ALIASES // MS aliases for "obsolete" func
#define _fputchar            fputchar
#define _fputwchar           fputwchar
#define _getcwd              getcwd
#define _wgetcwd             wgetcwd  // TODO: Align better: there is only _wgetcwd under win32 (check does wgetcwd ever existed?)
#define _chdir               chdir
#define _getpid              getpid
#endif

// Other MS-specific functions
// ---------------------------------------------

extern wchar_t *wgetcwd(wchar_t *buffer,  int maxlen);

NW_EXTERN_C_END

// C++ gates and tricks
// ---------------------------------------------

// MS mkdir trick (via C++)
// MS defines _mkdir func with one arg only, but POSIX uses 2 args for mkdir
// Some old MS code uses mkdir with one arg

#if !defined(__cplusplus)
#define       mkdir(nm)  _mkdir(nm)
#else
inline int    mkdir(const char *dirname) { return(_mkdir(dirname)); }
#endif

#endif // GCC
#endif // ...Included
