#include "StellaB1Tester.h"

#include <device/OprosTimer.h>
#include <device/OprosMath.h>
#include <device/OprosPrintMessage.h>

#include "debug_macro.h"

bool StellaB1Tester::TestAll()
{
	OprosSleep(100);
	if (TestInit() == false)
		return false;

	OprosSleep(100);	
	if (TestReset() == false)
		return false;

	OprosSleep(100);
	if (TestControlVelocity() == false)
		return false;

	OprosSleep(100);
	if (TestControlPosition() == false)
		return false;

	OprosSleep(100);
	if (TestControlStop() == false)
		return false;

	OprosSleep(100);
	if (TestSetGetScale() == false)
		return false;

	OprosSleep(100);
	if (TestSetGetAcceleration() == false)
		return false;

	OprosSleep(100);
	if (TestSetGetControlParameter() == false)
		return false;

	OprosSleep(100);
	if (TestSetGetWheelRadius() == false)
		return false;

	OprosSleep(100);
	if (TestSetGetAxleWidth() == false)
		return false;

	return true;
}

bool StellaB1Tester::TestInit()
{
	const StellaB1WheelController::StellaB1Profile defaultProfile;
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");
	PrintMessage("Initializing Stella B1 Test\r\n");

	PrintMessage("Start initializing Stella B1 : ");
	if ((errorCode = m_stellaB1->ControlInit()) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfWheelRadius(defaultProfile.wheelDiameter / 2.0f) == false)
		return false;

	if (CompareOfAxleWidth(defaultProfile.axleDistance) == false)
		return false;

	if (CompareOfAcceleration((float)DEG2RAD(defaultProfile.acceleration), (float)DEG2RAD(defaultProfile.acceleration)) == false)
		return false;

	if (CompareOfDeacceleration((float)DEG2RAD(defaultProfile.deacceleration), (float)DEG2RAD(defaultProfile.deacceleration)) == false)
		return false;

	if (CompareOfAngleScale(defaultProfile.rightWheelAngleScaleFactor, defaultProfile.leftWheelAngleScaleFactor) == false)
		return false;

	if (CompareOfDistacneScale(defaultProfile.rightWheelDistanceScaleFactor, defaultProfile.leftWheelDistanceSacleFactor) == false)
		return false;

	if (CompareOfControlVelocity(defaultProfile.velocityControlParameter, defaultProfile.velocityControlParameter) == false)
		return false;

	if (CompareOfControlPosition(defaultProfile.positionControlParameter, defaultProfile.positionControlParameter) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");
	return true;
}

bool StellaB1Tester::TestReset()
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Reseting Stella B1 Test\r\n");

	PrintMessage("Start reseting Stella B1 right wheel : ");
	if ((errorCode = m_stellaB1->ControlReset(StellaB1::CRESETR)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("Start reseting Stella B1 left wheel : ");
	if ((errorCode = m_stellaB1->ControlReset(StellaB1::CRESETL)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("Start reseting Stella B1 all wheel : ");
	if ((errorCode = m_stellaB1->ControlReset(StellaB1::CRESETA)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestControlVelocity( const float velocity /*= DEG2RAD(180.0) * 14.0*/ )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int delayTimeSec = 5;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Control Stella B1 Velocity Test\r\n");

	PrintMessage("Start constrol Stella B1 velocity : ");
	if ((errorCode = m_stellaB1->ControlVelocity(velocity, velocity)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("%dsec delay\r\n", delayTimeSec);
	OprosSleep(delayTimeSec * 1000);

	if (CompareOfVelocity(velocity, velocity) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestControlPosition( const float distance /*= 1.0f*/, const float angle /*= 45.0f*/)
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int delayTimeSec = 5;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Control Stella B1 Position Test\r\n");

	PrintMessage("Start initializing Stella B1 : ");
	if ((errorCode = m_stellaB1->ControlInit()) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("Start constrol Stella B1 position distance : ");
	if ((errorCode = m_stellaB1->ControlPositionDistance(distance, delayTimeSec)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("%dsec delay\r\n", delayTimeSec);
	OprosSleep(delayTimeSec * 1000);

	if(CompareOfPositionDistacne(distance, distance) == false)
		return false;

	PrintMessage("Start constrol Stella B1 position angle : ");
	if ((errorCode = m_stellaB1->ControlPositionAngle(angle, delayTimeSec)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("%dsec delay\r\n", delayTimeSec);
	OprosSleep(delayTimeSec * 1000);

	if (CompareOfPositionAngle(angle, distance, distance) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestControlStop()
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int delayTimeSec = 3;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Stopping Stella B1 Test\r\n");

	PrintMessage("Start stopping Stella B1 on mode 1 : ");
	if ((errorCode = m_stellaB1->ControlStop(StellaB1::CSTOP1)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("%dsec delay\r\n", delayTimeSec);
	OprosSleep(delayTimeSec * 1000);

	if (CompareOfVelocity(0, 0) == false)
		return false;

	PrintMessage("Start stopping Stella B1 on mode 2 : ");
	if ((errorCode = m_stellaB1->ControlStop(StellaB1::CSTOP1)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("%dsec delay\r\n", delayTimeSec);
	OprosSleep(delayTimeSec * 1000);

	if (CompareOfVelocity(0, 0) == false)
		return false;

	PrintMessage("Start stopping Stella B1 on mode 3 : ");
	if ((errorCode = m_stellaB1->ControlStop(StellaB1::CSTOP1)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("%dsec delay\r\n", delayTimeSec);
	OprosSleep(delayTimeSec * 1000);

	if (CompareOfVelocity(0, 0) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestSetGetScale( const float scale /*= 0.0f*/ )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Setting & Getting Stella B1 control scale Test\r\n");

	PrintMessage("Start setting Stella B1 control angle scale : ");
	if ((errorCode = m_stellaB1->SetScaleFactorAngle(scale, scale)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfAngleScale(scale, scale) == false)
		return false;	

	PrintMessage("Start setting Stella B1 control distance scale : ");
	if ((errorCode = m_stellaB1->SetScaleFactorDistance(scale, scale)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfDistacneScale(scale, scale) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestSetGetAcceleration( const float accel /*= 0.0f*/ )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Setting & Getting Stella B1 acceleration Test\r\n");

	PrintMessage("Start setting Stella B1 acceleration : ");
	if ((errorCode = m_stellaB1->SetVelocityAcceleration(accel, accel)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfAcceleration(accel, accel) == false)
		return false;

	PrintMessage("Start setting Stella B1 deacceleration : ");
	if ((errorCode = m_stellaB1->SetVelocityDeacceleration(accel, accel)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfDeacceleration(accel, accel) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestSetGetControlParameter( const float gainP /*= 30.0f*/, const float gainI /*= 200.0f*/, const float gainD /*= 0.005f*/, const float gainIl /*= 20.0f*/ )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	const StellaB1::ControlParameter parameter(gainP, gainI, gainD, gainIl);

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Setting & Getting Stella B1 control parameter Test\r\n");

	PrintMessage("Start setting Stella B1 control position left wheel : ");	
	if ((errorCode = m_stellaB1->SetControlPositionParameterLeftWheel(parameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");	

	PrintMessage("Start setting Stella B1 control position right wheel : ");	
	if ((errorCode = m_stellaB1->SetControlPositionParameterRightWheel(parameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfControlPosition(parameter, parameter) == false)
		return false;

	PrintMessage("Start setting Stella B1 control velocity left wheel : ");	
	if ((errorCode = m_stellaB1->SetControlVelocityParameterLeftWheel(parameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("Start setting Stella B1 control velocity right wheel : ");	
	if ((errorCode = m_stellaB1->SetControlVelocityParameterRightWheel(parameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfControlVelocity(parameter, parameter) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestSetGetWheelRadius( const float radius /*= 0.0752f*/ )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Setting & Getting Stella B1 wheel radius Test\r\n");

	PrintMessage("Start setting Stella B1 wheel radius : ");	
	if ((errorCode = m_stellaB1->SetWheelRadius(radius)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfWheelRadius(radius) == false)
		return false;	

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::TestSetGetAxleWidth( const float width /*= 0.289f*/ )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	PrintMessage("Setting & Getting Stella B1 axle width Test\r\n");

	PrintMessage("Start setting Stella B1 axle width : ");	
	if ((errorCode = m_stellaB1->SetAxleWidth(width)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	if(CompareOfAxleWidth(width) == false)
		return false;

	PrintMessage("//////////////////////////////////////////////////////////////////////////\r\n");

	return true;
}

bool StellaB1Tester::CompareOfVelocity( const float rightVelocity, const float leftVelocity )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("Compare of velocity : ");
	float leftWheelVelocity, rightWheelVelocity;
	if ((errorCode = m_stellaB1->GetVelocity(rightWheelVelocity, leftWheelVelocity)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(rightWheelVelocity, rightVelocity * (1.0 - m_errorTolerance), rightVelocity * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel velocity is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftWheelVelocity, leftWheelVelocity * (1.0 - m_errorTolerance), leftWheelVelocity * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel velocity is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfPositionDistacne( const float rightDistance, const float leftDistance )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("Compare of position distance : ");
	float leftPositionDistance, rightPositionDistance;
	if ((errorCode = m_stellaB1->GetPosition(rightPositionDistance, leftPositionDistance)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(rightPositionDistance, rightDistance * (1.0 - m_errorTolerance), rightDistance * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel position distance is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftPositionDistance, leftDistance * (1.0 - m_errorTolerance), leftDistance * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel position distance is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfPositionAngle( const float angle, const float previusRightWheelPosition, const float previusleftWheelPosition)
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;

	PrintMessage("Compare of position angle : ");
	float leftWheelPosition, rightWheelPosition;
	if ((errorCode = m_stellaB1->GetPosition(rightWheelPosition, leftWheelPosition)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	/*
	StellaB1WheelController::StellaB1Profile defaultProfile;
	float leftPositionAngle = (float)RAD2DEG(abs(atan2(leftWheelPosition - previusleftWheelPosition, defaultProfile.axleDistance / 2.0f)));
	float rightPositonAngle = (float)RAD2DEG(abs(atan2(rightWheelPosition - previusRightWheelPosition, defaultProfile.axleDistance / 2.0f)));

	if (IsBeValidRange(rightPositonAngle, angle * (1.0 - m_errorTolerance), angle * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel position angle is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftPositionAngle, angle * (1.0 - m_errorTolerance), angle * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel position angle is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	*/
	return true;
}

bool StellaB1Tester::CompareOfAngleScale( const float rightScale, const float leftScale )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	float leftWheelScale, rightWheelScale;

	PrintMessage("Compare of angle scale : ");	
	if ((errorCode = m_stellaB1->GetScaleFactorAnlge(rightWheelScale, leftWheelScale)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(rightWheelScale, rightScale * (1.0 - m_errorTolerance), rightScale * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel angle scale is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftWheelScale, leftScale * (1.0 - m_errorTolerance), leftScale * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel angle scale is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfDistacneScale( const float rightScale, const float leftScale  )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	float leftWheelScale, rightWheelScale;

	PrintMessage("Compare of distance scale : ");
	if ((errorCode = m_stellaB1->GetScaleFactorDistance(rightWheelScale, leftWheelScale)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(rightWheelScale, rightScale * (1.0 - m_errorTolerance), rightScale * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel distance scale is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftWheelScale, leftScale * (1.0 - m_errorTolerance), leftScale * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel distance scale is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfAcceleration( const float rightAccel, const float leftAccel )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	float leftWheelAccel, rightWheelAccel;

	PrintMessage("Compare of acceleration : ");	
	if ((errorCode = m_stellaB1->GetVelocityAcceleration(rightWheelAccel, leftWheelAccel)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(rightWheelAccel, rightAccel * (1.0 - m_errorTolerance), rightAccel * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel accleration is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftWheelAccel, leftAccel * (1.0 - m_errorTolerance), leftAccel * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel accleration is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfDeacceleration( const float rightDeaccel, const float leftDeaccel )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	float leftAccel, rightAccel;

	PrintMessage("Compare of deacceleration : ");	
	if ((errorCode = m_stellaB1->GetVelocityDeacceleration(rightAccel, leftAccel)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(rightAccel, rightDeaccel * (1.0 - m_errorTolerance), rightDeaccel * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Right wheel deacceleration is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(leftAccel, leftDeaccel * (1.0 - m_errorTolerance), leftDeaccel * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Left wheel deacceleration is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfControlPosition( const StellaB1::ControlParameter& rightWheel, const StellaB1::ControlParameter& leftWheel )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	StellaB1::ControlParameter gottenParameter;

	PrintMessage("Compare of control position left wheel : ");	
	if ((errorCode = m_stellaB1->GetControlPositionParameterLeftWheel(gottenParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(gottenParameter.gainP, leftWheel.gainP * (1.0 - m_errorTolerance), leftWheel.gainP * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain P is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainI, leftWheel.gainI * (1.0 - m_errorTolerance), leftWheel.gainI * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain I is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainD, leftWheel.gainD * (1.0 - m_errorTolerance), leftWheel.gainD * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain D is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainIL, leftWheel.gainIL * (1.0 - m_errorTolerance), leftWheel.gainIL * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain IL is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("Compare of control position right wheel : ");	
	if ((errorCode = m_stellaB1->GetControlPositionParameterRightWheel(gottenParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(gottenParameter.gainP, rightWheel.gainP * (1.0 - m_errorTolerance), rightWheel.gainP * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain P is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainI, rightWheel.gainI * (1.0 - m_errorTolerance), rightWheel.gainI * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain I is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainD, rightWheel.gainD * (1.0 - m_errorTolerance), rightWheel.gainD * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain D is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainIL, rightWheel.gainIL * (1.0 - m_errorTolerance), rightWheel.gainIL * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain IL is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfControlVelocity( const StellaB1::ControlParameter& rightWheel, const StellaB1::ControlParameter& leftWheel )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	StellaB1::ControlParameter gottenParameter;

	PrintMessage("Compare of control velocity left wheel : ");	
	if ((errorCode = m_stellaB1->GetControlVelocityParameterLeftWheel(gottenParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(gottenParameter.gainP, leftWheel.gainP * (1.0 - m_errorTolerance), leftWheel.gainP * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain P is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainI, leftWheel.gainI * (1.0 - m_errorTolerance), leftWheel.gainI * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain I is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainD, leftWheel.gainD * (1.0 - m_errorTolerance), leftWheel.gainD * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain D is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainIL, leftWheel.gainIL * (1.0 - m_errorTolerance), leftWheel.gainIL * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain IL is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");

	PrintMessage("Compare of control velocity right wheel : ");	
	if ((errorCode = m_stellaB1->GetControlVelocityParameterRightWheel(gottenParameter)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(gottenParameter.gainP, rightWheel.gainP * (1.0 - m_errorTolerance), rightWheel.gainP * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain P is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainI, rightWheel.gainI * (1.0 - m_errorTolerance), rightWheel.gainI * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain I is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainD, rightWheel.gainD * (1.0 - m_errorTolerance), rightWheel.gainD * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain D is not valid").c_str());
		return false;
	}
	if (IsBeValidRange(gottenParameter.gainIL, rightWheel.gainIL * (1.0 - m_errorTolerance), rightWheel.gainIL * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Gain IL is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfWheelRadius( const float radius )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	PrintMessage("Compare of wheel radius : ");	
	float wheelRadius;
	if ((errorCode = m_stellaB1->GetWheelRadius(wheelRadius)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(wheelRadius, radius * (1.0 - m_errorTolerance), radius * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Wheel radius is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

bool StellaB1Tester::CompareOfAxleWidth( const float width )
{
	StellaB1::ErrorCode errorCode = StellaB1::NONE_ERROR;
	PrintMessage("Compare of axle width : ");	
	float axleWidth;
	if ((errorCode = m_stellaB1->GetAxleWidth(axleWidth)) != StellaB1::NONE_ERROR)
	{
		PrintMessage(DEBUG_MESSAGE(StellaB1::ConvertToString(errorCode)).c_str());
		return false;
	}

	if (IsBeValidRange(axleWidth, width * (1.0 - m_errorTolerance), width * (1.0 + m_errorTolerance)) == false)
	{
		PrintMessage(DEBUG_MESSAGE("Axle width is not valid").c_str());
		return false;
	}
	PrintMessage("OK \r\n");
	return true;
}

