#pragma once

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "PositionSensor.h"
#include "ClientSocket.h"
#include "data.h"

class Simulator_PositionSensor : public PositionSensor
{
public:
	Simulator_PositionSensor(void);
	virtual ~Simulator_PositionSensor(void);
	ClientSocket	m_ClientSocket;
	struct data redata;

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);
	virtual int GetSensorValue(vector<ObjectPosition> &positionSensorData);

private:
	Property parameter;

	int deviceID;
	std::string SimulatorIP;
	std::string GPSName;
	std::string GyroName;
	std::string deviceName[2];
	int PortNumber;
	string RobotName;

};