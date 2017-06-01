// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCFile functions

#if !defined(NanoWinMFCAfxFileIncluded)
#define NanoWinMFCAfxFileIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <libgen.h>

#include "NanoWinFileFind.h"

#include "NanoWinMFCAfx.h"
#include "NanoWinMFCAfxStr.h"

class CFileFind : public CObject
{
  protected:
  WIN32_FIND_DATA  *m_found_state;
  WIN32_FIND_DATA  *m_next_state;
  HANDLE            m_handle;
  BOOL              m_state_filled;
  CString           m_search_root;

  static BOOL IsValidHandle(HANDLE handle)
  {
    if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
	{
      return(FALSE);
	}

    return(TRUE);
  }

  virtual void CloseContext()
  {
    if (!IsValidHandle(m_handle))
    {
      // Already closed or not open
    }
	else
	{
      FindClose(m_handle);
	}

   m_handle = NULL;
   m_state_filled = FALSE;

   m_search_root.Empty();
  }

  public:

  CFileFind()
  {
    m_handle = NULL;
    m_state_filled = FALSE;

    m_next_state  = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));
    m_found_state = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));

    if (m_next_state == NULL || m_found_state == NULL)
    {
      free(m_next_state);
      free(m_found_state);

      throw new CMemoryException();
    }
  }

  virtual ~CFileFind()
  {
    free(m_next_state);
    free(m_found_state);

    CloseContext();
  }

  virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0)
  {
    CloseContext();

    LPCTSTR fileMask = pstrName != NULL ? pstrName : WILDCARD_ALL_FILES;

	m_handle = ::FindFirstFile(fileMask, m_next_state);

    if (IsValidHandle(m_handle))
    {
      if (!BuildSearchRootDir(fileMask))
      {
        CloseContext();
      }
    }

	if (IsValidHandle(m_handle))
	{
      return(TRUE);
	}
	else
	{
      memset(m_next_state, 0, sizeof(*m_next_state)); // just in case
      memset(m_found_state, 0, sizeof(*m_found_state)); // just in case
      return(FALSE);
	}
  }

  virtual BOOL FindNextFile()
  {
    if (!IsValidHandle(m_handle))
	{
      return(FALSE);
	}

    m_state_filled = TRUE;

    WIN32_FIND_DATA *temp_data;
    
    temp_data     = m_found_state;
    m_found_state = m_next_state;
    m_next_state  = temp_data;

	BOOL result = ::FindNextFile(m_handle, m_next_state);

	return(result);
  }

  void Close()
  {
    // Note: we can restart by rusing CFindFile object, but MFC API does not gurantee that
    CloseContext();
  }

  // Found file info

  // Returns the file name, including the extension. 
  // c:\myhtml\myfile.txt returns the file name myfile.txt
  virtual CString GetFileName()
  const
  {
    if (!m_state_filled) { return CString(); } // TODO: Check should we throw exception?
	return(CString(m_found_state->cFileName));
  }

  // Returns true if the found file is a directory
  BOOL IsDirectory()
  const
  {
    if (!m_state_filled) { return FALSE; } // TODO: Check should we throw exception?
	return(m_found_state->bNwIsDirectory);
  }

  virtual BOOL IsDots()
  const
  {
    if (!m_state_filled) { return FALSE; } // TODO: Check should we throw exception?
    if ((m_found_state->cFileName[0] == '.') && (m_found_state->cFileName[1] == 0)) { return TRUE; } // Avoid string constants in inlines
    if ((m_found_state->cFileName[0] == '.') && (m_found_state->cFileName[1] == '.') && (m_found_state->cFileName[2] == 0)) { return TRUE; } // Avoid string constants in inlines
    return(FALSE);
  }

  // Returns file length in bytes.
  ULONGLONG GetLength()
  const
  {
    if (!m_state_filled) { return 0; } // TODO: Check should we throw exception?
    ULONGLONG result = m_found_state->nFileSizeHigh; 
	result <<= (sizeof(ULONG)*8);
	result |= m_found_state->nFileSizeLow;
  }

  virtual CString GetFilePath() const
  {
    if (!m_state_filled) { return CString(); } // TODO: Check should we throw exception?

    CString result(m_search_root);

    result.Append(m_found_state->cFileName);

    return result;
  }

  private :

  static bool CStringEndsWithChar(const CString &str, TCHAR ch)
  {
    size_t len = str.GetLength();

    return len > 0 && str.GetAt(len - 1) == ch;
  }

  // returns string allocated by malloc
  static char *BuildAbsoluteSearchPath(const char *pstrName)
  {
    char *pstrCopy = strdup(pstrName);

    if (pstrCopy == NULL)
    {
      return NULL;
    }

    char *pathName = dirname(pstrCopy);
    char *absName  = realpath(pathName,NULL);

    free(pstrCopy);

    return absName;
  }

  #ifdef UNICODE
  bool BuildSearchRootDir (LPCWSTR pstrName)
  {
    bool  ok      = false;
    char *absName = NULL;

    try
    {
      std::string pstrNameMb = NanoWin::StrConverter::Convert(pstrName);

      absName = BuildAbsoluteSearchPath(pstrNameMb.c_str());

      if (absName != NULL)
      {
        std::wstring absNameW = NanoWin::StrConverter::Convert(absName);

        m_search_root.SetString(absNameW.c_str());

        if (!CStringEndsWithChar(m_search_root, DIR_SEPARATOR_CHAR))
        {
          m_search_root.AppendChar(DIR_SEPARATOR_CHAR);
        }

        ok = true;
      }
    }
    catch (...)
    {
    }

    free(absName);

    return ok;
  }
  #else
  bool BuildSearchRootDir (LPCSTR pstrName)
  {
    bool  ok      = false;
    char *absName = NULL;

    absName = BuildAbsoluteSearchPath(pstrName);

    if (absName != NULL)
    {
      m_search_root.SetString(absName);

      if (!CStringEndsWithChar(m_search_root, DIR_SEPARATOR_CHAR))
      {
        m_search_root.AppendChar(DIR_SEPARATOR_CHAR);
      }

      ok = true;
    }

    free(absName);

    return ok;
  }
  #endif

  private :

  static const TCHAR WILDCARD_ALL_FILES[];

  #ifdef UNICODE
  static const wchar_t DIR_SEPARATOR_CHAR = L'/';
  #else
  static const char    DIR_SEPARATOR_CHAR = '/';
  #endif
};

// MFC Files
// ------------------------------------

class CFile : public CObject // TODO: LINUX: Implement me (via FILE)
{
  public:

  enum OpenFlags
  {
    modeRead         = (int)0x00000, // +
    modeWrite        = (int)0x00001, // +
    modeReadWrite    = (int)0x00002,

    shareCompat      = (int)0x00000,
    shareExclusive   = (int)0x00010,
    shareDenyWrite   = (int)0x00020,
    shareDenyRead    = (int)0x00030,
    shareDenyNone    = (int)0x00040, // +

//  modeNoInherit    = (int)0x00080,

    #if defined(UNICODE) || defined(_UNICODE)
    typeUnicode      = (int)0x00400, // used in derived classes (e.g. CStdioFile) only // +
    #endif

    modeCreate       = (int)0x01000, // +
    modeNoTruncate   = (int)0x02000,
    typeText         = (int)0x04000, // used in derived classes (e.g. CStdioFile) only // +
    typeBinary       = (int)0x08000, // used in derived classes (e.g. CStdioFile) only
//  osNoBuffer       = (int)0x10000,
//	osWriteThrough   = (int)0x20000,
//	osRandomAccess   = (int)0x40000,
//	osSequentialScan = (int)0x80000,
  };

  public:
  HANDLE                m_hFile;

  static const HANDLE   hFileNull; // = NULL;

  protected:

  BOOL IsOpen()
  {
    return(m_hFile != hFileNull);
  }

  public:

                     CFile (); // nothrow()
                     CFile (LPCTSTR lpszFileName,  UINT nOpenFlags); // throw()
  virtual           ~CFile ();

  virtual BOOL       Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);  // nothrow()
  virtual void       Close();
  virtual void       Abort(); // Close if open, // nothrow()

  virtual UINT       Read  (void* lpBuf, UINT nCount);
  virtual void       Write (const void* lpBuf, UINT nCount);
  virtual void       Flush ();

  virtual ULONGLONG  Seek  (LONGLONG lOff, UINT nFrom);

  void               SeekToBegin();
  ULONGLONG          SeekToEnd();

  virtual ULONGLONG  GetLength() const;
};

#endif // linux
#endif // ...Included
