// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileSys functions

#include "NanoWinFileSys.h"

#include "NanoWinStrConvert.h"
#include "NanoWinError.h"

#include <unistd.h>

NW_EXTERN_C_BEGIN

extern BOOL PathFileExistsA(const char *lpPath)
{
  if (lpPath == NULL)
  {
    SetLastError(ERROR_INVALID_DATA);
    return(FALSE);
  }

  if (access(lpPath, F_OK) == 0)
  {
    return(TRUE);
  }

  SetLastError(ERROR_FILE_NOT_FOUND);
  return(FALSE);
}

extern BOOL PathFileExistsW(const wchar_t *lpPath)
{
  NanoWin::WStrToStrClone sPath(lpPath);
  return(PathFileExistsA(sPath.c_str()));
}

NW_EXTERN_C_END
