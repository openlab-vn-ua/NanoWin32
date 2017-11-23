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

BOOL CException::GetErrorMessage(LPSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(strcpy_s(lpszError, nMaxError, "general error")) == 0)
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

BOOL CException::GetErrorMessage(LPWSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(wcscpy_s(lpszError, nMaxError, L"general error")) == 0)
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

void CException::Delete()
{
  if (autoDelete)
  {
    delete this;
  }
}

BOOL CMemoryException::GetErrorMessage(LPSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(strcpy_s(lpszError, nMaxError, "not enough memory")) == 0)
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

BOOL CMemoryException::GetErrorMessage(LPWSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(wcscpy_s(lpszError, nMaxError, L"not enough memory")) == 0)
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

BOOL CNotSupportedException::GetErrorMessage(LPSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(strcpy_s(lpszError, nMaxError, "not supported")) == 0)
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

BOOL CNotSupportedException::GetErrorMessage(LPWSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(wcscpy_s(lpszError, nMaxError, L"not supported")) == 0)
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

BOOL CInvalidArgException::GetErrorMessage(LPSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(strcpy_s(lpszError, nMaxError, "invalid argument")) == 0)
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

BOOL CInvalidArgException::GetErrorMessage(LPWSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(wcscpy_s(lpszError, nMaxError, L"invalid argument")) == 0)
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


BOOL CFileException::GetErrorMessage(LPSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(strcpy_s(lpszError, nMaxError, "file error")) == 0)
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

BOOL CFileException::GetErrorMessage(LPWSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  if (NW_SEXP_S(wcscpy_s(lpszError, nMaxError, L"file error")) == 0)
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
