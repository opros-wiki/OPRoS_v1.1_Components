//#include "stdafx.h"


#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#include "ClientSocket.h"
#define  BUFF_SIZE   1024

//typedef	struct{
//	char			command;
//	short			cnt;
//	unsigned int	DeviceID[128];
//	char			DeviceName[128][30];
//}	DeviceInfo;




ClientSocket::ClientSocket(void)
{
}

ClientSocket::~ClientSocket(void)
{
}

bool ClientSocket::enable(std::string SimulatorIP, int Port)
{
	struct sockaddr_in   server_addr;

//	char	buff[BUFF_SIZE+5];
	char	ip_addr[17];

	client_socket  = socket( PF_INET, SOCK_STREAM, 0);
	if( -1 == client_socket)
	{
		printf( "socket 积己 角菩\n");
		return false;
		//exit( 1);
	}

	memset( &server_addr, 0, sizeof( server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons( Port);
	strcpy_s(ip_addr,SimulatorIP.c_str());
	
	server_addr.sin_addr.s_addr= inet_addr( ip_addr);

	if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
	{
		printf( "立加 角菩\n");
		return false;
		//exit( 1);
	}


	return true;
}

//unsigned int ClientSocket::findSystem(std::string robotName)
//{
//	Reqmsg msg;
//	char *msgchar = new char[sizeof(msg)];
//	char message[10000];
//	int recv_cnt = 0;
//	RobotInfo info;
//	DeviceInfo	dInfo;
//	char name[30];
//	unsigned int RobotID;
//
//	strcpy_s(name ,robotName.c_str());
//	msg.command = 0x01;
//	msg.ID = 0x00;
//	memcpy(msgchar,&msg,sizeof(msg));
//	send(client_socket,msgchar,sizeof(msg),0);
//	while(recv_cnt == 0)
//	{
//		recv_cnt = recv(client_socket,message,10000,0);
//	}
//	memcpy(&info,message,sizeof(info));
//
//	for (int i = 0 ; i < info.cnt ; i++)
//	{
//		if (strcmp(name,info.RobotName[i]) == 0 )
//		{
//			RobotID = info.RobotID[i];
//			break;
//			//return info.RobotID[i];
//		}
//	}
//	memset(&msg,0,sizeof(msg));
//	memset(msgchar,0,sizeof*(msgchar));
//	msg.command = 0x02;
//	msg.ID = RobotID;
//	memcpy(msgchar,&msg,sizeof(msg));
//	send(client_socket,msgchar,sizeof(msg),0);
//
//	recv_cnt = 0;
//
//	while(recv_cnt == 0)
//	{
//		recv_cnt = recv(client_socket,message,10000,0);
//	}
//	memcpy(&dInfo,message,sizeof(message));
//
//
//
//
//	/*for (int i = 0 ; i < dInfo.cnt ; i++)
//	{
//		printf("Name[%d] : %s \n",i,dInfo.DeviceName[i]);
//	}*/
//
//	return 0;
//}

int ClientSocket::findSystem(std::string robotName,
							  std::string deviceName)
{
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
		return lid.deviceID[0];
	}
	return false;
}

bool ClientSocket::getValue(int id, float *value)
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
	//Sleep(20);
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

bool ClientSocket::setValue(int id, float *value, int cnt)
{
	DeviceValue	device;
	memset(&device,0,sizeof(device));
	device.command = 0x03;
	device.cnt = cnt;
	device.ID = id;

	printf("socket\n");

	for (int i = 0 ; i < device.cnt ; i++)
	{
		device.value[i] = *value++;
		printf("[%d] : %f\t",i,device.value[i]);
	}
	printf("\n");

	char *wmsg = new char [sizeof(device)];
	memcpy(wmsg,&device,sizeof(device));
	send(client_socket,wmsg,sizeof(device),0);
	Sleep(20);
	return true;

	/*DeviceValue device;
	memset(&device,0,sizeof(device));
	device.command = 0x03;
	device.cnt = 1;
	device.ID = id;
	device.value[0] = value;
	device.value[1] = value;
	char *wmsg = new char [sizeof(device)];
	memcpy(wmsg,&device,sizeof(device));
	send(client_socket,wmsg,sizeof(device),0);
	Sleep(20);
	return true;*/
}