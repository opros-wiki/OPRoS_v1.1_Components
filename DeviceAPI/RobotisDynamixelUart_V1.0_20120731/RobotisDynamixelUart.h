#pragma once

#include "Uart.h"
#include "DynamixelUart.h"
#include "JointController.h"

class DynamixelInfo
{
public:
	DynamixelInfo(Uart *uart, unsigned char id) {
		dynamixel = new DynamixelUart(uart, id);
	}
	virtual ~DynamixelInfo(void) {
		delete dynamixel;
	}

	DynamixelUart *dynamixel;
};

class RobotisDynamixelUart : public JointController
{
public:
	RobotisDynamixelUart(void);
	virtual ~RobotisDynamixelUart(void);

	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int SetPosition(vector<double> position, vector<unsigned long> time);
	int GetPosition(vector<double> &position);

private:
	Uart *uart;
#if defined(WIN32)
	HMODULE hOprosAPI;
#else
	void *hOprosAPI;
#endif
	vector<DynamixelInfo> joints;

	int InitializeUART(Property parameter);
};

