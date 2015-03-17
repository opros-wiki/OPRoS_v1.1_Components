#include "StellaB1.h"

#include <stdio.h>

#include <device/OprosMath.h>

#include <device/OprosTimer.h>

typedef StellaB1::ErrorCode ErrorCode;

StellaB1::StellaB1(Uart* uart_ /*= NULL*/) :
		m_uart(uart_)
{
}

StellaB1::~StellaB1()
{
	UnregisterUart();
}

void StellaB1::RegisterUart(Uart* uart_)
{
	m_mutexUart.Lock();
	m_uart = uart_;
	m_mutexUart.Unlock();
}

void StellaB1::UnregisterUart()
{
	m_mutexUart.Lock();
	m_uart = NULL;
	m_mutexUart.Unlock();
}

bool StellaB1::WriteCommand(const unsigned char* data, int size)
{
	if (m_uart == NULL)
		return false;

	int errorCount = 0;
	int totalSendSize = 0;

	for (errorCount = 0; errorCount < ERROR_TOLERANCE && totalSendSize < size;)
	{
		int sendSize = 0;
		if ((sendSize = m_uart->Write((unsigned char*) (data + totalSendSize),
				size - totalSendSize)) <= 0)
		{
			errorCount++;
			continue;
		}
		totalSendSize += sendSize;
	}

	if (errorCount >= ERROR_TOLERANCE)
		return false;

	return true;
}

bool StellaB1::ReadResponse(unsigned char* output, int allowableSize)
{
	if (m_uart == NULL)
		return false;

	int errocCount = 0;
	int totalReceiveSize = 0;
	bool hasRecevedSTX = false;

	for (errocCount = 0;
			errocCount < ERROR_TOLERANCE && totalReceiveSize < allowableSize;)
	{
		unsigned char tempReceiveData = 0;
		if (m_uart->Read(&tempReceiveData, 1) <= 0)
		{
			errocCount++;
			continue;
		}

		if (tempReceiveData == STX)
		{
			hasRecevedSTX = true;
		}
		else if (tempReceiveData == ETX)
		{
			output[totalReceiveSize] = '\0';
			break;
		}
		else if (hasRecevedSTX)
		{
			output[totalReceiveSize] = tempReceiveData;
			totalReceiveSize++;
		}
	}

	if (errocCount >= ERROR_TOLERANCE)
		return false;

	return true;
}

template<unsigned int commandSize>
ErrorCode StellaB1::Request(const unsigned char (&command)[commandSize])
{
	const int RESPONSE_SIZE = 32;
	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	m_mutexUart.Lock();

	if (WriteCommand(command, commandSize) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::SENDING_COMMAND_ERROR;
	}

	ReadResponse(response, RESPONSE_SIZE);

	m_mutexUart.Unlock();

	return VerifyErrorResponse(response);
}

template<unsigned int commandSize1, unsigned int commandSize2>
ErrorCode StellaB1::Request(const unsigned char (&command1)[commandSize1], const unsigned char (&command2)[commandSize2])
{
	const int RESPONSE_SIZE = 32;
	unsigned char response1[RESPONSE_SIZE] =
	{ 0, };
	unsigned char response2[RESPONSE_SIZE] =
	{ 0, };

	m_mutexUart.Lock();

	if (WriteCommand(command1, commandSize1) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::SENDING_COMMAND_ERROR;
	}

	OprosSleep(COMMAND_TIME_INTERVAL_MS);

	if (WriteCommand(command2, commandSize2) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::SENDING_COMMAND_ERROR;
	}

	ReadResponse(response1, RESPONSE_SIZE);
	ReadResponse(response2, RESPONSE_SIZE);

	m_mutexUart.Unlock();

	ErrorCode errorCode = StellaB1::NONE_ERROR;
	if ((errorCode = VerifyErrorResponse(response1)) != StellaB1::NONE_ERROR)
	{
		return errorCode;
	}
	if ((errorCode = VerifyErrorResponse(response2)) != StellaB1::NONE_ERROR)
	{
		return errorCode;
	}

	return StellaB1::NONE_ERROR;
}

template<unsigned int commandSize, unsigned int responseSize>
ErrorCode StellaB1::RequestResponse(const unsigned char (&command)[commandSize], unsigned char (&response)[responseSize])
{
	m_mutexUart.Lock();

	if (WriteCommand(command, commandSize) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::SENDING_COMMAND_ERROR;
	}

	if (ReadResponse(response, responseSize) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::RECEIVING_RESPONSE_ERROR;
	}

	m_mutexUart.Unlock();

	ErrorCode errorCode = StellaB1::NONE_ERROR;
	if ((errorCode = VerifyErrorResponse(response))
			!= StellaB1::NONE_ERROR)
	{
		return errorCode;
	}

	return StellaB1::NONE_ERROR;
}

template<unsigned int commandSize1, unsigned int responseSize1, unsigned int commandSize2, unsigned int responseSize2>
ErrorCode StellaB1::RequestResponse(const unsigned char (&command1)[commandSize1], unsigned char (&response1)[responseSize1], const unsigned char (&command2)[commandSize2], unsigned char (&response2)[responseSize2])
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	m_mutexUart.Lock();

	if (WriteCommand(command1, commandSize1) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::SENDING_COMMAND_ERROR;
	}

	OprosSleep(COMMAND_TIME_INTERVAL_MS);

	if (WriteCommand(command2, commandSize2) == false)
	{
		m_mutexUart.Unlock();
		return StellaB1::SENDING_COMMAND_ERROR;
	}

	if (ReadResponse(response1, responseSize1) == false)
	{
		errorCode = StellaB1::RECEIVING_RESPONSE_ERROR;
	}

	if (ReadResponse(response2, responseSize2) == false)
	{
		errorCode = StellaB1::RECEIVING_RESPONSE_ERROR;
	}

	m_mutexUart.Unlock();

	if (errorCode != StellaB1::NONE_ERROR)
	{
		return errorCode;
	}
	if ((errorCode = VerifyErrorResponse(response1))
			!= StellaB1::NONE_ERROR)
	{
		return errorCode;
	}
	if ((errorCode = VerifyErrorResponse(response2))
			!= StellaB1::NONE_ERROR)
	{
		return errorCode;
	}

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::VerifyErrorResponse(const unsigned char* data)
{
	const int dataSize = strlen((char*) data);

	if (dataSize > 5 && data[0] == 'e' && data[1] == 'r' && data[2] == 'r'
			&& data[3] == 'o' && data[4] == 'r')
	{
		return (ErrorCode) atoi((char*) (data + 5));
	}

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::ControlVelocity(float rightWhellRadPerSec,
		float leftWhellRadPerSec)
{
	char rightWheelDirection = rightWhellRadPerSec > 0 ? 'F' : 'B';
	char leftWheelDirection = leftWhellRadPerSec > 0 ? 'F' : 'B';
	int rightWheelSpeed = abs(bound(INTEGER(rightWhellRadPerSec), -999, 999));
	int leftWheelSpeed = abs(bound(INTEGER(leftWhellRadPerSec), -999, 999));

	const int COMMAND_SIZE = 9;
	if (rightWheelSpeed == leftWheelSpeed)
	{
		unsigned char command[COMMAND_SIZE] =
		{ STX, 'C', 'V', 'A', rightWheelDirection, (rightWheelSpeed / 100) % 10
				+ '0', (rightWheelSpeed / 10) % 10 + '0', rightWheelSpeed % 10
				+ '0', ETX };

		return Request(command);
	}
	else
	{
		unsigned char rightWheelCommand[COMMAND_SIZE] =
		{ STX, 'C', 'V', 'R', rightWheelDirection, (rightWheelSpeed / 100) % 10
				+ '0', (rightWheelSpeed / 10) % 10 + '0', rightWheelSpeed % 10
				+ '0', ETX };
		unsigned char leftWheelCommand[COMMAND_SIZE] =
		{ STX, 'C', 'V', 'L', leftWheelDirection, (leftWheelSpeed / 100) % 10
				+ '0', (leftWheelSpeed / 10) % 10 + '0', leftWheelSpeed % 10
				+ '0', ETX };

		return Request(rightWheelCommand,	leftWheelCommand);
	}

	return StellaB1::NONE_ERROR;
}

StellaB1::ErrorCode StellaB1::ControlVelocityLeftWheel(float radPerSec)
{
	char wheelDirection = radPerSec > 0 ? 'F' : 'B';
	int wheelSpeed = abs(bound(INTEGER(radPerSec), -999, 999));

	const int COMMAND_SIZE = 9;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'V', 'L', wheelDirection, (wheelSpeed / 100) % 10 + '0',
			(wheelSpeed / 10) % 10 + '0', wheelSpeed % 10 + '0', ETX };

	return Request(command);
}

ErrorCode StellaB1::ControlVelocityRightWheel(float radPerSec)
{
	char wheelDirection = radPerSec > 0 ? 'F' : 'B';
	int wheelSpeed = abs(bound(INTEGER(radPerSec), -999, 999));

	const int COMMAND_SIZE = 9;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'V', 'R', wheelDirection, (wheelSpeed / 100) % 10 + '0',
			(wheelSpeed / 10) % 10 + '0', wheelSpeed % 10 + '0', ETX };

	return Request(command);
}

ErrorCode StellaB1::ControlPositionDistance(float meter, int sec)
{
	char direction = meter > 0 ? 'F' : 'B';
	int centimeter = abs(bound(INTEGER(meter * 100), -999, 999));

	sec = abs(sec);

	const int COMMAND_SIZE = 18;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'P', 'A', 'R', '0', '0', '0', 'D', direction, (centimeter / 100)
			% 10 + '0', (centimeter / 10) % 10 + '0', centimeter % 10 + '0',
			'T', (sec / 100) % 10 + '0', (sec / 10) % 10 + '0', sec % 10 + '0',
			ETX };

	return Request(command);
}

ErrorCode StellaB1::ControlPositionAngle(float degree, int sec)
{
	char direction = degree > 0 ? 'L' : 'R';
	int degreePosition = abs(bound(INTEGER(degree), -999, 999));

	sec = abs(sec);

	const int COMMAND_SIZE = 18;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'P', 'A', direction, (degreePosition / 100) % 10 + '0',
			(degreePosition / 10) % 10 + '0', degreePosition % 10 + '0', 'D',
			'F', '0', '0', '0', 'T', (sec / 100) % 10 + '0', (sec / 10) % 10
					+ '0', sec % 10 + '0', ETX };

	return Request(command);
}

ErrorCode StellaB1::ControlStop(StopMode mode)
{
	const int COMMAND_SIZE = 8;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'S', 'T', 'O', 'P', mode, ETX };

	return Request(command);
}

ErrorCode StellaB1::ControlReset(ResetMode mode)
{
	const int COMMAND_SIZE = 9;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'R', 'E', 'S', 'E', 'T', mode, ETX };

	return Request(command);
}

ErrorCode StellaB1::ControlInit()
{
	const int COMMAND_SIZE = 7;
	unsigned char command[COMMAND_SIZE] =
	{ STX, 'C', 'I', 'N', 'I', 'T', ETX };

	return Request(command);
}

ErrorCode StellaB1::GetState(State& state)
{
	const int COMMAND_SIZE = 8;
	const int RESPONSE_SIZE = 32;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'G', 'S', 'T', 'A', 'T', 'E', ETX };
	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode errorCode = RequestResponse(command, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	state = (State) atoi((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetVelocity(float& rightWheelRadPerSec,
		float& leftWheelRadPerSec)
{
	const int COMMAND_SIZE = 12;
	const int RESPONSE_SIZE = 32;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'G', 'V', 'E', 'L', 'O', 'C', 'I', 'T', 'Y', 'A', ETX };
	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode errorCode = RequestResponse(command, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	float rightWheelSpeed;
	float leftWheelSpeed;
	char rightWheelDirection, leftWheelDirection;
	if (sscanf((char*) response, "%c%f%c%f", &rightWheelDirection,
			&rightWheelSpeed, &leftWheelDirection, &leftWheelSpeed) != 4)
	{
		return StellaB1::RECEIVING_RESPONSE_ERROR;
	}

	rightWheelRadPerSec =
			rightWheelDirection == 'F' ? rightWheelSpeed : -rightWheelSpeed;
	leftWheelRadPerSec =
			leftWheelDirection == 'F' ? leftWheelSpeed : -leftWheelSpeed;

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetPosition(float& rightWheelMeter, float& leftWheelMeter)
{
	const int COMMAND_SIZE = 12;
	const int RESPONSE_SIZE = 32;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'G', 'P', 'O', 'S', 'I', 'T', 'I', 'O', 'N', 'A', ETX };
	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode errorCode = RequestResponse(command, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	float rightWheelPosition;
	float leftWheelPosition;
	char rightWheelDirection, leftWheelDirection;
	if (sscanf((char*) response, "%c%f%c%f", &rightWheelDirection,
			&rightWheelPosition, &leftWheelDirection, &leftWheelPosition) != 4)
	{
		return StellaB1::RECEIVING_RESPONSE_ERROR;
	}

	rightWheelMeter =
			rightWheelDirection == 'F' ?
					rightWheelPosition : -rightWheelPosition;
	leftWheelMeter =
			leftWheelDirection == 'F' ? leftWheelPosition : -leftWheelPosition;

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetScaleFactorAnlge(float& rightWheel, float& leftWheel)
{
	const int COMMAND_SIZE = 7;
	const int RESPONSE_SIZE = 32;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'S', 'F', 'A', 'R', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'S', 'F', 'A', 'L', ETX };

	unsigned char rightWheelResponse[RESPONSE_SIZE] =
	{ 0, };
	unsigned char leftWheelResponse[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode erroCode = RequestResponse(
			rightWheelCommand, rightWheelResponse,
			leftWheelCommand, leftWheelResponse);
	if (erroCode != StellaB1::NONE_ERROR)
		return erroCode;

	rightWheel = (float) atof((char*) rightWheelResponse);
	leftWheel = (float) atof((char*) leftWheelResponse);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetScaleFactorDistance(float& rightWheel, float& leftWheel)
{
	const int COMMAND_SIZE = 7;
	const int RESPONSE_SIZE = 32;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'S', 'F', 'D', 'R', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'S', 'F', 'D', 'L', ETX };

	unsigned char rightWheelResponse[RESPONSE_SIZE] =
	{ 0, };
	unsigned char leftWheelResponse[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode erroCode = RequestResponse(
			rightWheelCommand, rightWheelResponse,
			leftWheelCommand, leftWheelResponse);
	if (erroCode != StellaB1::NONE_ERROR)
		return erroCode;

	rightWheel = (float) atof((char*) rightWheelResponse);
	leftWheel = (float) atof((char*) leftWheelResponse);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetVelocityAcceleration(float& rightWheelRadPerSecSec,
		float& leftWheelRadPerSecSec)
{
	const int COMMAND_SIZE = 6;
	const int RESPONSE_SIZE = 32;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'V', 'A', 'R', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'V', 'A', 'L', ETX };

	unsigned char rightWheelResponse[RESPONSE_SIZE] =
	{ 0, };
	unsigned char leftWheelResponse[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode erroCode = RequestResponse(
			rightWheelCommand, rightWheelResponse,
			leftWheelCommand, leftWheelResponse);
	if (erroCode != StellaB1::NONE_ERROR)
		return erroCode;

	rightWheelRadPerSecSec = (float) atof((char*) rightWheelResponse);
	leftWheelRadPerSecSec = (float) atof((char*) leftWheelResponse);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetVelocityDeacceleration(float& rightWheelRadPerSecSec,
		float& leftWheelRadPerSecSec)
{
	const int COMMAND_SIZE = 6;
	const int RESPONSE_SIZE = 32;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'V', 'D', 'R', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'G', 'V', 'D', 'L', ETX };

	unsigned char rightWheelResponse[RESPONSE_SIZE] =
	{ 0, };
	unsigned char leftWheelResponse[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode erroCode = RequestResponse(
			rightWheelCommand, rightWheelResponse,
			leftWheelCommand, leftWheelResponse);
	if (erroCode != StellaB1::NONE_ERROR)
		return erroCode;

	rightWheelRadPerSecSec = (float) atof((char*) rightWheelResponse);
	leftWheelRadPerSecSec = (float) atof((char*) leftWheelResponse);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetControlVelocityParameterRightWheel(
		ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 7;
	const int RESPONSE_SIZE = 32;

	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'P', 'R', ETX };

	errorCode = RequestResponse(controlPCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainP = (float) atof((char*) response);

	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'I', 'R', ETX };

	errorCode = RequestResponse(controlICommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainI = (float) atof((char*) response);

	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'D', 'R', ETX };

	errorCode = RequestResponse(controlDCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainD = (float) atof((char*) response);

	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'L', 'R', ETX };

	errorCode = RequestResponse(controlIlCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainIL = (float) atof((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetControlVelocityParameterLeftWheel(
		ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 7;
	const int RESPONSE_SIZE = 32;

	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'P', 'L', ETX };

	errorCode = RequestResponse(controlPCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainP = (float) atof((char*) response);

	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'I', 'L', ETX };

	errorCode = RequestResponse(controlICommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainI = (float) atof((char*) response);

	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'D', 'L', ETX };

	errorCode = RequestResponse(controlDCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainD = (float) atof((char*) response);

	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'V', 'L', 'L', ETX };

	errorCode = RequestResponse(controlIlCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainIL = (float) atof((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetControlPositionParameterRightWheel(
		ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 7;
	const int RESPONSE_SIZE = 32;

	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'P', 'R', ETX };

	errorCode = RequestResponse(controlPCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainP = (float) atof((char*) response);

	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'I', 'R', ETX };

	errorCode = RequestResponse(controlICommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainI = (float) atof((char*) response);

	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'D', 'R', ETX };

	errorCode = RequestResponse(controlDCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainD = (float) atof((char*) response);

	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'L', 'R', ETX };

	errorCode = RequestResponse(controlIlCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainIL = (float) atof((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetControlPositionParameterLeftWheel(
		ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 7;
	const int RESPONSE_SIZE = 32;

	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'P', 'L', ETX };

	errorCode = RequestResponse(controlPCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainP = (float) atof((char*) response);

	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'I', 'L', ETX };

	errorCode = RequestResponse(controlICommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainI = (float) atof((char*) response);

	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'D', 'L', ETX };

	errorCode = RequestResponse(controlDCommand, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainD = (float) atof((char*) response);

	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'G', 'G', 'P', 'L', 'L', ETX };

	errorCode = RequestResponse(controlIlCommand,	response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;
	controlParameter.gainIL = (float) atof((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetWheelRadius(float& meter)
{
	const int COMMAND_SIZE = 4;
	const int RESPONSE_SIZE = 32;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'G', 'R', ETX };
	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode errorCode = RequestResponse(command, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	meter = (float) atof((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::GetAxleWidth(float& meter)
{
	const int COMMAND_SIZE = 4;
	const int RESPONSE_SIZE = 32;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'G', 'W', ETX };
	unsigned char response[RESPONSE_SIZE] =
	{ 0, };

	ErrorCode errorCode = RequestResponse(command, response);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	meter = (float) atof((char*) response);

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::SetLimitCurrent(int miliampere)
{
	miliampere = bound(miliampere, 0, 9999);

	const int COMMAND_SIZE = 9;

	unsigned char command[COMMAND_SIZE] =
			{ STX, 'S', 'L', 'A', (miliampere / 1000) % 10 + '0', (miliampere
					/ 100) % 10 + '0', (miliampere / 10) % 10 + '0', miliampere
					% 10 + '0', ETX };

	return Request(command);
}

ErrorCode StellaB1::SetScaleFactorAngle(float rightWheel, float leftWheel)
{
	int rightWheelScaleFactor = bound(INTEGER(rightWheel * 1000), 0, 999);
	int leftWheelScaleFactor = bound(INTEGER(leftWheel * 1000), 0, 999);

	const int COMMAND_SIZE = 10;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'S', 'F', 'A', 'R', (rightWheelScaleFactor / 100) % 10 + '0',
			(rightWheelScaleFactor / 10) % 10 + '0', rightWheelScaleFactor % 10
					+ '0', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'S', 'F', 'A', 'L', (leftWheelScaleFactor / 100) % 10 + '0',
			(leftWheelScaleFactor / 10) % 10 + '0', leftWheelScaleFactor % 10
					+ '0', ETX };

	return Request(rightWheelCommand, leftWheelCommand);
}

ErrorCode StellaB1::SetScaleFactorDistance(float rightWheel, float leftWheel)
{
	int rightWheelScaleFactor = bound(INTEGER(rightWheel * 1000), 0, 999);
	int leftWheelScaleFactor = bound(INTEGER(leftWheel * 1000), 0, 999);

	const int COMMAND_SIZE = 10;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'S', 'F', 'D', 'R', (rightWheelScaleFactor / 100) % 10 + '0',
			(rightWheelScaleFactor / 10) % 10 + '0', rightWheelScaleFactor % 10
					+ '0', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'S', 'F', 'D', 'L', (leftWheelScaleFactor / 100) % 10 + '0',
			(leftWheelScaleFactor / 10) % 10 + '0', leftWheelScaleFactor % 10
					+ '0', ETX };

	return Request(rightWheelCommand,	leftWheelCommand);
}

ErrorCode StellaB1::SetVelocityAcceleration(float rightWheelRadPerSecSec,
		float leftWheelRadPerSecSec)
{
	int rightWheelAccel = bound(INTEGER(rightWheelRadPerSecSec * 10), 0, 99);
	int leftWheelAccel = bound(INTEGER(leftWheelRadPerSecSec * 10), 0, 99);

	const int COMMAND_SIZE = 9;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'V', 'A', 'R', (rightWheelAccel / 10) % 10 + '0', '.',
			rightWheelAccel % 10 + '0', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'V', 'A', 'L', (leftWheelAccel / 10) % 10 + '0', '.',
			leftWheelAccel % 10 + '0', ETX };

	return Request(rightWheelCommand, leftWheelCommand);
}

ErrorCode StellaB1::SetVelocityDeacceleration(float rightWheelRadPerSecSec,
		float leftWheelRadPerSecSec)
{
	int rightWheelAccel = bound(INTEGER(rightWheelRadPerSecSec * 10), 0, 99);
	int leftWheelAccel = bound(INTEGER(leftWheelRadPerSecSec * 10), 0, 99);

	const int COMMAND_SIZE = 9;

	unsigned char rightWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'V', 'D', 'R', (rightWheelAccel / 10) % 10 + '0', '.',
			rightWheelAccel % 10 + '0', ETX };
	unsigned char leftWheelCommand[COMMAND_SIZE] =
	{ STX, 'S', 'V', 'D', 'L', (leftWheelAccel / 10) % 10 + '0', '.',
			leftWheelAccel % 10 + '0', ETX };

	return Request(rightWheelCommand, leftWheelCommand);
}

ErrorCode StellaB1::SetControlVelocityParameterRightWheel(
		const ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 14;

	int controlP = bound(INTEGER(controlParameter.gainP * 1000), 0, 999999);
	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'P', 'R', (controlP / 100000) % 10 + '0', (controlP
			/ 10000) % 10 + '0', (controlP / 1000) % 10 + '0', '.', (controlP
			/ 100) % 10 + '0', (controlP / 10) % 10 + '0', controlP % 10 + '0',
			ETX };
	errorCode = Request(controlPCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlI = bound(INTEGER(controlParameter.gainI * 1000), 0, 999999);
	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'I', 'R', (controlI / 100000) % 10 + '0', (controlI
			/ 10000) % 10 + '0', (controlI / 1000) % 10 + '0', '.', (controlI
			/ 100) % 10 + '0', (controlI / 10) % 10 + '0', controlI % 10 + '0',
			ETX };
	errorCode = Request(controlICommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlD = bound(INTEGER(controlParameter.gainD * 1000), 0, 999999);
	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'D', 'R', (controlD / 100000) % 10 + '0', (controlD
			/ 10000) % 10 + '0', (controlD / 1000) % 10 + '0', '.', (controlD
			/ 100) % 10 + '0', (controlD / 10) % 10 + '0', controlD % 10 + '0',
			ETX };
	errorCode = Request(controlDCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlIl = bound(INTEGER(controlParameter.gainIL * 1000), 0, 999999);
	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'L', 'R', (controlIl / 100000) % 10 + '0', (controlIl
			/ 10000) % 10 + '0', (controlIl / 1000) % 10 + '0', '.', (controlIl
			/ 100) % 10 + '0', (controlIl / 10) % 10 + '0', controlIl % 10
			+ '0', ETX };
	errorCode = Request(controlIlCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::SetControlVelocityParameterLeftWheel(
		const ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 14;

	int controlP = bound(INTEGER(controlParameter.gainP * 1000), 0, 999999);
	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'P', 'L', (controlP / 100000) % 10 + '0', (controlP
			/ 10000) % 10 + '0', (controlP / 1000) % 10 + '0', '.', (controlP
			/ 100) % 10 + '0', (controlP / 10) % 10 + '0', controlP % 10 + '0',
			ETX };
	errorCode = Request(controlPCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlI = bound(INTEGER(controlParameter.gainI * 1000), 0, 999999);
	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'I', 'L', (controlI / 100000) % 10 + '0', (controlI
			/ 10000) % 10 + '0', (controlI / 1000) % 10 + '0', '.', (controlI
			/ 100) % 10 + '0', (controlI / 10) % 10 + '0', controlI % 10 + '0',
			ETX };
	errorCode = Request(controlICommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlD = bound(INTEGER(controlParameter.gainD * 1000), 0, 999999);
	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'D', 'L', (controlD / 100000) % 10 + '0', (controlD
			/ 10000) % 10 + '0', (controlD / 1000) % 10 + '0', '.', (controlD
			/ 100) % 10 + '0', (controlD / 10) % 10 + '0', controlD % 10 + '0',
			ETX };
	errorCode = Request(controlDCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlIl = bound(INTEGER(controlParameter.gainIL * 1000), 0, 999999);
	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'V', 'L', 'L', (controlIl / 100000) % 10 + '0', (controlIl
			/ 10000) % 10 + '0', (controlIl / 1000) % 10 + '0', '.', (controlIl
			/ 100) % 10 + '0', (controlIl / 10) % 10 + '0', controlIl % 10
			+ '0', ETX };
	errorCode = Request(controlIlCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::SetControlPositionParameterRightWheel(
		const ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 14;

	int controlP = bound(INTEGER(controlParameter.gainP * 1000), 0, 999999);
	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'P', 'R', (controlP / 100000) % 10 + '0', (controlP
			/ 10000) % 10 + '0', (controlP / 1000) % 10 + '0', '.', (controlP
			/ 100) % 10 + '0', (controlP / 10) % 10 + '0', controlP % 10 + '0',
			ETX };
	errorCode = Request(controlPCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlI = bound(INTEGER(controlParameter.gainI * 1000), 0, 999999);
	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'I', 'R', (controlI / 100000) % 10 + '0', (controlI
			/ 10000) % 10 + '0', (controlI / 1000) % 10 + '0', '.', (controlI
			/ 100) % 10 + '0', (controlI / 10) % 10 + '0', controlI % 10 + '0',
			ETX };
	errorCode = Request(controlICommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlD = bound(INTEGER(controlParameter.gainD * 1000), 0, 999999);
	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'D', 'R', (controlD / 100000) % 10 + '0', (controlD
			/ 10000) % 10 + '0', (controlD / 1000) % 10 + '0', '.', (controlD
			/ 100) % 10 + '0', (controlD / 10) % 10 + '0', controlD % 10 + '0',
			ETX };
	errorCode = Request(controlDCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlIl = bound(INTEGER(controlParameter.gainIL * 1000), 0, 999999);
	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'L', 'R', (controlIl / 100000) % 10 + '0', (controlIl
			/ 10000) % 10 + '0', (controlIl / 1000) % 10 + '0', '.', (controlIl
			/ 100) % 10 + '0', (controlIl / 10) % 10 + '0', controlIl % 10
			+ '0', ETX };
	errorCode = Request(controlIlCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::SetControlPositionParameterLeftWheel(
		const ControlParameter& controlParameter)
{
	ErrorCode errorCode = StellaB1::NONE_ERROR;
	const int COMMAND_SIZE = 14;

	int controlP = bound(INTEGER(controlParameter.gainP * 1000), 0, 999999);
	unsigned char controlPCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'P', 'L', (controlP / 100000) % 10 + '0', (controlP
			/ 10000) % 10 + '0', (controlP / 1000) % 10 + '0', '.', (controlP
			/ 100) % 10 + '0', (controlP / 10) % 10 + '0', controlP % 10 + '0',
			ETX };
	errorCode = Request(controlPCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlI = bound(INTEGER(controlParameter.gainI * 1000), 0, 999999);
	unsigned char controlICommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'I', 'L', (controlI / 100000) % 10 + '0', (controlI
			/ 10000) % 10 + '0', (controlI / 1000) % 10 + '0', '.', (controlI
			/ 100) % 10 + '0', (controlI / 10) % 10 + '0', controlI % 10 + '0',
			ETX };
	errorCode = Request(controlICommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlD = bound(INTEGER(controlParameter.gainD * 1000), 0, 999999);
	unsigned char controlDCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'D', 'L', (controlD / 100000) % 10 + '0', (controlD
			/ 10000) % 10 + '0', (controlD / 1000) % 10 + '0', '.', (controlD
			/ 100) % 10 + '0', (controlD / 10) % 10 + '0', controlD % 10 + '0',
			ETX };
	errorCode = Request(controlDCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	int controlIl = bound(INTEGER(controlParameter.gainIL * 1000), 0, 999999);
	unsigned char controlIlCommand[COMMAND_SIZE] =
	{ STX, 'S', 'G', 'P', 'L', 'L', (controlIl / 100000) % 10 + '0', (controlIl
			/ 10000) % 10 + '0', (controlIl / 1000) % 10 + '0', '.', (controlIl
			/ 100) % 10 + '0', (controlIl / 10) % 10 + '0', controlIl % 10
			+ '0', ETX };
	errorCode = Request(controlIlCommand);
	if (errorCode != StellaB1::NONE_ERROR)
		return errorCode;

	return StellaB1::NONE_ERROR;
}

ErrorCode StellaB1::SetWheelRadius(float meter)
{
	int wheelRadius = bound(INTEGER(meter * 10000), 0, 99999);

	const int COMMAND_SIZE = 10;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'S', 'R', (wheelRadius / 10000) % 10 + '0', '.', (wheelRadius / 1000)
			% 10 + '0', (wheelRadius / 100) % 10 + '0', (wheelRadius / 10) % 10
			+ '0', wheelRadius % 10 + '0', ETX };

	return Request(command);
}

ErrorCode StellaB1::SetAxleWidth(float meter)
{
	int wheelAxleWidth = bound(INTEGER(meter * 10000), 0, 99999);

	const int COMMAND_SIZE = 10;

	unsigned char command[COMMAND_SIZE] =
	{ STX, 'S', 'W', (wheelAxleWidth / 10000) % 10 + '0', '.', (wheelAxleWidth
			/ 1000) % 10 + '0', (wheelAxleWidth / 100) % 10 + '0',
			(wheelAxleWidth / 10) % 10 + '0', wheelAxleWidth % 10 + '0', ETX };

	return Request(command);
}

std::string StellaB1::ConvertToString(ErrorCode errorCode)
{
	switch (errorCode)
	{
	case NONE_ERROR:
		return "No Error";
	case RIGHT_WHEEL_OVERLOAD_ERROR:
		return "Right Wheel Overload, Please reset power.";
	case LEFT_WHEEL_OVERLOAD_ERROR:
		return "Left Wheel Overload, Please reset power.";
	case ALL_WHEEL_OVERLOAD_ERROR:
		return "All Wheel Overload, Please reset power.";
	case OVERVOLTAGE_ERROR:
		return "Over voltage, Please reset power.";
	case LOWVOLTAGE_ERROR:
		return "Low voltage, Please reset power.";
	case OVER_SPEED_ERROR:
		return "Over Speed";
	case OMMITTED_SOME_COMMAND_ERROR:
		return "Omitted Some Command";
	case INVALID_COMMAND_ERROR:
		return "Invalid Command";
	case INVALID_DIRECTION_COMMAND_ERROR:
		return "Invalid Direction Command";
	case OUT_OF_DATA_RANGE_ERROR:
		return "Out of Data Range";
	case SENDING_COMMAND_ERROR:
		return "Fail Send Command";
	case RECEIVING_RESPONSE_ERROR:
		return "Fail Receive Command";
	default:
		return "Unkwon Error";
	}
}

