// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// FileSys functions

#include "NanoWinFileSys.h"

#include "NanoWinStrConvert.h"
#include "NanoWinError.h"

#include <errno.h>
#include <unistd.h>

NW_EXTERN_C_BEGIN

extern BOOL WINAPI PathFileExistsA(const char *lpPath)
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

extern BOOL WINAPI PathFileExistsW(const wchar_t *lpPath)
{
  NanoWin::WStrToStrClone sPath(lpPath);
  return(PathFileExistsA(sPath.c_str()));
}

extern BOOL WINAPI DeleteFileA(LPCSTR  lpFileName)
{
  if (unlink(lpFileName) == 0)
  {
    return(TRUE);
  }
  else
  {
    if (errno == ENOENT)
    {
      SetLastError(ERROR_FILE_NOT_FOUND);
	}
	else if (errno == EACCES)
	{
      SetLastError(ERROR_ACCESS_DENIED);
	}
	else
	{
      SetLastError(NanoWinErrorByErrnoAtFail(errno));
	}

    return(FALSE);
  }
}

extern BOOL WINAPI DeleteFileW(LPCWSTR lpFileName)
{
  NanoWin::WStrToStrClone sFileName(lpFileName);
  return(DeleteFileA(sFileName.c_str()));
}

NW_EXTERN_C_END
