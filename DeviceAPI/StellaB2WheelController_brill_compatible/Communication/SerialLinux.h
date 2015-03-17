#pragma once

#ifndef	WIN32

#include "SerialImpl.h"

class SerialLinux : public SerialImpl
{
public:
	SerialLinux(string portName, unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl);
	virtual ~SerialLinux(void);

private:
	int handle;
	int ConfigureParameter(void);

public:
	int OpenSerial(void);
	int CloseSerial(void);
	int WriteSerial(unsigned char *data, int size);
	int ReadSerial(unsigned char *data, int size);
	int SetParameter(unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl);
};

#endif
