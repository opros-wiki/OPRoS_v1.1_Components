#pragma once

#include "GyroSensor.h"
#include "ClientSocket.h"
#include "data.h"

class Simulator_GyroSensor: public GyroSensor
{
public:
	Simulator_GyroSensor(void);
	virtual ~Simulator_GyroSensor(void);
	ClientSocket	m_ClientSocket;
	struct data redata;

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);
	virtual int GetSensorValue(vector<ObjectRotation> &GyroSensorData);

private:
	Property parameter;

	int deviceID;
	std::string SimulatorIP;
	std::string GPSName;
	std::string GyroName;
	std::string deviceName;
	int PortNumber;
	string RobotName;

};