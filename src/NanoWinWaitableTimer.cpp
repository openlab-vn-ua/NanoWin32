#include <stdlib.h>
#include <pthread.h>

#include "NanoWinEvents.h"
#include "NanoWinError.h"
#include "NanoWinWaitableTimer.h"

typedef struct
{
  LONG       milsTimeout;
  LONG       milsDue;
  pthread_t  wtThread;
  HANDLE     eventHandle;
  HANDLE     interEventHandle;
  bool       exitFlag;
} NanoWinWaitableTimer;

#define PTHREAD_T_NULL ((pthread_t)(NULL))    // NULL value for pthread_t

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
    waitableTimer->wtThread    = PTHREAD_T_NULL;
    waitableTimer->milsTimeout = 200;
    waitableTimer->exitFlag    = false;

    waitableTimer->eventHandle = CreateEventA(NULL, bManualReset, FALSE, NULL);

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

static void *WaitableTimerThreadRoutine (void *paramPtr) throw ()
{
  NanoWinWaitableTimer *waitableTimer = (NanoWinWaitableTimer*)paramPtr;
  DWORD waitMils = waitableTimer->milsDue;

  //ENABLING THE CANCEL FUNCTIONALITY
  int prevType;

  #ifndef __ANDROID_API__
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &prevType);
  #endif

  while (!waitableTimer->exitFlag)
  {
    DWORD res = WaitForSingleEvent(waitableTimer->interEventHandle, waitMils);
    waitMils = waitableTimer->milsTimeout;

    ResetEvent(waitableTimer->interEventHandle);

    if (waitableTimer->exitFlag)
    {
      break;
    }
    else if ((res == WAIT_TIMEOUT) || (res == WAIT_OBJECT_0))
    {
      SetEvent(waitableTimer->eventHandle);
    }
    else // error
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
  if (((pDueTime != NULL) &&  (pDueTime->QuadPart > 0L)) || // absolute utc time not supported yet
      pfnCompletionRoutine != NULL     ||
      lpArgToCompletionRoutine != NULL ||
      fResume) // not supported
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return FALSE;
  }

  NanoWinWaitableTimer *waitableTimer = (NanoWinWaitableTimer*)hTimer;

  if (pDueTime != NULL)
  {
    waitableTimer->milsDue = -(pDueTime->QuadPart) / 10000;
  }
  else
  {
    waitableTimer->milsDue = lPeriod;
  }

  waitableTimer->milsTimeout = lPeriod;

  if (waitableTimer->wtThread != PTHREAD_T_NULL)
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

  if (waitableTimer->wtThread != PTHREAD_T_NULL)
  {
    waitableTimer->exitFlag = true;
    #ifdef WAITABLE_TIMER_CANCELABLE  // if defined appearing "no rethrow" exception (compiler dependent)
    pthread_cancel(waitableTimer->wtThread);
    pthread_join(waitableTimer->wtThread, NULL); // Future idea?
    #else // stable version
    SetEvent(waitableTimer->interEventHandle);
    pthread_join(waitableTimer->wtThread, NULL);
    #endif
    waitableTimer->wtThread = PTHREAD_T_NULL;
  }

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

  free(waitableTimer);

  return TRUE;
}

//NOTE: this function is NanoWin-specific and does not exist in WinAPI
extern HANDLE        GetEventByWaitableTimer  (HANDLE                hTimer)
{
  return ((NanoWinWaitableTimer*)hTimer)->eventHandle;
}

