// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// CSocket implementation

#if !defined(NanoWinMFCAfxSocketIncluded)
#define NanoWinMFCAfxSocketIncluded

#include "NanoWinTypes.h"
#include "NanoWinMFCAfx.h"

#if defined LINUX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

typedef int SOCKET;

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR   (-1)
#define SOCKADDR       sockaddr 

NW_EXTERN_C_BEGIN

typedef void WSADATA;

extern BOOL AfxSocketInit (WSADATA *lpwsaData = NULL);

NW_EXTERN_C_END

class CSocket : public CObject
{
  public :

                   CSocket            ();

  BOOL             Create             (UINT nSocketPort = 0,
                                       int nSocketType = SOCK_STREAM,
                                       LPCSTR lpszSocketAddress = NULL);

  BOOL             Create             (UINT nSocketPort,
                                       int nSocketType,
                                       LPCWSTR lpszSocketAddress);

  virtual void     Close              ();

  BOOL             Connect            (LPCSTR     lpszHostAddress,
                                       UINT       nHostPort);
  BOOL             Connect            (LPCWSTR    lpszHostAddress,
                                       UINT       nHostPort);
  BOOL             Connect            (const SOCKADDR *lpSockAddr,
                                       int             nSockAddrLen);

  BOOL             Attach             (SOCKET hSocket);
  SOCKET           Detach             ();

  BOOL             SetSockOpt         (int         nOptionName,
                                       const void* lpOptionValue,
                                       int         nOptionLen,
                                       int         nLevel = SOL_SOCKET);

  virtual int      Receive            (void* lpBuf, int nBufLen, int nFlags = 0);

  virtual int      Send               (const void* lpBuf, int nBufLen, int nFlags = 0);

  void             CancelBlockingCall ();

  BOOL             IsBlocking         ();

  SOCKET           m_hSocket;
};


#endif

#endif


