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

// CFileFind
// --------------

const TCHAR CFileFind::WILDCARD_ALL_FILES[] = _T("*.*");

// CFile tools
// --------------

static void AfxFillFileExceptionWithWin32Error(CFileException *pe, DWORD copy_error_from_errcode)
{
}

static void AfxFillFileExceptionWithErrno(CFileException *pe, int copy_error_from_errno)
{
}

static void AfxThrowFileExceptionWithWin32Error(DWORD copy_error_from_errcode)
{
  throw new CFileException();
}

static void AfxThrowFileExceptionWithErrno(int copy_error_from_errno)
{
  throw new CFileException();
}

static void AfxThrowFileException(CFileException *copy_error_from_e)
{
  throw new CFileException();
}

static void AfxThrowFileException()
{
  throw new CFileException();
}

// CFile
// --------------

const HANDLE CFile::hFileNull = INVALID_HANDLE_VALUE;

CFile::CFile()
{
  InitAsClosed();
}

CFile::CFile (CAtlTransactionManager* pTM) // nothrow()
       : CFile()
{
  m_pTM = pTM;
}

CFile::CFile(HANDLE hFile)
{
  InitAsClosed();
  m_hFile = hFile;
  m_bCloseOnDelete = FALSE; // Do not close me if open externaly
}

CFile::CFile(LPCSTR lpszFileName, UINT nOpenFlags)
{
  InitAsClosed();

  CFileException errorInfo;

  if (!Open(lpszFileName, nOpenFlags, &errorInfo))
  {
    AfxThrowFileException(&errorInfo);
  }
}

CFile::CFile(LPCWSTR lpszFileName, UINT nOpenFlags)
{
  InitAsClosed();

  CFileException errorInfo;

  if (!Open(lpszFileName, nOpenFlags, &errorInfo))
  {
    AfxThrowFileException(&errorInfo);
  }
}

CFile::CFile(LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM)
       : CFile(lpszFileName, nOpenFlags)
{
  m_pTM = pTM;
}

CFile::CFile(LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM)
       : CFile(lpszFileName, nOpenFlags)
{
  m_pTM = pTM;
}

CFile::~CFile()
{
  if ((m_hFile != INVALID_HANDLE_VALUE) && (m_bCloseOnDelete))
  {
    Close();
  }
}

BOOL CFile::Open(LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError)
{
  m_pTM = pTM;
  return(Open(lpszFileName, nOpenFlags, pError));
}

BOOL CFile::Open(LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError)
{
  m_pTM = pTM;
  return(Open(lpszFileName, nOpenFlags, pError));
}

BOOL CFile::Open(LPCWSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
  ASSERT(lpszFileName != NULL);

  return Open(CW2A(lpszFileName),nOpenFlags,pError);
}
 
BOOL CFile::Open(LPCSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
  ASSERT(lpszFileName != NULL);
  ASSERT(m_hFile == INVALID_HANDLE_VALUE);

  InitAsClosed();

  if (lpszFileName == NULL)
  {
    AfxFillFileExceptionWithErrno(pError, EINVAL);
    return(FALSE);
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
      AfxFillFileExceptionWithErrno(pError, EINVAL);
      return(FALSE);
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
    AfxFillFileExceptionWithWin32Error(pError, GetLastError());
    return(FALSE);
  }
  else
  {
    m_bCloseOnDelete = TRUE;
    return TRUE;
  }
}

void CFile::Close()
{
  m_bCloseOnDelete = FALSE; // No matter what, do not try close me after this call
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    BOOL closeStatus = CloseFileHandle(m_hFile);

    m_hFile = INVALID_HANDLE_VALUE;

    if (!closeStatus)
    {
      InitAsClosed();
      AfxThrowFileException();
    }
  }
  InitAsClosed();
}

void CFile::Abort()
{
  m_bCloseOnDelete = FALSE; // No matter what, do not try close me after this call
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
    CloseFileHandle(m_hFile);
    m_hFile = INVALID_HANDLE_VALUE;
  }
  InitAsClosed();
}

void CFile::Flush()
{
  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  if (!FlushFileBuffers(m_hFile))
  {
    AfxThrowFileExceptionWithWin32Error(GetLastError());
  }
}

// Action methods

UINT CFile::Read(void* lpBuf, UINT nCount)
{
  DWORD readCount;

  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  if (!ReadFile(m_hFile, lpBuf, nCount, &readCount, NULL))
  {
    AfxThrowFileExceptionWithWin32Error(GetLastError());
  }

  return readCount;
}

void CFile::Write(const void* lpBuf, UINT nCount)
{
  DWORD writtenCount;

  ASSERT(m_hFile != INVALID_HANDLE_VALUE);

  if (!WriteFile(m_hFile, lpBuf, nCount, &writtenCount, NULL))
  {
    AfxThrowFileExceptionWithWin32Error(GetLastError());
  }
  else
  {
    if (writtenCount < nCount)
    {
      AfxThrowFileExceptionWithErrno(ENODATA);
    }
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
      AfxThrowFileExceptionWithWin32Error(GetLastError());
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
      AfxThrowFileExceptionWithWin32Error(GetLastError());
    }
  }

  return (((ULONGLONG)highPart) << (sizeof(DWORD) * 8)) | lowPart;
}

// CStdioFile
// --------------

CStdioFile::CStdioFile ()
  : CFile()
{
  m_pStream = NULL;
}

CStdioFile::CStdioFile (CAtlTransactionManager* pTM)
  : CFile(pTM)
{
  m_pStream = NULL;
}

CStdioFile::CStdioFile(FILE *stream)
  : CFile(NanoWinStdioFileAsFileHandle(stream))
{
  m_pStream = stream;
}

CStdioFile::CStdioFile(LPCSTR lpszFileName, UINT nOpenFlags)
  : CFile(lpszFileName,nOpenFlags)
{
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

CStdioFile::CStdioFile(LPCWSTR lpszFileName, UINT nOpenFlags)
  : CFile(lpszFileName,nOpenFlags)
{
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

CStdioFile::CStdioFile(LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM)
  : CFile(lpszFileName,nOpenFlags, pTM)
{
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

CStdioFile::CStdioFile(LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM)
  : CFile(lpszFileName,nOpenFlags, pTM)
{
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
}

BOOL CStdioFile::Open(LPCSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
  BOOL result = CFile::Open(lpszFileName, nOpenFlags, pError);
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
  return result;
}

BOOL CStdioFile::Open(LPCWSTR lpszFileName, UINT nOpenFlags, CFileException* pError)
{
  BOOL result = CFile::Open(lpszFileName, nOpenFlags, pError);
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
  return result;
}

BOOL CStdioFile::Open(LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError)
{
  BOOL result = CFile::Open(lpszFileName, nOpenFlags, pTM, pError);
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
  return result;
}

BOOL CStdioFile::Open(LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError)
{
  BOOL result = CFile::Open(lpszFileName, nOpenFlags, pTM, pError);
  m_pStream = NanoWinFileHandleAsStdioFILE(m_hFile);
  return result;
}

void CStdioFile::Close()
{
  CFile::Close();
  m_pStream = NULL;
}

void CStdioFile::Abort()
{
  CFile::Abort();
  m_pStream = NULL;
}

void CStdioFile::Flush()
{
  CFile::Flush();
}

// Action methods

void CStdioFile::WriteString(LPCSTR lpsz)
{
  if (lpsz == NULL)
  {
    AfxThrowFileExceptionWithErrno(EINVAL);
  }

  if (m_pStream == NULL)
  {
    AfxThrowFileException();
  }

  if (fputs(lpsz,m_pStream) < 0)
  {
    AfxThrowFileExceptionWithErrno(errno);
  }
}

void CStdioFile::WriteString(LPCWSTR lpsz)
{
  if (lpsz == NULL)
  {
    AfxThrowFileExceptionWithErrno(EINVAL);
  }

  if (m_pStream == NULL)
  {
    AfxThrowFileException();
  }

  NanoWin::WStrToStrClone str(lpsz);

  if (str.is_valid())
  {
    if (fputs(str.c_str(),m_pStream) < 0)
    {
      AfxThrowFileExceptionWithErrno(errno);
    }
  }
  else
  {
    AfxThrowFileException();
  }
}

LPSTR CStdioFile::ReadString(_Out_writes_z_(nMax) LPSTR lpsz, _In_ UINT nMax)
{
  if (lpsz == NULL)
  {
    AfxThrowFileExceptionWithErrno(EINVAL);
  }

  if (nMax < 1)
  {
    AfxThrowFileExceptionWithErrno(EINVAL);
  }

  if (m_pStream == NULL)
  {
    AfxThrowFileException();
  }

  return ReadToMbString(lpsz,nMax);
}

LPWSTR CStdioFile::ReadString(_Out_writes_z_(nMax) LPWSTR lpsz, _In_ UINT nMax)
{
  if (lpsz == NULL)
  {
    AfxThrowFileExceptionWithErrno(EINVAL);
  }

  if (nMax < 1)
  {
    AfxThrowFileExceptionWithErrno(EINVAL);
  }

  if (m_pStream == NULL)
  {
    AfxThrowFileException();
  }

  return ReadToWideString(lpsz,nMax);
}

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
      return NULL; // dummy bug trap, control would never be here since exception will be thrown
    }
  }
  else
  {
    return lpsz;
  }
}

BOOL CStdioFile::ReadString(CStringA& rString)
{
  CHAR buffer[255 + 1];

  rString.Empty();

  bool  eof       = false;
  bool  lineReady = false;

  while (!eof && !lineReady)
  {
    eof = ReadString(buffer, sizeof(buffer)) == NULL;

    if (!eof)
    {
      rString.Append(buffer);

      const CHAR *str       = rString.GetString();
      int         strLength = rString.GetLength();

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

BOOL CStdioFile::ReadString(CStringW& rString)
{
  WCHAR buffer[255 + 1];

  rString.Empty();

  bool  eof       = false;
  bool  lineReady = false;

  while (!eof && !lineReady)
  {
    eof = ReadString(buffer, sizeof(buffer) / sizeof(WCHAR)) == NULL;

    if (!eof)
    {
      rString.Append(buffer);

      const WCHAR *str       = rString.GetString();
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
