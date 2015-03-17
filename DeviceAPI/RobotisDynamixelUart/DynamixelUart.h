#pragma once

#include "Uart.h"

class DynamixelUart
{
public:
	DynamixelUart(Uart *uart, unsigned char id);
	virtual ~DynamixelUart(void);

private:
	Uart *uart;
	unsigned char id;
	unsigned char Checksum(unsigned char *buf, int dataSize);

	int SendPacket (unsigned char *data, int dataSize);
	int ReceivePacket (unsigned char *data, int dataSize);
	void DumpPacket (unsigned char *packet, int packetSize);
	bool Transaction (unsigned char *sendPacket, int sendPacketSize);
	bool Transaction (unsigned char *sendPacket, int sendPacketSize, unsigned char *receivePacket, int receivePacketSize);

protected:
	bool Ping ();
	bool ResetMemory();
	bool WriteByte (unsigned char index, unsigned char data);
	bool WriteReg (unsigned char index, unsigned short data);
	bool WriteBytes (unsigned char index, unsigned char *data, int dataSize, int len = 1, bool sync_flag = false);
	bool WriteWord (unsigned char index, unsigned short data);
	bool ReadByte (unsigned char index, unsigned char *data);
	bool ReadBytes (unsigned char index, unsigned char *data, int dataSize);
	bool ReadWord (unsigned char index, unsigned short *data);

public:
	bool GetModelNumber(unsigned short &modelNumber);
	bool GetFirmwareVersion(unsigned char &firmwareVersion);
	bool SetID(unsigned char id);
	bool GetID(unsigned char &id);
	bool SetBaudRate(unsigned char baudRate);
	bool GetBaudRate(unsigned char &baudRate);
	bool SetReturnDelayTime(unsigned char returnDelayTime);
	bool GetreturnDelayTime(unsigned char &returnDelayTime);
	bool SetCWAngleLimit(unsigned short cwAngleLimit);
	bool GetCWAngleLimit(unsigned short &cwAngleLimit);
	bool SetCCWAngleLimit(unsigned short ccwAngleLimit);
	bool GetCCWAngleLimit(unsigned short &ccwAngleLimit);
	bool SetTemperatureLimit(unsigned char temperatureLimit);
	bool GetTemperatureLimit(unsigned char &temperatureLimit);
	bool SetLowestVoltageLimit(unsigned char lowestVoltageLimit);
	bool GetLowestVoltageLimit(unsigned char &lowestVoltageLimit);
	bool SetHighestVoltageLimit(unsigned char highestVoltageLimit);
	bool GetHighestVoltageLimit(unsigned char &highestVoltageLimit);
	bool SetMaximumTorque(unsigned short maximumTorque);
	bool GetMaximumTorque(unsigned short &maximumTorque);
	bool SetStatusReturnLevel(unsigned char statusReturnLevel);
	bool GetStatusReturnLevel(unsigned char &statusReturnLevel);
	bool SetAlarmLED(unsigned char alarmLed);
	bool GetAlarmLED(unsigned char &alarmLed);
	bool SetTorqueEnable(unsigned char torqueEnable);
	bool GetTorqueEnable(unsigned char &torqueEnable);
	bool SetLED(unsigned char led);
	bool GetLED(unsigned char &led);
	bool SetCWComplianceMargin(unsigned char cwComplianceMargin);
	bool GetCWComplianceMargin(unsigned char &cwComplianceMargin);
	bool SetCCWComplianceMargin(unsigned char ccwComplianceMargin);
	bool GetCCWComplianceMargin(unsigned char &ccwComplianceMargin);
	bool SetCWComplianceSlope(unsigned char cwComplianceSlope);
	bool GetCWComplianceSlope(unsigned char &cwComplianceSlope);
	bool SetCCWComplianceSlope(unsigned char ccwComplianceSlope);
	bool GetCCWComplianceSlope(unsigned char &ccwComplianceSlope);
	bool SetGoalPosition(unsigned short goalPosition);
	bool GetGoalPosition(unsigned short &goalPosition);
	bool SetMovingSpeed(unsigned short movingSpeed);
	bool GetMovingSpeed(unsigned short &movingSpeed);
	bool SetTorqueLimit(unsigned short torqueLimit);
	bool GetTorqueLimit(unsigned short &torqueLimit);
	bool GetPresentPosition(unsigned short &presentPosition);
	bool GetPresentSpeed(unsigned short &presentSpeed);
	bool GetPresentLoad(unsigned short &presentLoad);
	bool GetPresentVoltage(unsigned char &presentVoltage);
	bool GetPresentTemperature(unsigned char &presentTemperature);
	bool GetRegistered(unsigned char &registered);
	bool GetMoving(unsigned char &moving);
	bool SetEEPROMLock(unsigned char eepromLock);
	bool GetEEPROMLock(unsigned char &eepromLock);
	bool SetPunch(unsigned short punch);
	bool GetPunch(unsigned short &punch);
};
