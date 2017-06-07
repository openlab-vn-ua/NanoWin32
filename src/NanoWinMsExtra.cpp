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

#if !defined(EOK)
#define  EOK  (0)
#endif

// Borrowed from NanoWinMsSafeString.cpp
#define invoke_err_handler(etext,earg,errcode) // TODO: call handler here
#define return_after_err_handler(etext,earg,errcode) return(errcode)

#define RSIZE_GET_CNT(MAX_MEM,ITEM) (sizeof(ITEM) == 1 ? (MAX_MEM) : ((MAX_MEM) / sizeof(ITEM)))
#define MEM_GET_SIZE(count,ITEM)    (sizeof(ITEM) == 1 ? (count) : ((count) * sizeof(ITEM)))

#define SP ":"

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
  #define FN "_dupenv_s"
  #define ITEM char

  if (buffer  == NULL) { return_after_err_handler(FN SP "buffer is null" , NULL, EINVAL); }
  if (varname == NULL) { return_after_err_handler(FN SP "varname is null", NULL, EINVAL); }

  char *value = getenv(varname);

  if (value != NULL)
  {
    size_t valueLen = strlen(value);

    *buffer = (char*)malloc(valueLen + 1);

    if (*buffer != NULL)
    {
      strcpy(*buffer,value);

      if (numberOfElements != NULL)
      {
        *numberOfElements = valueLen + 1;
      }

      return EOK;
    }
    else
    {
      errno = ENOMEM;

      return errno;
    }
  }
  else
  {
    *buffer = NULL;
    *numberOfElements = 0;

    return EOK;
  }

  #undef ITEM
  #undef FN
}

// extern errno_t _wdupenv_s(wchar_t **buffer, size_t *numberOfElements, const wchar_t *varname); // TODO: Implement me

extern wchar_t *wgetcwd(wchar_t *dest,  int destsz)
{
  // TODO: Draft implemenation (verify me)
  if (dest == NULL)
  {
    // dynamic buffer allocation (at least destsz chars), as GCC says
    if (destsz <= 0) { destsz = 0; }
    char *result;
    result = getcwd(NULL, 0); // force memory allocate
    if (result == NULL) { return(NULL); } // errno already at ENOMEM
	// now, result already here
	std::wstring resultw;
	try
	{
	  resultw = NanoWin::StrConverter::Convert(result);
	}
	catch(...)
	{
      free(result);
      errno = EINVAL; // invalid conversion
      return(NULL);
	}
	free(result);
	size_t size = resultw.length()+1;
	if (destsz < size) { destsz = size; }
	dest = static_cast<wchar_t*>(malloc(destsz * sizeof(wchar_t)));
	if (dest == NULL) { errno=ENOMEM; return(NULL); }
	if (wcscpy_s(dest, destsz, resultw.c_str()) != 0) { free(dest); errno=EINVAL; return(NULL); } // BUG trap
	return(dest);
  }
  else
  {
    // static buffer
    if (destsz <= 0) { errno=ERANGE; return(NULL); }
    char  tmp[MAX_PATH];
    char *result;
    result = getcwd(tmp, sizeof(tmp));
    if (result == NULL) { return(NULL); }
    size_t countx = 0;
    if (mbstowcs_s(&countx,dest,destsz,result,destsz) != 0) { dest[0] = '\0'; errno=ERANGE; return(NULL); }
	if (countx > destsz) { dest[0] = '\0'; errno=ERANGE; return(NULL); }
    return(dest);
  }
}

// MS string functions
// ---------------------------------------------

extern  char    *strlwr      (char    *s)  { if (s==NULL) { return(s); } char    *data = (s); while (*data != 0) { *data = (char)tolower (*data); data++;} return(s); }
extern  char    *strupr      (char    *s)  { if (s==NULL) { return(s); } char    *data = (s); while (*data != 0) { *data = (char)toupper (*data); data++;} return(s); }
extern  wchar_t *wcslwr      (wchar_t *s)  { if (s==NULL) { return(s); } wchar_t *data = (s); while (*data != 0) { *data = towlower(*data); data++;} return(s); }
extern  wchar_t *wcsupr      (wchar_t *s)  { if (s==NULL) { return(s); } wchar_t *data = (s); while (*data != 0) { *data = towupper(*data); data++;} return(s); }

// Path support (looks like MS specific)
// ---------------------------------------------

#define pathfunc_handle_errcode(errcode) { errno = (errcode); } // MS spce says func should assign errno

static bool     is_path_sep  (char     value) // would work for wchar_t also (?)
{
  if (value == NW_DIRECTORY_SEPARATOR_CHAR)     { return(true); }
  if (value == NW_DIRECTORY_SEPARATOR_ALT_CHAR) { return(true); }
  return(false);
}

extern char    *_fullpath    (char    *destabspath, const char    *srcrelpath, rsize_t destabspathsz)
{
  #define FN "_fullpath"
  #define ITEM char
  #define GETCWD getcwd
  #define STRDUP strdup
  #define STRLEN strlen
  #define STRCPY_S strcpy_s
  // <body> // invaliant for _fullpath and _wfullpath
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); pathfunc_handle_errcode(errcode); return(NULL); }

  if (destabspath != NULL)
  {
    if (destabspathsz <= 0)             { return_after_err_WMARKER(FN SP "destsz is zero"              , NULL, ERANGE); }
    if (destabspathsz >  RSIZE_MAX_CNT) { return_after_err_WMARKER(FN SP "destsz too large"            , NULL, ERANGE); }
  }

  if ((srcrelpath == NULL) || (srcrelpath[0] == 0))
  {
    // srcrelpath is empty : return current dir
    if (destabspath != NULL)
    {
      ITEM *result = GETCWD(destabspath, destabspathsz);
      if (result == NULL) { return_after_err_WMARKER(FN SP "destsz too small"  , NULL, ERANGE); }
      return(result);
    }
    else
    {
      return(GETCWD(NULL, 0)); // Use GNU glibc extension (that will malloc result buffer, as required for this func)
    }
  }

  if (is_path_sep(srcrelpath[0]))
  {
    // path start with "/", so this is absolute path already
    if (destabspath != NULL)
    {
      errno_t err = STRCPY_S(destabspath, destabspathsz, srcrelpath);
      if (err != EOK)
      {
        pathfunc_handle_errcode(err); // Error handler already been called
        return(NULL);
      }
      return(destabspath);
    }
    else
    {
      ITEM *result;
      result = STRDUP(srcrelpath);
      if (result == NULL) { pathfunc_handle_errcode(ENOMEM); return(NULL); }
      return(result);
    }
  }

  // srcrelpath is not empty and relative
  // we have to prefix it with current working dir

  if (destabspath != NULL)
  {
    ITEM *result = GETCWD(destabspath, destabspathsz);
    if (result == NULL)        { return_after_err_WMARKER(FN SP "destsz too small"  , NULL, ERANGE); }
    if (result != destabspath) { return_after_err_WMARKER(FN SP "dest pos shifted"  , NULL, EINVAL); } // BUG trap
    rsize_t len = STRLEN(result);
    if (len <= 0)              { return_after_err_WMARKER(FN SP "curr path is invalid" , NULL, EINVAL); } // BUG trap
    if (len >= destabspathsz)  { return_after_err_WMARKER(FN SP "destsz too small."    , NULL, ERANGE); } // BUG trap
    ITEM    *tail   = result + len;
    rsize_t  tailsz = destabspathsz - len;
    if (tailsz <= 0)           { return_after_err_WMARKER(FN SP "destsz too small"     , NULL, ERANGE); } // No room for at least 1 char
    // add path separator (we have room at tail, since tailsz > 0)
    *tail = NW_DIRECTORY_SEPARATOR_CHAR;
    tail++;
    tailsz--;
    // copy remainder
    errno_t  err = STRCPY_S(tail, tailsz, srcrelpath);
    if (err != EOK)
    {
      pathfunc_handle_errcode(err); // Error handler already been called
      return(NULL);
    }
    return(destabspath);
  }
  else
  {
    ITEM *result = GETCWD(NULL, 0); // Use GNU glibc extension (that will malloc result buffer, as required for this func)
    if (result == NULL)        { pathfunc_handle_errcode(ENOMEM); return(NULL); }
    rsize_t len = STRLEN(result);
    if (len <= 0)              { return_after_err_WMARKER(FN SP "curr path is invalid" , NULL, EINVAL); } // BUG trap
    rsize_t slen = STRLEN(srcrelpath);
    rsize_t destsz = (len+1+slen)+1; // gwd+'/'+srcrelpath+'\0'
    ITEM *resultx = static_cast<ITEM*>(realloc(result, MEM_GET_SIZE(destsz, ITEM)));
    if (resultx == NULL)       { free(result); pathfunc_handle_errcode(ENOMEM); return(NULL); }
    result = resultx;
    ITEM    *tail   = result + len;
    rsize_t  tailsz = destsz - len;
    if (tailsz <= 0)           { free(result); pathfunc_handle_errcode(ENOMEM); return(NULL); } // BUG reap
    // add path separator (we have room at tail, since tailsz > 0)
    *tail = NW_DIRECTORY_SEPARATOR_CHAR;
    tail++;
    tailsz--;
    // copy remainder
    errno_t  err = STRCPY_S(tail, tailsz, srcrelpath);
    if (err != EOK)
    {
      free(result);
      pathfunc_handle_errcode(err); // Error handler already been called
      return(NULL);
    }
    return(result);
  }

  #undef RSIZE_MAX_CNT
  #undef return_after_err_WMARKER
  // </body>
  #undef STRCPY_S
  #undef STRLEN
  #undef STRDUP
  #undef GETCWD
  #undef ITEM
  #undef FN
}

extern wchar_t *_wfullpath   (wchar_t *destabspath, const wchar_t *srcrelpath, rsize_t destabspathsz)
{
  #define FN "_wfullpath"
  #define ITEM wchar_t
  #define GETCWD wgetcwd
  #define STRDUP wcsdup
  #define STRLEN wcslen
  #define STRCPY_S wcscpy_s
  // <body> // invaliant for _fullpath and _wfullpath
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); pathfunc_handle_errcode(errcode); return(NULL); }

  if (destabspath != NULL)
  {
    if (destabspathsz <= 0)             { return_after_err_WMARKER(FN SP "destsz is zero"              , NULL, ERANGE); }
    if (destabspathsz >  RSIZE_MAX_CNT) { return_after_err_WMARKER(FN SP "destsz too large"            , NULL, ERANGE); }
  }

  if ((srcrelpath == NULL) || (srcrelpath[0] == 0))
  {
    // srcrelpath is empty : return current dir
    if (destabspath != NULL)
    {
      ITEM *result = GETCWD(destabspath, destabspathsz);
      if (result == NULL) { return_after_err_WMARKER(FN SP "destsz too small"  , NULL, ERANGE); }
      return(result);
    }
    else
    {
      return(GETCWD(NULL, 0)); // Use GNU glibc extension (that will malloc result buffer, as required for this func)
    }
  }

  if (is_path_sep(srcrelpath[0]))
  {
    // path start with "/", so this is absolute path already
    if (destabspath != NULL)
    {
      errno_t err = STRCPY_S(destabspath, destabspathsz, srcrelpath);
      if (err != EOK)
      {
        pathfunc_handle_errcode(err); // Error handler already been called
        return(NULL);
      }
      return(destabspath);
    }
    else
    {
      ITEM *result;
      result = STRDUP(srcrelpath);
      if (result == NULL) { pathfunc_handle_errcode(ENOMEM); return(NULL); }
      return(result);
    }
  }

  // srcrelpath is not empty and relative
  // we have to prefix it with current working dir

  if (destabspath != NULL)
  {
    ITEM *result = GETCWD(destabspath, destabspathsz);
    if (result == NULL)        { return_after_err_WMARKER(FN SP "destsz too small"  , NULL, ERANGE); }
    if (result != destabspath) { return_after_err_WMARKER(FN SP "dest pos shifted"  , NULL, EINVAL); } // BUG trap
    rsize_t len = STRLEN(result);
    if (len <= 0)              { return_after_err_WMARKER(FN SP "curr path is invalid" , NULL, EINVAL); } // BUG trap
    if (len >= destabspathsz)  { return_after_err_WMARKER(FN SP "destsz too small."    , NULL, ERANGE); } // BUG trap
    ITEM    *tail   = result + len;
    rsize_t  tailsz = destabspathsz - len;
    if (tailsz <= 0)           { return_after_err_WMARKER(FN SP "destsz too small"     , NULL, ERANGE); } // No room for at least 1 char
    // add path separator (we have room at tail, since tailsz > 0)
    *tail = NW_DIRECTORY_SEPARATOR_CHAR;
    tail++;
    tailsz--;
    // copy remainder
    errno_t  err = STRCPY_S(tail, tailsz, srcrelpath);
    if (err != EOK)
    {
      pathfunc_handle_errcode(err); // Error handler already been called
      return(NULL);
    }
    return(destabspath);
  }
  else
  {
    ITEM *result = GETCWD(NULL, 0); // Use GNU glibc extension (that will malloc result buffer, as required for this func)
    if (result == NULL)        { pathfunc_handle_errcode(ENOMEM); return(NULL); }
    rsize_t len = STRLEN(result);
    if (len <= 0)              { return_after_err_WMARKER(FN SP "curr path is invalid" , NULL, EINVAL); } // BUG trap
    rsize_t slen = STRLEN(srcrelpath);
    rsize_t destsz = (len+1+slen)+1; // gwd+'/'+srcrelpath+'\0'
    ITEM *resultx = static_cast<ITEM*>(realloc(result, MEM_GET_SIZE(destsz, ITEM)));
    if (resultx == NULL)       { free(result); pathfunc_handle_errcode(ENOMEM); return(NULL); }
    result = resultx;
    ITEM    *tail   = result + len;
    rsize_t  tailsz = destsz - len;
    if (tailsz <= 0)           { free(result); pathfunc_handle_errcode(ENOMEM); return(NULL); } // BUG reap
    // add path separator (we have room at tail, since tailsz > 0)
    *tail = NW_DIRECTORY_SEPARATOR_CHAR;
    tail++;
    tailsz--;
    // copy remainder
    errno_t  err = STRCPY_S(tail, tailsz, srcrelpath);
    if (err != EOK)
    {
      free(result);
      pathfunc_handle_errcode(err); // Error handler already been called
      return(NULL);
    }
    return(result);
  }

  #undef RSIZE_MAX_CNT
  #undef return_after_err_WMARKER
  // </body>
  #undef STRCPY_S
  #undef STRLEN
  #undef STRDUP
  #undef GETCWD
  #undef ITEM
  #undef FN
}

extern void     _makepath    (char    *destpath, const char    *srcdrive, const char    *srcdir, const char    *srcfname, const char    *srcext)
{
}

extern void     _wmakepath   (wchar_t *destpath, const wchar_t *srcdrive, const wchar_t *srcdir, const wchar_t *srcfname, const wchar_t *srcext)
{
}

extern errno_t  _makepath_s  (char    *destpath, rsize_t destsz, const char    *srcdrive, const char    *srcdir, const char    *srcfname, const char    *srcext)
{
}

extern errno_t  _wmakepath_s (wchar_t *destpath, rsize_t destsz, const wchar_t *srcdrive, const wchar_t *srcdir, const wchar_t *srcfname, const wchar_t *srcext)
{
}

extern errno_t  _splitpath_s (const char    *srcpath, char    *destdrive, rsize_t destdrivesz, char    *destdir, rsize_t destdirsz, char    *destfname, rsize_t destfnamesz, char    *destext, rsize_t destextsz)
{
}

extern errno_t  _wsplitpath_s(const wchar_t *srcpath, wchar_t *destdrive, rsize_t destdrivesz, wchar_t *destdir, rsize_t destdirsz, wchar_t *destfname, rsize_t destfnamesz, wchar_t *destext, rsize_t destextsz)
{
}

NW_EXTERN_C_END
