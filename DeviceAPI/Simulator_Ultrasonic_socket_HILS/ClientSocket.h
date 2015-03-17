#pragma once

class ClientSocket
{
public:
	ClientSocket(void);
	~ClientSocket(void);

	typedef	struct{
		char	command;
		int		ID;
	}	Reqmsg;

	typedef struct	{
		char	command;
		char	RobotName[30];
		char	deviceCnt;
		char	DeviceName[16][30];
	}	initDevice;

	typedef struct	{
		char command;
		char cnt;
		unsigned int deviceID[16];
	}	findDeviceID;

	typedef	struct{
		char	command;
		short	cnt;
		int		ID;
		float	value[128];
	}	DeviceValue;

	bool enable(std::string SimulatorIP,int Port);
	struct data	findSystem(std::string robotName, std::string dName[7],int dcount);
	bool setValue(int id, float value);
	int	client_socket;
};
