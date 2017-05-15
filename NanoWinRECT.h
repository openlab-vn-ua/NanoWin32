// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// RECT functions

#if !defined(NanoWinRECTIncluded)
#define NanoWinRECTIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

// Defined in Winnt.h & WinDef.h [basic graph primitives]

typedef struct _POINT
{
  LONG x;
  LONG y;
} POINT; NW_MAKE_PLP_TYPES_BY(POINT);

typedef struct _SIZE
{
  LONG cx;
  LONG cy;
} SIZE;  NW_MAKE_PLP_TYPES_BY(SIZE);

/*
https://msdn.microsoft.com/en-us/library/windows/desktop/dd162897(v=vs.85).aspx
By convention, the right and bottom edges of the rectangle are normally considered exclusive.
In other words, the pixel whose coordinates are ( right, bottom ) lies immediately outside of the rectangle.
For example, when RECT is passed to the FillRect function, the rectangle is filled up to, but not including, the right column and bottom row of pixels.
This structure is identical to the RECTL structure.
*/
typedef struct _RECT
{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT; NW_MAKE_PLP_TYPES_BY(RECT);

typedef RECT RECTL; NW_MAKE_PLP_TYPES_BY(RECTL);

extern BOOL IsRectEmpty   (const RECT *lprc);
extern BOOL EqualRect     (const RECT *lprc1, const RECT *lprc2);
extern BOOL CopyRec       (      RECT *lprcDst, const RECT *lprcSrc);
extern BOOL SetRectEmpty  (      RECT *lprcDst);
extern BOOL SetRect       (      RECT *lprcDst, int xLeft, int yTop, int xRight, int yBottom);
extern BOOL IntersectRect (      RECT *lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
