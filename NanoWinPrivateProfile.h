#ifndef __NANOWINPRIVATEPROFILE_H__
#define __NANOWINPRIVATEPROFILE_H__

#include "NanoWinTypes.h"

EXTERN_C_BEGIN

extern DWORD ProfGetPrivateProfileStringA (const char *lpszSection, const char *plszEntry,
           	                               const char *lpszDefault, char *lpReturnedString, DWORD nSize,
	                                       const char *lpszFilename);

extern DWORD ProfGetPrivateProfileStringW (const wchar_t *lpszSection, const wchar_t *plszEntry,
                                           const wchar_t *lpszDefault, wchar_t *lpReturnedString, DWORD nSize,
	                                       const wchar_t *lpszFilename);

// NOTE: current implementation only allows to check presence of the section (returns value > 0 if section
//       is present and 0 if not) it doesn't return actual section value
extern DWORD ProfGetPrivateProfileSectionA(const char *lpszSection,
	                                       char *lpReturnedString, DWORD nSize,
	                                       const char *lpszFilename);

extern UINT ProfGetPrivateProfileIntA     (const char *lpszSection, const char *lpszEntry, int vdefault,
	                                       const char *lpszFilename);

// NOTE: current Write... functions implementation doesn't support section or value removal
extern BOOL ProfWritePrivateProfileStringA(const char *lpszSection, const char *lpszEntry,
	                                       const char *lpszString,
	                                       const char *lpszFileName);

extern BOOL ProfWritePrivateProfileStringW(const wchar_t *lpszSection, const wchar_t *lpszEntry,
	                                       const wchar_t *lpszString,
	                                       const wchar_t *lpszFileName);

EXTERN_C_END

#define GetPrivateProfileStringA   ProfGetPrivateProfileStringA
#define GetPrivateProfileStringW   ProfGetPrivateProfileStringW
#define GetPrivateProfileSectionA  ProfGetPrivateProfileSectionA
#define GetPrivateProfileIntA      ProfGetPrivateProfileIntA
#define WritePrivateProfileStringA ProfWritePrivateProfileStringA
#define WritePrivateProfileStringW ProfWritePrivateProfileStringW

#endif
