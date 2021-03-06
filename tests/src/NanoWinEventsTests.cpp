#include "NWUnitTest.h"

#ifdef __linux
#include <unistd.h>
#include "NanoWinEvents.h"
#include "NanoWinThreads.h"
#else
 #include <windows.h>

 #define CloseEventHandle  CloseHandle
 #define CloseThreadHandle CloseHandle
 #define WaitForSingleEvent WaitForSingleObject
 #define WaitForSingleThread WaitForSingleObject
 #define WaitForMultipleEvents WaitForMultipleObjects
 #define WaitForMultipleThreads WaitForMultipleObjects
#endif

static DWORD WINAPI EventSignalSingleWaitTestThread (void *args)
 {
  SetEvent(*((HANDLE*)args));

  return 0;
 }

static DWORD WINAPI EventSignalMultiWaitTestThread (void *args)
 {
  SetEvent(*((HANDLE*)args));

  return 0;
 }

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinEventsTestGroup)

NW_TEST(NanoWinEventsTestGroup,CreateDestroyEventATest)
 {
  HANDLE eventHandle = CreateEventA(NULL,FALSE,FALSE,NULL);

  NW_CHECK_FALSE(eventHandle == NULL);

  NW_CHECK_TRUE(CloseEventHandle(eventHandle));
 };

NW_TEST(NanoWinEventsTestGroup,EventSignalNoWaitersTest)
 {
  HANDLE eventHandle = CreateEventA(NULL,FALSE,FALSE,NULL);

  NW_CHECK_FALSE(eventHandle == NULL);

  NW_CHECK_TRUE(SetEvent(eventHandle));

  NW_CHECK_TRUE(CloseEventHandle(eventHandle));
 };

NW_TEST(NanoWinEventsTestGroup,EventManualWaitForAlreadySignaledEventTest)
 {
  HANDLE eventHandle = CreateEventA(NULL,TRUE,FALSE,NULL);

  NW_CHECK_FALSE(eventHandle == NULL);

  NW_CHECK_TRUE(SetEvent(eventHandle));

  NW_CHECK_EQUAL(WAIT_OBJECT_0,WaitForSingleEvent(eventHandle,0));

  NW_CHECK_TRUE(CloseEventHandle(eventHandle));
 };

NW_TEST(NanoWinEventsTestGroup,EventManualSignalSingleWaitTest)
 {
  static HANDLE Signal = CreateEventA(NULL,TRUE,FALSE,NULL);

  NW_CHECK(Signal != NULL);

  HANDLE testThread = CreateThread(NULL,0,EventSignalSingleWaitTestThread,&Signal,0,NULL);

  NW_CHECK(testThread != NULL);

  NW_CHECK_EQUAL(WAIT_OBJECT_0,WaitForSingleEvent(Signal,INFINITE));

  NW_CHECK_EQUAL(WAIT_OBJECT_0,WaitForSingleThread(testThread,INFINITE));

  NW_CHECK_TRUE(CloseEventHandle(Signal));

  NW_CHECK_TRUE(CloseThreadHandle(testThread));
 }

NW_TEST(NanoWinEventsTestGroup,EventManualSignalMultiWaitTest)
 {
  static HANDLE Signal1 = CreateEventA(NULL,TRUE,FALSE,NULL);
  static HANDLE Signal2 = CreateEventA(NULL,TRUE,FALSE,NULL);

  NW_CHECK(Signal1 != NULL);
  NW_CHECK(Signal2 != NULL);

  HANDLE testThread1 = CreateThread(NULL,0,EventSignalMultiWaitTestThread,&Signal1,0,NULL);
  HANDLE testThread2 = CreateThread(NULL,0,EventSignalMultiWaitTestThread,&Signal2,0,NULL);

  NW_CHECK(testThread1 != NULL);
  NW_CHECK(testThread2 != NULL);

  HANDLE signalsToWait[] = { Signal1, Signal2 };

  DWORD waitResult = WaitForMultipleEvents(2,signalsToWait,TRUE,INFINITE);

  NW_CHECK(waitResult == WAIT_OBJECT_0);

  NW_CHECK_EQUAL_ULONGS(WAIT_OBJECT_0,WaitForSingleThread(testThread1,INFINITE));
  NW_CHECK_EQUAL_ULONGS(WAIT_OBJECT_0,WaitForSingleThread(testThread2,INFINITE));

  NW_CHECK_TRUE(CloseEventHandle(Signal1));
  NW_CHECK_TRUE(CloseEventHandle(Signal2));

  NW_CHECK_TRUE(CloseThreadHandle(testThread1));
  NW_CHECK_TRUE(CloseThreadHandle(testThread2));
 }

NW_TEST(NanoWinEventsTestGroup,EventManualSignalMultiWaitNoWaitAllTest)
 {
  static HANDLE Signal1 = CreateEventA(NULL,TRUE,FALSE,NULL);
  static HANDLE Signal2 = CreateEventA(NULL,TRUE,FALSE,NULL);

  NW_CHECK(Signal1 != NULL);
  NW_CHECK(Signal2 != NULL);

  HANDLE testThread1 = CreateThread(NULL,0,EventSignalMultiWaitTestThread,&Signal1,0,NULL);
  HANDLE testThread2 = CreateThread(NULL,0,EventSignalMultiWaitTestThread,&Signal2,0,NULL);

  NW_CHECK(testThread1 != NULL);
  NW_CHECK(testThread2 != NULL);

  HANDLE signalsToWait[] = { Signal1, Signal2 };

  DWORD waitResult = WaitForMultipleEvents(2,signalsToWait,FALSE,INFINITE);

  NW_CHECK(waitResult == WAIT_OBJECT_0 || waitResult == WAIT_OBJECT_0 + 1);

  NW_CHECK(WaitForSingleThread(testThread1,INFINITE) == WAIT_OBJECT_0);
  NW_CHECK(WaitForSingleThread(testThread2,INFINITE) == WAIT_OBJECT_0);

  NW_CHECK_TRUE(CloseThreadHandle(testThread1));
  NW_CHECK_TRUE(CloseThreadHandle(testThread2));

  NW_CHECK_TRUE(CloseEventHandle(Signal1));
  NW_CHECK_TRUE(CloseEventHandle(Signal2));
 }

NW_TEST(NanoWinEventsTestGroup,EventManualSignalMultiWaitCorrectIndexTest)
 {
  static HANDLE Signal1 = CreateEventA(NULL,TRUE,FALSE,NULL);
  static HANDLE Signal2 = CreateEventA(NULL,TRUE,FALSE,NULL);

  NW_CHECK(Signal1 != NULL);
  NW_CHECK(Signal2 != NULL);

  HANDLE testThread1 = CreateThread(NULL,0,EventSignalMultiWaitTestThread,&Signal2,0,NULL);

  NW_CHECK(testThread1 != NULL);

  HANDLE signalsToWait[] = { Signal1, Signal2 };

  DWORD waitResult = WaitForMultipleEvents(2,signalsToWait,FALSE,INFINITE);

  NW_CHECK(waitResult == WAIT_OBJECT_0 + 1);

  NW_CHECK_EQUAL_ULONGS(WAIT_OBJECT_0,WaitForSingleThread(testThread1,INFINITE));

  NW_CHECK_TRUE(CloseThreadHandle(testThread1));

  NW_CHECK_TRUE(CloseEventHandle(Signal1));
  NW_CHECK_TRUE(CloseEventHandle(Signal2));
 }

NW_TEST(NanoWinEventsTestGroup,EventAutoResetWaitForAlreadySignaledEventTest)
 {
  HANDLE eventHandle = CreateEventA(NULL,FALSE,FALSE,NULL);

  NW_CHECK_FALSE(eventHandle == NULL);

  NW_CHECK_TRUE(SetEvent(eventHandle));

  NW_CHECK_EQUAL_ULONGS(WAIT_OBJECT_0,WaitForSingleEvent(eventHandle,100));

  NW_CHECK_TRUE(CloseEventHandle(eventHandle));
 };

NW_END_TEST_GROUP()

static DWORD WINAPI ThreadsTestSleepingThread (void *args)
 {
  intptr_t sleepTimeInMicroSeconds = (intptr_t)args;

#ifdef LINUX
  usleep((useconds_t)sleepTimeInMicroSeconds);
#else
  Sleep(sleepTimeInMicroSeconds / 1000);
#endif

  return 0;
 }

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinThreadsTestGroup)

NW_TEST(NanoWinThreadsTestGroup,WaitForMultipleThreadsTest)
 {
  HANDLE threadHandles[2];

  threadHandles[0] = CreateThread(NULL,0,ThreadsTestSleepingThread,(void*)500000L,0,NULL);
  threadHandles[1] = CreateThread(NULL,0,ThreadsTestSleepingThread,(void*)200000L,0,NULL);

  NW_CHECK_FALSE(threadHandles[0] == NULL);
  NW_CHECK_FALSE(threadHandles[1] == NULL);

  DWORD waitResult = WaitForMultipleThreads(2,threadHandles,FALSE,1000);

  NW_CHECK_TRUE(waitResult == WAIT_OBJECT_0 || waitResult == WAIT_OBJECT_0 + 1);

  NW_CHECK_EQUAL_ULONGS(WAIT_OBJECT_0,WaitForSingleThread(threadHandles[0],INFINITE));
  NW_CHECK_EQUAL_ULONGS(WAIT_OBJECT_0,WaitForSingleThread(threadHandles[1],INFINITE));

  NW_CHECK_TRUE(CloseThreadHandle(threadHandles[0]));
  NW_CHECK_TRUE(CloseThreadHandle(threadHandles[1]));
 };

NW_END_TEST_GROUP()
