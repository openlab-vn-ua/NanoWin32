#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "NanoWinThreads.h"

#define NW32_MILLISECONDS_IN_SECOND     (1000)
#define NW32_NANOSECONDS_IN_MILLISECOND (1000000L)
#define NW32_NANOSECONDS_IN_SECOND      (NW32_MILLISECONDS_IN_SECOND * NW32_NANOSECONDS_IN_MILLISECOND)

typedef struct
{
  LPTHREAD_START_ROUTINE startFunc;
  LPVOID                 parameter;
} NW32ThreadStartInfo;

static void *NW32ThreadsThreadFunc(void *params)
{
  NW32ThreadStartInfo    *startInfo = (NW32ThreadStartInfo*)params;
  LPTHREAD_START_ROUTINE  startFunc = startInfo->startFunc;
  LPVOID                  parameter = startInfo->parameter;

  free(params);

  DWORD result = startFunc(parameter);

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

  if (threadStartInfo != NULL)
  {
    pthread_t threadHandle;

    threadStartInfo->startFunc = lpStartAddress;
    threadStartInfo->parameter = lpParameter;

    if (pthread_create(&threadHandle, NULL, NW32ThreadsThreadFunc, threadStartInfo) == 0)
    {
      result = (HANDLE)threadHandle;

      if (lpThreadId != NULL)
      {
        GenerateThreadIdByThreadHandle(lpThreadId, threadHandle);
      }
    }
    else
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

    join_status = pthread_timedjoin_np((pthread_t)hThread, &threadResult, &timeout);
  }
  else
  {
    join_status = pthread_join((pthread_t)hThread, &threadResult);
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

BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode)
{
  //FIXME: dwExitCode is not used (no GetExitCodeThread call is implemented)

  return pthread_cancel((pthread_t)hThread) == 0 ? TRUE : FALSE;
}


VOID ExitThread(_In_ DWORD dwExitCode)
{
  pthread_exit((void*)dwExitCode);
}

BOOL CloseThreadHandle(HANDLE hThread)
{
  //FIXME: simplified implementation - just trying to detach the thread
  //       without tracking if it was already joined in WaitForSingleThread call

  pthread_detach((pthread_t)hThread);

  return TRUE;
}
