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

class PathHook
{
  public:

  // The function should translate srcpath if need
  // translation result should be returned to *poutpath (initially *poutpath is equal srcpath)
  // translated path may use buffer[buffersz] to put new (updated) path into
  // return 0 on ok or errno value in case of fail
  virtual errno_t doTranslatePath(const char **poutpath, const char *srcpath, char *buffer, size_t buffersz)
  {
	*poutpath = srcpath; // just in case
    return(0);
  }
};

extern PathHook *PathHookIn;  // Translate incoming  to function path to
extern PathHook *PathHookOut; // Translate returning from function path // reserved, not used actaully (yet)

// Practical implemenation
// (made ALT_PATH_SEP to be PATH_SEP)
// This makes Linux support path sep for both '/' and '\' like Win32 API does

class PathHookAlignSlashes : public PathHook
{
  public:

  // override
  virtual errno_t doTranslatePath(const char **poutpath, const char *srcpath, char *buffer, size_t buffersz);
};

} // namespace NanoWin

#endif // linux
#endif // ...Included
