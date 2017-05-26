// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCFile functions

#if !defined(NanoWinMFCAfxFileIncluded)
#define NanoWinMFCAfxFileIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include "NanoWinFileFind.h"

#include "NanoWinMFCAfx.h"
#include "NanoWinMFCAfxStr.h"

class CFileFind : public CObject
{
  protected:
  WIN32_FIND_DATA   m_state;
  HANDLE            m_handle;
  BOOL              m_state_filled;

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
  }

  public:

  CFileFind()
  {
    m_handle = NULL;
    m_state_filled = FALSE;
  }

  virtual ~CFileFind()
  {
    CloseContext();
  }

  virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0)
  {
    CloseContext();

	m_handle = ::FindFirstFile(pstrName, &m_state);
	if (IsValidHandle(m_handle))
	{
      m_state_filled = TRUE;
      return(TRUE);
	}
	else
	{
      memset(&m_state, 0, sizeof(m_state)); // just in case
      return(FALSE);
	}
  }

  virtual BOOL FindNextFile()
  {
    if (!IsValidHandle(m_handle))
	{
      return(FALSE);
	}

	BOOL result = ::FindNextFile(m_handle, &m_state);

    if (!result)
    {
      CloseContext();
    }

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
    if (m_state_filled) { return CString(); } // TODO: Check should we throw exception?
	return(CString(m_state.cFileName));
  }

  // Returns true if the found file is a directory
  BOOL IsDirectory()
  const
  {
    if (m_state_filled) { return FALSE; } // TODO: Check should we throw exception?
	return(m_state.bNwIsDirectory);
  }

  virtual BOOL IsDots()
  const
  {
    if (m_state_filled) { return FALSE; } // TODO: Check should we throw exception?
    if ((m_state.cFileName[0] == '.') && (m_state.cFileName[1] == 0)) { return TRUE; } // Avoid string constants in inlines
    if ((m_state.cFileName[0] == '.') && (m_state.cFileName[1] == '.') && (m_state.cFileName[2] == 0)) { return TRUE; } // Avoid string constants in inlines
    return(FALSE);
  }

  // Returns file length in bytes.
  ULONGLONG GetLength()
  const
  {
    if (m_state_filled) { return 0; } // TODO: Check should we throw exception?
    ULONGLONG result = m_state.nFileSizeHigh; 
	result <<= (sizeof(ULONG)*8);
	result |= m_state.nFileSizeLow;
  }

  virtual CString GetFilePath() const; // TODO: Implement me
};

#endif // linux
#endif // ...Included
