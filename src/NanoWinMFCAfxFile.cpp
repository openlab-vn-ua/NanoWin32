// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCFile functions

#include "NanoWinMFCAfxFile.h"

#include "NanoWinFile.h"
#include "NanoWinError.h"

#ifndef AfxThrowInvalidArgException
 #define AfxThrowInvalidArgException() throw new CInvalidArgException()
#endif

#ifndef AfxThrowFileException
 #define AfxThrowFileException() throw new CFileException()
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

  m_hFile = CreateFile(lpszFileName,desiredAccess,0,NULL,creationDisposition,FILE_ATTRIBUTE_NORMAL,NULL);

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

NW_EXTERN_C_BEGIN


NW_EXTERN_C_END
