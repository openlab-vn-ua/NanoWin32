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
#define _tcsicmp            wcsicmp
#define _tcsclen            wcslen
#define _tcsstr             wcsstr
#define _tcschr             wcschr
#define _tcsrchr            wcsrchr
#define _tcscspn            wcscspn
#define _tcstok_s           wcstok_s
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
#define _tcsicmp            stricmp
#define _tcsclen            strlen
#define _tcsstr             strstr
#define _tcschr             strchr
#define _tcsrchr            strrchr
#define _tcscspn            strcspn
#define _tcstok_s           strtok_s
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define _tcsdup             wcsdup
#define _tcstod             wcstod
#define _tcstoul            wcstoul
#else
#define _tcsdup             strdup
#define _tcstod             strtod
#define _tcstoul            strtoul
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define _tstoi              _wtoi
#else
#define _tstoi              atoi
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

#if defined(UNICODE) || defined(_UNICODE)
#define _tfopen              _wfopen
#define _tfopen_s            _wfopen_s
#define _tfsopen             _wfsopen
#define _fputts              fputws
#define _fgetts              fgetws
#else
#define _tfopen              fopen
#define _tfopen_s            fopen_s
#define _tfsopen             _fsopen
#define _fputts              fputs
#define _fgetts              fgets
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define _ftprintf            fwprintf
#define _ftprintf_s          fwprintf_s
#define _ftscanf             fwscanf
#define _ftscanf_s           fwscanf_s
#else
#define _ftprintf            fprintf
#define _ftprintf_s          fprintf_s
#define _ftscanf             fscanf
#define _ftscanf_s           fscanf_s
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define _tprintf             wprintf
#define _tprintf_s           wprintf_s
#define _stprintf            swprintf
#define _stprintf_s          swprintf_s
#else
#define _tprintf             printf
#define _tprintf_s           printf_s
#define _stprintf            sprintf
#define _stprintf_s          sprintf_s
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define _stscanf             swscanf
#define _stscanf_s           swscanf_s
#else
#define _stscanf             sscanf
#define _stscanf_s           sscanf_s
#endif

#if defined(UNICODE) || defined(_UNICODE)

#define _tgetcwd             _wgetcwd

#define _tfullpath           _wfullpath
#define _tmakepath           _wmakepath
#define _tmakepath_s         _wmakepath_s
#define _tsplitpath          _wsplitpath
#define _tsplitpath_s        _wsplitpath_s

#else

#define _tgetcwd             getcwd

#define _tfullpath           _fullpath
#define _tmakepath           _makepath
#define _tmakepath_s         _makepath_s
#define _tsplitpath          _splitpath
#define _tsplitpath_s        _splitpath_s

#endif

#if defined(UNICODE) || defined(_UNICODE)
#define _tsystem             wsystem
#else
#define _tsystem             system
#endif

#endif // linux
#endif // ...Included
