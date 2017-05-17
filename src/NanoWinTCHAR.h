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
#include <wchar.h>
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

#if defined(UNICODE) || defined(_UNICODE)
#include <wchar.h>
#define _tcslen             wcslen
#define _tcscpy             wcscpy
#define _tcscpy_s           wcscpy_s
#define _tcscat             wcscat
#define _tcscat_s           wcscat_s
#define _tcsncpy            wcsncpy
#define _tcsncpy_s          wcsncpy_s
#define _tcsupr             wcsupr
#define _tcsupr_s           wcsupr_s
#define _tcslwr             wcslwr
#define _tcslwr_s           wcslwr_s
#define _tcscmp             wcscmp
#define _tcsclen            wcslen
#else
#include <string.h>
#define _tcslen             strlen
#define _tcscpy             strcpy
#define _tcscpy_s           strcpy_s
#define _tcscat             strcat
#define _tcscat_s           strcat_s
#define _tcsncpy            strncpy
#define _tcsncpy_s          strncpy_s
#define _tcsupr             strupr
#define _tcsupr_s           strupr_s
#define _tcslwr             strlwr
#define _tcslwr_s           strlwr_s
#define _tcscmp             strcmp
#define _tcsclen            strlen
#endif

#if defined(UNICODE) || defined(_UNICODE)
#include <wctype.h>
#define _istascii           iswascii
#define _istcntrl           iswcntrl
#define _istalpha           iswalpha
#define _istdigit           iswdigit
#else
#include <ctype.h>
#define _istascii           isascii
#define _istcntrl           iscntrl
#define _istalpha           isalpha
#define _istdigit           isdigit
#endif

#endif // linux
#endif // ...Included
