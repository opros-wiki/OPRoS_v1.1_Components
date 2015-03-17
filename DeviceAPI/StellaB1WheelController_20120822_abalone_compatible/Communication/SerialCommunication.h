#pragma once

#include <device/Uart.h>
#include <device/OprosLock.h>
#include "SerialImpl.h"

class SerialCommunication : public Uart
{
public:
	SerialCommunication(void);
	virtual ~SerialCommunication(void);

private:
	SerialImpl *handle;
	Property parameter;
	OprosLock lock;

public:
	int Initialize(Property parameter);
	int Finalize(void);
	int Enable(void);
	int Disable(void);
	int SetParameter(Property parameter);
	int GetParameter(Property &parameter);

	int Lock(void);
	int Unlock(void);

	int Write(unsigned char *data, int size);
	int Read(unsigned char *data, int size);
};
