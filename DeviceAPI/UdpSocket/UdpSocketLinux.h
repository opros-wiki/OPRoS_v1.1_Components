#pragma once

#if !defined(WIN32)

#include "UdpSocketImpl.h"

class UdpSocketLinux : public UdpSocketImpl
{
public:
	UdpSocketLinux(string ip, short port, unsigned long timeOut);
	virtual ~UdpSocketLinux(void);

private:
	int _socket;

public:
	int OpenUDPSocket(void);
	int CloseUDPSocket(void);
	int WriteUDPSocket(unsigned char *data, int size);
	int ReadUDPSOcket(unsigned char *data, int size);
	int SetParameter(string ip, short port, unsigned long timeOut);
};

#endif
