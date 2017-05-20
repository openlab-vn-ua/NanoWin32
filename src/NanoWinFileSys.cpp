// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileSys functions

#include "NanoWinFileSys.h"

#include "NanoWinStrConvert.h"
#include "NanoWinError.h"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>

NW_EXTERN_C_BEGIN

extern BOOL WINAPI PathFileExistsA(const char *lpPath)
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

extern BOOL WINAPI PathFileExistsW(const wchar_t *lpPath)
{
  NanoWin::WStrToStrClone sPath(lpPath);
  return(PathFileExistsA(sPath.c_str()));
}

static bool CurrDirCalculateRequiredBufferSize(DWORD *requiredBufferSize, DWORD startBufferSize)
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

extern DWORD WINAPI GetCurrentDirectoryA(DWORD nBufferLength, LPSTR lpBuffer)
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

extern DWORD WINAPI GetCurrentDirectoryW(DWORD nBufferLength, LPWSTR lpBuffer)
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

extern BOOL WINAPI DeleteFileA(LPCSTR  lpFileName)
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

extern BOOL WINAPI DeleteFileW(LPCWSTR lpFileName)
{
  NanoWin::WStrToStrClone sFileName(lpFileName);
  return(DeleteFileA(sFileName.c_str()));
}

NW_EXTERN_C_END
