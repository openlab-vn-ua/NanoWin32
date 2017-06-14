// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// PathHook functions

#if !defined(NanoWinPathHookIncluded)
#define NanoWinPathHookIncluded

// You can install path hooks to be used by functions:
// file functions

#include "NanoWinTypes.h"

#if defined LINUX

#include "NanoWinFileSys.h"
#include <string>

namespace NanoWin {

#define PathHookPathSizeDefLimit            (_MAX_PATH) // No limit still means some reasonable limit :)

class PathHookException
{
  public:
  virtual const char *message() const { return("Path conversion error"); }
};

class PathHook
{
  public:

  // Converts path in-place (by replacing path content)
  // maxLength specify maximum size of path in characters path[pathmaxsz]
  // You may ignore pathmaxsz in case you are not changing path length or you shortening it
  // You may pass PathHookPathSizeNoLimit to the func to provide system-default reasonable limit
  // return 0 if ok, errno in case of fail
  virtual errno_t     ConvertPath(char *path, size_t pathmaxsz = PathHookPathSizeDefLimit)
  {
    return(0);
  }

  // Returns path converted as std::string
  // Default implementation calls ConvertPath
  // May throw exception PathHookException in case conversion is impossible
  virtual std::string GetConvertedPath(const char *path)
  {
    std::string outpath(path);
	outpath.reserve(PathHookPathSizeDefLimit); // actually, this will reserve 1 more byte
	errno_t result = this->ConvertPath(const_cast<char*>(outpath.c_str()), outpath.capacity());
	if (result != 0) { throw PathHookException(); } // Fire exit
    return(outpath);
  }
};

extern PathHook *PathHookIn;
extern PathHook *PathHookOut;

} // namespace NanoWin

#define PathHookInSetHook(phook)            (NanoWin::PathHookIn = phook), 0) // EOK
#define PathHookInGetHook()                 const_cast<const PathHook*>(NanoWin::PathHookIn)
#define PathHookOutSetHook(phook)           (NanoWin::PathHookOut = phook), 0) // EOK
#define PathHookOutGetHook()                const_cast<const PathHook*>(NanoWin::PathHookOut)

// Use these macroses in functions that require
#define PathHookInGetConvertedPath(path)    ((NanoWin::PathHookIn == NULL)  ? std::string(path) : NanoWin::PathHookIn->GetConvertedPath())
#define PathHookInTmpConvertedPath(path)    ((NanoWin::PathHookIn == NULL)  ? (path) : NanoWin::PathHookIn->GetConvertedPath().c_str())
#define PathHookInConvertPath(path,pathsz)  ((NanoWin::PathHookIn == NULL)  ? (0) : NanoWin::PathHookIn->ConvertPath(path,pathsz))

// Use these macroses in functions that returns path
#define PathHookOutGetConvertedPath(path)   ((NanoWin::PathHookOut == NULL) ? std::string(path) : NanoWin::PathHookOut->GetConvertedPath())
#define PathHookOutTmpConvertedPath(path)   ((NanoWin::PathHookOut == NULL) ? (path) : NanoWin::PathHookOut->GetConvertedPath().c_str())
#define PathHookOutConvertPath(path,pathsz) ((NanoWin::PathHookOut == NULL) ? (0) : NanoWin::PathHookOut->ConvertPath(path,pathsz))

#endif // linux
#endif // ...Included
