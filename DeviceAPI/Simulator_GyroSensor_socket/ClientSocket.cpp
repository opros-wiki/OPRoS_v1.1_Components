#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "data.h"

#pragma comment(lib, "ws2_32.lib")

#include "ClientSocket.h"
#define  BUFF_SIZE   1024

ClientSocket::ClientSocket(void)
{
}

ClientSocket::~ClientSocket(void)
{
}

bool ClientSocket::enable(std::string SimulatorIP,int Port)
{
	struct sockaddr_in   server_addr;
	char	ip_addr[17];

	client_socket  = socket( PF_INET, SOCK_STREAM, 0);
	if( -1 == client_socket)
	{
		printf( "socket 생성 실패\n");
		return false;
	}

	memset( &server_addr, 0, sizeof( server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons(Port);
	strcpy_s(ip_addr,SimulatorIP.c_str());
	
	server_addr.sin_addr.s_addr= inet_addr( ip_addr);

	if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
	{
		printf( "접속 실패\n");
		return false;
	}


	return true;
}


struct data ClientSocket::findSystem(std::string robotName,
							  std::string deviceName)
{
	struct data redata;
	char message[10000];
	initDevice SendMsg;
	int	recv_cnt;
	SendMsg.command = 0x01;
	SendMsg.deviceCnt = 1;
	strcpy_s(SendMsg.RobotName,robotName.c_str());

	strcpy_s(SendMsg.DeviceName[0],deviceName.c_str());	
	char *cmsg = new char [sizeof(SendMsg)];
	memcpy(cmsg,&SendMsg,sizeof(SendMsg));
	send(client_socket,cmsg,sizeof(SendMsg),0);
	delete [] cmsg;

	recv_cnt = 0;
	memset(&message,0,10000);

	while( recv_cnt == 0)
	{
		recv_cnt = recv(client_socket,message,10000,0);
	}

	if (message[0] == 0x01)
	{
		findDeviceID	lid;
		memcpy(&lid,message,10000);
		redata.re_data = true;
		for (int i = 0 ; i < 1 ; i++)
		{
			redata.deviceID[i] = lid.deviceID[i];
		}
		return redata;
	}
	redata.re_data = false;
	return redata;
}

bool ClientSocket::getPosition(int id, float *value)
{
	Reqmsg msg;
	int	recv_cnt;
	char message[10000] ={0,};
	msg.command = 0x02;
	msg.ID = id;
	char *rmsg = new char [sizeof(msg)];
	memcpy(rmsg,&msg,sizeof(msg));
	send(client_socket,rmsg,sizeof(msg),0);
	delete [] rmsg;
	recv_cnt = 0;
	while( recv_cnt == 0 )
	{
		recv_cnt = recv(client_socket,message,10000,0);
	}
	if (message[0] == 0x02)
	{
		DeviceValue	Re_data;
		memcpy(&Re_data,message,10000);
		for (int i = 0 ; i < Re_data.cnt ; i++)
		{
			*value++ = Re_data.value[i];
		}
		return true;
	}
	return false;
}

bool ClientSocket::setVelocity(int id, float value)
{
	DeviceValue device;
	memset(&device,0,sizeof(device));
	device.command = 0x03;
	device.cnt = 2;
	device.ID = id;
	device.value[0] = value;
	device.value[1] = value;
	char *wmsg = new char [sizeof(device)];
	memcpy(wmsg,&device,sizeof(device));
	send(client_socket,wmsg,sizeof(device),0);
	Sleep(20);
	return true;
}
	




