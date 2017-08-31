#ifndef __NANOWINWAITABLETIMER_H__
#define __NANOWINWAITABLETIMER_H__

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

typedef void*        PTIMERAPCROUTINE;

// NOTE: the following parametes are not supported: lpTimerAttributes, lpTimerName (must be NULL)
extern HANDLE WINAPI CreateWaitableTimerA     (LPSECURITY_ATTRIBUTES lpTimerAttributes,
                                               BOOL                  bManualReset,
                                               LPCSTR                lpTimerName);

// NOTE: the following parametes are not supported: lpTimerAttributes, lpTimerName (must be NULL)
extern HANDLE WINAPI CreateWaitableTimerW     (LPSECURITY_ATTRIBUTES lpTimerAttributes,
                                               BOOL                  bManualReset,
                                               LPCWSTR               lpTimerName);

// NOTE: the following parametes are not supported: pDueTime, pfnCompletionRoutine,lpArgToCompletionRoutine (must be NULL)
//       fResume is not supported also and expected to be FALSE
extern BOOL WINAPI   SetWaitableTimer         (HANDLE                hTimer,
                                               const LARGE_INTEGER  *pDueTime,
                                               LONG                  lPeriod,
                                               PTIMERAPCROUTINE      pfnCompletionRoutine,
                                               LPVOID                lpArgToCompletionRoutine,
                                               BOOL                  fResume);

extern BOOL WINAPI   CloseWaitableTimerHandle (HANDLE                hObject);

//NOTE: this function is NanoWin-specific and does not exist in WinAPI
extern HANDLE        GetEventByWaitableTimer  (HANDLE                hTimer);

#if defined(UNICODE) || defined(_UNICODE)
#define CreateWaitableTimer CreateWaitableTimerW
#else
#define CreateWaitableTimer CreateWaitableTimerA
#endif

NW_EXTERN_C_END

#endif

#endif


