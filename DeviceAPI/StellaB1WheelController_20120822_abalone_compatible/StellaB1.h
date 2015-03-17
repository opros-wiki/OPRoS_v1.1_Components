#ifndef __STELLA_B1_H__
#define __STELLA_B1_H__

#include <string>
#include <device/Uart.h>

#include "SimpleLock.h"

class StellaB1
{
public:
	enum State { STEADY_STATE = 0, RIGHT_WHEEL_OVERLOAD_STATE = 1, LEFT_WHEEL_OVERLOAD_STATE = 2, ALL_WHEEL_OVERLOAD_STATE = 3, OVERVOLTAGE_STATE = 4, LOWVOLTAGE_STATE = 5, REACHED_DESIRED_POSITION_STATE = 6, RUNNING_WHEEL_STATE = 7 };
	enum ErrorCode { NONE_ERROR = 0,  RIGHT_WHEEL_OVERLOAD_ERROR = 1, LEFT_WHEEL_OVERLOAD_ERROR = 2, ALL_WHEEL_OVERLOAD_ERROR = 3, OVERVOLTAGE_ERROR = 4, LOWVOLTAGE_ERROR = 5, OVER_SPEED_ERROR = 8, OMMITTED_SOME_COMMAND_ERROR = 9, INVALID_COMMAND_ERROR = 10, INVALID_DIRECTION_COMMAND_ERROR = 11, OUT_OF_DATA_RANGE_ERROR = 12, SENDING_COMMAND_ERROR = 1000, RECEIVING_RESPONSE_ERROR = 2000};
	enum StopMode { CSTOP1 = '1', CSTOP2 = '2', CSTOP3 = '3' };
	enum ResetMode { CRESETA = 'A', CRESETR = 'R', CRESETL = 'L' };

	class ControlParameter
	{
	public:
		ControlParameter(float gainP_ = 0.0f, float gainI_ = 0.0f, float gainD_ = 0.0f, float gainIl_ = 0.0f)
			: gainP(gainP_), gainI(gainI_), gainD(gainD_), gainIL(gainIl_)
		{}

		float gainP;
		float gainI;
		float gainD;
		float gainIL;
	};

public:
	static const char STX = 0x02;
	static const char ETX = 0x03;
	static const int ERROR_TOLERANCE = 5;
	static const int COMMAND_TIME_INTERVAL_MS = 5;

public:
	StellaB1(Uart* uart_ = NULL);
	~StellaB1(void);

public:
	void RegisterUart(Uart* uart_);
	void UnregisterUart();

	static std::string ConvertToString(ErrorCode errorCode);

public:
	ErrorCode ControlVelocity(float rightWhellRadPerSec, float leftWhellRadPerSec);
	ErrorCode ControlVelocityLeftWheel(float radPerSec);
	ErrorCode ControlVelocityRightWheel(float radPerSec);

	ErrorCode ControlPositionDistance(float meter, int sec);
	ErrorCode ControlPositionAngle(float degree, int sec);

	ErrorCode ControlStop(StopMode mode);

	ErrorCode ControlReset(ResetMode mode);

	ErrorCode ControlInit();

	ErrorCode GetState(State& state);

	ErrorCode GetVelocity(float& rightWheelRadPerSec, float& leftWheelRadPerSec);

	ErrorCode GetPosition(float& rightWheelMeter, float& leftWheelMeter);

	ErrorCode GetScaleFactorAnlge(float& rightWheel, float& leftWheel);
	ErrorCode GetScaleFactorDistance(float& rightWheel, float& leftWheel);

	ErrorCode GetVelocityAcceleration(float& rightWheelRadPerSecSec, float& leftWheelRadPerSecSec);
	ErrorCode GetVelocityDeacceleration(float& rightWheelRadPerSecSec, float& leftWheelRadPerSecSec);

	ErrorCode GetControlVelocityParameterRightWheel(ControlParameter& controlParameter);
	ErrorCode GetControlVelocityParameterLeftWheel(ControlParameter& controlParameter);
	ErrorCode GetControlPositionParameterRightWheel(ControlParameter& controlParameter);
	ErrorCode GetControlPositionParameterLeftWheel(ControlParameter& controlParameter);

	ErrorCode GetWheelRadius(float& meter);
	
	ErrorCode GetAxleWidth(float& meter);

	ErrorCode SetLimitCurrent(int miliampere);

	ErrorCode SetScaleFactorAngle(float rightWheel, float leftWheel);
	ErrorCode SetScaleFactorDistance(float rightWheel, float leftWheel);

	ErrorCode SetVelocityAcceleration(float rightWheelRadPerSecSec, float leftWheelRadPerSecSec);
	ErrorCode SetVelocityDeacceleration(float rightWheelRadPerSecSec, float leftWheelRadPerSecSec);

	ErrorCode SetControlVelocityParameterRightWheel(const ControlParameter& controlParameter);
	ErrorCode SetControlVelocityParameterLeftWheel(const ControlParameter& controlParameter);
	ErrorCode SetControlPositionParameterRightWheel(const ControlParameter& controlParameter);
	ErrorCode SetControlPositionParameterLeftWheel(const ControlParameter& controlParameter);

	ErrorCode SetWheelRadius(float meter);

	ErrorCode SetAxleWidth(float meter);

private:
	bool ReadResponse(unsigned char* output, int allowableSize);
	bool WriteCommand(const unsigned char* data, int size);

	ErrorCode VerifyErrorResponse(const unsigned char* data);
	
	template<unsigned int commandSize>
	ErrorCode Request(const unsigned char (&command)[commandSize]);
	template<unsigned int commandSize1, unsigned int commandSize2>
	ErrorCode Request(const unsigned char (&command1)[commandSize1], const unsigned char (&command2)[commandSize2]);
	template<unsigned int commandSize, unsigned int responseSize>
	ErrorCode RequestResponse(const unsigned char (&command)[commandSize], unsigned char (&response)[responseSize]);
	template<unsigned int commandSize1, unsigned int responseSize1, unsigned int commandSize2, unsigned int responseSize2>
	ErrorCode RequestResponse(const unsigned char (&command1)[commandSize1], unsigned char (&response1)[responseSize1], const unsigned char (&command2)[commandSize2], unsigned char (&response2)[responseSize2]);

private:
	Uart* m_uart;
	SimpleMutex m_mutexUart;
};

#endif


