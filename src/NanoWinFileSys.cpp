// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileSys functions

#include "NanoWinFileSys.h"

#include "NanoWinStrConvert.h"
#include "NanoWinError.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>

NW_EXTERN_C_BEGIN

extern BOOL WINAPI PathFileExistsA (_In_ LPCSTR lpPath)
{
  if (lpPath == NULL)
  {
    SetLastError(ERROR_INVALID_DATA);
    return(FALSE);
  }

  if (access(lpPath, F_OK) == 0)
  {
    return(TRUE);
  }

  SetLastError(ERROR_FILE_NOT_FOUND);
  return(FALSE);
}

extern BOOL WINAPI PathFileExistsW (_In_ LPCWSTR lpPath)
{
  NanoWin::WStrToStrClone sPath(lpPath);
  return(PathFileExistsA(sPath.c_str()));
}

static bool CurrDirCalculateRequiredBufferSize (_Out_ DWORD *requiredBufferSize, _In_ DWORD startBufferSize)
{
  constexpr size_t MAX_BUFFER_SIZE = 32 * 1024;

  size_t currBufferSize = startBufferSize;
  bool requiredSizeFound = false;
  bool errorFlag = false;

  char *buffer = NULL;

  while (!errorFlag && !requiredSizeFound && currBufferSize <= MAX_BUFFER_SIZE)
  {
    char *temp = (char*)realloc(buffer, currBufferSize);

    if (temp != NULL)
    {
      buffer = temp;

      if (getcwd(buffer, currBufferSize) != NULL)
      {
        *requiredBufferSize = strlen(buffer) + 1;
        requiredSizeFound = true;
      }
      else
      {
        if (errno == ERANGE)
        {
          currBufferSize *= 2;
        }
        else
        {
          errorFlag = true;

          SetLastError(NanoWinErrorByErrnoAtFail(errno));
        }
      }
    }
    else
    {
      errorFlag = true;

      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    }
  }

  free(buffer);

  return requiredSizeFound;
}

extern DWORD WINAPI GetCurrentDirectoryA (_In_ DWORD nBufferLength, _Out_ LPSTR lpBuffer)
{
  DWORD result = 0;

  if (lpBuffer == NULL)
  {
    constexpr DWORD START_BUFFER_SIZE = 512;

    CurrDirCalculateRequiredBufferSize(&result, START_BUFFER_SIZE);
  }
  else
  {
    if (getcwd(lpBuffer, nBufferLength) != NULL)
    {
      // ok, return number of characters written not including null character

      result = strlen(lpBuffer);
    }
    else
    {
      if (errno == ERANGE)
      {
        // buffer is too small, need to return required size of buffer
        CurrDirCalculateRequiredBufferSize(&result, nBufferLength);
      }
      else
      {
        SetLastError(NanoWinErrorByErrnoAtFail(errno));
      }
    }
  }

  return result;
}

extern DWORD WINAPI GetCurrentDirectoryW (_In_ DWORD nBufferLength, _Out_ LPWSTR lpBuffer)
{
  char        buffer[PATH_MAX];
  DWORD       multiByteLen;
  char       *currBuffer = buffer;
  DWORD       currBufferSize = sizeof(buffer);

  // try to get current directory with local (non-heap) buffer
  multiByteLen = GetCurrentDirectoryA(sizeof(buffer), buffer);

  // dynamically allocate buffer if local buffer was too small.
  // need to do it in a loop since current dir can be changed between the
  // calls and returned length value may be outdated on subsequent call
  while (multiByteLen > 0 && multiByteLen >= currBufferSize && currBuffer != NULL)
  {
    if (currBuffer == buffer)
    {
      currBuffer = (char*)malloc(multiByteLen);
    }
    else
    {
      char *tempBuffer = (char*)realloc(currBuffer, multiByteLen);

      if (tempBuffer != NULL)
      {
        currBuffer = tempBuffer;
      }
      else
      {
        free(currBuffer);

        currBuffer = NULL;
      }
    }

    if (currBuffer != NULL)
    {
      currBufferSize = multiByteLen;
      multiByteLen   = GetCurrentDirectoryA(currBufferSize, currBuffer);
    }
  }

  DWORD result = 0;

  if      (multiByteLen == 0)
  {
  }
  else if (currBuffer == NULL)
  {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  }
  else
  {
    std::wstring wideCurrDir;

    try
    {
      wideCurrDir = NanoWin::StrConverter::Convert(currBuffer);
      size_t wideCurrDirLen = wideCurrDir.length();

      if (lpBuffer != NULL)
      {
        if (nBufferLength > wideCurrDirLen)
        {
          wcpcpy(lpBuffer, wideCurrDir.c_str());

          result = wideCurrDirLen;
        }
        else
        {
          result = wideCurrDirLen + 1;
        }
      }
      else
      {
        result = wideCurrDirLen + 1;
      }
    }
    catch (NanoWin::StrConverter::Error)
    {
      SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    }
  }

  if (currBuffer != buffer)
  {
    free(currBuffer);
  }

  return result;
}

extern BOOL WINAPI CreateDirectoryA (_In_ LPCSTR lpPathName, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  mode_t mode = lpSecurityAttributes == NULL ? (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) : (S_IRWXU);

  if (mkdir(lpPathName, mode) != 0)
  {
    switch (errno)
    {
      case (EEXIST) :
      {
        SetLastError(ERROR_ALREADY_EXISTS);
      } break;

      case (ENOENT) :
      {
        SetLastError(ERROR_PATH_NOT_FOUND);
      } break;

      default :
      {
        SetLastError(NanoWinErrorByErrnoAtFail(errno));
      }
    }

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

extern BOOL WINAPI CreateDirectoryW (_In_ LPCWSTR lpPathName, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  try
  {
    std::string mbPathName = NanoWin::StrConverter::Convert(lpPathName);

    return CreateDirectoryA(mbPathName.c_str(), lpSecurityAttributes);
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);

    return FALSE;
  }
}

extern BOOL  WINAPI CopyFileA (_In_ LPCSTR  lpExistingFileName, _In_ LPCSTR  lpNewFileName, _In_ BOOL bFailIfExists)
{
  SetLastError(ERROR_INVALID_FUNCTION); // TODO: Implement me
  return(FALSE);
}

extern BOOL  WINAPI CopyFileW (_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName, _In_ BOOL bFailIfExists)
{
  try
  {
    std::string mbExistingFileName = NanoWin::StrConverter::Convert(lpExistingFileName);
    std::string mbNewFileName      = NanoWin::StrConverter::Convert(lpNewFileName);

    return CopyFileA(mbExistingFileName.c_str(), mbNewFileName.c_str(), bFailIfExists);
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);

    return FALSE;
  }
}

extern BOOL WINAPI DeleteFileA (_In_ LPCSTR  lpFileName)
{
  if (unlink(lpFileName) == 0)
  {
    return(TRUE);
  }
  else
  {
    if (errno == ENOENT)
    {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
    else if (errno == EACCES)
    {
        SetLastError(ERROR_ACCESS_DENIED);
    }
    else
    {
        SetLastError(NanoWinErrorByErrnoAtFail(errno));
    }

    return(FALSE);
  }
}

extern BOOL WINAPI DeleteFileW (_In_ LPCWSTR lpFileName)
{
  NanoWin::WStrToStrClone sFileName(lpFileName);
  return(DeleteFileA(sFileName.c_str()));
}

extern DWORD GetFileAttributesA (_In_ LPCSTR  lpFileName)
{
  struct stat st;

  if (stat(lpFileName, &st) != 0)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));
    return(INVALID_FILE_ATTRIBUTES);
  }

  DWORD result = 0;

  if (S_ISDIR(st.st_mode))
  {
    result |= FILE_ATTRIBUTE_DIRECTORY;
  }

  return(result);
}

extern DWORD GetFileAttributesW (_In_ LPCWSTR lpFileName)
{
  try
  {
    std::string mbPathName = NanoWin::StrConverter::Convert(lpFileName);
    return GetFileAttributesA(mbPathName.c_str());
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    return INVALID_FILE_ATTRIBUTES;
  }
}

NW_EXTERN_C_END
