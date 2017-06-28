// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// PathHook functions: Standard library functions path-hook gates

#include "NanoWinPathHookGate.h"

#if defined LINUX

// Add following line to link to make this stuff work:
// --wrap open --wrap open64 ...
// Or, via g++
// -Wl,-wrap,open -Wl,-wrap,open64 -Wl,-wrap,openat -Wl,-wrap,openat64 -Wl,-wrap,creat -Wl,-wrap,creat64 -Wl,-wrap,access -Wl,-wrap,stat -Wl,-wrap,lstat
// -Wl,-wrap,scandir -Wl,-wrap,opendir -Wl,-wrap,__xstat -Wl,-wrap,__lxstat -Wl,-wrap,__xstat64 -Wl,-wrap,__lxstat64 -Wl,-wrap,chmod -Wl,-wrap,chown -Wl,-wrap,lchown
// -Wl,-wrap,symlink -Wl,-wrap,link -Wl,-wrap,mknod -Wl,-wrap,unlink -Wl,-wrap,mkfifo -Wl,-wrap,rename -Wl,-wrap,utime -Wl,-wrap,utimes -Wl,-wrap,mkdir -Wl,-wrap,rmdir
// -Wl,-wrap,realpath -Wl,-wrap,chdir -Wl,-wrap,mount -Wl,-wrap,fopen -Wl,-wrap,fopen64 -Wl,-wrap,freopen

#include <errno.h>

#include <stdio.h> // printf

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <stdarg.h>

#define NW_FUNC_LOG(log_exp)      log_exp

#define NW_PROC_LOG(sfunc,sparam,ssrc,sres)  NW_FUNC_LOG(printf("XLT %s (%s='%s'<'%s')\n", (sfunc), (sparam), (sres), (ssrc)));

#define NW_PROC_FNAME(n)          const char *n##_x = n;
#define NW_PROC_FNAME_LOG(f,n)    NW_PROC_LOG(f,#n,n,n##_x)
#define NW_PROC_FNAME_CSTR(n)     (n##_x)
#define NW_PROC_FNAME_OK(n)       ((&(n##_x)) != NULL) // (true)
#define NW_PROC_FNAME_ERRNO(n)    (NW_PROC_FNAME_OK(n) ? 0 : EINVAL)

// The gates
// ==================================================================

// Special cases for open,open64,openat,openat64
// ----------------------------------------------

// open

#if !defined(open) // open is not a macro

NW_EXTERN_C int NW_WRAP_REAL(open) (const char *file, int oflag, ...); // + mode_t mode
NW_EXTERN_C int NW_WRAP_GATE(open) (const char *file, int oflag, ...)  // + mode_t mode
{
  mode_t mode;

  if (((oflag & O_CREAT) != 0) || ((oflag & O_TMPFILE) != 0))
  {
    va_list arg;
    va_start(arg, oflag);
    mode = va_arg(arg, mode_t);
    va_end(arg);
  }

  NW_PROC_FNAME(file);
  NW_PROC_FNAME_LOG("open", file);

  if (!NW_PROC_FNAME_OK(file)) { errno = NW_PROC_FNAME_ERRNO(file); return(-1); }

  return(NW_WRAP_REAL(open)(NW_PROC_FNAME_CSTR(file), oflag, mode));
}

#endif

NW_EXTERN_C int NW_WRAP_REAL(open64) (const char *file, int oflag, ...); // + mode_t mode
NW_EXTERN_C int NW_WRAP_GATE(open64) (const char *file, int oflag, ...)  // + mode_t mode
{
  mode_t mode;

  if (((oflag & O_CREAT) != 0) || ((oflag & O_TMPFILE) != 0))
  {
    va_list arg;
    va_start(arg, oflag);
    mode = va_arg(arg, mode_t);
    va_end(arg);
  }

  NW_PROC_FNAME(file);
  NW_PROC_FNAME_LOG("open64", file);

  if (!NW_PROC_FNAME_OK(file)) { errno = NW_PROC_FNAME_ERRNO(file); return(-1); }

  return(NW_WRAP_REAL(open64)(NW_PROC_FNAME_CSTR(file), oflag, mode));
}

// openat

#if !defined(openat) // open is not a macro

NW_EXTERN_C int NW_WRAP_REAL(openat) (int fd, const char *file, int oflag, ...); // + mode_t mode
NW_EXTERN_C int NW_WRAP_GATE(openat) (int fd, const char *file, int oflag, ...)  // + mode_t mode
{
  mode_t mode;

  if (((oflag & O_CREAT) != 0) || ((oflag & O_TMPFILE) != 0))
  {
    va_list arg;
    va_start(arg, oflag);
    mode = va_arg(arg, mode_t);
    va_end(arg);
  }

  NW_PROC_FNAME(file);
  NW_PROC_FNAME_LOG("openat", file);

  if (!NW_PROC_FNAME_OK(file)) { errno = NW_PROC_FNAME_ERRNO(file); return(-1); }

  return(NW_WRAP_REAL(openat)(fd, NW_PROC_FNAME_CSTR(file), oflag, mode));
}

#endif

NW_EXTERN_C int NW_WRAP_REAL(openat64) (int fd, const char *file, int oflag, ...); // + mode_t mode
NW_EXTERN_C int NW_WRAP_GATE(openat64) (int fd, const char *file, int oflag, ...)  // + mode_t mode
{
  mode_t mode;

  if (((oflag & O_CREAT) != 0) || ((oflag & O_TMPFILE) != 0))
  {
    va_list arg;
    va_start(arg, oflag);
    mode = va_arg(arg, mode_t);
    va_end(arg);
  }

  NW_PROC_FNAME(file);
  NW_PROC_FNAME_LOG("openat64", file);

  if (!NW_PROC_FNAME_OK(file)) { errno = NW_PROC_FNAME_ERRNO(file); return(-1); }

  return(NW_WRAP_REAL(openat64)(fd, NW_PROC_FNAME_CSTR(file), oflag, mode));
}

// Other functions
// ----------------------------------------------

NW_EXTERN_C_BEGIN

#define GATE(func, ret_t, ... ) \
  ret_t NW_WRAP_REAL(func)(__VA_ARGS__); \
  ret_t NW_WRAP_GATE(func)(__VA_ARGS__) \
  { static const char *FNAME = #func;

#define GEND() \
  }

#define FOREWARD(func) NW_WRAP_REAL(func)

#define XLATE(path, reterr) \
        NW_PROC_FNAME(path); \
        NW_PROC_FNAME_LOG(FNAME, path); \
        if (!NW_PROC_FNAME_OK(path)) { errno = NW_PROC_FNAME_ERRNO(path); return(reterr); }

#define XLATED(path) NW_PROC_FNAME_CSTR(path)

// GATE(open, int, const char *pathname, int flags, mode_t mode) // handled specially
// GATE(open64, int, const char *pathname, int flags, mode_t mode) // handled specially
// GATE(openat, int, const char *pathname, int flags, mode_t mode) // handled specially
// GATE(openat64, int, const char *pathname, int flags, mode_t mode) // handled specially

GATE(creat, int, const char *pathname, mode_t mode) // may produce double hook via open
{
  XLATE(pathname, -1);
  return FOREWARD(creat)(XLATED(pathname), mode);
}
GEND()

GATE(creat64, int, const char *pathname, mode_t mode) // may produce double hook via open64
{
  XLATE(pathname, -1);
  return FOREWARD(creat64)(XLATED(pathname), mode);
}
GEND()

GATE(access, int, const char *pathname, int mode)
{
  XLATE(pathname, -1);
  return FOREWARD(access)(XLATED(pathname), mode);
}
GEND()

GATE(stat, int, const char *path, struct stat *buf)
{
  XLATE(path, -1);
  return FOREWARD(stat)(XLATED(path), buf );
}
GEND()

GATE(lstat, int, const char *path, struct stat *buf)
{
  XLATE(path, -1);
  return FOREWARD(lstat)(XLATED(path), buf );
}
GEND()

GATE(scandir, int, const char *dirp, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **))
{
  XLATE(dirp, -1);
  return FOREWARD(scandir)(XLATED(dirp), namelist, filter, compar );
}
GEND()

GATE(opendir, DIR*, const char *name)
{
  XLATE(name, NULL);
  return FOREWARD(opendir)(XLATED(name));
}
GEND()

GATE(__xstat, int, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename, -1);
  return FOREWARD(__xstat)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(__lxstat, int, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename, -1);
  return FOREWARD(__lxstat)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(__xstat64, int, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename, -1);
  return FOREWARD(__xstat64)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(__lxstat64, int, int __ver, __const char *__filename, struct stat *__stat_buf)
{
  XLATE(__filename, -1);
  return FOREWARD(__lxstat64)( __ver, XLATED(__filename), __stat_buf);
}
GEND()

GATE(chmod, int, const char *path, mode_t mode)
{
  XLATE(path, -1);
    return FOREWARD(chmod)(XLATED(path), mode);
}
GEND()

GATE(chown, int, const char *path, uid_t owner, gid_t group)
{
  XLATE(path, -1);
  return FOREWARD(chown)(XLATED(path), owner, group);
}
GEND()

GATE(lchown, int, const char *path, uid_t owner, gid_t group)
{
  XLATE(path, -1);
  return FOREWARD(lchown)(XLATED(path), owner, group);
}
GEND()

GATE(symlink, int, const char *oldpath, const char *newpath)
{
  XLATE(oldpath, -1);
  XLATE(newpath, -1);
  return FOREWARD(symlink)(XLATED(oldpath), XLATED(newpath));
}
GEND()

GATE(link, int, const char *oldpath, const char *newpath)
{
  XLATE(oldpath, -1);
  XLATE(newpath, -1);
  return FOREWARD(link)(XLATED(oldpath), XLATED(newpath));
}
GEND()

GATE(mknod, int, const char *pathname, mode_t mode, dev_t dev)
{
  XLATE(pathname, -1);
  return FOREWARD(mknod)(XLATED(pathname), mode, dev);
}
GEND()

GATE(unlink, int, const char *pathname)
{
  XLATE(pathname, -1);
  return FOREWARD(unlink)(XLATED(pathname));
}
GEND()

GATE(mkfifo, int, const char *pathname, mode_t mode)
{
  XLATE(pathname, -1);
  return FOREWARD(mkfifo)(XLATED(pathname), mode);
}
GEND()

GATE(rename, int, const char *oldpath, const char *newpath)
{
  XLATE(oldpath, -1);
  XLATE(newpath, -1);
  return FOREWARD(rename)(XLATED(oldpath), XLATED(newpath));
}
GEND()

GATE(utime, int, const char *filename, const struct utimbuf *times)
{
  XLATE(filename, -1);
  return FOREWARD(utime)(XLATED(filename), times);
}
GEND()

GATE(utimes, int, const char *filename, const struct timeval times[2])
{
  XLATE(filename, -1);
  return FOREWARD(utimes)(XLATED(filename), times);
}
GEND()

GATE(mkdir, int, const char *pathname, mode_t mode)
{
  XLATE(pathname, -1);
  return FOREWARD(mkdir)(XLATED(pathname), mode);
}
GEND()

GATE(rmdir, int, const char *pathname)
{
  XLATE(pathname, -1);
  return FOREWARD(rmdir)(XLATED(pathname));
}
GEND()

// Special

GATE(realpath, char *, const char *path, char *resolved_path)
{
  XLATE(path, NULL);
  return FOREWARD(realpath)(XLATED(path), resolved_path);
}
GEND()

GATE(chdir, int, const char *pathname)
{
  XLATE(pathname, -1);
  return FOREWARD(chdir)(XLATED(pathname));
}
GEND()

// Exotic

GATE(mount, int, const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
  XLATE(source, -1);
  XLATE(target, -1);
  return FOREWARD(mount)(XLATED(source), XLATED(target), filesystemtype, mountflags, data);
}
GEND()

// Top level

GATE(fopen, FILE *, const char *path, const char *mode) // may produce double hooks
{
  XLATE(path, NULL);
  return FOREWARD(fopen)(XLATED(path), mode);
}
GEND()

GATE(fopen64, FILE *, const char *path, const char *mode) // may produce double hooks
{
  XLATE(path, NULL);
  return FOREWARD(fopen64)(XLATED(path), mode);
}
GEND()

GATE(freopen, FILE *, const char *path, const char *mode, FILE *stream) // may produce double hooks
{
  XLATE(path, NULL);
  return FOREWARD(freopen)(XLATED(path), mode, stream);
}
GEND()

NW_EXTERN_C_END

#endif // linux
