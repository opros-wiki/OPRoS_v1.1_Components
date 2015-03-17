#pragma once

#include <string>

using namespace std;

class SerialImpl
{
public:
	SerialImpl(string portName, unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl)
	{
		this->portName = portName;
		this->timeOut = timeOut;
		this->baudRate = baudRate;
		this->dataBits = dataBits;
		this->stopBits = stopBits;
		this->parity = parity;
		this->flowControl = flowControl;
	};
	virtual ~SerialImpl(void) { };

protected:
	string portName;
	unsigned long timeOut;
	unsigned long baudRate;
	char dataBits;
	char stopBits;
	char parity;
	char flowControl;

public:
	virtual int OpenSerial(void) = 0;
	virtual int CloseSerial(void) = 0;
	virtual int WriteSerial(unsigned char *data, int size) = 0;
	virtual int ReadSerial(unsigned char *data, int size) = 0;
	virtual int SetParameter(unsigned long timeOut, unsigned long baudRate, char dataBits, char stopBits, char parity, char flowControl) = 0;
};
