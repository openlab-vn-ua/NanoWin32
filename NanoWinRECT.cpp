// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// RECT functions

#include "NanoWinRECT.h"

NW_EXTERN_C_BEGIN

extern BOOL IsRectEmpty(const RECT *lprc)
{
  if (lprc == NULL) { return TRUE; }
  return(FALSE); // TODO: Implement me!
}

extern BOOL EqualRect  (const RECT *lprc1, const RECT *lprc2)
{
  if ((lprc1 == NULL) && (lprc2 == NULL)) { return TRUE;  }
  if ((lprc1 != NULL) && (lprc2 == NULL)) { return FALSE; }
  if ((lprc1 == NULL) && (lprc2 != NULL)) { return FALSE; }

  if (lprc1->left   != lprc2->left)   { return FALSE; }
  if (lprc1->top    != lprc2->top)    { return FALSE; }
  if (lprc1->right  != lprc2->right)  { return FALSE; }
  if (lprc1->bottom != lprc2->bottom) { return FALSE; }

  return(TRUE);
}

NW_EXTERN_C_END
