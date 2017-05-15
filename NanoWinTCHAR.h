// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// TCHAR definitions

#if !defined(NanoWinTCHARIncluded)
#define NanoWinTCHARIncluded

#include "NanoWinTypes.h"

#if defined LINUX

/*
https://blogs.msdn.microsoft.com/oldnewthing/20040212-00/?p=40643/

TEXT vs. _TEXT vs. _T, and UNICODE vs. _UNICODE
The plain versions without the underscore affect the character set the Windows header files treat as default. 
So if you define UNICODE, then GetWindowText will map to GetWindowTextW instead of GetWindowTextA, 
for example. Similarly, the TEXT macro will map to L"..." instead of "...".

The versions with the underscore affect the character set the C runtime header files treat as default. 
So if you define _UNICODE, then _tcslen will map to wcslen instead of strlen, for example. 
Similarly, the _TEXT macro will map to L"..." instead of "...". 

What about _T? Okay, I don't know about that one. Maybe it was just to save somebody some typing. 
*/

#if defined(UNICODE) || defined(_UNICODE)
typedef wchar_t              TCHAR;
typedef LPCWSTR              LPCTSTR;
typedef LPWSTR               LPTSTR;
#define __T(text)            L##text
#else
typedef char                 TCHAR;
typedef LPCSTR               LPCTSTR;
typedef LPSTR                LPTSTR;
#define __T(text)            text
#endif

#define _TCHAR               TCHAR      // alias with underscore

#define TEXT(text)           __T(text)
#define _TEXT(text)          __T(text)  // alias with underscore

#define _T(text)             __T(text)

#endif // linux
#endif // ...Included
