#ifndef __NANOWINTHREADS_H__
#define __NANOWINTHREADS_H__

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

typedef DWORD(*PTHREAD_START_ROUTINE) (LPVOID lpThreadParameter);

typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

extern HANDLE CreateThread
(
	LPSECURITY_ATTRIBUTES  lpThreadAttributes,
	SIZE_T                 dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID                 lpParameter,
	DWORD                  dwCreationFlags,
	LPDWORD                lpThreadId
);

extern BOOL TerminateThread
(
	HANDLE hThread,
	DWORD  dwExitCode
);

extern BOOL CloseThreadHandle(HANDLE hThread);

extern DWORD WaitForSingleThread(HANDLE hThread, DWORD dwMilliseconds);

NW_EXTERN_C_END

#endif

#endif
