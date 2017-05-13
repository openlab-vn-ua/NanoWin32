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

// Definitions from WinDef.h & Winnt.h [graph primitives]

typedef struct _RECT
{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT; NW_MAKE_PLP_TYPES_BY(RECT);

typedef struct _POINT
{
  LONG x;
  LONG y;
} POINT; NW_MAKE_PLP_TYPES_BY(POINT);


extern BOOL IsRectEmpty(const RECT *lprc);
extern BOOL EqualRect  (const RECT *lprc1, const RECT *lprc2);

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
