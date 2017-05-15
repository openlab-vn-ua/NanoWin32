// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCApp functions
// Micro stubs to allow code just to compile for command line run (no UI)

#if !defined(NanoWinMFCAfxWinIncluded)
#define NanoWinMFCAfxWinIncluded

#include "NanoWinTypes.h"

#if defined LINUX

class CWinApp { };

#define AfxWinInit(a,b,c,d) (TRUE)

#endif // linux
#endif // ...Included
