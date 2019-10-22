#include <time.h>
#include <errno.h>
#include <pthread.h>

#include <vector>

#include "NanoWinEvents.h"

#define NW32_MILLISECONDS_IN_SECOND     (1000)
#define NW32_NANOSECONDS_IN_MILLISECOND (1000000L)
#define NW32_NANOSECONDS_IN_SECOND      (NW32_MILLISECONDS_IN_SECOND * NW32_NANOSECONDS_IN_MILLISECOND)

namespace {

  class NW32EventError {};

  class NW32EventWaiter;

  class NW32Event
  {
  public:

                     NW32Event(bool                initialState,
                               bool                auto_reset);
    void             Set();
    void             Reset();
    bool             AddWaiter(NW32EventWaiter    *waiter);
    void             RemoveWaiter(NW32EventWaiter    *waiter);

    void             IncRefSafe();
    void             DecRefSafe();

  private:

    ~NW32Event();

    void             IncRef();
    void             DecRef();

    unsigned int                         ref_count;

    struct WaiterInfo
    {
      NW32EventWaiter *waiter;
      bool             notified;

      WaiterInfo(NW32EventWaiter *waiter)
      {
        this->waiter   = waiter;
        this->notified = false;
      }
    };

    typedef std::vector<WaiterInfo>      WaiterList;

    mutable pthread_mutex_t              state_lock;
    bool                                 state;

    bool                                 auto_reset;

    WaiterList                           waiters;
  };

  class NW32EventWaiter
  {
  public:

    NW32EventWaiter();
    ~NW32EventWaiter();

    void             Signal(NW32Event        *sourceEvent);
    DWORD            Wait(NW32Event         **events,
                          unsigned int        eventCount,
                          bool                waitAll,
                          DWORD               milliseconds);
  private:

    static void      TimeSpecAddMillis(struct timespec    *ts,
                                       DWORD               millis);

    bool             FindEventIndex(unsigned int       *index,
                                    const NW32Event    *event) const;

    pthread_mutex_t  lock;
    pthread_cond_t   condition;
    unsigned int     leftSignalCount;
    bool             isActive;
    NW32Event      **events;
    unsigned int     eventCount;
    unsigned int     signaledEventIndex;
  };

  NW32Event::NW32Event(bool                initialState,
                       bool                auto_reset)
  {
    this->state = initialState;
    this->auto_reset = auto_reset;

    if (pthread_mutex_init(&state_lock, NULL) != 0)
    {
      throw NW32EventError();
    }

    ref_count = 1;
  }

  NW32Event::~NW32Event()
  {
    pthread_mutex_destroy(&state_lock);
  }

  void    NW32Event::IncRefSafe()
  {
    pthread_mutex_lock(&state_lock);

    IncRef();

    pthread_mutex_unlock(&state_lock);
  }

  void    NW32Event::DecRefSafe()
  {
    pthread_mutex_lock(&state_lock);

    unsigned int updated_ref_count = --ref_count;

    pthread_mutex_unlock(&state_lock);

    if (updated_ref_count == 0)
    {
      delete this;
    }
  }

  void    NW32Event::IncRef()
  {
    ref_count++;
  }

  void    NW32Event::DecRef()
  {
    ref_count--;

    if (ref_count == 0)
    {
      delete this;
    }
  }

  void    NW32Event::Set()
  {
    bool signaled = false;

    if (pthread_mutex_lock(&state_lock) != 0)
    {
      throw NW32EventError();
    }

    IncRef();

    if (!state)
    {
      state = true;
      signaled = true;
    }

    if (signaled)
    {
      for (WaiterList::iterator it = waiters.begin();
        it != waiters.end();
        ++it)
      {
        try
        {
          if (!it->notified)
          {
            it->notified = true;
            it->waiter->Signal(this);

            if (auto_reset)
            {
              state = false;

              break;
            }
          }
        }
        catch (...)
        {
          // ignore exceptions from some waiters to make sure that other
          // get signal
        }
      }

      if (!auto_reset)
      {
        for (WaiterList::iterator it = waiters.begin();
             it != waiters.end();
             ++it)
        {
          it->notified = true;
        }
      }
    }

    pthread_mutex_unlock(&state_lock);

    DecRefSafe();
  }

  void          NW32Event::Reset()
  {
    if (pthread_mutex_lock(&state_lock) != 0)
    {
      throw NW32EventError();
    }

    IncRef();

    state = false;

    pthread_mutex_unlock(&state_lock);

    DecRefSafe();
  }

  bool          NW32Event::AddWaiter(NW32EventWaiter    *waiter)
  {
    if (pthread_mutex_lock(&state_lock) != 0)
    {
      throw NW32EventError();
    }

    bool signaled = state;

    if (!signaled)
    {
      try
      {
        waiters.push_back(WaiterInfo(waiter));

        IncRef();
      }
      catch (...)
      {
        pthread_mutex_unlock(&state_lock);

        throw;
      }
    }
    else
    {
      if (auto_reset)
      {
        state = false;
      }
    }

    pthread_mutex_unlock(&state_lock);

    return signaled;
  }

  void          NW32Event::RemoveWaiter(NW32EventWaiter    *waiter)
  {
    if (pthread_mutex_lock(&state_lock) != 0)
    {
      throw NW32EventError();
    }

    bool removed = false;

    try
    {
      for (WaiterList::size_type i = 0; i < waiters.size() && !removed; i++)
      {
        if (waiters[i].waiter == waiter)
        {
          removed = true;

          waiters.erase(waiters.begin() + i);
        }
      }
    }
    catch (...)
    {
      pthread_mutex_unlock(&state_lock);

      throw;
    }

    pthread_mutex_unlock(&state_lock);

    if (removed)
    {
      DecRefSafe();
    }
  }

  NW32EventWaiter::NW32EventWaiter()
  {
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
      throw NW32EventError();
    }

    if (pthread_cond_init(&condition, NULL) != 0)
    {
      pthread_mutex_destroy(&lock);

      throw NW32EventError();
    }
  }

  NW32EventWaiter::~NW32EventWaiter()
  {
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&lock);
  }

  void             NW32EventWaiter::Signal(NW32Event          *sourceEvent)
  {
    if (pthread_mutex_lock(&lock) != 0)
    {
      //TODO: find out how to handle this error condition to prevent
      //      leak of event and waiter
    }

    if (isActive)
    {
      if (leftSignalCount > 0)
      {
        leftSignalCount--;
      }

      unsigned int eventIndex;

      if (FindEventIndex(&eventIndex, sourceEvent))
      {
        if (eventIndex < signaledEventIndex)
        {
          signaledEventIndex = eventIndex;
        }
      }

      pthread_cond_signal(&condition);
    }

    pthread_mutex_unlock(&lock);
  }

  bool               NW32EventWaiter::FindEventIndex
  (unsigned int       *index,
   const NW32Event    *event) const
  {
    bool found = false;
    unsigned int curr_index = 0;

    while (!found && curr_index < eventCount)
    {
      if (event == events[curr_index])
      {
        found = true;
        *index = curr_index;
      }

      curr_index++;
    }

    return found;
  }

  DWORD              NW32EventWaiter::Wait
  (NW32Event         **events,
   unsigned int        eventCount,
   bool                waitAll,
   DWORD               milliseconds)
  {
    struct timespec finishTime;

    if (milliseconds != INFINITE)
    {
      if (clock_gettime(CLOCK_REALTIME, &finishTime) != 0)
      {
        throw NW32EventError();
      }

      TimeSpecAddMillis(&finishTime, milliseconds);
    }

    if (pthread_mutex_lock(&lock) == 0)
    {
      this->events = events;
      this->eventCount = eventCount;
      this->signaledEventIndex = eventCount;
      this->isActive = false;
      leftSignalCount = eventCount;

      pthread_mutex_unlock(&lock);
    }
    else
    {
      throw NW32EventError();
    }

    bool done = false;
    bool timeout = false;
    bool error = false;

    while (!done && !timeout && !error)
    {
      if (pthread_mutex_lock(&lock) == 0)
      {
        if (!isActive)
        {
          // first entry, need to check all events for signaled state
          // and adjust leftSignalCount accordingly

          for (unsigned int eventIndex = 0; eventIndex < eventCount; eventIndex++)
          {
            if (events[eventIndex]->AddWaiter(this))
            {
              leftSignalCount--;

              if (eventIndex < signaledEventIndex)
              {
                signaledEventIndex = eventIndex;
              }
            }
          }

          isActive = true;
        }

        done = (( waitAll && leftSignalCount == 0) ||
                (!waitAll && leftSignalCount < eventCount));

        if (!done)
        {
          int waitResult;

          if (milliseconds != INFINITE)
          {
            waitResult = pthread_cond_timedwait(&condition, &lock, &finishTime);
          }
          else
          {
            waitResult = pthread_cond_wait(&condition, &lock);
          }

          if (waitResult == 0)
          {
            // ok, on next iteration completed condition will be checked
          }
          else if (waitResult == ETIMEDOUT)
          {
            timeout = true;
          }
          else // error
          {
            error = true;
          }
        }

        pthread_mutex_unlock(&lock);
      }
      else
      {
        error = true;
      }
    }

    for (unsigned int eventIndex = 0; eventIndex < eventCount; eventIndex++)
    {
      try
      {
        events[eventIndex]->RemoveWaiter(this);
      }
      catch (...)
      {
      }
    }

    if (error)
    {
      return WAIT_FAILED;
    }
    else if (timeout)
    {
      return WAIT_TIMEOUT;
    }
    else
    {
      pthread_mutex_lock(&lock);

      DWORD result = WAIT_OBJECT_0 + signaledEventIndex;

      pthread_mutex_unlock(&lock);

      return result;
    }
  }

  void       NW32EventWaiter::TimeSpecAddMillis
  (struct timespec    *ts,
   DWORD               millis)
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
}

extern HANDLE CreateEventA(
  LPSECURITY_ATTRIBUTES lpEventAttributes,
  BOOL                  bManualReset,
  BOOL                  bInitialState,
  LPCSTR                lpName
)
{
  if (lpEventAttributes != NULL) // not supported
  {
    return NULL;
  }

  if (lpName != NULL) // not supported
  {
    return NULL;
  }

  HANDLE result = NULL;

  try
  {
    result = new NW32Event(bInitialState, !bManualReset);
  }
  catch (...)
  {
  }

  return result;
}

extern HANDLE CreateEventW(
  LPSECURITY_ATTRIBUTES lpEventAttributes,
  BOOL                  bManualReset,
  BOOL                  bInitialState,
  LPCWSTR                lpName
)
{
  if (lpEventAttributes != NULL) // not supported
  {
    return NULL;
  }

  return CreateEventA(lpEventAttributes, bManualReset, bInitialState, NULL);
}

extern BOOL SetEvent(HANDLE hEvent)
{
  try
  {
    ((NW32Event*)hEvent)->Set();
  }
  catch (...)
  {
    return FALSE;
  }

  return TRUE;
}

extern BOOL ResetEvent(HANDLE hEvent)
{
  try
  {
    ((NW32Event*)hEvent)->Reset();
  }
  catch (...)
  {
    return FALSE;
  }

  return TRUE;
}

extern BOOL CloseEventHandle(HANDLE hObject)
{
  try
  {
    ((NW32Event*)hObject)->DecRefSafe();
  }
  catch (...)
  {
    return FALSE;
  }

  return TRUE;
}

extern DWORD WaitForMultipleEvents
(
  DWORD   nCount,
  const HANDLE *lpHandles,
  BOOL    bWaitAll,
  DWORD   dwMilliseconds
)
{
  if (nCount == 0 || lpHandles == NULL)
  {
    return WAIT_FAILED;
  }

  try
  {
    NW32EventWaiter waiter;

    return waiter.Wait((NW32Event**)lpHandles, nCount, bWaitAll, dwMilliseconds);
  }
  catch (...)
  {
    return WAIT_FAILED;
  }
}

extern DWORD WaitForSingleEvent
(
  HANDLE hHandle,
  DWORD  dwMilliseconds
)
{
  return WaitForMultipleEvents(1, &hHandle, TRUE, dwMilliseconds);
}

extern DWORD SignalObjectAndWait
(
  HANDLE hObjectToSignal,
  HANDLE hObjectToWaitOn,
  DWORD  dwMilliseconds,
  BOOL   bAlertable
)
{
  if (SetEvent(hObjectToSignal) != 0)
  {
    return WaitForSingleEvent(hObjectToWaitOn, dwMilliseconds);
  }
  else
  {
    return WAIT_FAILED;
  }
}
