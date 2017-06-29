// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Error suuport functions and constants

#include "NanoWinOther.h"
#include "NanoWinError.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

typedef struct
{
  DWORD          errorCode;
  const char    *errorMessageA;
  const wchar_t *errorMessageW;
} NanoWinErrorMessageTableEntry;

NW_EXTERN_C_BEGIN

static __thread DWORD LastError = ERROR_SUCCESS;

static char    NanoWinErrorMessageGenericErrorFormatA[] = "Generic error (%u)";
static wchar_t NanoWinErrorMessageGenericErrorFormatW[] = L"Generic error (%u)";

static NanoWinErrorMessageTableEntry NanoWinErrorMessageTable[]
{
  { ERROR_SUCCESS, "OK, no error", L"OK, no error" },
  { ERROR_INVALID_FUNCTION, "Incorrect function", L"Incorrect function" },
  { ERROR_FILE_NOT_FOUND, "The system cannot find the file specified", L"The system cannot find the file specified" },
  { ERROR_PATH_NOT_FOUND, "The system cannot find the path specified", L"The system cannot find the path specified" },
  { ERROR_TOO_MANY_OPEN_FILES, "The system cannot open the file", L"The system cannot open the file" },
  { ERROR_ACCESS_DENIED, "Access is denied", L"Access is denied" },
  { ERROR_INVALID_HANDLE, "The handle is invalid", L"The handle is invalid" },
  { ERROR_NOT_ENOUGH_MEMORY, "Not enough storage is available to process this command", L"Not enough storage is available to process this command" },
  { ERROR_INVALID_DATA, "The data is invalid", L"The data is invalid" },
  { ERROR_SHARING_VIOLATION, "The process cannot access the file because it is being used by another process", L"The process cannot access the file because it is being used by another process" },
  { ERROR_NOT_SUPPORTED, "The request is not supported", L"The request is not supported" },
  { ERROR_FILE_EXISTS, "The file exists", L"The file exists" },
  { ERROR_INVALID_PARAMETER, "The parameter is incorrect", L"The parameter is incorrect" },
  { ERROR_INSUFFICIENT_BUFFER, "The data area passed to a system call is too small", L"The data area passed to a system call is too small" },
  { ERROR_ALREADY_EXISTS, "Cannot create a file when that file already exists", L"Cannot create a file when that file already exists" },
  { ERROR_ENVVAR_NOT_FOUND, "The system could not find the environment option that was entered", L"The system could not find the environment option that was entered" },
  { ERROR_FILENAME_EXCED_RANGE, "The filename or extension is too long", L"The filename or extension is too long" },
  { ERROR_INVALID_FLAGS, "Invalid flags", L"Invalid flags" },
  { ERROR_NO_UNICODE_TRANSLATION, "No mapping for the Unicode character exists in the target multi-byte code page", L"No mapping for the Unicode character exists in the target multi-byte code page" },
  { ERROR_RESOURCE_LANG_NOT_FOUND, "The specified resource language ID cannot be found in the image file", L"The specified resource language ID cannot be found in the image file" },
};

static NanoWinErrorMessageTableEntry *NanoWinErrorMessageTableFindEntry(DWORD errorCode)
{
  bool               found = false;
  unsigned int       index = 0;
  const unsigned int size  = sizeof(NanoWinErrorMessageTable) / sizeof(NanoWinErrorMessageTable[0]);

  for (index = 0; !found && index < size; index++)
  {
    if (NanoWinErrorMessageTable[index].errorCode == errorCode)
    {
      found = true;
    }
  }

  return found ? &NanoWinErrorMessageTable[index - 1] : NULL;
}

extern  DWORD NanoWinGetLastError(void)
{
  return(LastError);
}

extern  DWORD NanoWinSetLastError  // Set last error code (Returns dwErrCode)
              (
                DWORD  dwErrCode
              )
{
  LastError = dwErrCode;
  return(dwErrCode);
}

extern  DWORD NanoWinSetLastErrorEx // Set last error code (Returns dwErrCode)
              (
                DWORD  dwErrCode,
                DWORD  dwType     // Ignored (as in Win32)
              )
{
  return(NanoWinSetLastError(dwErrCode));
}

// Converts errno to Win32Error, converts errno=0 to specified value win32 error value
extern  DWORD NanoWinErrorByErrnoRaw (errno_t errno_value, DWORD winErrorForZeroErrno)
{
  #define CASE_RETURN(errno_val,win32_error_val) case(errno_val): { return(win32_error_val); } break;

  switch(errno_value)
  {
    // Success
    case(0): { return(winErrorForZeroErrno); } break;

    #if defined(ENOMEM)
    CASE_RETURN(ENOMEM,ERROR_NOT_ENOUGH_MEMORY);
    #endif

    #if defined(EINVAL)
    CASE_RETURN(EINVAL,ERROR_INVALID_DATA); // ?
    #endif

    #if defined(EACCES)
    CASE_RETURN(EACCES,ERROR_ACCESS_DENIED);
    #endif

    #if defined(ENFILE)
    CASE_RETURN(ENFILE,ERROR_TOO_MANY_OPEN_FILES);
    #endif

    #if defined(EMFILE)
    CASE_RETURN(EMFILE,ERROR_TOO_MANY_OPEN_FILES); // ?
    #endif

    #if defined(ENOENT)
    CASE_RETURN(ENOENT,ERROR_FILE_NOT_FOUND); // ERROR_PATH_NOT_FOUND ?
    #endif

    // Unknown errno
	default:
    {
      // return(NW_DEFAULT_ERROR_AT_FAIL); // will be at end of function
    }
  }

  return(NW_DEFAULT_ERROR_AT_FAIL);
  #undef CASE_RETURN
}

#define NW_DEFAULT_ERROR_AT_FAIL   ERROR_INVALID_DATA

// Converts errno to Win32Error, converts errno=0 to NW_DEFAULT_ERROR_AT_FAIL(ERROR_INVALID_DATA)
extern  DWORD NanoWinErrorByErrnoAtFail(errno_t errno_value)
{
  return NanoWinErrorByErrnoRaw (errno_value, NW_DEFAULT_ERROR_AT_FAIL);
}

// constants for unsupported FormatMessage flags defined here to check
// for unsupported modes
#ifndef FORMAT_MESSAGE_ARGUMENT_ARRAY
 #define FORMAT_MESSAGE_ARGUMENT_ARRAY (0x00002000)
#endif

#ifndef FORMAT_MESSAGE_FROM_HMODULE
 #define FORMAT_MESSAGE_FROM_HMODULE   (0x00000800)
#endif

#ifndef FORMAT_MESSAGE_FROM_STRING
 #define FORMAT_MESSAGE_FROM_STRING    (0x00000400)
#endif

extern DWORD WINAPI FormatMessageA(_In_     DWORD   dwFlags,
                                   _In_opt_ LPCVOID lpSource,
                                   _In_     DWORD   dwMessageId,
                                   _In_     DWORD   dwLanguageId,
                                   _Out_    LPSTR   lpBuffer,
                                   _In_     DWORD   nSize,
                                   _In_opt_ va_list *Arguments)
{
  if (((dwFlags & FORMAT_MESSAGE_ARGUMENT_ARRAY) != 0) ||
      ((dwFlags & FORMAT_MESSAGE_FROM_HMODULE) != 0) ||
      ((dwFlags & FORMAT_MESSAGE_FROM_STRING) != 0))
  {
    SetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  if ((dwFlags & FORMAT_MESSAGE_FROM_SYSTEM) == 0)
  {
    SetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  if (dwLanguageId != MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
  {
    SetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  NanoWinErrorMessageTableEntry *messageInfo = NanoWinErrorMessageTableFindEntry(dwMessageId);

  size_t messageLen = 0;

  if (messageInfo != NULL)
  {
    messageLen = strlen(messageInfo->errorMessageA);
  }
  else
  {
    // since there is no string or char conversion, we can safely assume that snprintf can't return error
    messageLen = (size_t)snprintf(NULL,0,NanoWinErrorMessageGenericErrorFormatA,(unsigned int)dwMessageId);
  }

  size_t requiredSize = messageLen + 1;
  char  *destPtr      = NULL;

  if ((dwFlags & FORMAT_MESSAGE_ALLOCATE_BUFFER) != 0)
  {
    if (requiredSize < nSize)
    {
      requiredSize = nSize;
    }

    destPtr = (char*)LocalAlloc(LMEM_FIXED,requiredSize);

    if (destPtr != NULL)
    {
      *((char**)lpBuffer) = destPtr;
    }
    else
    {
      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    }
  }
  else
  {
    if (nSize >= requiredSize)
    {
      destPtr = lpBuffer;
    }
    else
    {
      SetLastError(ERROR_INSUFFICIENT_BUFFER);
    }
  }

  if (destPtr != NULL)
  {
    if (messageInfo != NULL)
    {
      strcpy(destPtr,messageInfo->errorMessageA);
    }
    else
    {
      snprintf(destPtr,requiredSize,NanoWinErrorMessageGenericErrorFormatA,(unsigned int)dwMessageId);
    }

    return messageLen;
  }
  else
  {
    return 0;
  }
}

extern DWORD WINAPI FormatMessageW(_In_     DWORD   dwFlags,
                                   _In_opt_ LPCVOID lpSource,
                                   _In_     DWORD   dwMessageId,
                                   _In_     DWORD   dwLanguageId,
                                   _Out_    LPWSTR  lpBuffer,
                                   _In_     DWORD   nSize,
                                   _In_opt_ va_list *Arguments)
{
  if (((dwFlags & FORMAT_MESSAGE_ARGUMENT_ARRAY) != 0) ||
      ((dwFlags & FORMAT_MESSAGE_FROM_HMODULE) != 0) ||
      ((dwFlags & FORMAT_MESSAGE_FROM_STRING) != 0))
  {
    SetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  if ((dwFlags & FORMAT_MESSAGE_FROM_SYSTEM) == 0)
  {
    SetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  if (dwLanguageId != MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
  {
    SetLastError(ERROR_NOT_SUPPORTED);

    return 0;
  }

  NanoWinErrorMessageTableEntry *messageInfo = NanoWinErrorMessageTableFindEntry(dwMessageId);

  size_t messageLen;

  if (messageInfo != NULL)
  {
    messageLen = wcslen(messageInfo->errorMessageW);
  }
  else
  {
    // since there is no string or char conversion, we can safely assume that swprintf can't return error
    messageLen = (size_t)swprintf(NULL,0,NanoWinErrorMessageGenericErrorFormatW,(unsigned int)dwMessageId);
  }

  size_t requiredSize = messageLen + 1;
  wchar_t *destPtr    = 0;

  if ((dwFlags & FORMAT_MESSAGE_ALLOCATE_BUFFER) != 0)
  {
    if (requiredSize < nSize)
    {
      requiredSize = nSize;
    }

    destPtr = (wchar_t*)LocalAlloc(LMEM_FIXED,requiredSize * sizeof(wchar_t));

    if (destPtr != NULL)
    {
      *((wchar_t**)lpBuffer) = destPtr;
    }
    else
    {
      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    }
  }
  else
  {
    if (nSize >= requiredSize)
    {
      destPtr = lpBuffer;
    }
    else
    {
      SetLastError(ERROR_INSUFFICIENT_BUFFER);
    }
  }

  if (destPtr != NULL)
  {
    if (messageInfo != NULL)
    {
      wcscpy(destPtr,messageInfo->errorMessageW);
    }
    else
    {
      swprintf(destPtr,requiredSize,NanoWinErrorMessageGenericErrorFormatW,(unsigned int)dwMessageId);
    }

    return messageLen;
  }
  else
  {
    return 0;
  }
}

NW_EXTERN_C_END
