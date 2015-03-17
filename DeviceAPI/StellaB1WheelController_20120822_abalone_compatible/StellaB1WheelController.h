#ifndef __STELLA_B1_WHEEL_CONTROLLER_H__
#define __STELLA_B1_WHEEL_CONTROLLER_H__

#include <device/WheelController.h>
#include <device/Uart.h>
#include <device/OprosTimer.h>

#include "SimpleLock.h"
#include "StellaB1.h"

class StellaB1WheelController : public WheelController
{
public:
	class StellaB1Profile
	{
	public:
		StellaB1Profile()
		{
			//Stella B1 Factory Defaults
			//Reference STELLA B1 MOBILE ROBOT / PLATFORM USER MANUAL, P.35

			wheelDiameter = 0.0752f * 2.0f;
			axleDistance = 0.289f;
			reductionRatio = 1.0f / 14.0f;
			encoderCPR = 256.0f;
			maximumVelocity = 5.0f;
			acceleration = (float)RAD2DEG(0.4);
			deacceleration = (float)RAD2DEG(0.1);
			
			rightWheelAngleScaleFactor = 0.0f;
			leftWheelAngleScaleFactor = 0.0f;

			rightWheelDistanceScaleFactor = 0.0f;
			leftWheelDistanceSacleFactor = 0.0f;

			velocityControlParameter.gainP = 30.0f;
			velocityControlParameter.gainI = 200.0f;
			velocityControlParameter.gainD = 0.005f;	
			velocityControlParameter.gainIL = 20.0f;

			positionControlParameter.gainP = 4.4726f;
			positionControlParameter.gainI = 0.015f;
			positionControlParameter.gainD = 7.0398f;
			positionControlParameter.gainIL = 100.0f;
		}

		float wheelDiameter;
		float axleDistance;
		float reductionRatio;
		float encoderCPR;
		float maximumVelocity;
		float acceleration;
		float deacceleration;
		
		float rightWheelAngleScaleFactor;
		float leftWheelAngleScaleFactor;

		float rightWheelDistanceScaleFactor;
		float leftWheelDistanceSacleFactor;

		StellaB1::ControlParameter velocityControlParameter;
		StellaB1::ControlParameter positionControlParameter;
	};

	class WheelSpeed 
	{
	public:
		WheelSpeed(unsigned time_ = 0, float rightSpeed_ = 0, float leftSpeed_ = 0)
		{
			time = time_;
			rightSpeed = rightSpeed_;
			leftSpeed = leftSpeed_;
		}

		unsigned long time;
		float rightSpeed;
		float leftSpeed;
	};

public:
	StellaB1WheelController();
	~StellaB1WheelController();

public:
	int Initialize(Property parameter);
	int Finalize();
	int Enable(); 
	int Disable();
	int SetParameter(Property parameter); 
	int GetParameter(Property &parameter);
	int OnExecute(); 

public:
	int SetPosition(ObjectLocation position);
	int GetPosition(ObjectLocation& position);						
	int GetOdometery(vector<long>& odometery);						
	int DriveWheel(double linearVelocity, double angularVelocity);
	int MoveWheel(double distance, double linearVelocity);			
	int RotateWheel(double angle, double angularVelocity);			
	int StopWheel();												
	int IsWheelRunning(bool& isWheelRunning);		

private:
	bool SetStellaB1Profile(Property& parameter);
	bool ProcessOdometricLocalization();

private:
	Uart* m_uart;
	StellaB1 m_stellaB1;

	OprosTimer m_timer;

	StellaB1Profile m_profile;
	ObjectLocation m_wheelPosition;
	SimpleMutex m_mutexWheelPosition;

	SimpleReaderWriterLock m_rwLockStatus;

	WheelSpeed m_previousWheelSpeed;
};

#endif
