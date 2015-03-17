#pragma once

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
//#include <boost/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include "device/WheelController.h"
#include "ClientSocket.h"
#include "device/CAN.h"
//#include "ServoActuator.h"
//#include "SimActuator.h"

class Simulator_WheelComp : public WheelController
{
public:
	Simulator_WheelComp (void);
	virtual ~Simulator_WheelComp (void);
	ClientSocket	m_ClientSocket;
	struct data redata;

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	/*int Move(double distance);
	int Rotate(double degree);
	int Drive(double forwardVelocity, double angularVelocity);
	int Stop(void);
	int EmergencyStop(void);
	int SetPosition(ObjectLocation position);
	int GetPosition(ObjectLocation &position);*/
	
	int SetPosition(ObjectLocation position);
	int GetPosition(ObjectLocation &position);
	//int GetOdometery(vector<long> &odometery);
	int DriveWheel(double linearVelocity, double angularVelocity);
	int MoveWheel(double distance, double linearVelocity);
	int RotateWheel(double angle, double angularVelocity);
	int StopWheel(void);
	//int IsWheelRunning(bool &isWheelRunning);

	void EstimateAcceleration(double &forwardVelocity, double &angularVelocity, double deltaTime);

private:
	Property parameter;
	//WheelControllerProfile _profile;

	/**
	*	이전 시간의 Mobility 왼쪽 바퀴의 누적 각도값을 나타내는 변수이다.
	*/
	float leftWheelAnglePrev;
	/**
	*	이전 시간의 Mobility 오른쪽 바퀴의 누적 각도값을 나타내는 변수이다.
	*/
	float rightWheelAnglePrev;
	/**
	*	Mobility가 현재까지 직진 이동한 거리를 나타내는 변수이다.
	*/
	double currentForward;

	double currentForwarderr;
	/**
	*	Mobility가 현재까지 회전한 각도를 나타내는 변수이다.
	*/
	double currentTurn;
	double currentTurnerr;
	/**
	*	Mobility가 직진할 목표 거리를 나타내는 변수이다.
	*/
	double targetForward;

	
	/**
	*	Mobility가 회전할 목표 각도를 나타내는 변수이다.
	*/
	double targetTurn;
	
	double Velocity;
	double preVelocity;
	double Accel;
	double preAccel;
	double jerk;


	/**
	*	Mobility의 현재 위치 x, y, theta를 나타내는 변수이다.
	*/
	ObjectLocation position;
	/**
	*	Mobility의 위치 에러 x, y, theta를 나타내는 변수이다.
	*/
	ObjectLocation variance;

	/**
	*	Mobility가 움직이고 있는지를 나타내는 변수이다.
	*/
	bool isRunning;
	
	/**
	*	Mobility를 제어하기 위한 명령을 나타내는 변수이다.
	*/
	int mobileCommand;

	bool UpdatePosition();

	long taskPeriod;
	void StartTask(long period);
	void StopTask();
	void DoTask();

	double _forwardVelocity;
	double _forwardAccel;
	double _centripetalAccel;

	//boost::thread *pThread;
	bool threadOn;
	void ThreadFunc();

#if defined(WIN32)
	HMODULE hOprosAPI;
#else
	void *hOprosAPI;
#endif
	//ServoActuator *leftWheel, *rightWheel;
	//SimActuator simulator;
	std::string SimulatorIP;
	std::string	robotName;
	std::string rMotorName;
	std::string lMotorName;
	std::string lEncName;
	std::string rEncName;
	std::string gps;
	std::string gyro;

	int PortNumber;

	int InitializeServoActuator(Property parameter);
	int FinalizeServoActuator(void);
};

/* Robin Parameter
WheelDiameter     : 0.255
AxleDistance      :  0.395
VarianceDistance  : 0.02
VarianceDirection : 1.0
SafeRadius        : 0.4
MaximumVelocity   : 0.5
Acceleration      : 0.5
 */