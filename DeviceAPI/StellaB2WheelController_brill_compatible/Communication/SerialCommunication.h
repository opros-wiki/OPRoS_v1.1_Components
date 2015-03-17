#pragma once

#include <comm/Uart.h>
#include <Property.h>
#include <comm/OprosLock.h>
#include "SerialImpl.h"

class SerialCommunication : public Uart
{
public:
	SerialCommunication(void);
	virtual ~SerialCommunication(void);

private:
	SerialImpl *handle;
	OPRoS::Property parameter;
	OprosLock lock;

public:
	int InitalizeSerial(OPRoS::Property parameter);
	int FinalizeSerial(void);
	int EnableSerial(void);
	int DisableSerial(void);
	int SetParameterSerial(OPRoS::Property parameter);
	int GetParameterSerial(OPRoS::Property &parameter);

	int Lock(void);
	int Unlock(void);

	int Write(unsigned char *data, int size);
	int Read(unsigned char *data, int size);
};
