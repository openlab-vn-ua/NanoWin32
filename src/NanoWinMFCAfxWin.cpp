// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCApp functions
// Subset of Windows API GUI functions stubs for MFC applications

#include "NanoWinMFCAfxWin.h"

#include "NanoWinError.h"

static int NanoWinGetMessageBoxDefaultButton(int nType)
{
  int button = NW_MB_GET_BUTTON(nType);
  switch(button)
  {
	case(MB_OK                       ) : return(IDOK);
	case(MB_OKCANCEL                 ) : return(NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON2 ? IDCANCEL   : IDOK);
	case(MB_ABORTRETRYIGNORE         ) : return(NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON3 ? IDIGNORE   : NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON2 ? IDRETRY    : IDABORT);
	case(MB_YESNOCANCEL              ) : return(NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON3 ? IDCANCEL   : NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON2 ? IDNO       : IDYES);
	case(MB_YESNO                    ) : return(NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON2 ? IDNO       : IDYES);
	case(MB_RETRYCANCEL              ) : return(NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON2 ? IDCANCEL   : IDRETRY);
	case(MB_CANCELTRYCONTINUE        ) : return(NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON3 ? IDCONTINUE : NW_MB_GET_DEFBUTTON(nType) == MB_DEFBUTTON2 ? IDTRYAGAIN : IDCANCEL);
  }
  return(0); // Fail: Unknown button to select // return(IDOK);
}

NW_EXTERN_C int WINAPI MessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
  NW_UNREFERENCED_PARAMETER(hWnd);
  NW_UNREFERENCED_PARAMETER(lpText);
  NW_UNREFERENCED_PARAMETER(lpCaption);
  return(NanoWinGetMessageBoxDefaultButton(uType));
}

NW_EXTERN_C int WINAPI MessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
  NW_UNREFERENCED_PARAMETER(hWnd);
  NW_UNREFERENCED_PARAMETER(lpText);
  NW_UNREFERENCED_PARAMETER(lpCaption);
  return(NanoWinGetMessageBoxDefaultButton(uType));
}
