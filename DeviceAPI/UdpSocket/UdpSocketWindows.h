#pragma once

#if defined(WIN32)
#include "UdpSocketImpl.h"
#include <Winsock.h>

class UdpSocketWindows : public UdpSocketImpl
{
public:
	UdpSocketWindows(string ip, short port, unsigned long timeOut);
	virtual ~UdpSocketWindows(void);

private:
	WSADATA wsaData;
	SOCKET hSocket;

public:
	int OpenUDPSocket(void);
	int CloseUDPSocket(void);
	int WriteUDPSocket(unsigned char *data, int size);
	int ReadUDPSOcket(unsigned char *data, int size);
	int SetParameter(string ip, short port, unsigned long timeOut);
};

#endif
