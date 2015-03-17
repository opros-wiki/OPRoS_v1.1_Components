#pragma once


#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "InfraredSensor.h"
#include "ClientSocket.h"
#define PSD_MAX_SIZE			7

class Simulator_IR : public InfraredSensor
{
public:
	Simulator_IR(void);
	virtual ~Simulator_IR(void);

	ClientSocket	m_ClientSocket;
	struct data redata;

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	virtual int SetSensorValue(vector<float> &PsdSensorData);

private:

	Property parameter;
	
	int sensorCount;

	string SimulatorIP;
	string DeviceName[PSD_MAX_SIZE];
	string deviceName;
	string RobotName;
	int	PortNumber;
};
