// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Module functions

#include "NanoWinModule.h"

#include "NanoWinError.h"
#include "NanoWinStrConvert.h"

#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>

NW_EXTERN_C_BEGIN

extern HANDLE  WINAPI GetCurrentProcess()
{
  return(NW_HANDLE_CURR_PROCCESS);
}

extern DWORD   WINAPI GetCurrentProcessId(void)
{
  return(getpid());
}

extern HMODULE WINAPI GetModuleHandleA(LPCSTR_NULLONLY  lpModuleName)
{
  if (lpModuleName == NULL)
  {
    return(GetCurrentProcess());
  }

  return(NULL);
}

extern HMODULE WINAPI GetModuleHandleW(LPCWSTR_NULLONLY lpModuleName)
{
  if (lpModuleName == NULL)
  {
    return(GetCurrentProcess());
  } 

  return(NULL);
}

extern DWORD WINAPI   GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ LPSTR lpFilename, _In_ DWORD nSize)
{
  if (hModule != NULL)
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  ssize_t fileNameLen = readlink("/proc/self/exe",lpFilename,nSize);

  if (fileNameLen >= 0)
  {
    if ((DWORD)fileNameLen < nSize)
    {
      lpFilename[fileNameLen] = '\0';

      return fileNameLen;
    }
    else
    {
      if (nSize > 0)
      {
        lpFilename[nSize - 1] = '\0';
      }

      NanoWinSetLastError(ERROR_INSUFFICIENT_BUFFER);

      return nSize;
    }
  }
  else
  {
    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));

    return 0;
  }
}

extern DWORD WINAPI   GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ LPWSTR lpFilename, _In_ DWORD nSize)
{
  if (hModule != NULL)
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  char  mbBufferStatic[256];
  char *mbBufferDynamic = NULL;

  ssize_t mbBufferSize = sizeof(mbBufferStatic);

  ssize_t fileNameLen      = readlink("/proc/self/exe",mbBufferStatic,mbBufferSize - 1);
  bool    allocationFailed = false;

  while (fileNameLen >= mbBufferSize - 1)
  {
    mbBufferSize *= 2;

    char *tempBuffer = mbBufferDynamic;

    tempBuffer = (char*)realloc(mbBufferDynamic,mbBufferSize);

    if (tempBuffer != NULL)
    {
      mbBufferDynamic = tempBuffer;
      fileNameLen     = readlink("/proc/self/exe",mbBufferDynamic,mbBufferSize - 1);
    }
    else
    {
      allocationFailed = true;
    }
  }

  if      (allocationFailed)
  {
    fileNameLen = 0;

    NanoWinSetLastError(ERROR_NOT_ENOUGH_MEMORY);
  }
  else if (fileNameLen < 0)
  {
    fileNameLen = 0;

    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));
  }
  else
  {
    char *mbBuffer = mbBufferDynamic == NULL ? mbBufferStatic : mbBufferDynamic;

    mbBuffer[fileNameLen] = '\0';

    NanoWin::StrToWStrClone wideBuffer(mbBuffer);

    if (wideBuffer.is_valid())
    {
      size_t requiredLen = wcslen(wideBuffer.c_str());

      if (requiredLen < nSize)
      {
        wcscpy(lpFilename,wideBuffer.c_str());

        fileNameLen = requiredLen;
      }
      else
      {
        if (nSize > 0)
        {
          memcpy(lpFilename,wideBuffer.c_str(),sizeof(WCHAR) * (nSize - 1));
          lpFilename[nSize - 1] = L'\0';
        }

        fileNameLen = nSize;

        NanoWinSetLastError(ERROR_INSUFFICIENT_BUFFER);
      }
    }
    else
    {
      fileNameLen = 0;

      NanoWinSetLastError(ERROR_NO_UNICODE_TRANSLATION);
    }
  }

  if (mbBufferDynamic != NULL)
  {
    free(mbBufferDynamic);
  }

  return fileNameLen;
}

// Dlls

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL. (Use GetLastError)
extern HMODULE WINAPI LoadLibraryA(_In_ LPCSTR  lpFileName)
{
  if (lpFileName == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  int iLoadFlags = RTLD_NOW; // RTLD_LAZY
  HMODULE result = dlopen(lpFileName, iLoadFlags);
  // if (result == NULL) { printf("Dl error : %s\n", dlerror()); } // DEBUG
  if (result == NULL) { SetLastError(ERROR_INVALID_PARAMETER); } // TODO: (not critical) Provide more details leter (somehow?)
  return(result);
}

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL (use GetLastError to get error code)
extern HMODULE WINAPI LoadLibraryW(_In_ LPCWSTR lpFileName)
{
  if (lpFileName == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  try
  {
    std::string mbFileName = NanoWin::StrConverter::Convert(lpFileName);
    return LoadLibraryA(mbFileName.c_str());
  }
  catch (NanoWin::StrConverter::Error)
  {
    SetLastError(ERROR_NO_UNICODE_TRANSLATION);
    return FALSE;
  }
}

// Frees the loaded dynamic-link library
extern BOOL    WINAPI FreeLibrary(_In_ HMODULE hModule)
{
  if (hModule    == NULL)    { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(FALSE); }
  if (dlclose(hModule) != 0) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); return(FALSE); }
  return(TRUE);
}

// Retrieves the address of an exported function or variable from the specified dynamic-link library
// If the function fails, the return value is NULL (use GetLastError to get error code)
// Note: The only version exists in Win32 is ASCII?
extern FARPROC WINAPI GetProcAddress(_In_ HMODULE hModule, _In_ LPCSTR lpProcName)
{
  if (hModule    == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  if (lpProcName == NULL) { SetLastError(NW_DEFAULT_ERROR_AT_FAIL); return(NULL); }
  PVOID result = dlsym(hModule, lpProcName);
  if (result == NULL) { SetLastError(NanoWinErrorByErrnoAtFail(errno)); }
  return(result);
}

NW_EXTERN_C_END
