// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCAfx functions

#include "NanoWinMsSafeString.h"
#include "NanoWinMFCAfx.h"

// implementation of abstract virtual destructor is required, without it it would be
// impossible to instantiate its descendants
CObject::~CObject()
{
}

#define EXCEPTION_DEF_HELP_CONTEXT (0)

BOOL CException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (_tcscpy_s(lpszError, nMaxError, _T("general error")) == 0)
  {
    if (pnHelpContext != NULL)
    {
      *pnHelpContext = EXCEPTION_DEF_HELP_CONTEXT;
    }

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

BOOL CMemoryException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (_tcscpy_s(lpszError, nMaxError, _T("not enough memory")) == 0)
  {
    if (pnHelpContext != NULL)
    {
      *pnHelpContext = EXCEPTION_DEF_HELP_CONTEXT;
    }

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

BOOL CNotSupportedException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (_tcscpy_s(lpszError, nMaxError, _T("not supported")) == 0)
  {
    if (pnHelpContext != NULL)
    {
      *pnHelpContext = EXCEPTION_DEF_HELP_CONTEXT;
    }

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

BOOL CInvalidArgException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (_tcscpy_s(lpszError, nMaxError, _T("invalid argument")) == 0)
  {
    if (pnHelpContext != NULL)
    {
      *pnHelpContext = EXCEPTION_DEF_HELP_CONTEXT;
    }

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

BOOL CFileException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (_tcscpy_s(lpszError, nMaxError, _T("file error")) == 0)
  {
    if (pnHelpContext != NULL)
    {
      *pnHelpContext = EXCEPTION_DEF_HELP_CONTEXT;
    }

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
