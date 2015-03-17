#ifndef __STELLA_B1_TEST_H__
#include "StellaB1WheelController.h"

class StellaB1Tester
{
public:
	StellaB1Tester(StellaB1* stellb1_, float errorTolerance_ = 0.2f)
		: m_stellaB1(stellb1_), m_errorTolerance(errorTolerance_)
	{}

public:
	bool TestAll();

	bool TestInit();
	bool TestReset();

	bool TestControlVelocity(const float velocity = DEG2RAD(90.0) * 14.0);
	bool TestControlPosition(const float distance = 0.5f, const float angle = 60.0f);
	bool TestControlStop();
	bool TestSetGetScale(const float scale = 0.0f);
	bool TestSetGetAcceleration(const float accel = 0.0f);
	bool TestSetGetControlParameter(const float gainP = 30.0f, const float gainI = 200.0f, const float gainD = 0.005f, const float gainIl = 20.0f);
	bool TestSetGetWheelRadius(const float radius = 0.0752f);
	bool TestSetGetAxleWidth(const float width = 0.289f);

private:
	inline bool IsBeValidRange(double value, double min, double max)
	{
		return (min <= value && value <= max)? true : false;
	}

	bool CompareOfVelocity(const float rightVelocity, const float leftVelocity);
	bool CompareOfPositionDistacne(const float rightDistance, const float leftDistance);
	bool CompareOfPositionAngle(const float angle, const float previusRightWheelPosition, const float previusleftWheelPosition);
	bool CompareOfAngleScale(const float rightScale, const float leftScale);
	bool CompareOfDistacneScale(const float rightScale, const float leftScale);
	bool CompareOfAcceleration(const float rightAccel, const float leftAccel);
	bool CompareOfDeacceleration(const float rightDeaccel, const float leftDeaccel);
	bool CompareOfControlPosition(const StellaB1::ControlParameter& rightWheel, const StellaB1::ControlParameter& leftWheel);
	bool CompareOfControlVelocity(const StellaB1::ControlParameter& rightWheel, const StellaB1::ControlParameter& leftWheel);
	bool CompareOfWheelRadius(const float radius);
	bool CompareOfAxleWidth(const float width);

private:
	StellaB1* m_stellaB1;
	float m_errorTolerance;
};
#endif
