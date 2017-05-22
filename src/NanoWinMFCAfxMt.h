// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// AfxMt functions (subset)

#if !defined(NanoWinMFCAfxMtIncluded)
#define NanoWinMFCAfxMtIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include "NanoWinEvents.h"
#include "NanoWinCriticalSection.h"
#include "NanoWinMFCAfx.h"

//Maybe later...
//class CSyncObject : public CObject
//class CCriticalSection : public CSyncObject

class CCriticalSection
{
  protected:
  CRITICAL_SECTION m_sect;

  public:
  CCriticalSection()
  {
    InitializeCriticalSection(&m_sect);
  }

  ~CCriticalSection()
  {
    DeleteCriticalSection(&m_sect);
  }

  BOOL Lock()
  {
    EnterCriticalSection(&m_sect);
	return(TRUE);
  }

  BOOL Unlock()
  {
    LeaveCriticalSection(&m_sect);
	return(TRUE);
  }
};

class CSingleLock
{
  // CCriticalSection the only locker supported
  protected:
  CCriticalSection *mpObject;

  public:
  CSingleLock(CCriticalSection *pObject)
  {
    mpObject = pObject;
  }
  // CSingleLock(CCriticalSection *pObject, BOOL bInitialLock = FALSE); // Not supported yet

  ~CSingleLock()
  {
  }

  void Lock()
  {
    mpObject->Lock();
  }

  void Unlock()
  {
    mpObject->Unlock();
  }
};


class CEvent
{
  public :

  CEvent(BOOL bInitiallyOwn = FALSE,
         BOOL bManualReset = FALSE,
         LPCSTR lpszName = NULL,
         LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
  {
    m_hObject = CreateEventA(lpsaAttribute,bManualReset,bInitiallyOwn,lpszName);

    #if defined(_ATL_NO_EXCEPTIONS)
      assert(m_hObject != NULL);
    #else
      throw new CMemoryException;
    #endif
  }

  CEvent(BOOL bInitiallyOwn = FALSE,
         BOOL bManualReset = FALSE,
         LPCWSTR lpszName = NULL,
         LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
  {
    m_hObject = CreateEventW(lpsaAttribute,bManualReset,bInitiallyOwn,lpszName);

    #if defined(_ATL_NO_EXCEPTIONS)
      assert(m_hObject != NULL);
    #else
      throw new CMemoryException;
    #endif
  }

  BOOL SetEvent()
  {
    return ::SetEvent(m_hObject);
  }

  BOOL ResetEvent()
  {
    return ::ResetEvent(m_hObject);
  }

  HANDLE m_hObject;
};

#endif // linux
#endif // ...Included
