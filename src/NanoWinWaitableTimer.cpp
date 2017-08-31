#include <stdlib.h>
#include <pthread.h>

#include "NanoWinEvents.h"
#include "NanoWinError.h"
#include "NanoWinWaitableTimer.h"

typedef struct
{
  LONG       milsTimeout;
  pthread_t  wtThread;
  HANDLE     eventHandle;
  HANDLE     interEventHandle;
} NanoWinWaitableTimer;

extern HANDLE WINAPI CreateWaitableTimerA     (LPSECURITY_ATTRIBUTES lpTimerAttributes,
                                               BOOL                  bManualReset,
                                               LPCSTR                lpTimerName)
{
  HANDLE result = NULL;

  if (lpTimerAttributes != NULL) // not supported
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return NULL;
  }

  if (lpTimerName != NULL) // not supported
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return NULL;
  }

  NanoWinWaitableTimer *waitableTimer = (NanoWinWaitableTimer*)malloc(sizeof(NanoWinWaitableTimer));

  if (waitableTimer != NULL)
  {
    waitableTimer->wtThread    = NULL;
    waitableTimer->milsTimeout = 200;

    waitableTimer->eventHandle = CreateEventA(NULL, FALSE, FALSE, NULL);

    if (waitableTimer->eventHandle != NULL)
    {
      waitableTimer->interEventHandle = CreateEventA(NULL, FALSE, TRUE, NULL);

      if (waitableTimer->interEventHandle != NULL)
      {
        result = (HANDLE)waitableTimer;
      }
      else
      {
        CloseEventHandle(waitableTimer->eventHandle);
      }
    }

    if (result == NULL)
    {
      free(waitableTimer);
    }
  }
  else
  {
    NanoWinSetLastError(ERROR_NOT_ENOUGH_MEMORY);
  }

  return result;
}

extern HANDLE WINAPI CreateWaitableTimerW     (LPSECURITY_ATTRIBUTES lpTimerAttributes,
                                               BOOL                  bManualReset,
                                               LPCWSTR               lpTimerName)
{
  if (lpTimerName != NULL) // not supported
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return NULL;
  }

  return CreateWaitableTimerA(lpTimerAttributes,bManualReset,NULL);
}

static void *WaitableTimerThreadRoutine (void *paramPtr)
{
  NanoWinWaitableTimer *waitableTimer = (NanoWinWaitableTimer*)paramPtr;

  HANDLE tmp = (HANDLE)WaitableTimerThreadRoutine;

  while (true)
  {
    DWORD res = WaitForSingleEvent(waitableTimer->interEventHandle, waitableTimer->milsTimeout);

    ResetEvent(waitableTimer->interEventHandle);

    if ((res == WAIT_TIMEOUT) || (res == WAIT_OBJECT_0))
    {
      SetEvent(waitableTimer->eventHandle);
    }
    else
    {
      break;
    }
  }

  return NULL;
}

extern BOOL WINAPI   SetWaitableTimer         (HANDLE                hTimer,
                                               const LARGE_INTEGER  *pDueTime,
                                               LONG                  lPeriod,
                                               PTIMERAPCROUTINE      pfnCompletionRoutine,
                                               LPVOID                lpArgToCompletionRoutine,
                                               BOOL                  fResume)
{
  if (pDueTime != NULL                 ||
      pfnCompletionRoutine != NULL     ||
      lpArgToCompletionRoutine != NULL ||
      fResume) // not supported
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return NULL;
  }

  NanoWinWaitableTimer *waitableTimer = (NanoWinWaitableTimer*)hTimer;

  waitableTimer->milsTimeout = lPeriod;

  if (waitableTimer->wtThread != NULL)
  {
    SetEvent(waitableTimer->interEventHandle);
  }
  else
  {
    int res = pthread_create(&waitableTimer->wtThread, NULL, WaitableTimerThreadRoutine, waitableTimer);

    if (res != 0)
    {
      NanoWinSetLastError(NanoWinErrorByErrnoAtFail(res));

      return FALSE;
    }
  }

  return TRUE;
}

extern BOOL WINAPI   CloseWaitableTimerHandle (HANDLE                hObject)
{
  if (hObject == NULL)
  {
    return TRUE;
  }

  NanoWinWaitableTimer *waitableTimer = (NanoWinWaitableTimer*)hObject;

  if (waitableTimer->eventHandle != NULL)
  {
    CloseEventHandle(waitableTimer->eventHandle);
    waitableTimer->eventHandle = NULL;
  }

  if (waitableTimer->interEventHandle != NULL)
  {
    CloseEventHandle(waitableTimer->interEventHandle);
    waitableTimer->interEventHandle = NULL;
  }

  if (waitableTimer->wtThread != NULL)
  {
    pthread_cancel(waitableTimer->wtThread);
    waitableTimer->wtThread = NULL;
  }

  free(waitableTimer);

  return TRUE;
}

//NOTE: this function is NanoWin-specific and does not exist in WinAPI
extern HANDLE        GetEventByWaitableTimer  (HANDLE                hTimer)
{
  return ((NanoWinWaitableTimer*)hTimer)->eventHandle;
}

