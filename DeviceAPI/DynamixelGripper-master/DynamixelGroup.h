#ifndef __DYNAMIXEL_GROUP_H__
#define __DYNAMIXEL_GROUP_H__

#include <vector>
#include <device/UART.h>

#include "DynamixelUART.h"

class DynamixelGroup
{
public:
	DynamixelGroup(Uart* uart_ = NULL);
	~DynamixelGroup();

public:
	void SetUart(Uart* uart_);
	void Add(DynamixelUART& dynamixel);
	size_t CountDynamixel();
	void Clear();

	bool SetGoalPosition(const vector<unsigned short>& goalPosition);
	bool SetTorqueEnable(bool isEnabled);


	DynamixelUART& operator[](size_t index);

private:
	DynamixelUART broadcastDynamixel;
	std::vector<DynamixelUART> dynamixelVector;
	Uart* uart;
};

#endif