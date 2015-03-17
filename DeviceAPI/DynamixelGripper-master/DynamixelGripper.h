#ifndef __DYNAMIXEL_MANIPULATOR_H__
#define __DYNAMIXEL_MANIPULATOR_H__

#include <device/Manipulator.h>
#include <device/Gripper.h>
#include <device/ServoActuator.h>
#include <oprostypes.h>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include "DynamicLibraryLoader.h"
#include "DynamixelUART.h"
#include "DynamixelGroup.h"

#include "MessageQueue.h"

class DynamixelGripper : public Gripper
{
public:
	enum GripperCommand
	{
		START_GRIPPING = 1, STOP_GRIPPING = START_GRIPPING + 1
	};

protected:
	struct DynamixelProperty
	{
		DynamixelProperty()
			: id(0), maximumPower(0.0), maximuVelocity(0.0)
			, complianceMargine(0), compliacneSlope(0)
			, minimumPositionLimit(0.0), maximumPositionLimit(0.0)
			, positionResolution(0.0), positionOffset(0.0)
		{}

		unsigned char id;
		unsigned char complianceMargine;
		unsigned char compliacneSlope;
		double positionResolution;
		double positionOffset;
		double maximumPower;
		double maximuVelocity;
		double minimumPositionLimit;
		double maximumPositionLimit;
	};
	
	struct GripperDynamixelProperty : public DynamixelProperty
	{
		GripperDynamixelProperty()
			: maximumLoad(0.0)
		{}

		double maximumLoad;
	};

public:
	DynamixelGripper();
	virtual ~DynamixelGripper();

public:
	virtual int Initialize(Property parameter);
	virtual int Finalize();
	virtual int Enable();
	virtual int Disable();
	virtual int SetParameter(Property parameter);
	virtual int GetParameter(Property &parameter);

public:
	virtual int RunHoming();
	virtual int Stop();
	virtual int EmergencyStop();
	virtual int SetPosition(vector<double> position, vector<unsigned long> time);
	virtual int GetPosition(vector<double>& position);

public:
	virtual int StartGripping();
	virtual int StopGripping();
	virtual int IsGripped(bool &isGripped);

private:
	bool Setting(Property& parameter);
	bool EnableDynamixel(DynamixelUART& dynamixel, const DynamixelProperty& property);
	
	inline unsigned short ConvertPowerUnitToDynamixel(const double& percent);
	inline unsigned short ConvertPositionUnitToDynamixel(const double& degree, const double& offset, const double& resolution);
	inline unsigned short ConvertVelocityUnitToDynamixel(const double& rpm);

	inline double ConvertPowerUnitToPercent(unsigned short dynamixelValue);
	inline double ConvertPositionUnitToDegree(unsigned short dynamixelValue, const double& offset, const double& resolution);
	inline double ConvertVelocityUnitToRPM(unsigned short dynamixelValue);
	inline double ConvertLoadUnitToPercent(unsigned short dynamixelValue);

private:
	void GripperControlThreadHandler();

private:
	boost::shared_ptr<DynamicLibraryLoader> uartLibraryLoader;
	DynamixelGroup dynamixelGroup;
	vector<DynamixelProperty> dynamixelPropertyVector;

	boost::shared_ptr<DynamixelUART> gripper;
	GripperDynamixelProperty gripperProperty;
	
	MessageQueue<GripperCommand> gripperMessageQueue;
	boost::thread* gripperControlThread;

	Uart* uart;
	bool mIsGripped;
	vector<double> previousPosition;
};

#endif //__DYNAMIXEL_MANIPULATOR_H__


