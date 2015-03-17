#pragma once

#if defined(WIN32)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "WheelController.h"
#include "Manipulator.h"



class KNU_HRC01_MC_V20 : public WheelController
{
public:
	KNU_HRC01_MC_V20 (void);
	virtual ~KNU_HRC01_MC_V20 (void);

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int Move(double distance);
	int Rotate(double degree);
	int Drive(double forwardVelocity, double angularVelocity);
	int Stop(void);
	int EmergencyStop(void);
	int SetPosition(ObjectLocation position);
	int GetPosition(ObjectLocation &position);

private:
	Property parameter;
	WheelControllerProfile _profile;

	/**
	*	���� �ð��� Mobility ���� ������ ���ڴ� ���� ��Ÿ���� �����̴�.
	*/
	double leftWheelAnglePrev;
	/**
	*	���� �ð��� Mobility ���� ������ ���ڴ� ���� ��Ÿ���� �����̴�.
	*/
	double rightWheelAnglePrev;
	/**
	*	Mobility�� ������� ���� �̵��� �Ÿ��� ��Ÿ���� �����̴�.
	*/
	double currentForward;
	/**
	*	Mobility�� ������� ȸ���� ������ ��Ÿ���� �����̴�.
	*/
	double currentTurn;
	/**
	*	Mobility�� ������ ��ǥ �Ÿ��� ��Ÿ���� �����̴�.
	*/
	double targetForward;
	/**
	*	Mobility�� ȸ���� ��ǥ ������ ��Ÿ���� �����̴�.
	*/
	double targetTurn;

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
	void EstimateAcceleration (double &forwardVelocity, double &angularVelocity, double deltaTime);


	boost::thread *pThread;
	bool threadOn;
	void ThreadFunc();

#if defined(WIN32)
	HMODULE hOprosAPI;
#else

#endif
	Manipulator *CNRobot;

	int InitializeManipulator(Property parameter);
	int FinalizeManipulator(void);
};
/* Kobot  Track Parameter
	<property name="APIName" type="string">KNU_HRC01_MC_V20_1.dll</property>
		<property name="WheelDiameter" type="double">0.21</property>
		1���� = 66cm ���� ���� =66/3.14 =21.019
		<property name="AxleDistance" type="double">0.46</property>
		<property name="VarianceDistance" type="double">0.05</property>
		<property name="VarianceDirection" type="double">1.0</property>
		<property name="SafeRadius" type="double">0.3</property>
		<property name="MaximumVelocity" type="double">0.29</property>
		<property name="Acceleration" type="double">0.5</property>

	<property name="ManipulatorAPIName" type="string">CNRobot_HRB-MTC-B10.dll</property>

		<property name="Size" type="int">2</property>
		<property name="MaximumPower0" type="double">1.0</property>
		<property name="LinePerRevolution0" type="double">600.0</property>
		<property name="ReductionRatio0" type="double">274.0</property>
		<property name="MaximumVelocity0" type="double">360.0</property>
		<property name="Acceleration0" type="double">360.0</property>
		<property name="MinimumPositionLimit0" type="double">-1000000.0</property>
		<property name="MaximumPositionLimit0" type="double">1000000.0</property>

		<property name="Kp0" type="unsigned short">100</property>
		<property name="Ki0" type="unsigned short">0</property>
		<property name="Kd0" type="unsigned short">400</property>
		<property name="ISum0" type="unsigned short">1000</property>

		<property name="MaximumPower1" type="double">1.0</property>
		<property name="LinePerRevolution1" type="double">600.0</property>
		<property name="ReductionRatio1" type="double">274.0</property>
		<property name="MaximumVelocity1" type="double">360.0</property>
		<property name="Acceleration1" type="double">360.0</property>
		<property name="MinimumPositionLimit1" type="double">-1000000.0</property>
		<property name="MaximumPositionLimit1" type="double">1000000.0</property>

		<property name="Kp1" type="unsigned short">100</property>
		<property name="Ki1" type="unsigned short">0</property>
		<property name="Kd1" type="unsigned short">400</property>
		<property name="ISum1" type="unsigned short">1000</property>

������ ������ ���� ������ ���� �ٸ��� ������ ��������
Drive()
	velocity[0] = RAD2DEG(f - a);	
	velocity[1] = -RAD2DEG(f + a);
UpdatePosition()
	leftWheelAngle=WheelAnglePrev[0];
	rightWheelAngle=-WheelAnglePrev[1];

���������� ȸ���ϱ� ���� �ٲ���
Rotate()
	targetTurn = currentTurn + DEG2RAD (-degree);

 */