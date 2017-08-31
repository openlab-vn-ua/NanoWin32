#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "NanoWinError.h"
#include "NanoWinStrConvert.h"
#include "NanoWinMFCAfxSocket.h"

extern BOOL AfxSocketInit(WSADATA *)
{
  return TRUE;
}

CSocket::CSocket()
{
  m_hSocket = INVALID_SOCKET;
}

BOOL CSocket::Create(UINT nSocketPort, int nSocketType, LPCSTR lpszSocketAddress)
{
  if (nSocketPort != 0 || lpszSocketAddress != NULL)
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return FALSE;
  }

  m_hSocket = socket(AF_INET, nSocketType, 0);

  if (m_hSocket < 0)
  {
    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

BOOL CSocket::Create(UINT nSocketPort, int nSocketType, LPCWSTR lpszSocketAddress)
{
  if (lpszSocketAddress != NULL)
  {
    NanoWinSetLastError(ERROR_NOT_SUPPORTED);

    return FALSE;
  }

  return Create(nSocketPort,nSocketType,(LPCSTR)NULL);
}

void CSocket::Close()
{
  if (m_hSocket != INVALID_SOCKET)
  {
    close(m_hSocket);
  }
}

BOOL CSocket::Connect(LPCSTR lpszHostAddress, UINT nHostPort)
{
  struct sockaddr_in  serv_addr;
  struct hostent     *server;

  server = gethostbyname(lpszHostAddress);

  if (server == NULL)
  {
    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(h_errno));

    return FALSE;
  }

  memset(&serv_addr,0,sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  memcpy(server->h_addr,&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons((uint16_t)nHostPort);

  return Connect((SOCKADDR*)&serv_addr, sizeof(serv_addr));
}

BOOL CSocket::Connect(LPCWSTR lpszHostAddress, UINT nHostPort)
{
  return Connect(NanoWin::StrConverter::Convert(lpszHostAddress).c_str(),nHostPort);
}

BOOL CSocket::Connect(const SOCKADDR *lpSockAddr, int nSockAddrLen)
{
  if (connect(m_hSocket, (sockaddr*)lpSockAddr, nSockAddrLen) < 0)
  {
    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

BOOL CSocket::Attach(SOCKET hSocket)
{
  m_hSocket = hSocket;

  return TRUE;
}

SOCKET CSocket::Detach()
{
  SOCKET result = m_hSocket;
  m_hSocket = INVALID_SOCKET;

  return result;
}

BOOL CSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
{
  if (setsockopt(m_hSocket, nLevel, nOptionName, lpOptionValue, nOptionLen) < 0)
  {
    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));

    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

int CSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
  ssize_t result = recv(m_hSocket,lpBuf,nBufLen,nFlags);

  if (result < 0)
  {
    result = SOCKET_ERROR;

    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));
  }

  return (int)result;
}

int CSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
  ssize_t result = send(m_hSocket,lpBuf,nBufLen,nFlags);

  if (result < 0)
  {
    result = SOCKET_ERROR;

    NanoWinSetLastError(NanoWinErrorByErrnoAtFail(errno));
  }

  return (int)result;
}

void CSocket::CancelBlockingCall()
{
  close(m_hSocket);
}

BOOL CSocket::IsBlocking()
{
  return (fcntl(m_hSocket,F_GETFL) & O_NONBLOCK) == 0;
}
