#ifndef __NANOWINEVENTS_H__
#define __NANOWINEVENTS_H__

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

extern HANDLE CreateEventA(
  LPSECURITY_ATTRIBUTES lpEventAttributes,
  BOOL                  bManualReset,
  BOOL                  bInitialState,
  LPCSTR                lpName
);

extern HANDLE CreateEventW(
  LPSECURITY_ATTRIBUTES lpEventAttributes,
  BOOL                  bManualReset,
  BOOL                  bInitialState,
  LPCWSTR               lpName
);

extern BOOL SetEvent(HANDLE hEvent);
extern BOOL ResetEvent(HANDLE hEvent);

extern BOOL CloseEventHandle(HANDLE hObject);

#ifdef UNICODE
#define CreateEvent CreateEventW
#else
#define CreateEvent CreateEventA
#endif

extern DWORD WaitForMultipleEvents // WaitForMultipleObjects implementation for events
(
  DWORD         nCount,
  const HANDLE *lpHandles,
  BOOL          bWaitAll,
  DWORD         dwMilliseconds
);

extern DWORD WaitForSingleEvent //  WaitForSingleObject implementation for events
(
  HANDLE hHandle,
  DWORD  dwMilliseconds
);

extern DWORD SignalObjectAndWait
(
  HANDLE hObjectToSignal,
  HANDLE hObjectToWaitOn,
  DWORD  dwMilliseconds,
  BOOL   bAlertable
);

NW_EXTERN_C_END

#endif

#endif
