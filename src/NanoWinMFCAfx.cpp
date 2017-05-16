// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCAfx functions

#include "NanoWinMFCAfx.h"

BOOL CException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  // TODO: Implement me
  lpszError[0] = 0;
  return FALSE;
}

BOOL CMemoryException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  // TODO: Implement me
  lpszError[0] = 0;
  return FALSE;
}

BOOL CNotSupportedException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  // TODO: Implement me
  lpszError[0] = 0;
  return FALSE;
}

BOOL CInvalidArgException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
const
{
  // TODO: Implement me
  lpszError[0] = 0;
  return FALSE;
}

