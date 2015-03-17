#ifndef	_TCP_SOCKET_WINDOWS_H_
#define	_TCP_SOCKET_WINDOWS_H_

#if defined(WIN32)
#include <Winsock.h>

#include "TcpSocketImpl.h"

class TcpSocketWindows : public TcpSocketImpl
{
public:
	TcpSocketWindows(string ip, short port, unsigned long timeOut);
	virtual ~TcpSocketWindows(void);

public:
	int OpenTCPSocket(void);
	int CloseTCPSocket(void);
	int WriteTCPSocket(unsigned char *data, int size);
	int ReadTCPSOcket(unsigned char *data, int size);
	int SetParameter(string ip, short port, unsigned long timeOut);

private:
	WSADATA wsaData;
	SOCKET hSocket;
	bool isInitialized;
};

#endif

#endif	//	_TCP_SOCKET_WINDOWS_H_
