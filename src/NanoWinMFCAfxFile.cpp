// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCFile functions

#include "wchar.h"
#include "limits.h"
#include "stdio.h"

#include "NanoWinMFCAfxFile.h"

#include "NanoWinFile.h"
#include "NanoWinError.h"
#include "NanoWinStrConvert.h"
#include "NanoWinInternal.h"

#ifndef AfxThrowInvalidArgException
 #define AfxThrowInvalidArgException() throw new CInvalidArgException()
#endif

#ifndef AfxThrowFileException
 #define AfxThrowFileException() throw new CFileException()
#endif


#ifdef UNICODE
const TCHAR CFileFind::WILDCARD_ALL_FILES[] = L"*.*";
#else
const TCHAR CFileFind::WILDCARD_ALL_FILES[] = "*.*";
#endif

const HANDLE CFile::hFileNull = INVALID_HANDLE_VALUE;

CFile::CFile()
{
  m_hFile = INVALID_HANDLE_VALUE;
}

CFile::CFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
  m_hFile = INVALID_HANDLE_VALUE;

  CFileException errorInfo;

  if (!Open(lpszFileName, nOpenFlags, &errorInfo))
  {
    AfxThrowFileException();
  }
}

CFile::~CFile()
{
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    Close();
  }
}

BOOL CFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
  ASSERT(lpszFileName != NULL);
  ASSERT(m_hFile == INVALID_HANDLE_VALUE);

  if (lpszFileName == NULL)
  {
    AfxThrowInvalidArgException();
  }

  DWORD desiredAccess = 0;

  switch (nOpenFlags & 3)
  {
    case (modeRead):
    {
      desiredAccess = GENERIC_READ;
    } break;

    case (modeWrite):
    {
      desiredAccess = GENERIC_WRITE;
    } break;

    case (modeReadWrite):
    {
      desiredAccess = GENERIC_READ | GENERIC_WRITE;
    } break;

    default :
    {
      AfxThrowInvalidArgException();
    }
  }

  DWORD creationDisposition = OPEN_EXISTING;

  if (nOpenFlags & modeCreate)
  {
    creationDisposition = (nOpenFlags & modeNoTruncate) ? OPEN_ALWAYS : CREATE_ALWAYS;
  }

  DWORD shareMode = 0;
  UINT  shareFlags = nOpenFlags & 0x70;

  if      (shareFlags == shareDenyWrite)
  {
    shareMode = FILE_SHARE_READ;
  }
  else if (shareFlags == shareDenyRead)
  {
    shareMode = FILE_SHARE_WRITE;
  }
  else if (shareFlags == shareDenyNone)
  {
    shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
  }

  m_hFile = CreateFile(lpszFileName,desiredAccess,shareMode,NULL,creationDisposition,FILE_ATTRIBUTE_NORMAL,NULL);

  if (m_hFile == INVALID_HANDLE_VALUE)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

void CFile::Close()
{
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    BOOL closeStatus = CloseFileHandle(m_hFile);

    m_hFile = INVALID_HANDLE_VALUE;

    if (!closeStatus)
    {
      AfxThrowFileException();
    }
  }
}

void CFile::Abort()
{
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    CloseFileHandle(m_hFile);

    m_hFile = INVALID_HANDLE_VALUE;
  }
}

UINT CFile::Read(void* lpBuf, UINT nCount)
{
  DWORD readCount;

  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  if (!ReadFile(m_hFile, lpBuf, nCount, &readCount, NULL))
  {
    AfxThrowFileException();
  }

  return readCount;
}

void CFile::Write(const void* lpBuf, UINT nCount)
{
  DWORD writtenCount;

  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  if (!WriteFile(m_hFile, lpBuf, nCount, &writtenCount, NULL))
  {
    AfxThrowFileException();
  }
  else
  {
    if (writtenCount < nCount)
    {
      AfxThrowFileException();
    }
  }
}

void CFile::Flush()
{
  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  if (!FlushFileBuffers(m_hFile))
  {
    AfxThrowFileException();
  }
}

ULONGLONG CFile::Seek(LONGLONG lOff, UINT nFrom)
{
  LARGE_INTEGER offset;

  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  offset.QuadPart = lOff;

  offset.LowPart = SetFilePointer(m_hFile,offset.LowPart,&offset.HighPart,nFrom);

  if (offset.LowPart == INVALID_SET_FILE_POINTER)
  {
    if (GetLastError() != ERROR_SUCCESS)
    {
      AfxThrowFileException();
    }
  }

  return offset.QuadPart;
}

void CFile::SeekToBegin()
{
  Seek(0,FILE_BEGIN);
}

ULONGLONG CFile::SeekToEnd()
{
  return Seek(0,FILE_END);
}

ULONGLONG CFile::GetLength() const
{
  DWORD lowPart;
  DWORD highPart;

  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  lowPart = GetFileSize(m_hFile,&highPart);

  if (lowPart == INVALID_FILE_SIZE)
  {
    if (GetLastError() != ERROR_SUCCESS)
    {
      AfxThrowFileException();
    }
  }

  return (((ULONGLONG)highPart) << (sizeof(DWORD) * 8)) | lowPart;
}

CStdioFile::CStdioFile ()
{
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

CStdioFile::CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags)
  : CFile(lpszFileName,nOpenFlags)
{
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

BOOL CStdioFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
  BOOL result = CFile::Open(lpszFileName, nOpenFlags, pError);

  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);

  return result;
}

void CStdioFile::WriteString(LPCTSTR lpsz)
{
  if (lpsz == NULL)
  {
    AfxThrowInvalidArgException();
  }

  if (m_pStream == NULL)
  {
    AfxThrowFileException();
  }

  #if defined(UNICODE) || defined(_UNICODE)
  {
    NanoWin::WStrToStrClone str(lpsz);

    if (str.is_valid())
    {
      if (fputs(str.c_str(),m_pStream) < 0)
      {
        AfxThrowFileException();
      }
    }
    else
    {
      AfxThrowFileException();
    }
  }
  #else
  {
    if (fputs(lpsz,m_pStream) < 0)
    {
      AfxThrowFileException();
    }
  }
  #endif
}

LPTSTR CStdioFile::ReadString(_Out_writes_z_(nMax) LPTSTR lpsz, _In_ UINT nMax)
{
  if (lpsz == NULL)
  {
    AfxThrowInvalidArgException();
  }

  if (nMax < 1)
  {
    AfxThrowInvalidArgException();
  }

  if (m_pStream == NULL)
  {
    AfxThrowFileException();
  }

  #if defined(UNICODE) || defined(_UNICODE)
  {
    return ReadToWideString(lpsz,nMax);
  }
  #else
  {
    return ReadToMbString(lpsz,nMax);
  }
  #endif
}

#if defined(UNICODE) || defined(_UNICODE)
BOOL CStdioFile::ReadToWideChar(WCHAR *wch)
{
  mbstate_t convState;
  char      buffer[MB_LEN_MAX];

  memset(&convState,0,sizeof(convState));

  size_t offset        = 0;
  bool   eof           = false;
  bool   charCompleted = false;

  while (!charCompleted && !eof)
  {
    int ch = getc(m_pStream);

    if (ch != EOF)
    {
      if (offset >= sizeof(buffer))
      {
        // it is possible for offset to be >= MB_LEN_MAX if input stream
        // contains redundant shift sequences
        AfxThrowFileException();
      }

      buffer[offset++] = (char)ch;

      wchar_t tempChar;

      size_t n = mbrtowc(&tempChar,buffer,offset,&convState);

      if      (n == static_cast<size_t>(-1))
      {
        // cannot convert mb char sequence
        AfxThrowFileException();
      }
      else if (n == static_cast<size_t>(-2))
      {
        // need next char(s) to complete conversion
      }
      else
      {
        *wch          = tempChar;
        charCompleted = true;
      }
    }
    else
    {
      eof = feof(m_pStream) && offset == 0;

      // throw an exception on i/o error or if stream contains
      // incomplete multi-byte sequence at the end
      if (!eof)
      {
        AfxThrowFileException();
      }
    }
  }

  return !eof;
}

LPWSTR CStdioFile::ReadToWideString(_Out_writes_z_(nMax) LPWSTR lpsz, _In_ UINT nMax)
{
  UINT offset   = 0;
  UINT maxCount = nMax - 1; // caller must ensure that nMax > 0
  bool eofReached    = false;
  bool lineCompleted = false;

  try
  {
    while (!lineCompleted && !eofReached && offset < maxCount)
    {
      wchar_t currChar;

      eofReached = ReadToWideChar(&currChar);

      if (!eofReached)
      {
        if (currChar == L'\n')
        {
          // remove trailing '\r' if present
          if (offset > 0 && lpsz[offset - 1] == L'\r')
          {
            offset--;
          }

          lineCompleted = true;
        }

        lpsz[offset++] = currChar;
      }
    }
  }
  catch (...)
  {
    lpsz[offset] = L'\0';

    throw;
  }

  lpsz[offset] = L'\0';

  return offset > 0 ? lpsz : NULL;
}

#else

LPSTR CStdioFile::ReadToMbString(_Out_writes_z_(nMax) LPSTR  lpsz, _In_ UINT nMax)
{
  if (fgets(lpsz,nMax,m_pStream) == NULL)
  {
    if (feof(m_pStream))
    {
      lpsz[0] = '\0';

      return NULL;
    }
    else
    {
      AfxThrowFileException();
    }
  }
  else
  {
    return lpsz;
  }
}
#endif // UNICODE

BOOL CStdioFile::ReadString(CString& rString)
{
  TCHAR buffer[255 + 1];

  rString.Empty();

  bool  eof       = false;
  bool  lineReady = false;

  while (!eof && !lineReady)
  {
    eof = ReadString(buffer, sizeof(buffer)) == NULL;

    if (!eof)
    {
      rString.Append(buffer);

      const TCHAR *str       = rString.GetString();
      int          strLength = rString.GetLength();

      if (str[strLength - 1] == '\n')
      {
        lineReady = true;

        if (strLength > 1 && str[strLength - 2] == '\r')
        {
          rString.Truncate(strLength - 2);
        }
        else
        {
          rString.Truncate(strLength - 1);
        }
      }
    }
  }

  return !(eof && rString.GetLength() == 0);
}

void CStdioFile::Close()
{
  CFile::Close();

  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

NW_EXTERN_C_BEGIN


NW_EXTERN_C_END
