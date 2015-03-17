#pragma once

#include "Manipulator.h"

class DongbuRobotTetraPanTilt : public Manipulator
{
public:
	DongbuRobotTetraPanTilt(void);
	virtual ~DongbuRobotTetraPanTilt(void);

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int RunHoming(void);
	int SetPosition(vector<double> position, vector<unsigned long> time);
	int GetPosition(vector<double> &position);


private:
	std::string robotIP;
	int robotPort;

	bool PowerDown (int servoID);
	bool SetServoPosition (int servoID, int position);
	bool GetServoPosition (int servoID, int *position);
	bool SetServoSpeed (int servoID, int speed);
};

