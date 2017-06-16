// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileFind functions

#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "NanoWinInternal.h"
#include "NanoWinStrConvert.h"
#include "NanoWinMsSafeString.h"
#include "NanoWinFileFind.h"

#include "NanoWinError.h"

typedef struct
{
  DIR  *dirHandle;
  char *dirName;
  char *fileMask;
} NanoWinFileFindHandle;

typedef struct
{
  bool isWideVersion;

  union
  {
    WIN32_FIND_DATAA *multiByteData;
    WIN32_FIND_DATAW *wideCharData;
  };
} NanoWinFileFindData;

#define NanoWinFindFileDirSeparatorChar ('/')

NW_EXTERN_C_BEGIN

//NOTE: result string can be overwritten by consequent calls, so
//      must be copied somewhere before next call to this function
static char *FilePathGetDirectory(const char *fullName)
{
  char *clonedFullName = strdup(fullName);

  if (clonedFullName == NULL)
  {
    return NULL;
  }

  char *directory = dirname(clonedFullName);

  if (directory != NULL)
  {
    directory = strdup(directory);
  }

  free(clonedFullName);

  return directory;
}

//NOTE: result string can be overwritten by consequent calls, so
//      must be copied somewhere before next call to this function
static char *FilePathGetFileName(const char *fullName)
{
  char *clonedFullName = strdup(fullName);

  if (clonedFullName == NULL)
  {
    return NULL;
  }

  char *fileName = basename(clonedFullName);

  if (fileName != NULL)
  {
    fileName = strdup(fileName);
  }

  free(clonedFullName);

  return fileName;
}

static NanoWinFileFindHandle *NanoWinFindFileOpenHandle(char *dirName, char *fileMask)
{
  bool ok = true;
  NanoWinFileFindHandle *handle = (NanoWinFileFindHandle*)malloc(sizeof(NanoWinFileFindHandle));

  if (handle == NULL)
  {
    ok = false;

    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  }

  if (ok)
  {
    handle->dirHandle = opendir(dirName);

    if (handle->dirHandle == NULL)
    {
      ok = false;

      SetLastError(NanoWinErrorByErrnoAtFail(errno));
    }
  }

  if (ok)
  {
    handle->dirName = dirName;
    handle->fileMask = fileMask;
  }
  else
  {
    free(dirName);
    free(fileMask);

    if (handle != NULL)
    {
      if (handle->dirHandle != NULL)
      {
        closedir(handle->dirHandle);
      }

      free(handle);

      handle = NULL;
    }
  }

  return handle;
}

static void NanoWinFindFileCloseHandle(NanoWinFileFindHandle *handle)
{
  if (handle != NULL)
  {
    if (handle->dirHandle != NULL)
    {
      closedir(handle->dirHandle);
    }

    if (handle->dirName != NULL)
    {
      free(handle->dirName);
    }

    if (handle->fileMask != NULL)
    {
      free(handle->fileMask);
    }

    free(handle);
  }
}

static bool NanoWinJoinDirAndFileName(char *fullFileName, size_t fullFileNameMaxSize, const char *dirName, const char *fileName)
{
  size_t dirNameLen      = strlen(dirName);
  size_t fileNameLen     = strlen(fileName);
  size_t fullFileNameLen = dirNameLen + 1 + fileNameLen;

  if (fullFileNameLen < fullFileNameMaxSize)
  {
    memcpy(fullFileName, dirName, dirNameLen);
    fullFileName[dirNameLen] = NanoWinFindFileDirSeparatorChar;
    memcpy(&fullFileName[dirNameLen + 1], fileName, fileNameLen);
    fullFileName[fullFileNameLen] = '\0';

    return true;
  }
  else
  {
    return false;
  }
}

static void NanoWinFillFindData(NanoWinFileFindData *data, const char *dirName, struct dirent *entry)
{
  struct stat entryData;
  char        fullFileName[PATH_MAX];

  DWORD fileSizeHigh   = 0;
  DWORD fileSizeLow    = 0;
  DWORD fileAttributes = 0;

  if (NanoWinJoinDirAndFileName(fullFileName, sizeof(fullFileName), dirName, entry->d_name))
  {
    if (stat(fullFileName, &entryData) == 0)
    {
      if (S_ISDIR(entryData.st_mode))
      {
        fileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
      }
      else if (S_ISREG(entryData.st_mode))
      {
        if (sizeof(entryData.st_size) > sizeof(DWORD))
        {
          fileSizeLow  = static_cast<DWORD>(entryData.st_size & 0xFFFFFFFFLU);
          // just to prevent warning here:
          // fileSizeHigh = entryData.st_size >> (sizeof(DWORD) * 8);
          entryData.st_size >>= (sizeof(DWORD)/2 * 8);
          entryData.st_size >>= (sizeof(DWORD)/2 * 8);
          fileSizeHigh = static_cast<DWORD>(entryData.st_size & 0xFFFFFFFFLU);
        }
        else
        {
          fileSizeLow  = static_cast<DWORD>(entryData.st_size);
          fileSizeHigh = 0;
        }

        if ((entryData.st_mode & S_IWUSR) == 0) // Valid on files only, because absense of "write" permission on directory means different thing
        {
          fileAttributes |= FILE_ATTRIBUTE_READONLY;
        }
        else
        {
          fileAttributes = FILE_ATTRIBUTE_NORMAL;
        }
      }
      else
      {
        fileAttributes = 0; // Keep it zero for system files, devices, etc
      }
    }
  }

  if (data->isWideVersion)
  {
    WIN32_FIND_DATAW * wideCharData = data->wideCharData;

    memset(wideCharData, 0, sizeof(*wideCharData));

    NanoWin::StrToWStrClone wideFileName(entry->d_name);

    wcsncpy_s(wideCharData->cFileName, sizeof(wideCharData->cFileName) / sizeof(wideCharData->cFileName[0]),
      wideFileName.c_str(), sizeof(wideCharData->cFileName) / sizeof(wideCharData->cFileName[0]) - 1);

    wideCharData->nFileSizeHigh    = fileSizeHigh;
    wideCharData->nFileSizeLow     = fileSizeLow;
    wideCharData->dwFileAttributes = fileAttributes;
  }
  else
  {
    WIN32_FIND_DATAA * multiByteData = data->multiByteData;

    memset(multiByteData, 0, sizeof(*multiByteData));

    strncpy_s(multiByteData->cFileName, sizeof(multiByteData->cFileName), entry->d_name, sizeof(multiByteData->cFileName) - 1);

    multiByteData->nFileSizeHigh    = fileSizeHigh;
    multiByteData->nFileSizeLow     = fileSizeLow;
    multiByteData->dwFileAttributes = fileAttributes;
  }
}

static bool NanoWinFindFileSeekToNextFile(NanoWinFileFindHandle *handle, NanoWinFileFindData *state)
{
  bool ok    = true;
  bool found = false;

  while (ok && !found)
  {
    errno = 0;

    struct dirent *entry = readdir(handle->dirHandle);

    if (entry != NULL)
    {
      if (NanoWinFileNameMatchMaskA(entry->d_name, handle->fileMask))
      {
        found = true;

        NanoWinFillFindData(state, handle->dirName, entry);
      }
    }
    else
    {
      ok = false;

      if (errno == 0)
      {
        SetLastError(ERROR_FILE_NOT_FOUND);
      }
      else
      {
        SetLastError(NanoWinErrorByErrnoAtFail(errno));
      }
    }
  }

  return found;
}

static HANDLE FindFirstFileUniversal(const char *Mask, NanoWinFileFindData *data)
{
  char *dirName  = FilePathGetDirectory(Mask);
  char *fileMask = FilePathGetFileName(Mask);

  if (dirName == NULL || fileMask == NULL)
  {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    
    if (dirName != NULL)
    {
      free(dirName);
    }

    if (fileMask != NULL)
    {
      free(fileMask);
    }

    return INVALID_HANDLE_VALUE;
  }

  NanoWinFileFindHandle *handle = NanoWinFindFileOpenHandle(dirName,fileMask);

  if (handle != NULL)
  {
    if (!NanoWinFindFileSeekToNextFile(handle, data))
    {
      NanoWinFindFileCloseHandle(handle);

      handle = NULL;
    }
  }

  return handle != NULL ? handle : INVALID_HANDLE_VALUE; 
}

extern HANDLE FindFirstFileA(const char *Mask, WIN32_FIND_DATAA *state)
{ 
  NanoWinFileFindData data;

  data.isWideVersion = false;
  data.multiByteData = state;

  return FindFirstFileUniversal(Mask, &data);
}

extern BOOL   FindNextFileA(HANDLE handle, WIN32_FIND_DATAA *state)
{
  NanoWinFileFindData data;

  data.isWideVersion = false;
  data.multiByteData = state;

  return NanoWinFindFileSeekToNextFile((NanoWinFileFindHandle*)handle, &data);
}

extern HANDLE FindFirstFileW(const wchar_t *Mask, WIN32_FIND_DATAW *state)
{
  NanoWinFileFindData data;

  data.isWideVersion = true;
  data.wideCharData  = state;

  NanoWin::WStrToStrClone wideCharMask(Mask);

  return FindFirstFileUniversal(wideCharMask.c_str(), &data);
}

extern BOOL   FindNextFileW(HANDLE handle, WIN32_FIND_DATAW *state)
{
  NanoWinFileFindData data;

  data.isWideVersion = true;
  data.wideCharData  = state;

  return NanoWinFindFileSeekToNextFile((NanoWinFileFindHandle*)handle, &data);
}

extern BOOL   FindClose(HANDLE hFindFile)
{
  if (hFindFile == NULL || hFindFile == INVALID_HANDLE_VALUE)
  {
    return FALSE;
  }
  else
  {
    NanoWinFindFileCloseHandle((NanoWinFileFindHandle*)hFindFile);
  }

  return TRUE;
}

NW_EXTERN_C_END
