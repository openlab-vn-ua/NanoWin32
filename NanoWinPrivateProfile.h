#ifndef __NANOWINPRIVATEPROFILE_H__
#define __NANOWINPRIVATEPROFILE_H__

#include "NanoWinTypes.h"

#if defined(LINUX)
#else
#include <windows.h>"
#endif

NW_EXTERN_C_BEGIN

extern DWORD NanoWinGetPrivateProfileStringA 
                                      (const char *lpszSection, const char *plszEntry,
                                       const char *lpszDefault, char *lpReturnedString, DWORD nSize,
                                       const char *lpszFilename);

extern DWORD NanoWinGetPrivateProfileStringW 
                                      (const wchar_t *lpszSection, const wchar_t *plszEntry,
                                       const wchar_t *lpszDefault, wchar_t *lpReturnedString, DWORD nSize,
                                       const wchar_t *lpszFilename);

// NOTE: current implementation only allows to check presence of the section (returns value > 0 if section
//       is present and 0 if not) it doesn't return actual section value
extern DWORD NanoWinGetPrivateProfileSectionA
                                      (const char *lpszSection,
                                       char *lpReturnedString, DWORD nSize,
                                       const char *lpszFilename);

extern UINT NanoWinGetPrivateProfileIntA 
                                      (const char *lpszSection, const char *lpszEntry, int vdefault,
                                       const char *lpszFilename);

// NOTE: current Write... functions implementation doesn't support section or value removal
extern BOOL NanoWinWritePrivateProfileStringA
                                      (const char *lpszSection, const char *lpszEntry,
                                       const char *lpszString,
                                       const char *lpszFileName);

extern BOOL NanoWinWritePrivateProfileStringW
                                      (const wchar_t *lpszSection, const wchar_t *lpszEntry,
                                       const wchar_t *lpszString,
                                       const wchar_t *lpszFileName);

NW_EXTERN_C_END

#if defined(LINUX)

#define GetPrivateProfileStringA       NanoWinGetPrivateProfileStringA
#define GetPrivateProfileStringW       NanoWinGetPrivateProfileStringW
#define GetPrivateProfileSectionA      NanoWinGetPrivateProfileSectionA
#define GetPrivateProfileIntA          NanoWinGetPrivateProfileIntA
#define WritePrivateProfileStringA     NanoWinWritePrivateProfileStringA
#define WritePrivateProfileStringW     NanoWinWritePrivateProfileStringW

#if defined(UNICODE) || defined(_UNICODE)
#define GetPrivateProfileString        GetPrivateProfileStringW
#define GetPrivateProfileSection       NW_FORCE_COMPILE_ERROR // Not implemented (yet)
#define GetPrivateProfileInt           NW_FORCE_COMPILE_ERROR // Not implemented (yet)
#define WritePrivateProfileString      WritePrivateProfileStringW
#else
#define GetPrivateProfileString        GetPrivateProfileStringA
#define GetPrivateProfileSection       GetPrivateProfileSectionA
#define GetPrivateProfileInt           GetPrivateProfileIntA
#define WritePrivateProfileString      WritePrivateProfileStringA
#endif

#endif

#endif
