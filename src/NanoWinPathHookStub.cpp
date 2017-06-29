// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// PathHook functions: Standard library functions path-hook stub for link

#include "NanoWinPathHookStub.h"

#if defined LINUX

#include "NanoWinPathHookGate.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Note: Stub functions exists only for linkage they never actually called

#define NW_MAKE_STUB(symbol) NW_EXTERN_C int NW_WRAP_STUB(symbol)(...) { return(-1); }

#define STUB(func, ...)  NW_MAKE_STUB(func)

STUB(open, int, const char *pathname, int flags, mode_t mode)
STUB(open64, int, const char *pathname, int flags, mode_t mode)
STUB(openat, int, const char *pathname, int flags, mode_t mode)
STUB(openat64, int, const char *pathname, int flags, mode_t mode)

STUB(creat, int, const char *pathname, mode_t mode)
STUB(creat64, int, const char *pathname, mode_t mode)

STUB(access, int, const char *pathname, int mode)
STUB(stat, int, const char *path, struct stat *buf)
STUB(lstat, int, const char *path, struct stat *buf)
STUB(scandir, int, const char *dirp, struct dirent ***namelist, int (*filter)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **))
STUB(opendir, DIR*, const char *name)
STUB(__xstat, int, int __ver, __const char *__filename, struct stat *__stat_buf)
STUB(__lxstat, int, int __ver, __const char *__filename, struct stat *__stat_buf)
STUB(__xstat64, int, int __ver, __const char *__filename, struct stat *__stat_buf)
STUB(__lxstat64, int, int __ver, __const char *__filename, struct stat *__stat_buf)
STUB(chmod, int, const char *path, mode_t mode)
STUB(chown, int, const char *path, uid_t owner, gid_t group)
STUB(lchown, int, const char *path, uid_t owner, gid_t group)
STUB(symlink, int, const char *oldpath, const char *newpath)
STUB(link, int, const char *oldpath, const char *newpath)
STUB(mknod, int, const char *pathname, mode_t mode, dev_t dev)
STUB(unlink, int, const char *pathname)
STUB(mkfifo, int, const char *pathname, mode_t mode)
STUB(rename, int, const char *oldpath, const char *newpath)
STUB(utime, int, const char *filename, const struct utimbuf *times)
STUB(utimes, int, const char *filename, const struct timeval times[2])
STUB(mkdir, int, const char *pathname, mode_t mode)
STUB(rmdir, int, const char *pathname)

// Special

STUB(realpath, char *, const char *path, char *resolved_path)
STUB(chdir, int, const char *pathname)

// Exotic

STUB(mount, int, const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)

// Top level

STUB(fopen, FILE *, const char *path, const char *mode) // may produce double hooks
STUB(fopen64, FILE *, const char *path, const char *mode) // may produce double hooks
STUB(freopen, FILE *, const char *path, const char *mode, FILE *stream) // may produce double hooks

#endif // linux
