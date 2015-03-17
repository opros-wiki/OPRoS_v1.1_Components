#pragma once

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "device/Camera.h"
#include "ClientSocket.h"

class Simulator_Vision : public Camera
{
public:
	Simulator_Vision(void);
	virtual ~Simulator_Vision(void);
	ClientSocket	m_ClientSocket;

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);
	virtual int GetImage(unsigned char *image);

private:

	Property parameter;
	std::string SimulatorIP;
	std::string DeviceName;
	string RobotName;
	int PortNumber;
	int deviceID;

	unsigned char buffer[230400];
};