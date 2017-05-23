// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileSys functions

#if !defined(NanoWinFileSysIncluded)
#define NanoWinFileSysIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <limits.h>

// Path name constants
// ------------------------------------------

// Note: under MSWIN _MAX filesystems constants values should include room for trailing '\0'

// _MAX_PATH : Maximum length of full path // Win32:(260) // Linux:4096
#if defined(PATH_MAX)
#define _MAX_PATH   PATH_MAX       // Maximum size of full path (including '\0') // Linux:PATH_MAX includes trailing '\0'
#else
#define _MAX_PATH   (4096)         // Maximum size of full path (including '\0') Some reasonable assumptions
#endif

// _MAX_FNAME : Maximum size of filename component // Linux:255
#if defined(NAME_MAX)
#define _MAX_FNAME  (NAME_MAX+1)  // Maximum size of file name + extension (including '\0') // NAME_MAX not includes '\0', add room for it
#else
#define _MAX_FNAME  PATH_MAX      // Maximum size of file name + extension (including '\0')
#endif

#define _MAX_DIR    _MAX_PATH     // Maximum size of directory component (including '\0') // Win32:(MAX_PATH-12)
#define _MAX_DRIVE  _MAX_FNAME    // Maximum size of drive component (including '\0') // Linux: can be just 1
#define _MAX_EXT    _MAX_FNAME    // Maximum size of {file name} extension component (including '\0') // Win32:(MAX_PATH-4)?

// Maximum size of full path (including '\0')
// Alias w/o undescore
#define MAX_PATH    _MAX_PATH

NW_EXTERN_C_BEGIN

extern BOOL  WINAPI PathFileExistsA (LPCSTR lpPath);
extern BOOL  WINAPI PathFileExistsW (LPCWSTR lpPath);

// Win32 API: Directory stuff
// ------------------------------------------

extern DWORD WINAPI GetCurrentDirectoryA (_In_ DWORD nBufferLength, _Out_ LPSTR lpBuffer);
extern DWORD WINAPI GetCurrentDirectoryW (_In_ DWORD nBufferLength, _Out_ LPWSTR lpBuffer);

extern BOOL  WINAPI CreateDirectoryA (_In_ LPCSTR  lpPathName, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes);
extern BOOL  WINAPI CreateDirectoryW (_In_ LPCWSTR lpPathName, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes);

extern BOOL  WINAPI DeleteFileA (_In_ LPCSTR  lpFileName);
extern BOOL  WINAPI DeleteFileW (_In_ LPCWSTR lpFileName);

// File Attributes
// ------------------------------------------

// The only attributes supported so far

#define INVALID_FILE_ATTRIBUTES  (0xFFFF)
#define FILE_ATTRIBUTE_DIRECTORY (0x10)

// Retrieves file system attributes for a specified file or directory
// If the function succeeds, the return value contains the attributes of the specified file or directory.
// For a list of attribute values and their descriptions, see File Attribute Constants.
// If the function fails, the return value is INVALID_FILE_ATTRIBUTES. To get extended error information, call GetLastError.
extern DWORD GetFileAttributesA (_In_ LPCSTR  lpFileName);
extern DWORD GetFileAttributesW (_In_ LPCWSTR lpFileName);

// Unicode gates
// ------------------------------------------

#if defined(UNICODE) || defined(_UNICODE)
#define GetCurrentDirectory        GetCurrentDirectoryW
#define PathFileExists             PathFileExistsW
#define CreateDirectory            CreateDirectoryW
#define DeleteFile                 DeleteFileW
#define GetFileAttributes          GetFileAttributesW
#else
#define GetCurrentDirectory        GetCurrentDirectoryA
#define PathFileExists             PathFileExistsA
#define CreateDirectory            CreateDirectoryA
#define DeleteFile                 DeleteFileA
#define GetFileAttributes          GetFileAttributesA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
