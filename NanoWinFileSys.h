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

// Note: under MSWIN _MAX filesystems constants values should include room for trailing '\0'

// _MAX_PATH : Maximum length of full path // Win32:(260) // Linux:4096
#if defined(PATH_MAX)
#define _MAX_PATH   PATH_MAX       // PATH_MAX includes trailing '\0'
#else
#define _MAX_PATH   (4096)         // Some reasonable assumptions
#endif

// _MAX_FNAME : Maximum length of filename component // Linux:255
#if defined(NAME_MAX)
#define _MAX_FNAME  (NAME_MAX+1)  // NAME_MAX not includes '\0', add room for it
#else
#define _MAX_FNAME  PATH_MAX
#endif

#define _MAX_DIR    _MAX_PATH     // Maximum length of directory component // Win32:(MAX_PATH-12)
#define _MAX_DRIVE  _MAX_FNAME    // Maximum length of drive component
#define _MAX_EXT    _MAX_FNAME    // Maximum length of extension component // Win32:(MAX_PATH-4)?

// Alias w/o undescore
#define MAX_PATH    _MAX_PATH

NW_EXTERN_C_BEGIN

extern BOOL PathFileExistsA         (const char    *lpPath);
extern BOOL PathFileExistsW         (const wchar_t *lpPath);

// Win32 API: Directory stuff
#define GetCurrentDirectoryA( a,b)	getcwd(b,a)

#define CreateDirectoryA(p,s)      (mkdir((p),(s) == NULL ? \
                                               (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |  S_IROTH | S_IWOTH) : (S_IRUSR | S_IWUSR) \
                                           ) != 0 ? 0 : 1)
#define CreateDirectoryW(p,s)      (0) // TODO: Implement me

#define DeleteFile                 unlink // TODO: LINUX: Verify return value policy

#if defined(UNICODE) || defined(_UNICODE)
#define PathFileExists             PathFileExistsW
#define CreateDirectory            CreateDirectoryW
#else
#define GetCurrentDirectory        GetCurrentDirectoryA
#define PathFileExists             PathFileExistsA
#define CreateDirectory            CreateDirectoryA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
