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

struct WIN32_FIND_DATAA
{
  char cFileName[MAX_PATH]; // _MAX_FNAME ?
};

NW_EXTERN_C_BEGIN

inline HANDLE FindFirstFileA(const char *Mask, WIN32_FIND_DATAA *state) { return INVALID_HANDLE_VALUE; }
inline BOOL   FindNextFileA(HANDLE handle, WIN32_FIND_DATAA *state) { return FALSE; }
inline BOOL   FindClose(HANDLE hFindFile) { return TRUE; }

#if defined(UNICODE) || defined(_UNICODE)
#else
#define WIN32_FIND_DATA WIN32_FIND_DATAA
#define FindFirstFile   FindFirstFileA
#define FindNextFile    FindNextFileA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
