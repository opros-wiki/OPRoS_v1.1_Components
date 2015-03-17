#pragma once

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "UltrasonicSensor.h"
#include "ClientSocket.h"
#define	ULTRA_SONIC_MAX_SIZE 7


class Simulator_Ultrasonic : public UltrasonicSensor
{
public:
	Simulator_Ultrasonic(void);
	virtual ~Simulator_Ultrasonic(void);

	ClientSocket	m_ClientSocket;
	struct data redata;

public:

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);
		
	virtual int SetSensorValue(vector<float>  &ultrasonicSensorData);
private:
	
	Property parameter;

	int sensorCount;

	string SimulatorIP;
	string DeviceName[ULTRA_SONIC_MAX_SIZE];
	string deviceName;
	string RobotName;
	int PortNumber;
};
