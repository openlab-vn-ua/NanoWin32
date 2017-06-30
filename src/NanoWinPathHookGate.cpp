// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// PathHook functions: Standard library functions path-hook gates

#include "NanoWinPathHookGate.h"

#if defined LINUX

// THIS MODULE PERFORM FILE SYSTEM HOOKS TO ALLOW PATH SEP TO BE '/' OR '\' [WORKS UNDER LINUX ONLY]

// Add following line to link to make this stuff work:
// --wrap open --wrap open64 ...
// Or, via g++
// -Wl,-wrap,open -Wl,-wrap,open64 -Wl,-wrap,openat -Wl,-wrap,openat64 -Wl,-wrap,creat -Wl,-wrap,creat64 -Wl,-wrap,access -Wl,-wrap,stat -Wl,-wrap,lstat
// -Wl,-wrap,scandir -Wl,-wrap,opendir -Wl,-wrap,__xstat -Wl,-wrap,__lxstat -Wl,-wrap,__xstat64 -Wl,-wrap,__lxstat64 -Wl,-wrap,chmod -Wl,-wrap,chown -Wl,-wrap,lchown
// -Wl,-wrap,symlink -Wl,-wrap,link -Wl,-wrap,mknod -Wl,-wrap,unlink -Wl,-wrap,mkfifo -Wl,-wrap,rename -Wl,-wrap,utime -Wl,-wrap,utimes -Wl,-wrap,mkdir -Wl,-wrap,rmdir
// -Wl,-wrap,chdir
// -Wl,-wrap,mount
// -Wl,-wrap,fopen -Wl,-wrap,fopen64 -Wl,-wrap,freopen
// -Wl,-wrap,realpath -Wl,-wrap,dirname
// -Wl,-wrap,__xpg_basename
// -Wl,-wrap,basename // this will lead to error if someone try to call basename from string.h

#include <errno.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <stdarg.h>

// Translation function
// ==================================================================

#include "NanoWinPathHook.h"

// The function should translate srcpath if need
// translation result should be returned to *poutpath (initially *poutpath is equal srcpath)
// translated path may use buffer[buffersz] to put new (updated) path into
// return 0 on ok or errno value in case of fail
static errno_t translate_path(const char **poutpath, const char *srcpath, char *buffer, size_t buffersz)
{
  NanoWin::PathHook *xlt = NanoWin::PathHookIn;
  if (xlt != NULL)
  {
    return(xlt->doTranslatePath(poutpath, srcpath, buffer, buffersz));
  }
  else
  {
    *poutpath = srcpath; // just in case
    return(0);
  }
}

// The gates tools
// ==================================================================

#include "NanoWinMsSafeString.h" // _countof(x)
#include "NanoWinFileSys.h"

// Middle level macros

// Write arbitrary text to log, use NW_FUNC_LOG(printf(...)) [would do nothing with log_exp on release]
#define NW_FUNC_LOG(log_exp)      // log_exp

// Write func param translation from src to res [all "s" parameters are "strings"]
#define NW_PROC_LOG(sfunc,sparam,ssrc,sres,ierr)  NW_FUNC_LOG(printf("XLT %s (%s='%s'<'%s') %d %s\n", (sfunc), (sparam), ((sres) == NULL ? "NULL" : (sres)), ((ssrc) == NULL ? "NULL" : (ssrc)), (int) ierr, (ierr == 0 ? "OK" : "FAIL")));

// Vars
#define NW_PROC_PARAM_VSTR(n)     n##_x                                     // translated param var name
#define NW_PROC_PARAM_VBUF(n)     n##_b                                     // translated param var name for buffer
#define NW_PROC_PARAM_VERR(n)     n##_e                                     // translated param var name for action errno

// Action
// Declare translated var(s) and do translation
#define NW_PROC_PARAM_CORE(n) \
       const char *NW_PROC_PARAM_VSTR(n) = (n); \
       char        NW_PROC_PARAM_VBUF(n)[PATH_MAX]; \
       errno_t     NW_PROC_PARAM_VERR(n) = 0; \
	   NW_PROC_PARAM_VERR(n) = translate_path(&NW_PROC_PARAM_VSTR(n), (n), NW_PROC_PARAM_VBUF(n), _countof(NW_PROC_PARAM_VBUF(n)));

// Result
#define NW_PROC_PARAM_CSTR(n)     (NW_PROC_PARAM_VSTR(n))                   // translation result
#define NW_PROC_PARAM_OK(n)       (NW_PROC_PARAM_VERR(n) == 0)              // (true) if translation was ok
#define NW_PROC_PARAM_ERRNO(n)    (NW_PROC_PARAM_VERR(n))                   // errno of translation or 0 if no error

// Aux
#define NW_PROC_PARAM_LOG(f,n)    NW_PROC_LOG(f,#n,n,NW_PROC_PARAM_CSTR(n), NW_PROC_PARAM_ERRNO(n)) // write translation result to log

#define NW_FUNC_VAR_USED(a)       do { if (&(a) != NULL) { } } while(0) // just to skip warning

// The gates
// ==================================================================

NW_EXTERN_C_BEGIN

// Top level macros

#define GATE(func, ret_t, ret_err, ... ) \
  ret_t NW_WRAP_REAL(func)(__VA_ARGS__); \
  ret_t NW_WRAP_GATE(func)(__VA_ARGS__) \
  { static const char *FNAME = #func; NW_FUNC_VAR_USED(FNAME); \
    static ret_t FFAIL = ret_err;

#define GEND() \
  }

#define FOREWARD(func) NW_WRAP_REAL(func)

#define XLATE(path) \
        NW_PROC_PARAM_CORE(path); \
        NW_PROC_PARAM_LOG(FNAME, path); \
        if (!NW_PROC_PARAM_OK(path)) { errno = NW_PROC_PARAM_ERRNO(path); return(FFAIL); }

#define XLATED(path) NW_PROC_PARAM_CSTR(path)

// open handled specially, since last param "mode_t mode" is optional

#define FUNC_FILL_OPTIONAL_PARAM(prevparam, varname, vartype) \
  { \
    va_list arg; \
    va_start(arg, prevparam); \
    varname = va_arg(arg, vartype); \
    va_end(arg); \
  } 

#define OPEN_NEED_MODE_PARAM(oflag) (((oflag & O_CREAT) != 0) || ((oflag & O_TMPFILE) != 0))

GATE(open, int, -1, const char *pathname, int flags, ...)
{
  mode_t mode = 0;
  if (OPEN_NEED_MODE_PARAM(flags)) { FUNC_FILL_OPTIONAL_PARAM(flags, mode, mode_t); }
  XLATE(pathname);
  return FOREWARD(open)(XLATED(pathname), flags, mode);
}
GEND()

GATE(open64, int, -1, const char *pathname, int flags, ...)
{
  mode_t mode = 0;
  if (OPEN_NEED_MODE_PARAM(flags)) { FUNC_FILL_OPTIONAL_PARAM(flags, mode, mode_t); }
  XLATE(pathname);
  return FOREWARD(open64)(XLATED(pathname), flags, mode);
}
GEND()

GATE(openat, int, -1, int fd, const char *pathname, int flags, ...)
{
  mode_t mode = 0;
  if (OPEN_NEED_MODE_PARAM(flags)) { FUNC_FILL_OPTIONAL_PARAM(flags, mode, mode_t); }
  XLATE(pathname);
  return FOREWARD(openat)(fd, XLATED(pathname), flags, mode);
}
GEND()

GATE(openat64, int, -1, int fd, const char *pathname, int flags, ...)
{
  mode_t mode = 0;
  if (OPEN_NEED_MODE_PARAM(flags)) { FUNC_FILL_OPTIONAL_PARAM(flags, mode, mode_t); }
  XLATE(pathname);
  return FOREWARD(openat64)(fd, XLATED(pathname), flags, mode);
}
GEND()

GATE(creat, int, -1, const char *pathname, mode_t mode) // may produce double hook via open
{
  XLATE(pathname);
  return FOREWARD(creat)(XLATED(pathname), mode);
}
GEND()

GATE(creat64, int, -1, const char *pathname, mode_t mode) // may produce double hook via open64
{
  XLATE(pathname);
  return FOREWARD(creat64)(XLATED(pathname), mode);
}
GEND()

GATE(access, int, -1, const char *pathname, int mode)
{
  XLATE(pathname);
  return FOREWARD(access)(XLATED(pathname), mode);
}
GEND()

GATE(stat, int, -1, const char *path, struct stat *buf)
{
  XLATE(path);
  return FOREWARD(stat)(XLATED(path), buf );
}
GEND()

GATE(lstat, int, -1, const char *path, struct stat *buf)
{
  XLATE(path);
  return FOREWARD(lstat)(XLATED(path), buf );
}
GEND()

GATE(scandir, int, -1, const char *dirp, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **))
{
  XLATE(dirp);
  return FOREWARD(scandir)(XLATED(dirp), namelist, filter, compar );
}
GEND()

GATE(opendir, DIR*, NULL, const char *name)
{
  XLATE(name);
  return FOREWARD(opendir)(XLATED(name));
}
GEND()

GATE(__xstat, int, -1, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename);
  return FOREWARD(__xstat)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(__lxstat, int, -1, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename);
  return FOREWARD(__lxstat)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(__xstat64, int, -1, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename);
  return FOREWARD(__xstat64)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(__lxstat64, int, -1, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename);
  return FOREWARD(__lxstat64)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(chmod, int, -1, const char *path, mode_t mode)
{
  XLATE(path);
  return FOREWARD(chmod)(XLATED(path), mode);
}
GEND()

GATE(chown, int, -1, const char *path, uid_t owner, gid_t group)
{
  XLATE(path);
  return FOREWARD(chown)(XLATED(path), owner, group);
}
GEND()

GATE(lchown, int, -1, const char *path, uid_t owner, gid_t group)
{
  XLATE(path);
  return FOREWARD(lchown)(XLATED(path), owner, group);
}
GEND()

GATE(symlink, int, -1, const char *oldpath, const char *newpath)
{
  XLATE(oldpath);
  XLATE(newpath);
  return FOREWARD(symlink)(XLATED(oldpath), XLATED(newpath));
}
GEND()

GATE(link, int, -1, const char *oldpath, const char *newpath)
{
  XLATE(oldpath);
  XLATE(newpath);
  return FOREWARD(link)(XLATED(oldpath), XLATED(newpath));
}
GEND()

GATE(mknod, int, -1, const char *pathname, mode_t mode, dev_t dev)
{
  XLATE(pathname);
  return FOREWARD(mknod)(XLATED(pathname), mode, dev);
}
GEND()

GATE(unlink, int, -1, const char *pathname)
{
  XLATE(pathname);
  return FOREWARD(unlink)(XLATED(pathname));
}
GEND()

GATE(mkfifo, int, -1, const char *pathname, mode_t mode)
{
  XLATE(pathname);
  return FOREWARD(mkfifo)(XLATED(pathname), mode);
}
GEND()

GATE(rename, int, -1, const char *oldpath, const char *newpath)
{
  XLATE(oldpath);
  XLATE(newpath);
  return FOREWARD(rename)(XLATED(oldpath), XLATED(newpath));
}
GEND()

GATE(utime, int, -1, const char *filename, const struct utimbuf *times)
{
  XLATE(filename);
  return FOREWARD(utime)(XLATED(filename), times);
}
GEND()

GATE(utimes, int, -1, const char *filename, const struct timeval times[2])
{
  XLATE(filename);
  return FOREWARD(utimes)(XLATED(filename), times);
}
GEND()

GATE(mkdir, int, -1, const char *pathname, mode_t mode)
{
  XLATE(pathname);
  return FOREWARD(mkdir)(XLATED(pathname), mode);
}
GEND()

GATE(rmdir, int, -1, const char *pathname)
{
  XLATE(pathname);
  return FOREWARD(rmdir)(XLATED(pathname));
}
GEND()

// Special

GATE(chdir, int, -1, const char *pathname)
{
  XLATE(pathname);
  return FOREWARD(chdir)(XLATED(pathname));
}
GEND()

// Exotic

GATE(mount, int, -1, const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
  XLATE(source);
  XLATE(target);
  return FOREWARD(mount)(XLATED(source), XLATED(target), filesystemtype, mountflags, data);
}
GEND()

// Top level

GATE(fopen, FILE *, NULL, const char *path, const char *mode) // may produce double hooks
{
  XLATE(path);
  return FOREWARD(fopen)(XLATED(path), mode);
}
GEND()

GATE(fopen64, FILE *, NULL, const char *path, const char *mode) // may produce double hooks
{
  XLATE(path);
  return FOREWARD(fopen64)(XLATED(path), mode);
}
GEND()

GATE(freopen, FILE *, NULL, const char *path, const char *mode, FILE *stream) // may produce double hooks
{
  XLATE(path);
  return FOREWARD(freopen)(XLATED(path), mode, stream);
}
GEND()

// Fname translators

GATE(realpath, char *, NULL, const char *path, char *resolved_path)
{
  XLATE(path);
  return FOREWARD(realpath)(XLATED(path), resolved_path);
}
GEND()

// Notes for dirname
// We have to do path translation in-place
// (that is, we have to update the path itself, as function contract allow that)
// We cannot afford "extension" of path length, since there may be not enough room in source string
// Also, we have to update path in place because actual function may return pointer to data inside the argument
// Moreover, not we are not updating errno, as function contract does not permit that

GATE(dirname, char *, ".", char *path)
{
  if (path == NULL)
  {
    return FOREWARD(dirname)(path); // nothing to translate
  }

  // Keep global errno, this function does not afffect it
  errno_t errno_saved = errno;
  NW_PROC_PARAM_CORE(path);
  errno =  errno_saved;

  if (!NW_PROC_PARAM_OK(path))
  {
    NW_PROC_PARAM_LOG(FNAME, path);
    return(FFAIL);
  }

  if (NW_PROC_PARAM_CSTR(path) != path)
  {
    // There was translation
    size_t oldlen = strlen(path);
    size_t newlen = strlen(NW_PROC_PARAM_CSTR(path));
    if (newlen > oldlen)
    {
      // There may not be enough room to copy result back to source
      NW_PROC_PARAM_VERR(path) = ENOMEM;
      NW_PROC_PARAM_LOG(FNAME, path);
      return(FFAIL);
    }

    strcpy(path, NW_PROC_PARAM_CSTR(path));
  }

  NW_PROC_PARAM_LOG(FNAME, path);
  return FOREWARD(dirname)(path); // path already xlated, as it copied back
}
GEND()

// basename is a bit tricky part
// actually glibc defines 2 functions:
// 1. basename (string.h) weak alias to hidden symbol __basename (we cannot hook __basename for some reason)
// 2. __xpg_basename (libgen.h)
// First one does not modify argument (that can be achived for slashes translation only)

NW_EXTERN_C_END

#include <libgen.h> // basename now is macro that maps to __xpg_basename

NW_EXTERN_C_BEGIN

#if defined(basename)

GATE(basename, char *, ".", char *path)
{
  if (path == NULL)
  {
    return FOREWARD(basename)(path); // nothing to translate
  }

  // Keep global errno, this function does not afffect it
  errno_t errno_saved = errno;
  NW_PROC_PARAM_CORE(path);
  errno =  errno_saved;

  if (!NW_PROC_PARAM_OK(path))
  {
    NW_PROC_PARAM_LOG(FNAME, path);
    return(FFAIL);
  }

  if (NW_PROC_PARAM_CSTR(path) != path)
  {
    // There was translation
    size_t oldlen = strlen(path);
    size_t newlen = strlen(NW_PROC_PARAM_CSTR(path));
    if (newlen > oldlen)
    {
      // There may not be enough room to copy result back to source
      NW_PROC_PARAM_VERR(path) = ENOMEM;
      NW_PROC_PARAM_LOG(FNAME, path);
      return(FFAIL);
    }

    strcpy(path, NW_PROC_PARAM_CSTR(path));
  }

  NW_PROC_PARAM_LOG(FNAME, path);
  return FOREWARD(basename)(path); // path already xlated, as it copied back
}
GEND()

#endif

NW_EXTERN_C_END

#endif // linux
