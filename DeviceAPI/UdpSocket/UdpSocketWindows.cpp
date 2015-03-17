#if defined(WIN32)

#pragma comment (lib,"ws2_32.lib")

#include "UdpSocketWindows.h"

UdpSocketWindows::UdpSocketWindows(string ip, short port, unsigned long timeOut)
:UdpSocketImpl(ip, port, timeOut)
{
	hSocket = INVALID_SOCKET;
}

UdpSocketWindows::~UdpSocketWindows(void)
{
	CloseUDPSocket();
}

int UdpSocketWindows::OpenUDPSocket(void)
{
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1;
	}

	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(hSocket == INVALID_SOCKET) {
		return -1;
	}

	SOCKADDR_IN tcp_sock_addr;

	memset(&tcp_sock_addr, 0, sizeof(tcp_sock_addr));
	tcp_sock_addr.sin_family = AF_INET;
	tcp_sock_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	tcp_sock_addr.sin_port = htons((short)port);

	if(connect(hSocket, (SOCKADDR *)&tcp_sock_addr, sizeof(tcp_sock_addr)) == SOCKET_ERROR) {
		CloseUDPSocket();
		return -1;
	}

	int opt_timeout = timeOut;

	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt_timeout, sizeof(opt_timeout)) == SOCKET_ERROR) {
		CloseUDPSocket();
		return -1;
	}

	if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&opt_timeout, sizeof(opt_timeout)) == SOCKET_ERROR) {
		CloseUDPSocket();
		return -1;
	}
	
	return 0;
}

int UdpSocketWindows::CloseUDPSocket(void)
{
	if(hSocket != INVALID_SOCKET) {
		closesocket(hSocket);
		hSocket = INVALID_SOCKET;
	}

	WSACleanup();

	return 0;
}

int UdpSocketWindows::WriteUDPSocket(unsigned char *data, int size)
{
	if(hSocket == INVALID_SOCKET) {
		return -1;
	}

	return send(hSocket, (char *)data, size, 0);
}

int UdpSocketWindows::ReadUDPSOcket(unsigned char *data, int size)
{
	if(hSocket == INVALID_SOCKET) {
		return -1;
	}

	return recv(hSocket, (char *)data, size, 0);
}

int UdpSocketWindows::SetParameter(string ip, short port, unsigned long timeOut)
{
	CloseUDPSocket();

	this->ip = ip;
	this->port = port;
	this->timeOut = timeOut;

	return OpenUDPSocket();
}

#endif