#pragma once

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "Encoder.h"
#include "ClientSocket.h"
#define	TACHOMETER_MAX_SIZE 8


class Simulator_Tachometer_socket : public Encoder
{
public:
	Simulator_Tachometer_socket(void);
	virtual ~Simulator_Tachometer_socket(void);

	ClientSocket	m_ClientSocket;
	struct data redata;

public:

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);
	int getValue(int index, std::valarray<double> *value);
	virtual int GetSensorValue(vector<double> &TachometerData);

private:
	
	Property parameter;

	int sensorCount;
	string SimulatorIP;
	string DeviceName[TACHOMETER_MAX_SIZE];
	string deviceName;
	string RobotName;
	int PortNumber;

	int ReadEncoderArray (float sensorValue [], int &sensorSize);
};
