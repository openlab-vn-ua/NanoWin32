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

extern BOOL  WINAPI CopyFileA (_In_ LPCSTR  lpExistingFileName, _In_ LPCSTR  lpNewFileName, _In_ BOOL bFailIfExists);
extern BOOL  WINAPI CopyFileW (_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName, _In_ BOOL bFailIfExists);

extern BOOL WINAPI  MoveFileA (_In_ LPCSTR  lpExistingFileName, _In_ LPCSTR  lpNewFileName);
extern BOOL WINAPI  MoveFileW (_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName);

// File Attributes
// ------------------------------------------

// The only attributes supported so far

#define INVALID_FILE_ATTRIBUTES  (0xFFFF)
#define FILE_ATTRIBUTE_READONLY  (0x01)
#define FILE_ATTRIBUTE_DIRECTORY (0x10)
#define FILE_ATTRIBUTE_NORMAL    (0x80) // A file that does not have other attributes set. This attribute is valid only when used alone. (not a mask!)

// Retrieves file system attributes for a specified file or directory
// If the function succeeds, the return value contains the attributes of the specified file or directory.
// For a list of attribute values and their descriptions, see File Attribute Constants.
// If the function fails, the return value is INVALID_FILE_ATTRIBUTES. To get extended error information, call GetLastError.
extern DWORD GetFileAttributesA (_In_ LPCSTR  lpFileName);
extern DWORD GetFileAttributesW (_In_ LPCWSTR lpFileName);
// NOTE: only set/clear FILE_ATTRIBUTE_READONLY for files are supported
extern BOOL  SetFileAttributesA (_In_ LPCSTR lpFileName, _In_ DWORD dwFileAttributes);
extern BOOL  SetFileAttributesW (_In_ LPCWSTR lpFileName, _In_ DWORD dwFileAttributes);

// Unicode gates
// ------------------------------------------

#if defined(UNICODE) || defined(_UNICODE)
#define GetCurrentDirectory        GetCurrentDirectoryW
#define PathFileExists             PathFileExistsW
#define CreateDirectory            CreateDirectoryW
#define DeleteFile                 DeleteFileW
#define CopyFile                   CopyFileW
#define GetFileAttributes          GetFileAttributesW
#define SetFileAttributes          SetFileAttributesW
#define MoveFile                   MoveFileW
#else
#define GetCurrentDirectory        GetCurrentDirectoryA
#define PathFileExists             PathFileExistsA
#define CreateDirectory            CreateDirectoryA
#define DeleteFile                 DeleteFileA
#define CopyFile                   CopyFileA
#define GetFileAttributes          GetFileAttributesA
#define SetFileAttributes          SetFileAttributesA
#define MoveFile                   MoveFileA
#endif

NW_EXTERN_C_END

// Nano Win extra
// ------------------------------------------

#if 0 // Reserved
#if defined(LINUX)
#define NW_PATH_LIST_SEPARATOR_CHAR      ':'
#define NW_PATH_LIST_SEPARATOR_WCHAR     L':'
#define NW_PATH_LIST_SEPARATOR_STR       ":"
#else // Win native
#define NW_PATH_LIST_SEPARATOR_CHAR      ';'
#define NW_PATH_LIST_SEPARATOR_WCHAR     L';'
#define NW_PATH_LIST_SEPARATOR_STR       ";"
#endif
#endif

#if defined(LINUX)
#define NW_DIRECTORY_SEPARATOR_CHAR      '/'
#define NW_DIRECTORY_SEPARATOR_WCHAR     L'/'
#define NW_DIRECTORY_SEPARATOR_STR       "/"
#define NW_DIRECTORY_SEPARATOR_ALT_CHAR  '\\'  // Not directly allowed by API, but can be supported via hook
#define NW_DIRECTORY_SEPARATOR_ALT_WCHAR L'\\' // Not directly allowed by API, but can be supported via hook
#define NW_DIRECTORY_SEPARATOR_ALT_STR   "\\"  // Not directly allowed by API, but can be supported via hook
#else // Win native
#define NW_DIRECTORY_SEPARATOR_CHAR      '\\'
#define NW_DIRECTORY_SEPARATOR_WCHAR     L'\\'
#define NW_DIRECTORY_SEPARATOR_STR       "\\"
#define NW_DIRECTORY_SEPARATOR_ALT_CHAR  '/'  // Allowed in API
#define NW_DIRECTORY_SEPARATOR_ALT_WCHAR L'/' // Allowed in API
#define NW_DIRECTORY_SEPARATOR_ALT_STR   "/"  // Allowed in API
#endif

#define NW_DIRECTORY_NAME_PARENT         ".."
#define NW_DIRECTORY_NAME_CURRENT        "."

#define NW_FILE_EXT_SEPARATOR_CHAR       '.'  // separator between fname (AKA stem) and ext (AKA extension)
#define NW_FILE_EXT_SEPARATOR_WCHAR      L'.' // separator between fname (AKA stem) and ext (AKA extension)

#endif // linux
#endif // ...Included
