// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// ShellEx functions

#include <string.h>

#include "NanoWinShellEx.h"

#include "NanoWinError.h"
#include "NanoWinFileSys.h"
#include "NanoWinStrConvert.h"

#define NanoWinShellExDirSepChar    ('/')
#define NanoWinShellExDirSepAltChar ('\\')

NW_EXTERN_C_BEGIN

extern int SHCreateDirectoryExW
           (
            HWND                       hwnd,
            LPCWSTR                    pszPath,
            const SECURITY_ATTRIBUTES *psa
           )
{
  try
  {
    return SHCreateDirectoryExA(hwnd,NanoWin::StrConverter::Convert(pszPath).c_str(),psa);
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);

    return ERROR_NO_UNICODE_TRANSLATION;
  }
}

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

extern int SHCreateDirectoryExA
           (
            HWND                       hwnd,
            LPCSTR                     pszPath,
            const SECURITY_ATTRIBUTES *psa
           )
{
  // Error codes: (at least): [see doc]
  // ERROR_SUCCESS:
  // ERROR_FILE_EXISTS:
  // ERROR_ALREADY_EXISTS:

  constexpr size_t PATH_MAX_LEN = (248 - 1); // according to MS documentation

  if (hwnd != NULL) { return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL)); }

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

            memcpy(&subPath[startPos],&pszPath[startPos],sepPos - startPos + 1);

            if (subPath[sepPos] != '\0')
            {
              subPath[sepPos]   = NanoWinShellExDirSepChar;
              subPath[++sepPos] = '\0';
            }
          }
        }
      } while (ok && pszPath[startPos] != '\0');

      if (ok && pathExists)
      {
        ok = false; SetLastError(ERROR_ALREADY_EXISTS);
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
    return(NanoWinSetLastError(NW_DEFAULT_ERROR_AT_FAIL));
  }
}

NW_EXTERN_C_END
