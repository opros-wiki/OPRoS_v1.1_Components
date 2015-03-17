#pragma once

#include "Socket.h"
#include "OprosLock.h"

#if defined(WIN32)
#include "UdpSocketWindows.h"

#else
#include "UdpSocketLinux.h"

#endif

class UdpSocket : public Socket
{
public:
	UdpSocket(void);
	virtual ~UdpSocket(void);

private:
	UdpSocketImpl *udpSocket;
	OprosLock lock;

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
};

