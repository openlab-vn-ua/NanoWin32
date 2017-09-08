#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "NanoWinEvents.h"
#include "NanoWinThreads.h"
#include "NanoWinError.h"

#define NW32_MILLISECONDS_IN_SECOND     (1000)
#define NW32_NANOSECONDS_IN_MILLISECOND (1000000L)
#define NW32_NANOSECONDS_IN_SECOND      (NW32_MILLISECONDS_IN_SECOND * NW32_NANOSECONDS_IN_MILLISECOND)

namespace
{
  struct NanoWinThread
  {
    pthread_t threadHandle;
    HANDLE eventHandle;
  };

  struct NW32ThreadStartInfo
  {
    LPTHREAD_START_ROUTINE startFunc;
    LPVOID                 parameter;
    NanoWinThread         *threadInfo;
  };
}

static void NW32ThreadsCleanUpFuncSignalEvent(void *arg)
{
  HANDLE eventHandle = (HANDLE)arg;

  SetEvent(eventHandle);
}

static void *NW32ThreadsThreadFunc(void *params)
{
  NW32ThreadStartInfo    *startInfo   = (NW32ThreadStartInfo*)params;
  LPTHREAD_START_ROUTINE  startFunc   = startInfo->startFunc;
  LPVOID                  parameter   = startInfo->parameter;
  HANDLE                  eventHandle = startInfo->threadInfo->eventHandle;

  free(params);

  DWORD result = 0;

  pthread_cleanup_push(NW32ThreadsCleanUpFuncSignalEvent,eventHandle);

  result = startFunc(parameter);

  pthread_cleanup_pop(1);

  return (void*)(intptr_t)result;
}

static void GenerateThreadIdByThreadHandle(LPDWORD lpThreadId, pthread_t threadHandle)
{
  if (sizeof(threadHandle) <= sizeof(DWORD))
  {
    *lpThreadId = (DWORD)threadHandle;
  }
  else
  {
    *lpThreadId = (DWORD)(((DWORD)threadHandle) ^ ((unsigned long long)threadHandle >> sizeof(DWORD) * 8));
  }
}

HANDLE CreateThread(LPSECURITY_ATTRIBUTES  lpThreadAttributes,
  SIZE_T                 dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,
  LPVOID                 lpParameter,
  DWORD                  dwCreationFlags,
  LPDWORD                lpThreadId)
{
  if (lpThreadAttributes != NULL)
  {
    return(NULL);
  }

  if (dwStackSize != 0)
  {
    return(NULL);
  }

  if (dwCreationFlags != 0)
  {
    return(NULL);
  }

  HANDLE               result = NULL;
  NW32ThreadStartInfo *threadStartInfo = (NW32ThreadStartInfo*)malloc(sizeof(NW32ThreadStartInfo));
  NanoWinThread       *threadInfo = (NanoWinThread*)malloc(sizeof(NanoWinThread));

  if (threadStartInfo != NULL && threadInfo != NULL)
  {
    threadStartInfo->startFunc  = lpStartAddress;
    threadStartInfo->parameter  = lpParameter;
    threadStartInfo->threadInfo = threadInfo;

    threadInfo->eventHandle = CreateEventA(NULL,TRUE,FALSE,NULL);

    if (threadInfo->eventHandle != NULL)
    {
      if (pthread_create(&threadInfo->threadHandle, NULL, NW32ThreadsThreadFunc, threadStartInfo) == 0)
      {
        result = (HANDLE)threadInfo;

        if (lpThreadId != NULL)
        {
          GenerateThreadIdByThreadHandle(lpThreadId, threadInfo->threadHandle);
        }
      }
    }
  }

  if (result == NULL)
  {
    if (threadInfo != NULL)
    {
      if (threadInfo->eventHandle != NULL)
      {
        CloseEventHandle(threadInfo->eventHandle);
      }

      free(threadInfo);
    }

    if (threadStartInfo != NULL)
    {
      free(threadStartInfo);
    }
  }

  return result;
}

static void TimeSpecAddTimeInMillis(struct timespec *ts, DWORD millis)
{
  long seconds = millis / NW32_MILLISECONDS_IN_SECOND;
  long nanoSeconds = ((long)(millis % NW32_MILLISECONDS_IN_SECOND)) * NW32_NANOSECONDS_IN_MILLISECOND;

  ts->tv_sec += seconds;
  ts->tv_nsec += nanoSeconds;

  if (ts->tv_nsec >= NW32_NANOSECONDS_IN_SECOND)
  {
    ts->tv_sec += ts->tv_nsec / NW32_NANOSECONDS_IN_SECOND;
    ts->tv_nsec %= NW32_NANOSECONDS_IN_SECOND;
  }
}

DWORD WaitForSingleThread(HANDLE hThread, DWORD dwMilliseconds)
{
  void           *threadResult;
  int             join_status;

  if (dwMilliseconds != INFINITE)
  {
    struct timespec timeout;

    if (clock_gettime(CLOCK_REALTIME, &timeout) != 0)
    {
      return WAIT_FAILED;
    }

    TimeSpecAddTimeInMillis(&timeout, dwMilliseconds);

    join_status = pthread_timedjoin_np(((NanoWinThread*)hThread)->threadHandle, &threadResult, &timeout);
  }
  else
  {
    join_status = pthread_join(((NanoWinThread*)hThread)->threadHandle, &threadResult);
  }

  if      (join_status == 0)
  {
    return WAIT_OBJECT_0;
  }
  else if (join_status == ETIMEDOUT)
  {
    return WAIT_TIMEOUT;
  }
  else
  {
    return WAIT_FAILED;
  }
}

extern DWORD WaitForMultipleThreads (DWORD         nCount,
                                     const HANDLE *lpHandles,
                                     BOOL          bWaitAll,
                                     DWORD         dwMilliseconds)
{
  if (nCount == 0 || lpHandles == NULL)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return WAIT_FAILED;
  }

  HANDLE *eventHandles = (HANDLE*)malloc(sizeof(HANDLE) * nCount);

  if (eventHandles == NULL)
  {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    return WAIT_FAILED;
  }

  for (DWORD i = 0; i < nCount; i++)
  {
    eventHandles[i] = ((NanoWinThread*)lpHandles[i])->eventHandle;
  }

  DWORD result = WaitForMultipleEvents(nCount,eventHandles,bWaitAll,dwMilliseconds);

  free(eventHandles);

  return result;
}

BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode)
{
  //FIXME: dwExitCode is not used (no GetExitCodeThread call is implemented)

  return pthread_cancel(((NanoWinThread*)hThread)->threadHandle) == 0 ? TRUE : FALSE;
}

VOID ExitThread(_In_ DWORD dwExitCode)
{
  pthread_exit((void*)(intptr_t)dwExitCode);
}

BOOL CloseThreadHandle(HANDLE hThread)
{
  //FIXME: simplified implementation - just trying to detach the thread
  //       without tracking if it was already joined in WaitForSingleThread call

  pthread_detach(((NanoWinThread*)hThread)->threadHandle);

  CloseEventHandle(((NanoWinThread*)hThread)->eventHandle);

  return TRUE;
}
