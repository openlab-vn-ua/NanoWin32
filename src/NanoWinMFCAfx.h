// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCAfx functions
// Micro stubs to allow code just to compile for command line run (no UI)

#if !defined(NanoWinMFCAfxIncluded)
#define NanoWinMFCAfxIncluded

#include "NanoWinTypes.h"
#include "NanoWinTCHAR.h"

#if defined LINUX

// Debug primitives (subset)
// --------------------------------------

#include <assert.h>

// "void" ASSERT(f)      evaluate f only on debug, fails if f is false (do not evaluates on release)
// "void" VERIFY(f)      evaluate f always, fail if false on debug (ignoring result on release)
// "void" DEBUG_ONLY(f)  evaluate f only on debug (result is not tested)

#define ASSERT(f)            assert(f)
#if defined(_DEBUG) && !defined(NDEBUG)
#define DEBUG_ONLY(f)        (f)
#define VERIFY(f)            ASSERT(f)
#else
#define DEBUG_ONLY(f)        ((void)0)
#define VERIFY(f)            ((void)(f))
#endif

// Basic CObject (subset)
// --------------------------------------

#define AFXAPI // just a marker for functions

class CObject
{
  public:

  virtual ~CObject() = 0;  // Virtual destructors are manadatory to all decendants

  protected:
  CObject() { }; // You cannot instaniate CObject directly

  private:
  CObject(const CObject& objectSrc);              // You cannot copy, unless you do it explicitely // no implementation (so would produce link error)
  void operator=(const CObject& objectSrc);       // You cannot copy, unless you do it explicitely // no implementation (so would produce link error)
};

#define DECLARE_DYNAMIC(c) // Nothing as for now

// Basic CException (subset)
// --------------------------------------

class CException : public CObject
{
  // Abstract
  public:

  // Override this to report errors
  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;

  // Used to just call non-cost version? // No this hook in this implemenation
  // virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

  // Reports exception (somehow to stderror) [call-only]
  // Prototype differ from MFC, because no agrs are supported here, original prototype:
  // virtual int ReportError(UINT nType = MB_OK, UINT nMessageID = 0);
  // Stubbed ReportError() uses no params and is not virtual since override is not supported here
  int  ReportError(); // TODO: Implement me

  // This function checks to see if the CException object was created on the heap, 
  // and if so, it calls the delete operator on the object.
  void Delete(); // TODO: Implement me

  // Abstract since there is no destructor declared here
};

class CSimpleException : public CException
{
  // Just intermidiate class here
  // Abstract since there is no destructor declared here
};

// Some usefull exceptions
// --------------------------------------

class CMemoryException : public CSimpleException
{
  public:
  virtual ~CMemoryException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

class CNotSupportedException : public CSimpleException
{
  public:
  virtual ~CNotSupportedException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

class CInvalidArgException : public CSimpleException
{
  public:
  virtual ~CInvalidArgException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

class CFileException : public CSimpleException
{
  public:
  virtual ~CFileException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

// Some usefull types
// --------------------------------------
// atltypes.h

#include "NanoWinRECT.h"

class CSize : public SIZE
{
  public:

  CSize(int initCX, int initCY) // throw()
  {
    cx = initCX; cy = initCY;
  }

  CSize() // throw()
  {
    cx = 0; cy = 0;
  }
};

class CPoint : public POINT
{
  public:

  CPoint(int initX, int initY) // throw()
  {
    x = initX; y = initY;
  }

  CPoint() // throw()
  {
    x = 0; y = 0;
  }
};

class CRect : public RECT
{
  public:

  CRect()
  {
    left = 0;
    top = 0;
    right = 0;
    bottom = 0;
  }

  CRect(const RECT &src)
  {
	  left = src.left;
	  top = src.top;
	  right = src.right;
	  bottom = src.bottom;
  }

  CRect(int inLeft, int inTop, int inRight, int inBottom)
  {
    left = inLeft;
    top = inTop;
    right = inRight;
    bottom = inBottom;
  }

  void SetRect(int inLeft, int inTop, int inRight, int inBottom)
  {
    left = inLeft;
    top = inTop;
    right = inRight;
    bottom = inBottom;
  }

  BOOL IsRectNull() const
  {
    if (left != 0) return(FALSE);
    if (top != 0) return(FALSE);
    if (right != 0) return(FALSE);
    if (bottom != 0) return(FALSE);
    return(TRUE);
  }

  BOOL IsRectEmpty() const
  {
    return(::IsRectEmpty(this));
  }

  void SetRectEmpty()
  {
    ::SetRectEmpty(this);
  }

  int Width() const
  {
     return right - left;
  }

  int Height() const
  {
     return bottom - top;
  }

  CSize Size() const
  {
     return CSize(Width(), Height());
  }

  BOOL PtInRect(_In_ POINT point)
  const // throw();
  {
    return ::PtInRect(this,point);
  }

  CPoint CenterPoint()
  const // throw();
  {
    return(CPoint(((left+right)/2),((top+bottom)/2)));
  }

  void DeflateRect(_In_ int l, _In_ int t, _In_ int r, _In_ int b) // throw();
  {
    left += l;
	top += t;
	right -= r;
	bottom -= b;
  }

  void DeflateRect(_In_ int x, _In_ int y) // throw();
  {
    ::InflateRect(this, -x, -y);
  }

  void DeflateRect(_In_ SIZE isize) // throw();
  {
    DeflateRect(isize.cx, isize.cy);
  }

  void InflateRect(_In_ int l, _In_ int t, _In_ int r, _In_ int b) // throw();
  {
    left -= l;
	top -= t;
	right += r;
	bottom += b;
  }

  void InflateRect(_In_ int x, _In_ int y) // throw();
  {
    ::InflateRect(this, x, y);
  }

  void InflateRect(_In_ SIZE isize) // throw();
  {
    InflateRect(isize.cx, isize.cy);
  }
};

#endif // linux
#endif // ...Included
