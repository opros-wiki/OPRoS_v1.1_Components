#ifndef _ROBOTEVER_MOBILITY_H_
#define _ROBOTEVER_MOBILITY_H_

#include "Mobility.h"

#define ENCODER 512
#define GEAR_RATIO	51

#define RIGHT_WHEEL 1
#define LEFT_WHEEL	2

#define CW	0
#define CCW 1


using namespace OPRoS;

class RobotEver_Mobility : public Mobility
{
public:
	RobotEver_Mobility(void);
	virtual ~RobotEver_Mobility(void);

public:

	bool initialize(Property parameter);
	bool finalize(void);
	bool enable(void);
	bool disable(void);
	bool setParameter(Property parameter);
	Property getParameter(void);
	bool setPosition(ObjectLocation position);
	bool getPosition(ObjectLocation *position);
	bool readEncoder(long *leftEncoder, long *rightEncoder);
	bool move(double distance);
	bool rotate(double degree);

	bool drive(double forwardVelocity, double angularVelocity);
	bool stop(void);
	bool isWheelRunning(void);
	bool execute(void);
	bool update(void);

private:
	int buf[3], buf1[3], buf2[3];
	ObjectLocation location;

	int distance1;
	int degree1;

	Property parameter;
	long leftWheelEncoderPrev;

	long rightWheelEncoderPrev;

	double currentForward;

	double currentTurn;
	double targetForward;
	double targetTurn;
	ObjectLocation position;
	ObjectLocation variance;
	bool isRunning;
	int mobilityCommand;
	double wheelDiameter;
	double axleDistance;
	double varianceDistance;
	double varianceDirection;
	double safeRadius;
	double maximumVelocity;
	double acceleration;
	bool setParameter (int index, int parameter);
	bool getParameter (int index, int *parameter);

};


#endif // _DASA_ROBOT_TETRA_MOBILITY_H_