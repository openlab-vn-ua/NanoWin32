// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// PathHook functions

#include "NanoWinPathHook.h"

// Practical implemenation

#include <errno.h>
#include <string.h>
#include "NanoWinTypes.h" // errno_t
#include "NanoWinFileSys.h" // NW_DIRECTORY_SEPARATOR_* constants

#ifndef EOK
#define EOK (0)
#endif

namespace NanoWin {

// Translate slashes (to made path immune to '/' or '\'). Makes path under linux work like under win32

// override
errno_t PathHookAlignSlashes::doTranslatePath(const char **poutpath, const char *srcpath, char *buffer, size_t buffersz)
{
  #define REP_FROM_C NW_DIRECTORY_SEPARATOR_ALT_CHAR // Under linux = '\\'
  #define REP_TO_C   NW_DIRECTORY_SEPARATOR_CHAR     // Under Linux = '/'

  if ((srcpath != NULL) && (srcpath[0] != 0))
  {
    // path not void
    const char *cptr = strchr(srcpath, REP_FROM_C);
    if (cptr != NULL)
    {
      // we have something to replace
      errno_t result = EOK;

      if (result == EOK)
      {
        size_t srclen = strlen(srcpath);
        if (srclen >= buffersz)
        {
          result = ERANGE;
        }
        else
        {
          strcpy(buffer, srcpath);
        }
      }

      if (result == EOK)
      {
        const char *rptr = (strchr(buffer, REP_FROM_C));
        while(rptr != NULL)
        {
          *(const_cast<char*>(rptr)) = REP_TO_C;
          rptr++;
          rptr = strchr(rptr, REP_FROM_C);
        }
      }

      if (result == EOK)
      {
        *poutpath = buffer; // now points to translated path
      }

      return(result);
    }
  }

  return(0);

  #undef REP_TO_C
  #undef REP_FROM_C
}

// Default path hook

static 
PathHookAlignSlashes  PathHookInDefault;

// Path hook vars with default path handler installation

//extern 
PathHook *PathHookIn  = &PathHookInDefault;

//extern 
PathHook *PathHookOut = NULL;

} // namespace NanoWin
