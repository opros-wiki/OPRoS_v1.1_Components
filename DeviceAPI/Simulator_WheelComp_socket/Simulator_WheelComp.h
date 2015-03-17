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
	*	���� �ð��� Mobility ���� ������ ���� �������� ��Ÿ���� �����̴�.
	*/
	float leftWheelAnglePrev;
	/**
	*	���� �ð��� Mobility ������ ������ ���� �������� ��Ÿ���� �����̴�.
	*/
	float rightWheelAnglePrev;
	/**
	*	Mobility�� ������� ���� �̵��� �Ÿ��� ��Ÿ���� �����̴�.
	*/
	double currentForward;

	double currentForwarderr;
	/**
	*	Mobility�� ������� ȸ���� ������ ��Ÿ���� �����̴�.
	*/
	double currentTurn;
	double currentTurnerr;
	/**
	*	Mobility�� ������ ��ǥ �Ÿ��� ��Ÿ���� �����̴�.
	*/
	double targetForward;

	
	/**
	*	Mobility�� ȸ���� ��ǥ ������ ��Ÿ���� �����̴�.
	*/
	double targetTurn;
	
	double Velocity;
	double preVelocity;
	double Accel;
	double preAccel;
	double jerk;


	/**
	*	Mobility�� ���� ��ġ x, y, theta�� ��Ÿ���� �����̴�.
	*/
	ObjectLocation position;
	/**
	*	Mobility�� ��ġ ���� x, y, theta�� ��Ÿ���� �����̴�.
	*/
	ObjectLocation variance;

	/**
	*	Mobility�� �����̰� �ִ����� ��Ÿ���� �����̴�.
	*/
	bool isRunning;
	
	/**
	*	Mobility�� �����ϱ� ���� ����� ��Ÿ���� �����̴�.
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