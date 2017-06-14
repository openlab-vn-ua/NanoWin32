// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Additional functions (not part of Win API, but provided by NanoWin)

#include "NanoWinPlus.h"
#include "NanoWinError.h"
#include "NanoWinFileSys.h"
#include "NanoWinStrConvert.h"

#include <string.h>

#define NanoWinShellExDirSepChar    NW_DIRECTORY_SEPARATOR_CHAR
#define NanoWinShellExDirSepAltChar NW_DIRECTORY_SEPARATOR_ALT_CHAR

// returns either position of found char or strlen(str) if char wasn't found
static size_t StrFindChars(const char *str, char ch, char altCh)
{
  const char *ptr = str;

  while (*ptr != '\0' && *ptr != ch && *ptr != altCh)
  {
    ptr++;
  }

  return (size_t)(ptr - str);
}

// Return Win32 API error code
NW_EXTERN_C DWORD  NanoWinCreateDirectoryPathA
                   (
                     LPCSTR                     pszPath,
                     const SECURITY_ATTRIBUTES *psa
                   )
{
  // Error codes: (at least): [see doc]
  // ERROR_SUCCESS:
  // ERROR_FILE_EXISTS:
  // ERROR_ALREADY_EXISTS:

  constexpr size_t PATH_MAX_LEN = (_MAX_FNAME - 1); // according to MS documentation

  bool   ok     = true;
  size_t sepPos = StrFindChars(pszPath,NanoWinShellExDirSepChar,NanoWinShellExDirSepAltChar);

  if (pszPath[sepPos] != '\0')
  {
    // subdirs(or absolute path) found in path, need to check if parent dirs need to be created

    char subPath[PATH_MAX_LEN + 1];

    if (sepPos < PATH_MAX_LEN)
    {
      memcpy(subPath,pszPath,sepPos);

      subPath[sepPos] = NanoWinShellExDirSepChar;
      subPath[++sepPos]   = '\0';

      size_t startPos = 0;
      bool   pathExists = false;

      // sepPos points to the char after separator
      // subPath contains current path prefix with trailing path separator (if any)
      do
      {
        pathExists = PathFileExistsA(subPath);

        if (!pathExists)
        {
          ok = CreateDirectoryA(subPath, const_cast<SECURITY_ATTRIBUTES*>(psa)); 
        }

        if (ok)
        {
          startPos = sepPos;

          if (pszPath[startPos] != '\0')
          {
            sepPos += StrFindChars(&pszPath[startPos],NanoWinShellExDirSepChar,NanoWinShellExDirSepAltChar);

            if (sepPos <= PATH_MAX_LEN) // it's ok if sepPos == PATH_MAX_LEN if pszPath[sepPos] == 0
            {
              memcpy(&subPath[startPos],&pszPath[startPos],sepPos - startPos + 1);

              if (subPath[sepPos] != '\0')
              {
                if (sepPos < PATH_MAX_LEN)
                {
                  subPath[sepPos]   = NanoWinShellExDirSepChar;
                  subPath[++sepPos] = '\0';
                }
                else
                {
                  ok = false;
                  SetLastError(ERROR_FILENAME_EXCED_RANGE);
                }
              }
            }
            else
            {
              ok = false;
              SetLastError(ERROR_FILENAME_EXCED_RANGE);
            }
          }
        }
      } while (ok && pszPath[startPos] != '\0');

      if (ok && pathExists)
      {
        ok = false;
        SetLastError(ERROR_ALREADY_EXISTS);
      }
    }
    else
    {
      ok = false;
      SetLastError(ERROR_FILENAME_EXCED_RANGE);
    }
  }
  else
  {
    // no subdirs found in path, just create directory

    ok = CreateDirectoryA(pszPath, const_cast<SECURITY_ATTRIBUTES*>(psa)); 
  }
    
  if (ok)
  {
    return(ERROR_SUCCESS);
  }
  else if (GetLastError() != ERROR_SUCCESS)
  {
    return(GetLastError());
  }
  else // GetLastError() not set, but we have failed to action
  {
    SetLastError(ERROR_FILENAME_EXCED_RANGE);
    return(ERROR_FILENAME_EXCED_RANGE);
  }
}

// Return Win32 API error code
NW_EXTERN_C DWORD  NanoWinCreateDirectoryPathW
                   (
                     LPCWSTR                    pszPath,
                     const SECURITY_ATTRIBUTES *psa
                   )
{
  try
  {
    return NanoWinCreateDirectoryPathA(NanoWin::StrConverter::Convert(pszPath).c_str(),psa);
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    return ERROR_NO_UNICODE_TRANSLATION;
  }
}


#if 0
// Extra path manipulation functions
// Move me to NanoWinPlus.h/.cpp

#include <string>
// returns path combines with basepath + tailpath added
std::string PathCombine(const char *basepath, const char *tailpath)
{
	#define is_path_sep(c) (((c) == '\\') ? true : (((c) == '/') ? true : false))
	#define is_empty(s)    (((s)[0] == 0) ? true : false)

	// Public domain code
	// Edge bases
	if ((basepath == NULL) && (tailpath == NULL))		{ return(""); }
	if ((basepath != NULL) && (tailpath == NULL))		{ return(basepath); }
	if ((basepath == NULL) && (tailpath != NULL))		{ return(tailpath); }

	if (is_empty(basepath)) { return(tailpath); }
	if (is_empty(tailpath)) { return(basepath); }

	// TODO: implement me

	#undef  is_empty
	#undef  is_path_sep
}
#endif
