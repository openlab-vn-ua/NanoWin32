// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Microsoft safe string extensions

#if !defined(NanoWinMsvcSafeStringIncluded)
#define NanoWinMsvcSafeStringIncluded

#if defined(__GNUC__)

#if !defined(_countof)
#define _countof(arr)		(sizeof(arr)/sizeof(arr[0]))
#endif

#if !defined(NW_ERRNO_T_DEFINED)
#define NW_ERRNO_T_DEFINED
typedef int errno_t;		// Not defined by GNU C (standard assumed errno is int always)
#endif

// MS "safe" versions : string functions
#define memcpy_s(a,b,c,d)	memcpy(a,c,d)
#define strcpy_s(a,b,c)     (strcpy(a,c),(errno_t)0)
#define strcat_s(a,b,c)     strcat(a,c)
#define strncpy_s(a,b,c,d)  strncpy(a,c,d) // ?? TODO: LINUX: Implement me correctry (support for dest always null terminated, etc)
#define strtok_s            strtok_r // TODO: Check me
#define wcscpy_s(a,b,c)		(wcscpy(a,c),(errno_t)0)

#endif // GCC
#endif // ...Included
