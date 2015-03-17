#pragma once

#include "Socket.h"
#include "OprosLock.h"

#if defined(WIN32)
#include "TcpSocketWindows.h"

#else
#include "TcpSocketLinux.h"

#endif

class TcpSocket : public Socket
{
public:
	TcpSocket(void);
	virtual ~TcpSocket(void);
public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int Lock(void);
	int Unlock(void);

	int Write(unsigned char *data, int size);
	int Read(unsigned char *data, int size);

private:
	TcpSocketImpl *tcpSocket;
	OprosLock lock;
};

