// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft extra functions defined, but missing under linux/unix/GCC

#include "NanoWinMsExtra.h"

#include "NanoWinFileSys.h"
#include "NanoWinStrConvert.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <ctype.h> // tolower
#include <wctype.h> // towlower

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

extern wchar_t *wgetcwd(wchar_t *dest,  int destsz)
{
  // TODO: Draft implemenation
  char  tmp[MAX_PATH];
  char *result;
  result = getcwd(tmp, sizeof(tmp));
  if (result == NULL) { return(NULL); }
  size_t countx = 0;
  if (mbstowcs_s(&countx,dest,destsz,result,destsz) != 0) { return(NULL); }
  return(dest);
}

// MS string functions
// ---------------------------------------------

extern  char    *strlwr      (char    *s)  { if (s==NULL) { return(s); } char    *data = (s); while (*data != 0) { *data = (char)tolower (*data); data++;} return(s); }
extern  char    *strupr      (char    *s)  { if (s==NULL) { return(s); } char    *data = (s); while (*data != 0) { *data = (char)toupper (*data); data++;} return(s); }
extern  wchar_t *wcslwr      (wchar_t *s)  { if (s==NULL) { return(s); } wchar_t *data = (s); while (*data != 0) { *data = towlower(*data); data++;} return(s); }
extern  wchar_t *wcsupr      (wchar_t *s)  { if (s==NULL) { return(s); } wchar_t *data = (s); while (*data != 0) { *data = towupper(*data); data++;} return(s); }

NW_EXTERN_C_END
