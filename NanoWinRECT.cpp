// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// RECT functions

#include "NanoWinRECT.h"

NW_EXTERN_C_BEGIN

extern BOOL IsRectEmpty(const RECT *lprc)
{
  // Note: Windows: NotEmpty if width > 0 and height > 0
  if (lprc == NULL) { return TRUE; }

  if (lprc->top   >= lprc->bottom)  { return TRUE; }
  if (lprc->left  >= lprc->right)   { return TRUE; }

  return(FALSE);
}

extern BOOL EqualRect  (const RECT *lprc1, const RECT *lprc2)
{
  if ((lprc1 == NULL) && (lprc2 == NULL)) { return TRUE;  }
  if ((lprc1 != NULL) && (lprc2 == NULL)) { return FALSE; }
  if ((lprc1 == NULL) && (lprc2 != NULL)) { return FALSE; }

  if (lprc1->top    != lprc2->top)    { return FALSE; }
  if (lprc1->left   != lprc2->left)   { return FALSE; }
  if (lprc1->right  != lprc2->right)  { return FALSE; }
  if (lprc1->bottom != lprc2->bottom) { return FALSE; }

  return(TRUE);
}

extern BOOL CopyRec       (      RECT *lprcDst, const RECT *lprcSrc)
{
  if (lprcDst == NULL) { return FALSE; }
  if (lprcSrc == NULL) { return FALSE; }
  *lprcDst = *lprcSrc;
  return(TRUE);
}

extern BOOL SetRectEmpty  (      RECT *lprcDst)
{
  if (lprcDst == NULL) { return FALSE; }
  lprcDst->left   = 0;
  lprcDst->top    = 0;
  lprcDst->right  = 0;
  lprcDst->bottom = 0;
  return(TRUE);
}

extern BOOL SetRect       (      RECT *lprcDst, int xLeft, int yTop, int xRight, int yBottom)
{
  if (lprcDst == NULL) { return FALSE; }
  lprcDst->left   = xLeft;
  lprcDst->top    = yTop;
  lprcDst->right  = xRight;
  lprcDst->bottom = yBottom;
  return(TRUE);
}

extern BOOL IntersectRect (      RECT *lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2)
{
  if (lprcDst  == NULL) { return FALSE; }
  if (lprcSrc1 == NULL) { return FALSE; }
  if (lprcSrc2 == NULL) { return FALSE; }

  if (IsRectEmpty(lprcSrc1)) { SetRectEmpty(lprcDst); return FALSE; }
  if (IsRectEmpty(lprcSrc2)) { SetRectEmpty(lprcDst); return FALSE; }

  if (lprcSrc1->left >= lprcSrc2->right)  { SetRectEmpty(lprcDst); return FALSE; } 
  if (lprcSrc1->top  >= lprcSrc2->bottom) { SetRectEmpty(lprcDst); return FALSE; } 
  if (lprcSrc2->left >= lprcSrc1->right)  { SetRectEmpty(lprcDst); return FALSE; } 
  if (lprcSrc2->top  >= lprcSrc1->bottom) { SetRectEmpty(lprcDst); return FALSE; } 

  // We have some intersect

  #define MAX(x,y) ((x) > (y) ? (x) : (y))
  #define MIN(x,y) ((x) < (y) ? (x) : (y))

  lprcDst->left   = MAX(lprcSrc1->left,lprcSrc2->left);
  lprcDst->top    = MAX(lprcSrc1->top,lprcSrc2->top);
  lprcDst->right  = MIN(lprcSrc1->right,lprcSrc2->right);
  lprcDst->bottom = MIN(lprcSrc1->right,lprcSrc2->bottom);

  #undef MIN
  #undef MAX

  return(TRUE);
}

NW_EXTERN_C_END
