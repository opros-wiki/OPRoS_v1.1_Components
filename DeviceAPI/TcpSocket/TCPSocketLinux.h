#pragma once
#include "TcpSocketImpl.h"

#if !defined(WIN32)

class TcpSocketLinux : public TcpSocketImpl
{
public:
	TcpSocketLinux(string ip, short port, unsigned long timeOut);
	virtual ~TcpSocketLinux(void);

private:
	int _socket;

public:
	int OpenTCPSocket(void);
	int CloseTCPSocket(void);
	int WriteTCPSocket(unsigned char *data, int size);
	int ReadTCPSOcket(unsigned char *data, int size);
	int SetParameter(string ip, short port, unsigned long timeOut);
};

#endif
