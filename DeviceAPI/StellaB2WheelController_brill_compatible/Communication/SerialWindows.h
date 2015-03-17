#pragma once

#ifdef	WIN32
#include <windows.h>
#include "SerialImpl.h"

class SerialWindows : public SerialImpl
{
public:
	SerialWindows(string portName, unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl);
	virtual ~SerialWindows(void);

private:
	HANDLE handle;

public:
	int OpenSerial(void);
	int CloseSerial(void);
	int WriteSerial(unsigned char *data, int size);
	int ReadSerial(unsigned char *data, int size);
	int SetParameter(unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl);
};

#endif
