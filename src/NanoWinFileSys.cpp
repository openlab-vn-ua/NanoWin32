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
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
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
        *requiredBufferSize = static_cast<DWORD>(strlen(buffer) + 1);
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

      result = static_cast<DWORD>(strlen(lpBuffer));
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

          result = static_cast<DWORD>(wideCurrDirLen);
        }
        else
        {
          result = static_cast<DWORD>(wideCurrDirLen + 1);
        }
      }
      else
      {
        result = static_cast<DWORD>(wideCurrDirLen + 1);
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

static FILE *OpenFileForCopySrc(const char *fileName)
{
  FILE *result = fopen(fileName,"rb");

  if (result == NULL)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));
  }

  return result;
}

static FILE *OpenFileForCopyDst(const char *fileName, bool failIfExists)
{
  FILE *result = NULL;

  if (failIfExists)
  {
    // use the restrictive mode (owner read-write only) here assuming that after
    // copying file content its mode will be copied from source file 
    int handle = open(fileName,O_EXCL | O_CREAT, S_IRUSR | S_IWUSR);

    if (handle >= 0)
    {
      result = fdopen(handle,"wb");

      if (result == NULL)
      {
        close(handle);
      }
    }
  }
  else
  {
   result = fopen(fileName,"wb");
  }

  if (result == NULL)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));
  }

  return result;
}

static bool CopyFileContent(FILE *dstFile, FILE *srcFile)
{
  constexpr size_t COPY_BLOCK_SIZE = 32 * 1024;

  unsigned char buffer[COPY_BLOCK_SIZE];

  bool done = false;
  bool ok   = true;

  while (ok && !done)
  {
    size_t read_size = fread(buffer,1,sizeof(buffer),srcFile);

    if (read_size < sizeof(buffer))
    {
      ok = ferror(srcFile) == 0;
    }

    if (ok && read_size > 0)
    {
      if (fwrite(buffer, 1, read_size, dstFile) < read_size)
      {
        ok = false;
      }
    }
    else
    {
      done = read_size == 0;
    }
  }

  if (!ok)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));
  }

  return ok;
}

static bool CopyFileAttributes(const char *dstFileName, const char *srcFileName)
{
  struct stat fileStat;

  bool ok = false;

  if (stat(srcFileName,&fileStat) == 0)
  {
    ok = chown(dstFileName, fileStat.st_uid, fileStat.st_gid) == 0;
    ok = chmod(dstFileName, fileStat.st_mode & (~S_IFMT)) && ok;
  }

  return ok;
}

extern BOOL  WINAPI CopyFileA (_In_ LPCSTR  lpExistingFileName, _In_ LPCSTR  lpNewFileName, _In_ BOOL bFailIfExists)
{
  BOOL  ok = FALSE;
  FILE *srcFileStream = OpenFileForCopySrc(lpExistingFileName);

  if (srcFileStream != NULL)
  {
    FILE *dstFileStream = OpenFileForCopyDst(lpNewFileName,bFailIfExists);

    if (dstFileStream != NULL)
    {
      ok = CopyFileContent(dstFileStream,srcFileStream);

      if (fclose(dstFileStream) != 0)
      {
        if (ok)
        {
          ok = FALSE; // FIXME: set last error
          SetLastError(NanoWinErrorByErrnoAtFail(errno));
        }
      }
    }

    fclose(srcFileStream);
  }

  if (ok)
  {
    CopyFileAttributes(lpNewFileName,lpExistingFileName);
  }

  return(ok);
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
  else if (S_ISREG(st.st_mode))
  {
    if ((st.st_mode & S_IWUSR) == 0) // Valid on files only, because absense of "write" permission on directory means different thing
    {
      result |= FILE_ATTRIBUTE_READONLY;
    }
    else
    {
      result = FILE_ATTRIBUTE_NORMAL;
    }
  }
  else
  {
    result = 0; // Keep it zero for system files, devices, etc
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

extern BOOL  SetFileAttributesA(_In_ LPCSTR lpFileName, _In_ DWORD dwFileAttributes)
{
  struct stat st;

  if (stat(lpFileName, &st) != 0)
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));
    return(FALSE);
  }

  if (!S_ISREG(st.st_mode))
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return(FALSE);
  }
  else
  {
    if (dwFileAttributes == FILE_ATTRIBUTE_NORMAL ||
        dwFileAttributes == 0 ||
        dwFileAttributes == FILE_ATTRIBUTE_READONLY)
    {
      mode_t oldMode = st.st_mode;
      mode_t newMode;

      if (dwFileAttributes & FILE_ATTRIBUTE_READONLY)
      {
        newMode  = oldMode & (~S_IWUSR);
        newMode &= ~S_IWGRP;
        newMode &= ~S_IWOTH;
      }
      else
      {
        newMode = oldMode | S_IWUSR; // enable "write" permissions for file owner only
      }

      if (newMode != oldMode)
      {
        if (chmod(lpFileName, newMode) == 0)
        {
          return TRUE;
        }
        else
        {
          SetLastError(NanoWinErrorByErrnoAtFail(errno));
          return(FALSE);
        }
      }
      else
      {
        return TRUE;
      }
    }
    else
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return(FALSE);
    }
  }
}

extern BOOL  SetFileAttributesW (_In_ LPCWSTR lpFileName, _In_ DWORD dwFileAttributes)
{
  try
  {
    std::string mbPathName = NanoWin::StrConverter::Convert(lpFileName);
    return SetFileAttributesA(mbPathName.c_str(),dwFileAttributes);
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    return FALSE;
  }
}

extern BOOL WINAPI  MoveFileA(_In_ LPCSTR  lpExistingFileName, _In_ LPCSTR  lpNewFileName)
{
  if (rename(lpExistingFileName, lpNewFileName) == 0)
  {
    return TRUE;
  }
  else
  {
    SetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
}

extern BOOL WINAPI  MoveFileW(_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName)
{
  try
  {
    std::string lpExistingFileNameMb = NanoWin::StrConverter::Convert(lpExistingFileName);
    std::string lpNewFileNameMb = NanoWin::StrConverter::Convert(lpNewFileName);

    return MoveFileA(lpExistingFileNameMb.c_str(),lpNewFileNameMb.c_str());
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);

    return FALSE;
  }
}

NW_EXTERN_C_END
