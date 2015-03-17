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

bool ClientSocket::enable(std::string SimulatorIP, int Port)
{
	struct sockaddr_in   server_addr;

	char	ip_addr[17];

	client_socket  = socket( PF_INET, SOCK_STREAM, 0);
	if( -1 == client_socket)
	{
		printf( "socket 积己 角菩\n");
		return false;
	}

	memset( &server_addr, 0, sizeof( server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons(Port);
	strcpy_s(ip_addr,SimulatorIP.c_str());
	
	server_addr.sin_addr.s_addr= inet_addr( ip_addr);

	if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
	{
		printf( "立加 角菩\n");
		return false;
	}


	return true;
}

struct data ClientSocket::findSystem(std::string robotName,
							  std::string deviceName[7],int dcount)
{
	struct data redata;
	char message[10000];
	initDevice SendMsg;
	int	recv_cnt;
	SendMsg.command = 0x01;
	SendMsg.deviceCnt = dcount;
	strcpy_s(SendMsg.RobotName,robotName.c_str());
	for (int i = 0 ; i < dcount ; i++)
	{
		strcpy_s(SendMsg.DeviceName[i],deviceName[i].c_str());
	}
	
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
		redata.re_data =true;
		for (int i = 0 ; i < dcount ; i++)
		{
			redata.deviceID[i] = lid.deviceID[i];
		}
		return redata;
	}
	redata.re_data = false;
	return redata;
}

bool ClientSocket::setValue(int id, float value)
{
	DeviceValue	device;
	memset(&device,0,sizeof(device));
	device.command = 0x03;
	device.cnt = 1;
	device.ID = id;
	device.value[0] = value;

	char *wmsg = new char [sizeof(device)];
	memcpy(wmsg,&device,sizeof(device));
	send(client_socket,wmsg,sizeof(device),0);
	Sleep(20);
	return true;
}