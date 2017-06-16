// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// File functions

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "NanoWinFile.h"
#include "NanoWinError.h"
#include "NanoWinFileSys.h"
#include "NanoWinStrConvert.h"

#ifdef __GNUC__
 #define open_nw    open64
 #define fseeko_nw  fseeko64
 #define ftello_nw  ftello64
 #define fstat_nw   fstat64

 #define off_nw_t   off64_t
 #define stat_nw_t  stat64
#else
 #define open_nw    open
 #define fseeko_nw  fseeko
 #define ftello_nw  ftello
 #define fstat_nw   fstat

 #define off_nw_t   off_t
 #define stat_nw_t  stat
#endif

NW_EXTERN_C_BEGIN

extern HANDLE WINAPI CreateFileA(_In_     LPCSTR                lpFileName,
                                 _In_     DWORD                 dwDesiredAccess,
                                 _In_     DWORD                 dwShareMode,
                                 _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 _In_     DWORD                 dwCreationDisposition,
                                 _In_     DWORD                 dwFlagsAndAttributes,
                                 _In_opt_ HANDLE                hTemplateFile)
{
  if (lpSecurityAttributes != NULL) { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported
  if (hTemplateFile != NULL)        { SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; } // not supported

  int openFlags = 0;
  bool alreadyExists = false; // is set and checked for CREATE_ALWAYS and OPEN_ALWAYS only for setting last error (non-atomic)

  switch (dwCreationDisposition)
  {
    case (CREATE_NEW) :
    {
      openFlags |= O_CREAT | O_EXCL;
    } break;

    case (CREATE_ALWAYS) :
    {
      openFlags |= O_CREAT | O_TRUNC;
      alreadyExists = PathFileExistsA(lpFileName);
    } break;

    case (OPEN_EXISTING) :
    {
      // no special flags required
    } break;

    case (OPEN_ALWAYS) :
    {
      openFlags |= O_CREAT;
      alreadyExists = PathFileExistsA(lpFileName);
    } break;

    case (TRUNCATE_EXISTING) :
    {
      if ((dwDesiredAccess & GENERIC_WRITE) == 0)
      {
        SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; 
      }

      openFlags |= O_TRUNC;
    } break;

    default :
    {
     SetLastError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; 
    }
  }

  const char *openMode;

  if (dwDesiredAccess & GENERIC_WRITE)
  {
    if (dwDesiredAccess & GENERIC_READ)
    {
      openFlags |= O_RDWR;
      openMode   = "r+";
    }
    else
    {
      openFlags |= O_WRONLY;
      openMode   = "w";
    }
  }
  else
  {
    openFlags |= O_RDONLY;
    openMode   = "r";
  }

  int permissions;

  if (dwFlagsAndAttributes & FILE_ATTRIBUTE_READONLY)
  {
    permissions = S_IRUSR | S_IRGRP | S_IROTH;
  }
  else // using FILE_ATTRIBUTE_NORMAL by default
  {
    permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  }

  int fileHandle = open_nw(lpFileName,openFlags,permissions);

  if (fileHandle == -1)
  {
    if (errno == EEXIST)
    {
      SetLastError(ERROR_FILE_EXISTS);
    }
    else if (errno == ENOENT)
    {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
    else
    {
      SetLastError(NanoWinErrorByErrnoAtFail(errno));
    }

    return INVALID_HANDLE_VALUE;
  }

  if (flock(fileHandle, (dwShareMode == 0 ? LOCK_EX : LOCK_SH) | LOCK_NB) != 0)
  {
    if (errno == EWOULDBLOCK)
    {
      SetLastError(ERROR_SHARING_VIOLATION);
    }
    else
    {
      SetLastError(NanoWinErrorByErrnoAtFail(errno));
    }

    close(fileHandle);

    return INVALID_HANDLE_VALUE;
  }

  FILE *stream = fdopen(fileHandle,openMode);

  if (stream == NULL)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    close(fileHandle);

    return INVALID_HANDLE_VALUE;
  }

  if (dwCreationDisposition == CREATE_ALWAYS ||
      dwCreationDisposition == OPEN_ALWAYS)
  {
   SetLastError(alreadyExists ? ERROR_ALREADY_EXISTS : ERROR_SUCCESS);
  }

  return stream;
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

  *lpNumberOfBytesWritten = static_cast<DWORD>(fwrite(lpBuffer,1,nNumberOfBytesToWrite,(FILE*)hFile));

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

  *lpNumberOfBytesRead = static_cast<DWORD>(fread(lpBuffer,1,nNumberOfBytesToRead,(FILE*)hFile));

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
  off_nw_t offset = lpDistanceToMoveHigh == NULL ? lDistanceToMove : (off_nw_t)(((ULONGLONG)(*lpDistanceToMoveHigh) << 32) | (ULONGLONG)(lDistanceToMove));
  int     whence = 0;

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

  if (fseeko_nw((FILE*)hFile, offset, whence) != 0)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return INVALID_SET_FILE_POINTER;
  }
  
  off_nw_t pos = ftello_nw((FILE*)hFile);

  if (pos == (off_nw_t)-1)
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
  struct stat_nw_t fileInfo;

  if (fstat_nw(fileno((FILE*)hFile), &fileInfo) == 0)
  {
    if (lpFileSizeHigh != NULL)
    {
      *lpFileSizeHigh = (DWORD)(fileInfo.st_size >> 32);
    }

    return (DWORD)(fileInfo.st_size & 0xFFFFFFFFU);
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
