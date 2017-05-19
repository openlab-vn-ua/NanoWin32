// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft extra functions defined, but missing under linux/unix/GCC

#include "NanoWinMsExtra.h"

#include "NanoWinStrConvert.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

NW_EXTERN_C_BEGIN

extern FILE  *wfopen(const wchar_t *name, const wchar_t *mode)
{
  NanoWin::WStrToStrClone cname(name);
  NanoWin::WStrToStrClone cmode(mode);
  return(fopen(cname.c_str(), cmode.c_str()));
}

extern int    _mkdir(const char *dirname)
{
  return(mkdir(dirname,(S_IRWXU | S_IRGRP | S_IXGRP |  S_IROTH | S_IXOTH)));
}

extern int    _wmkdir(const wchar_t *dirname)
{
  NanoWin::WStrToStrClone cdirname(dirname);
  return(_mkdir(cdirname.c_str()));
}

extern errno_t _dupenv_s(char **buffer, size_t *numberOfElements, const char *varname)
{
  // TODO: Implement me
  if (buffer != NULL) { (*buffer) = NULL; }
  if (numberOfElements != NULL) { (*numberOfElements) = 0; }
  return(ENOMEM);
}

// extern errno_t _wdupenv_s(wchar_t **buffer, size_t *numberOfElements, const wchar_t *varname); // TODO: Implement me

extern errno_t mbstowcs_s
                (
                  size_t        *outCount,
                  wchar_t       *dest,
                  rsize_t        destsz,
                  const char    *src,
                  rsize_t        count
                )
{
  // TODO: Draft implemenation
  (*outCount) = 0;
  size_t result = mbstowcs(dest, src, count);
  if (((ssize_t)result) < 0) { return(EINVAL); }
  (*outCount) = result;
  return(0); // EOK
}

extern  errno_t wcstombs_s
                (
                  size_t        *outCount,
                  char          *dest,
                  rsize_t        destsz,
                  const wchar_t *src,
                  rsize_t        count
                )
{
  // TODO: Draft implemenation
  (*outCount) = 0;
  size_t result = wcstombs(dest, src, count);
  if (((ssize_t)result) < 0) { return(EINVAL); }
  (*outCount) = result;
  return(0); // EOK
}

NW_EXTERN_C_END
