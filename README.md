# NanoWin32
Win32 API subset implementation via POSIX (MSVC Windows apps migration to GNU C Linux)

# Background
In general, if you want (not a complete list):
* Create Win32 windows application with GCC (Under Windows) : Use MinGW (http://www.mingw.org/)
* Create POSIX applications under Windows (and potentially run it under Linux) : Use CygWin (https://www.cygwin.com/)
* Port heavy Win32 dependent or GUI application to Linux : use Winelib (https://wiki.winehq.org/Winelib)
* Port existing Win32 app or lib (non-GUI, not heavy win32 dependent) to Linux with minimum changes : Use NanoWin32!

# Goal
To provide simple windows.h substitution to simulate most usefull Win32 API types and functions for 32 bit and 64 bit POSIX platforms.
No all functions and types are provided, nor all functions have 100% equivalent, but in most cases source need small (or no) changes to compile. 
This library designed to implement only simple use cases, but this may allow to port large projects to Linux with minimum (or no) modifications. We plan to have a small library footprint and small codebase. 

# No GUI
There is no plan to implement any GUI functionality or GDI functionality of Win32 here. If you need such functionality its better to see Winelib project (https://wiki.winehq.org/Winelib).

# How to use

## Basic

Include NanoWin.h instead of Windows.h. Note: you may also create symlink to NanoWin.h as Windows.h.
This file includes other NanoWin files and provides you with implemented subset of Win32 types and functions

## Advanced

Other files you probably may need to include:

### Microsoft extra features
* NanoWinTCHAR.h        // Nanowin substitute for TCHAR.h
* NanoWinMsExtra.h      // Extra functions defined in MS VS that is missing under GNU/POSIX like fullpath
* NanoWinMsSafeString.h // Safe string functions like strcpy_s, memset_s and MS VS templates for them
* NanoWinMsSafePrintf.h // Microsoft safe vsprintf_s, sprintf_s extensions
* NanoWinMsSafeScanf.h  // Microsoft safe sscanf_s, fscanf_s extensions (some exotic cases not supported, see note)

### Microsoft MFC subset
* NanoWinMFCAfx*.h      // Nanowin MFC Classes simulators (include it instead of MFCAfx*.h files)
* NanoWinMFCAfxNUI.h    // Nanowin NoGUI stub for some basic MFC GUI functions (actions will be just output to console)

### Quirks and tricks (include if you really need it)

These files alter existing POSIX functions to emulate non-POSIX MS VC behaviour, so include it only if you need it:

* NanoWinMsPrintf.h     // MSVC-like wprintf/wcsanf format support for %s,%c format chars (non-POSIX)
* NanoWinPathHook*.cpp  // Add to your project to allow path separator chars both as '/' and '\' . 
                           To provide this functionality you have to add link-stage hooks, 
                           see link note in NanoWinPathHookGate.cpp

