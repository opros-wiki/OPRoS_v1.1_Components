#pragma once

//#include "SimLaserScanner.h"
#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include "device/LaserScanner.h"
#include "ClientSocket.h"


class Simulator_LaserScanner : public LaserScanner
{
public:
	Simulator_LaserScanner(void);
	virtual ~Simulator_LaserScanner(void);
	ClientSocket	m_ClientSocket;

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	//int GetLaserScannerData(vector<double> &laserScannerData);
	int GetSensorValue(vector<double> &sensorValue);
	int SetLaserScannerData(vector<float>  &laserScannerData);
private:

	Property parameter;


	int maximumStep;
	int startStep;
	int endStep;
	int deltaStep;
	int scanStepSize;
	vector<int> scannedData;
	int deviceID;

	// OPRoS Simulator Code
	//SimLaserScanner simulator;
	std::string SimulatorIP;
	std::string DeviceName;
	string RobotName;
	int PortNumber;

	bool ReadLaserSensorArray (float sensorValue [], int &sensorSize);
};
