#pragma once

#include <string>

using namespace std;

class TcpSocketImpl
{
public:
	TcpSocketImpl(string ip, short port, unsigned long timeOut)
	{
		this->ip = ip;
		this->port = port;
		this->timeOut = timeOut;
	};
	virtual ~TcpSocketImpl(void) { };

protected:
	string ip;
	short port;
	unsigned long timeOut;

public:
	virtual int OpenTCPSocket(void) = 0;
	virtual int CloseTCPSocket(void) = 0;
	virtual int WriteTCPSocket(unsigned char *data, int size) = 0;
	virtual int ReadTCPSOcket(unsigned char *data, int size) = 0;
	virtual int SetParameter(string ip, short port, unsigned long timeOut) = 0;
};