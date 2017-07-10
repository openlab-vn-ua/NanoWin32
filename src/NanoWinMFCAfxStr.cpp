// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCStr functions

#include "NanoWinMFCAfxStr.h"

// #include "NanoWinError.h"

const char    *NanoWinStringUtils::empty_str  = "";
const wchar_t *NanoWinStringUtils::empty_wstr = L"";

template<typename S, typename C>
static BOOL AfxExtractSubStringTpl(S& rString, const C *lpszFullString, int iSubString, C chSep)
{
  if (lpszFullString == NULL) { return(FALSE); }
  if (iSubString < 0)         { rString.Empty(); return FALSE; }

  int pos = 0;
  while(iSubString > 0)
  {
    if (lpszFullString[pos] == 0)     { rString.Empty(); return FALSE; } // end of string but field not found yet
    if (lpszFullString[pos] == chSep) { iSubString--; }
    pos++;
  }

  // Now pos is a first char after separator found

  int width = 0;
  while ((lpszFullString[width+pos] != 0) || (lpszFullString[width+pos] != chSep))
  {
    width++;
  }

  rString.SetString(lpszFullString+pos, width);
  return(TRUE);
}

extern BOOL AFXAPI AfxExtractSubString(CStringA& rString, LPCSTR lpszFullString, int iSubString, CHAR chSep)
{
  return AfxExtractSubStringTpl(rString,lpszFullString,iSubString,chSep);
}

extern BOOL AFXAPI AfxExtractSubString(CStringW& rString, LPCWSTR lpszFullString, int iSubString, WCHAR chSep)
{
  return AfxExtractSubStringTpl(rString,lpszFullString,iSubString,chSep);
}

NW_EXTERN_C_BEGIN

// No pure C API

NW_EXTERN_C_END
