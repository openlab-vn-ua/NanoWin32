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

extern wchar_t *wgetcwd(wchar_t *dest, size_t destsz)
{
  // Note: under MSVC destsz is int (both for _getcwd and _wgetcwd) [looks not aligned with POSIX]
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
    if (mbstowcs_s(&countx,dest,destsz,result,destsz) != 0) { dest[0] = 0; errno=ERANGE; return(NULL); }
    if (countx > destsz) { dest[0] = 0; errno=ERANGE; return(NULL); }
    return(dest);
  }
}

NW_EXTERN_C_END

// MS string functions
// ---------------------------------------------

NW_EXTERN_C_BEGIN

extern  char    *strlwr      (char    *s)  { if (s==NULL) { return(s); } char    *data = (s); while (*data != 0) { *data = (char)tolower (*data); data++;} return(s); }
extern  char    *strupr      (char    *s)  { if (s==NULL) { return(s); } char    *data = (s); while (*data != 0) { *data = (char)toupper (*data); data++;} return(s); }
extern  wchar_t *wcslwr      (wchar_t *s)  { if (s==NULL) { return(s); } wchar_t *data = (s); while (*data != 0) { *data = towlower(*data); data++;} return(s); }
extern  wchar_t *wcsupr      (wchar_t *s)  { if (s==NULL) { return(s); } wchar_t *data = (s); while (*data != 0) { *data = towupper(*data); data++;} return(s); }

NW_EXTERN_C_END

// Path support (looks like MS specific)
// ---------------------------------------------

#define pathfunc_handle_errcode(errcode) { errno = (errcode); } // MS spce says func should assign errno

static bool     is_path_sep  (char     value)
{
  if (value == NW_DIRECTORY_SEPARATOR_CHAR)     { return(true); }
  if (value == NW_DIRECTORY_SEPARATOR_ALT_CHAR) { return(true); }
  return(false);
}

static bool     is_path_sep  (wchar_t  value)
{
  if (value == NW_DIRECTORY_SEPARATOR_CHAR)     { return(true); }
  if (value == NW_DIRECTORY_SEPARATOR_ALT_CHAR) { return(true); }
  return(false);
}

NW_EXTERN_C_BEGIN

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
  rsize_t  THE_LIMIT = _MAX_PATH;
  _makepath_s(destpath, THE_LIMIT, srcdrive, srcdir, srcfname, srcext);
}

extern void     _wmakepath   (wchar_t *destpath, const wchar_t *srcdrive, const wchar_t *srcdir, const wchar_t *srcfname, const wchar_t *srcext)
{
  rsize_t  THE_LIMIT = _MAX_PATH;
  _wmakepath_s(destpath, THE_LIMIT, srcdrive, srcdir, srcfname, srcext);
}

extern errno_t  _makepath_s  (char    *destpath, rsize_t destsz, const char    *srcdrive, const char    *srcdir, const char    *srcfname, const char    *srcext)
{
  #define FN "_makepath_s"
  #define ITEM char
  #define STRLEN strlen
  #define STRCAT_S strcat_s
  // <body> // invaliant for _makepath_s and _wmakepath_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); pathfunc_handle_errcode(errcode); return(errcode); }

  if (destpath == NULL)        { return_after_err_WMARKER(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_WMARKER(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_WMARKER(FN SP "destsz too large"            , NULL, ERANGE); }

  destpath[0] = 0;

  errno_t rc = EOK;

  if ((srcdrive != NULL) && (srcdrive[0] != 0))
  {
    rc = STRCAT_S(destpath, destsz, srcdrive);
	if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
  }

  if ((srcdir   != NULL) && (srcdir[0] != 0))
  {
    ITEM DSEPS[2]; DSEPS[0] = NW_DIRECTORY_SEPARATOR_CHAR; DSEPS[1] = 0;
    rsize_t len = STRLEN(srcdir);
    rc = STRCAT_S(destpath, destsz, srcdir);
    if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
    if ((len > 0) && !is_path_sep(srcdir[len-1]))
    {
      rc = STRCAT_S(destpath, destsz, DSEPS);
      if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
    }
  }

  if ((srcfname != NULL) && (srcfname[0] != 0))
  {
    rc = STRCAT_S(destpath, destsz, srcfname);
    if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
  }

  if ((srcext   != NULL) && (srcext[0] != 0))
  {
    ITEM ESEPS[2]; ESEPS[0] = NW_FILE_EXT_SEPARATOR_CHAR; ESEPS[1] = 0;
	if (srcext[0] != NW_FILE_EXT_SEPARATOR_CHAR)
	{
      rc = STRCAT_S(destpath, destsz, ESEPS);
      if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
	}
    rc = STRCAT_S(destpath, destsz, srcext);
    if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
  }

  return(EOK);

  #undef RSIZE_MAX_CNT
  #undef return_after_err_WMARKER
  // </body>
  #undef STRCAT_S
  #undef STRLEN
  #undef ITEM
  #undef FN
}

extern errno_t  _wmakepath_s (wchar_t *destpath, rsize_t destsz, const wchar_t *srcdrive, const wchar_t *srcdir, const wchar_t *srcfname, const wchar_t *srcext)
{
  #define FN "_wmakepath_s"
  #define ITEM wchar_t
  #define STRLEN wcslen
  #define STRCAT_S wcscat_s
  // <body> // invaliant for _makepath_s and _wmakepath_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define return_after_err_WMARKER(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); pathfunc_handle_errcode(errcode); return(errcode); }

  if (destpath == NULL)        { return_after_err_WMARKER(FN SP "dest is null"                , NULL, EINVAL); }
  if (destsz <= 0)             { return_after_err_WMARKER(FN SP "destsz is zero"              , NULL, ERANGE); }
  if (destsz >  RSIZE_MAX_CNT) { return_after_err_WMARKER(FN SP "destsz too large"            , NULL, ERANGE); }

  destpath[0] = 0;

  errno_t rc = EOK;

  if ((srcdrive != NULL) && (srcdrive[0] != 0))
  {
    rc = STRCAT_S(destpath, destsz, srcdrive);
	if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
  }

  if ((srcdir   != NULL) && (srcdir[0] != 0))
  {
    ITEM DSEPS[2]; DSEPS[0] = NW_DIRECTORY_SEPARATOR_CHAR; DSEPS[1] = 0;
    rsize_t len = STRLEN(srcdir);
    rc = STRCAT_S(destpath, destsz, srcdir);
    if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
    if ((len > 0) && !is_path_sep(srcdir[len-1]))
    {
      rc = STRCAT_S(destpath, destsz, DSEPS);
      if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
    }
  }

  if ((srcfname != NULL) && (srcfname[0] != 0))
  {
    rc = STRCAT_S(destpath, destsz, srcfname);
    if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
  }

  if ((srcext   != NULL) && (srcext[0] != 0))
  {
    ITEM ESEPS[2]; ESEPS[0] = NW_FILE_EXT_SEPARATOR_CHAR; ESEPS[1] = 0;
	if (srcext[0] != NW_FILE_EXT_SEPARATOR_CHAR)
	{
      rc = STRCAT_S(destpath, destsz, ESEPS);
      if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
	}
    rc = STRCAT_S(destpath, destsz, srcext);
    if (rc != EOK) { pathfunc_handle_errcode(rc); return(rc); }
  }

  return(EOK);

  #undef RSIZE_MAX_CNT
  #undef return_after_err_WMARKER
  // </body>
  #undef STRCAT_S
  #undef STRLEN
  #undef ITEM
  #undef FN
}

NW_EXTERN_C_END

static bool     is_drv_sep   (char     value)
{
  if (value == ':') { return(true); } // coment me under LINUX production
  return(false);
}

static bool     is_drv_sep   (wchar_t  value)
{
  if (value == ':') { return(true); } // coment me under LINUX production
  return(false);
}

NW_EXTERN_C_BEGIN

extern errno_t  _splitpath_s (const char    *srcpath, char    *destdrive, rsize_t destdrivesz, char    *destdir, rsize_t destdirsz, char    *destfname, rsize_t destfnamesz, char    *destext, rsize_t destextsz)
{
  #define FN "_splitpath_s"
  #define ITEM char
  #define STRLEN strlen
  #define STRNCPY_S strncpy_s
  // <body> // invaliant for _splitpath_s and _wsplitpath_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define zero_dest(dest,destsz)  { if (dest != NULL) { if ((destsz > 0) && (destsz <= RSIZE_MAX_CNT)) { dest[0] = 0; } } }
  #define zero_all_dests()        { zero_dest(destdrive,destdrivesz); zero_dest(destdir,destdirsz); zero_dest(destfname,destfnamesz); zero_dest(destext,destextsz); }
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); zero_all_dests(); pathfunc_handle_errcode(errcode); return(errcode); }

  #define check_dest(dest,destsz) \
  if ((dest != NULL) && (destsz <= 0))            { return_after_err_FILLDST(FN SP #destsz " is zero"   , NULL, EINVAL); } \
  if ((dest != NULL) && (destsz > RSIZE_MAX_CNT)) { return_after_err_FILLDST(FN SP #destsz " too large" , NULL, EINVAL); } \
  if ((dest == NULL) && (destsz != 0))            { return_after_err_FILLDST(FN SP #destsz " non zero"  , NULL, EINVAL); }

  if (srcpath == NULL)         { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  check_dest(destdrive,destdrivesz);
  check_dest(destdir,destdirsz);
  check_dest(destfname,destfnamesz);
  check_dest(destext,destextsz);

  rsize_t slen = STRLEN(srcpath);

  zero_all_dests();

  if (slen <= 0)                 { return(EOK); } // Nothing to do

  // Extract fname

  rsize_t i;

  rsize_t dpos = RSIZE_MAX; // drive[:]path sep pos
  rsize_t ppos = RSIZE_MAX; // path[/]name sep pos
  rsize_t epos = RSIZE_MAX; // name[.]ext sep pos

  i = 0;
  while(i < slen)
  {
    // scan string in reverce
    rsize_t pos = slen-i-1;
    ITEM item = srcpath[pos];
    if ((dpos == RSIZE_MAX) && (ppos == RSIZE_MAX) && (epos == RSIZE_MAX) && (item == NW_FILE_EXT_SEPARATOR_CHAR))
    {
      epos = pos;
    }
    else if ((dpos == RSIZE_MAX) && (ppos == RSIZE_MAX) && is_path_sep(item))
    {
      ppos = pos;
    }
    else if ((dpos == RSIZE_MAX) && is_drv_sep(item))
    {
      dpos = pos;
    }
    else
    {
      i++;
    }
  }

  // now, store results
  errno_t rc;

  if ((dpos != RSIZE_MAX) && (destdrive != NULL))
  {
    rc = STRNCPY_S(destdrive, destdrivesz, srcpath, dpos+1);
    if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
  }

  if ((epos != RSIZE_MAX) && (destext != NULL))
  {
    rc = STRNCPY_S(destext, destextsz, srcpath+epos, slen-epos);
    if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
  }

  // Now, handle path and name

  if (ppos != RSIZE_MAX)
  {
    // path/name sep found
    if (destdir != NULL)
    {
      const ITEM *srcdir = srcpath;
      rsize_t     plen = slen;
      if (dpos != RSIZE_MAX) { srcdir += (dpos+1); plen -= (dpos+1); }
      plen -= (slen-ppos-1);
      rc = STRNCPY_S(destdir, destdirsz, srcdir, plen);
      if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
    }

    if (destfname != NULL)
    {
      const ITEM *srcname = srcpath;
      rsize_t     nlen = slen;
      srcname += (ppos+1); nlen -= (ppos+1);
      if (epos != RSIZE_MAX) { nlen -= (slen-epos); }
      if (nlen > 0)
      {
        rc = STRNCPY_S(destfname, destfnamesz, srcname, nlen);
        if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
      }
      else
      {
        zero_dest(destfname, destfnamesz);
      }
    }
  }
  else
  {
    // no path separator: only name is here
    if (destdir != NULL)
    {
      zero_dest(destdir,destdirsz);
    }

    if (destfname != NULL)
    {
      const ITEM *srcname = srcpath;
      rsize_t     nlen = slen;
      if (dpos != RSIZE_MAX) { srcname += (dpos+1); nlen -= (dpos+1); }
      if (epos != RSIZE_MAX) { nlen -= (slen-epos); }
      if (nlen > 0)
      {
        rc = STRNCPY_S(destfname, destfnamesz, srcname, nlen);
        if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
      }
      else
      {
        zero_dest(destfname, destfnamesz);
      }
    }
  }

  return(EOK);

  #undef check_dest

  #undef return_after_err_FILLDST
  #undef zero_all_dests
  #undef zero_dest
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNCPY_S
  #undef STRLEN
  #undef ITEM
  #undef FN
}

extern errno_t  _wsplitpath_s(const wchar_t *srcpath, wchar_t *destdrive, rsize_t destdrivesz, wchar_t *destdir, rsize_t destdirsz, wchar_t *destfname, rsize_t destfnamesz, wchar_t *destext, rsize_t destextsz)
{
  #define FN "_wsplitpath_s"
  #define ITEM wchar_t
  #define STRLEN wcslen
  #define STRNCPY_S wcsncpy_s
  // <body> // invaliant for _splitpath_s and _wsplitpath_s
  #define RSIZE_MAX_CNT        RSIZE_GET_CNT(RSIZE_MAX_STR, ITEM)
  #define zero_dest(dest,destsz)  { if (dest != NULL) { if ((destsz > 0) && (destsz <= RSIZE_MAX_CNT)) { dest[0] = 0; } } }
  #define zero_all_dests()        { zero_dest(destdrive,destdrivesz); zero_dest(destdir,destdirsz); zero_dest(destfname,destfnamesz); zero_dest(destext,destextsz); }
  #define return_after_err_FILLDST(etext,earg,errcode) { invoke_err_handler(etext,earg,errcode); zero_all_dests(); pathfunc_handle_errcode(errcode); return(errcode); }

  #define check_dest(dest,destsz) \
  if ((dest != NULL) && (destsz <= 0))            { return_after_err_FILLDST(FN SP #destsz " is zero"   , NULL, EINVAL); } \
  if ((dest != NULL) && (destsz > RSIZE_MAX_CNT)) { return_after_err_FILLDST(FN SP #destsz " too large" , NULL, EINVAL); } \
  if ((dest == NULL) && (destsz != 0))            { return_after_err_FILLDST(FN SP #destsz " non zero"  , NULL, EINVAL); }

  if (srcpath == NULL)         { return_after_err_FILLDST(FN SP "src is null"                 , NULL, EINVAL); }

  check_dest(destdrive,destdrivesz);
  check_dest(destdir,destdirsz);
  check_dest(destfname,destfnamesz);
  check_dest(destext,destextsz);

  rsize_t slen = STRLEN(srcpath);

  zero_all_dests();

  if (slen <= 0)                 { return(EOK); } // Nothing to do

  // Extract fname

  rsize_t i;

  rsize_t dpos = RSIZE_MAX; // drive[:]path sep pos
  rsize_t ppos = RSIZE_MAX; // path[/]name sep pos
  rsize_t epos = RSIZE_MAX; // name[.]ext sep pos

  i = 0;
  while(i < slen)
  {
    // scan string in reverce
    rsize_t pos = slen-i-1;
    ITEM item = srcpath[pos];
    if ((dpos == RSIZE_MAX) && (ppos == RSIZE_MAX) && (epos == RSIZE_MAX) && (item == NW_FILE_EXT_SEPARATOR_CHAR))
    {
      epos = pos;
    }
    else if ((dpos == RSIZE_MAX) && (ppos == RSIZE_MAX) && is_path_sep(item))
    {
      ppos = pos;
    }
    else if ((dpos == RSIZE_MAX) && is_drv_sep(item))
    {
      dpos = pos;
    }
    else
    {
      i++;
    }
  }

  // now, store results
  errno_t rc;

  if ((dpos != RSIZE_MAX) && (destdrive != NULL))
  {
    rc = STRNCPY_S(destdrive, destdrivesz, srcpath, dpos+1);
    if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
  }

  if ((epos != RSIZE_MAX) && (destext != NULL))
  {
    rc = STRNCPY_S(destext, destextsz, srcpath+epos, slen-epos);
    if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
  }

  // Now, handle path and name

  if (ppos != RSIZE_MAX)
  {
    // path/name sep found
    if (destdir != NULL)
    {
      const ITEM *srcdir = srcpath;
      rsize_t     plen = slen;
      if (dpos != RSIZE_MAX) { srcdir += (dpos+1); plen -= (dpos+1); }
      plen -= (slen-ppos-1);
      rc = STRNCPY_S(destdir, destdirsz, srcdir, plen);
      if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
    }

    if (destfname != NULL)
    {
      const ITEM *srcname = srcpath;
      rsize_t     nlen = slen;
      srcname += (ppos+1); nlen -= (ppos+1);
      if (epos != RSIZE_MAX) { nlen -= (slen-epos); }
      if (nlen > 0)
      {
        rc = STRNCPY_S(destfname, destfnamesz, srcname, nlen);
        if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
      }
      else
      {
        zero_dest(destfname, destfnamesz);
      }
    }
  }
  else
  {
    // no path separator: only name is here
    if (destdir != NULL)
    {
      zero_dest(destdir,destdirsz);
    }

    if (destfname != NULL)
    {
      const ITEM *srcname = srcpath;
      rsize_t     nlen = slen;
      if (dpos != RSIZE_MAX) { srcname += (dpos+1); nlen -= (dpos+1); }
      if (epos != RSIZE_MAX) { nlen -= (slen-epos); }
      if (nlen > 0)
      {
        rc = STRNCPY_S(destfname, destfnamesz, srcname, nlen);
        if (rc != EOK) { zero_all_dests(); pathfunc_handle_errcode(rc); return(rc); }
      }
      else
      {
        zero_dest(destfname, destfnamesz);
      }
    }
  }

  return(EOK);

  #undef check_dest

  #undef return_after_err_FILLDST
  #undef zero_all_dests
  #undef zero_dest
  #undef RSIZE_MAX_CNT
  // </body>
  #undef STRNCPY_S
  #undef STRLEN
  #undef ITEM
  #undef FN
}

NW_EXTERN_C_END

// Unicode versions of some calls
// ---------------------------------------------

NW_EXTERN_C_BEGIN

extern int      wsystem  (const wchar_t *lpCommand)
{
  if (lpCommand == NULL)
  {
    int result = system(NULL);
    if (result == 0)
	{
      set_errno(ENOENT);
    }

    return(result);
  }
  else
  {
    NanoWin::WStrToStrClone sCommand(lpCommand);

	if (!sCommand.is_valid()) { set_errno(EINVAL); return(-1); }

	int result = system(sCommand.c_str());
	if (result == -1)
    {
      // it is not clear should stdlib set errno in that case
      // we try to keep existing errno here, overwritting it only on case its zero
      if (errno == 0) { set_errno(ENOEXEC); }
    }

    return(result);
  }
}

NW_EXTERN_C_END
