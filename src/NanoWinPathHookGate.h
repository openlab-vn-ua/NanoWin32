// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// PathHook functions: Standard library functions path-hook gates

#if !defined(NanoWinPathHookGateIncluded)
#define NanoWinPathHookGateIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#define NW_WRAP_GATE(symbol) __wrap_##symbol
#define NW_WRAP_REAL(symbol) __real_##symbol
#define NW_WRAP_STUB(symbol) __real_##symbol

#endif // linux
#endif // ...Included
