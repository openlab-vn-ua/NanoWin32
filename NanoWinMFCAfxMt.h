// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// AfxMt functions (subset)

#if !defined(NanoWinMFCAfxMtIncluded)
#define NanoWinMFCAfxMtIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include "NanoWinCriticalSection.h"

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


// class CEvent // TODO: Implement me

#endif // linux
#endif // ...Included
