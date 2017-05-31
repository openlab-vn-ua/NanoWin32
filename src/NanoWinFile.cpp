// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// File functions

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "NanoWinFile.h"
#include "NanoWinError.h"
#include "NanoWinFileSys.h"
#include "NanoWinStrConvert.h"

NW_EXTERN_C_BEGIN

extern HANDLE WINAPI CreateFileA(_In_     LPCSTR                lpFileName,
                                 _In_     DWORD                 dwDesiredAccess,
                                 _In_     DWORD                 dwShareMode,
                                 _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 _In_     DWORD                 dwCreationDisposition,
                                 _In_     DWORD                 dwFlagsAndAttributes,
                                 _In_opt_ HANDLE                hTemplateFile)
{
  if (dwShareMode != 0)                              { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported
  if (dwDesiredAccess == 0)                          { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported
  if (lpSecurityAttributes != NULL)                  { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported
  if (dwFlagsAndAttributes != FILE_ATTRIBUTE_NORMAL) { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported
  if (hTemplateFile != NULL)                         { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported

  const size_t openModeStrMaxLen = 2; // "w+" for example

  char   openMode[openModeStrMaxLen + 1];
  size_t openModePos = 0;

  memset(openMode,0,sizeof(openMode));

  if (dwDesiredAccess & GENERIC_WRITE)
  {
    openMode[openModePos++] = 'w';

    if (dwDesiredAccess & GENERIC_READ)
    {
      openMode[openModePos++] = '+';
    }
  }
  else if (dwDesiredAccess & GENERIC_READ)
  {
    openMode[openModePos++] = 'r';
  }

  bool fileExists = PathFileExistsA(lpFileName);

  if (!fileExists && (dwCreationDisposition == OPEN_EXISTING || dwCreationDisposition == TRUNCATE_EXISTING))
  {
    SetLastError(ERROR_FILE_NOT_FOUND);

    return INVALID_HANDLE_VALUE;
  }
  else if (fileExists && dwCreationDisposition == CREATE_NEW)
  {
    SetLastError(ERROR_FILE_EXISTS);

    return INVALID_HANDLE_VALUE;
  }

  FILE *stream = fopen(lpFileName,openMode);

  if (stream != NULL)
  {
    if (dwCreationDisposition == CREATE_ALWAYS ||
        dwCreationDisposition == OPEN_ALWAYS)
    {
      SetLastError(fileExists ? ERROR_ALREADY_EXISTS : 0);
    }

    return stream;
  }
  else
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return INVALID_HANDLE_VALUE;
  }
}

extern HANDLE WINAPI CreateFileW(_In_     LPCWSTR               lpFileName,
                                 _In_     DWORD                 dwDesiredAccess,
                                 _In_     DWORD                 dwShareMode,
                                 _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 _In_     DWORD                 dwCreationDisposition,
                                 _In_     DWORD                 dwFlagsAndAttributes,
                                 _In_opt_ HANDLE                hTemplateFile)
{
  NanoWin::WStrToStrClone lpFileNameMb(lpFileName);

  return(CreateFileA(lpFileNameMb.c_str(),dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile));
}

extern BOOL WINAPI CloseFileHandle(_In_ HANDLE hObject)
{
  if (fclose((FILE*)hObject) == 0)
  {
    return TRUE;
  }
  else
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
}

extern BOOL WINAPI WriteFile(_In_        HANDLE       hFile,
                             _In_        LPCVOID      lpBuffer,
                             _In_        DWORD        nNumberOfBytesToWrite,
                             _Out_opt_   LPDWORD      lpNumberOfBytesWritten,
                             _Inout_opt_ LPOVERLAPPED lpOverlapped)
{
  *lpNumberOfBytesWritten = 0;

  if (lpOverlapped != NULL) { SetLastError(ERROR_INVALID_PARAMETER); return FALSE; } // not supported

  *lpNumberOfBytesWritten = fwrite(lpBuffer,1,nNumberOfBytesToWrite,(FILE*)hFile);

  if (*lpNumberOfBytesWritten < nNumberOfBytesToWrite)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

extern BOOL WINAPI ReadFile(_In_        HANDLE       hFile,
                            _Out_       LPVOID       lpBuffer,
                            _In_        DWORD        nNumberOfBytesToRead,
                            _Out_opt_   LPDWORD      lpNumberOfBytesRead,
                            _Inout_opt_ LPOVERLAPPED lpOverlapped)
{
  *lpNumberOfBytesRead = 0;

  if (lpOverlapped != NULL) { SetLastError(ERROR_INVALID_PARAMETER); return FALSE; } // not supported

  *lpNumberOfBytesRead = fread(lpBuffer,1,nNumberOfBytesToRead,(FILE*)hFile);

  if (*lpNumberOfBytesRead < nNumberOfBytesToRead && ferror((FILE*)hFile))
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

extern DWORD WINAPI SetFilePointer(_In_        HANDLE hFile,
                                   _In_        LONG   lDistanceToMove,
                                   _Inout_opt_ PLONG  lpDistanceToMoveHigh,
                                   _In_        DWORD  dwMoveMethod)
{
  off_t offset = lpDistanceToMoveHigh == NULL ? lDistanceToMove : (off_t)(((ULONGLONG)(*lpDistanceToMoveHigh) << 32) | (ULONGLONG)(lDistanceToMove));
  int   whence = 0;

  if      (dwMoveMethod == FILE_BEGIN)
  {
    whence = SEEK_SET;
  }
  else if (dwMoveMethod == FILE_CURRENT)
  {
    whence = SEEK_CUR;
  }
  else if (dwMoveMethod == FILE_END)
  {
    whence = SEEK_END;
  }
  else
  {
    SetLastError(ERROR_INVALID_PARAMETER);

    return INVALID_SET_FILE_POINTER;
  }

  if (fseeko((FILE*)hFile, offset, whence) != 0)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return INVALID_SET_FILE_POINTER;
  }
  
  off_t pos = ftello((FILE*)hFile);

  if (pos == (off_t)-1)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return INVALID_SET_FILE_POINTER;
  }

  if (pos > 0xFFFFFFFFLL)
  {
    if (lpDistanceToMoveHigh == NULL)
    {
      SetLastError(ERROR_INVALID_PARAMETER); // not sure what error code should be used here

      return INVALID_SET_FILE_POINTER;
    }
    else
    {
      SetLastError(0);

      *lpDistanceToMoveHigh = (LONG)(pos >> 32);

      return (DWORD)(pos & 0xFFFFFFFFU);
    }
  }
  else
  {
    SetLastError(0);

    if (lpDistanceToMoveHigh != NULL)
    {
      *lpDistanceToMoveHigh = 0;
    }

    return (DWORD)pos;
  }
}

extern BOOL WINAPI FlushFileBuffers(_In_ HANDLE hFile)
{
  if (fflush((FILE*)hFile) == 0)
  {
    return TRUE;
  }
  else
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
}

extern DWORD WINAPI GetFileSize(_In_ HANDLE hFile, _Out_opt_ LPDWORD lpFileSizeHigh)
{
  struct stat fileInfo;

  if (fstat(fileno((FILE*)hFile), &fileInfo) == 0)
  {
    if (lpFileSizeHigh != NULL)
    {
      *lpFileSizeHigh = fileInfo.st_size >> 32;
    }

    return (DWORD)(fileInfo.st_size && 0xFFFFFFFFU);
  }
  else
  {
    if (lpFileSizeHigh != NULL)
    {
      *lpFileSizeHigh = 0; // it is not clear from documentation if it must be set to zero
    }

    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return INVALID_FILE_SIZE;
  }
}

NW_EXTERN_C_END
