# NanoWin32
Win32 API subset implementation via POSIX (MSVC Windows apps migration to GNU C Linux)

# Background
In general, if you want (not a complete list):
* Create Win32 windows application with GCC (Under Windows) : Use MinGW (http://www.mingw.org/)
* Create POSIX applications under Windows (and potentially run it under Linux) : Use CygWin (https://www.cygwin.com/)
* Port heavy Win32 dependent or GUI application to Linux : use Winelib (https://wiki.winehq.org/Winelib)
* Port existing Win32 app or lib (non-GUI, not heavy win32 dependent) to Linux with minimum changes : Use NanoWin32!

# Goal
To provide simple windows.h substitution to simulate most Win32 API types and functions for 32 bit and 64 bit POSIX platforms.
No all functions and types are provided, nor all functions have 100% equivalent, but in most cases source need small (or no) changes to compile. This library designed to implement only simple use cases, but this may allow to port large projects to Linux with minimum (or no) modifications. We plan to have a small library footprint and small codebase. 

# No GUI
There is no plan to implement any GUI functionality or GDI functionality of Win32 here. If you need such functionality its better to see Winelib project (https://wiki.winehq.org/Winelib).
