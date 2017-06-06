// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileFind functions

#if !defined(NanoWinFileFindIncluded)
#define NanoWinFileFindIncluded

#include "NanoWinTypes.h"
#include "NanoWinFileSys.h"

#if defined LINUX

#define WIN32_FIND_DATA_FNAME_MAX  MAX_PATH // _MAX_FNAME ?

typedef struct _WIN32_FIND_DATAA
{
  DWORD    dwFileAttributes;
  char     cFileName[WIN32_FIND_DATA_FNAME_MAX];
  DWORD    nFileSizeHigh;
  DWORD    nFileSizeLow;
} WIN32_FIND_DATAA; NW_MAKE_PLP_TYPES_BY(WIN32_FIND_DATAA);

typedef struct _WIN32_FIND_DATAW
{
  DWORD    dwFileAttributes;
  wchar_t  cFileName[WIN32_FIND_DATA_FNAME_MAX];
  DWORD    nFileSizeHigh;
  DWORD    nFileSizeLow;
} WIN32_FIND_DATAW; NW_MAKE_PLP_TYPES_BY(WIN32_FIND_DATAW);

NW_EXTERN_C_BEGIN

extern HANDLE FindFirstFileA(const char *Mask, WIN32_FIND_DATAA *state);
extern BOOL   FindNextFileA (HANDLE handle, WIN32_FIND_DATAA *state);

extern HANDLE FindFirstFileW(const wchar_t *Mask, WIN32_FIND_DATAW *state);
extern BOOL   FindNextFileW (HANDLE handle, WIN32_FIND_DATAW *state);

extern BOOL   FindClose(HANDLE hFindFile);

#if defined(UNICODE) || defined(_UNICODE)
#define WIN32_FIND_DATA      WIN32_FIND_DATAW
#define FindFirstFile        FindFirstFileW
#define FindNextFile         FindNextFileW
#else
#define WIN32_FIND_DATA      WIN32_FIND_DATAA
#define FindFirstFile        FindFirstFileA
#define FindNextFile         FindNextFileA
#endif

NW_MAKE_PLP_TYPES_BY(WIN32_FIND_DATA);

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
