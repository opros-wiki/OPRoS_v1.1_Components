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
		int		cnt;
		int		ID;
		unsigned char	value[230400];
	}	DeviceValue;

	bool enable(std::string SimulatorIP, int Port);
	int	findSystem(std::string robotName, std::string dName);
	bool getValue(int id, unsigned char *value);
	bool setValue(int id, float *value, int cnt);

	int	client_socket;
	int StepSize;
};
