#if defined(WIN32)

#include "TcpSocketWindows.h"

#pragma comment (lib,"ws2_32.lib")

TcpSocketWindows::TcpSocketWindows(string ip, short port, unsigned long timeOut)
: TcpSocketImpl(ip, port, timeOut)
{
	hSocket = INVALID_SOCKET;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		isInitialized = false;
	}
	else {
		isInitialized = true;
	}
}

TcpSocketWindows::~TcpSocketWindows(void)
{
	CloseTCPSocket();

	WSACleanup();
}

int TcpSocketWindows::OpenTCPSocket(void)
{
	CloseTCPSocket();
	
	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET) {
		return -1;
	}

	SOCKADDR_IN tcp_sock_addr;

	memset(&tcp_sock_addr, 0, sizeof(tcp_sock_addr));
	tcp_sock_addr.sin_family = AF_INET;
	tcp_sock_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	tcp_sock_addr.sin_port = htons((short)port);

	if(connect(hSocket, (SOCKADDR *)&tcp_sock_addr, sizeof(tcp_sock_addr)) == SOCKET_ERROR) {
		CloseTCPSocket();
		return -1;
	}

	int opt_timeout = timeOut;

	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt_timeout, sizeof(opt_timeout)) == SOCKET_ERROR) {
		CloseTCPSocket();
		return -1;
	}

	if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&opt_timeout, sizeof(opt_timeout)) == SOCKET_ERROR) {
		CloseTCPSocket();
		return -1;
	}
	
	return 0;
}

int TcpSocketWindows::CloseTCPSocket(void)
{
	if(hSocket != INVALID_SOCKET) {
		closesocket(hSocket);
		hSocket = INVALID_SOCKET;
	}

	return 0;
}

int TcpSocketWindows::WriteTCPSocket(unsigned char *data, int size)
{
	if(hSocket == INVALID_SOCKET) {
		return -1;
	}

	return send(hSocket, (char *)data, size, 0);
}

int TcpSocketWindows::ReadTCPSOcket(unsigned char *data, int size)
{
	if(hSocket == INVALID_SOCKET) {
		return -1;
	}

	return recv(hSocket, (char *)data, size, 0);
}

int TcpSocketWindows::SetParameter(string ip, short port, unsigned long timeOut)
{
	CloseTCPSocket();

	this->ip = ip;
	this->port = port;
	this->timeOut = timeOut;

	return OpenTCPSocket();
}

#endif
