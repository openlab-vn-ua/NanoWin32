// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// File functions

#if !defined(NanoWinFileIncluded)
#define NanoWinFileIncluded

#include "NanoWinTypes.h"
#include "NanoWinFileSys.h"

#if defined LINUX

// Overlapped structure
typedef void OVERLAPPED; // Not supported as var
typedef OVERLAPPED *LPOVERLAPPED;

#define FILE_SHARE_READ  (0x01)
#define FILE_SHARE_WRITE (0x02)

#define GENERIC_READ  (0x80000000)
#define GENERIC_WRITE (0x40000000)

#define CREATE_NEW        (1)
#define CREATE_ALWAYS     (2)
#define OPEN_EXISTING     (3)
#define OPEN_ALWAYS       (4)
#define TRUNCATE_EXISTING (5)

#define FILE_BEGIN        (0)
#define FILE_CURRENT      (1)
#define FILE_END          (2)

#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#define INVALID_FILE_SIZE        ((DWORD)0xFFFFFFFF)

NW_EXTERN_C_BEGIN

//NOTE: in current CreateFile implementation file existence check and creation/opening is not atomic
//      also very restricted (most common) subset of dwDesiredAccess and dwCreationDisposition is
//      supported
extern HANDLE WINAPI CreateFileA(_In_     LPCSTR                lpFileName,
                                 _In_     DWORD                 dwDesiredAccess,
                                 _In_     DWORD                 dwShareMode,
                                 _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 _In_     DWORD                 dwCreationDisposition,
                                 _In_     DWORD                 dwFlagsAndAttributes,
                                 _In_opt_ HANDLE                hTemplateFile);

extern HANDLE WINAPI CreateFileW(_In_     LPCWSTR               lpFileName,
                                 _In_     DWORD                 dwDesiredAccess,
                                 _In_     DWORD                 dwShareMode,
                                 _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                 _In_     DWORD                 dwCreationDisposition,
                                 _In_     DWORD                 dwFlagsAndAttributes,
                                 _In_opt_ HANDLE                hTemplateFile);

extern BOOL WINAPI CloseFileHandle(_In_ HANDLE hObject); // CloseHandle replacement for files

extern BOOL WINAPI WriteFile(_In_        HANDLE       hFile,
                             _In_        LPCVOID      lpBuffer,
                             _In_        DWORD        nNumberOfBytesToWrite,
                             _Out_opt_   LPDWORD      lpNumberOfBytesWritten,
                             _Inout_opt_ LPOVERLAPPED lpOverlapped);

extern BOOL WINAPI ReadFile(_In_        HANDLE       hFile,
                            _Out_       LPVOID       lpBuffer,
                            _In_        DWORD        nNumberOfBytesToRead,
                            _Out_opt_   LPDWORD      lpNumberOfBytesRead,
                            _Inout_opt_ LPOVERLAPPED lpOverlapped);

extern DWORD WINAPI SetFilePointer(_In_        HANDLE hFile,
                                   _In_        LONG   lDistanceToMove,
                                   _Inout_opt_ PLONG  lpDistanceToMoveHigh,
                                   _In_        DWORD  dwMoveMethod);

extern BOOL WINAPI FlushFileBuffers(_In_ HANDLE hFile);

extern DWORD WINAPI GetFileSize(_In_      HANDLE  hFile, _Out_opt_ LPDWORD lpFileSizeHigh);

NW_EXTERN_C_END
// Unicode gates
// ------------------------------------------

#if defined(UNICODE) || defined(_UNICODE)
#define CreateFile        CreateFileW
#else
#define CreateFile        CreateFileA
#endif

#endif // linux
#endif // ...Included
