// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// NanoWin is designed to help port C/C++ code written for MS VS to GNU Linux

// NanoWin.h substitution for Windows.h
// replace #include <Windows.h> with #include <NanoWin.h>
// You may also rename NanoWin.h to Windows.h (or use soft link to map)

// Other files you probably may need to include:

// Microsoft extra features
// NanoWinTCHAR.h        // Nanowin substitute for TCHAR.h
// NanoWinMsExtra.h      // Extra functions defined in MS VS that is missing under GNU/POSIX like _fullpath
// NanoWinMsSafeString.h // Safe string functions like strcpy_s, memset_s and MS VS templates for them
// NanoWinMsSafePrintf.h // Microsoft safe vsprintf_s, sprintf_s extensions
// NanoWinMsSafeScanf.h  // Microsoft safe sscanf_s, fscanf_s extensions (some exotic cases not supported, see note)

// Microsoft MFC subset
// NanoWinMFCAfx*.h      // Nanowin MFC Classes simulators (include it instead of MFCAfx*.h files)
// NanoWinMFCAfxNUI.h    // Nanowin NoGUI stub for some basic MFC GUI functions (actions will be just output to console)

// Quirks and tricks (include if you really need it)
// NanoWinMsPrintf.h     // MSVC-compatible wprintf/wcsanf format support for %s,%c format chars (non-POSIX)
// NanoWinPathHook*.cpp  // Add to your project to allow path sep as '/' and '\\' (see link note in NanoWinPathHookGate.cpp)

#if !defined(NanoWinIncluded)
#define NanoWinIncluded

#include "NanoWinTypes.h"
#include "NanoWinSystem.h"
#include "NanoWinOther.h"
#include "NanoWinError.h"
#include "NanoWinEnv.h"
#include "NanoWinFileSys.h"
#include "NanoWinFileFind.h"
#include "NanoWinFile.h"
#include "NanoWinCriticalSection.h"
#include "NanoWinEvents.h"
#include "NanoWinModule.h"
#include "NanoWinBmp.h"
#include "NanoWinRECT.h"
#include "NanoWinStrConvert.h"
#include "NanoWinThreads.h"
#include "NanoWinWaitableTimer.h"
#include "NanoWinQueryPerf.h" // QueryPerformanceCounter and alike
#include "NanoWinPrivateProfile.h" // .ini file functions

// Some xtrats

#include "NanoWinShellEx.h" // SHCreateDirectory // originaly declared in "Shlobj.h"

#endif
