// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// ShellEx functions

#if !defined(NanoWinShellExIncluded)
#define NanoWinShellExIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

// Defined in shlobj.h

#ifndef NW_HWND_DEFINED
#define NW_HWND_DEFINED
NW_PNULLONLY_TYPE(HWND);
#endif

extern int SHCreateDirectoryExW
           (
            HWND                       hwnd,
            LPCWSTR                    pszPath,
            const SECURITY_ATTRIBUTES *psa
           );

extern int SHCreateDirectoryExA
           (
            HWND                       hwnd,
            LPCSTR                     pszPath,
            const SECURITY_ATTRIBUTES *psa
           );

#if defined(UNICODE) || defined(_UNICODE)
#define  SHCreateDirectoryEx   SHCreateDirectoryExW
#else
#define  SHCreateDirectoryEx   SHCreateDirectoryExA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
