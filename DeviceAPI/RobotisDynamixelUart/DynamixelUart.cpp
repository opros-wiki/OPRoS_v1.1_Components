#include <assert.h>
#include <string.h>

#include "DynamixelUart.h"
#include "DynamixelUARTDef.h"

#include "OprosPrintMessage.h"
#include "OprosTimer.h"

#define RETRY_COUNT				2
#define TRANSACTION_DELAY		3
#define TRANSACTION_TIMEOUT		6

#define	MAX_PACKET_SIZE			256
#define MAX_SYNCPACKET_SIZE		143

DynamixelUart::DynamixelUart(Uart *uart, unsigned char id)
{
	this->uart = uart;
	this->id = id;
}

DynamixelUart::~DynamixelUart(void)
{
	uart = NULL;
	id = 0xFF;
}

unsigned char DynamixelUart::Checksum(unsigned char *buf, int dataSize)
{
	unsigned char result = 0;
	for(int i = 0; i < dataSize; ++i) {
		result += *(buf + i);
	}
	return ~result;
}


void DynamixelUart::DumpPacket (unsigned char *packet, int packetSize)
{
	assert (packetSize < MAX_PACKET_SIZE);

	char text[MAX_PACKET_SIZE*3+1];

	for (int i=0, n=0; i<packetSize; ++i) {
#if defined(WIN32)
		n += _snprintf_s (text+n, sizeof(text)-n, sizeof(text)-n-1, "%02X ", (unsigned int)packet[i]);
#else
		n += snprintf(text+n, sizeof(text)-n-1, "%02X ", (unsigned int)packet[i]);
#endif
	}
}

int DynamixelUart::SendPacket (unsigned char *data, int dataSize)
{
	return uart->Write (data, dataSize);
}

int DynamixelUart::ReceivePacket (unsigned char *data, int dataSize)
{
	assert (4 < dataSize);

	int n;
	int readDataSize = 0;
	OprosTimer oprosTimer;

	oprosTimer.StartTimeTick();

	while (readDataSize < 4) {
		n = uart->Read (data + readDataSize, dataSize - readDataSize);
		if (0 < n) {
			readDataSize += n;
		}
		else if (n < 0) {
			return -1;
		}
		if (oprosTimer.GetTimeTick() > TRANSACTION_TIMEOUT) {
			break;
		}
	}
	if (4 <= readDataSize && data[0] == (unsigned char)0xFF && data[1] == (unsigned char)0xFF) {
		int packetSize = 4 + data[3];
		if (dataSize < packetSize) {
			return -1;
		}

		while (readDataSize < packetSize) {
			n = uart->Read (data + readDataSize, packetSize - readDataSize);
			if (0 < n) {
				readDataSize += n;
			}
			else if (n < 0) {
				return -1;
			}
			if (oprosTimer.GetTimeTick() > TRANSACTION_TIMEOUT) {
				break;
			}
		}
		if (packetSize == readDataSize) {
			unsigned char cs = Checksum (&data[2], packetSize - 3);
			if (data[packetSize - 1] == cs) {
				return packetSize;
			}
		}
	}
	
	return -1;
}

bool DynamixelUart::Transaction (unsigned char *sendPacket, int sendPacketSize)
{
	int ret = -1;
	for (int i=0; i<RETRY_COUNT; ++i) {
		ret = SendPacket (sendPacket, sendPacketSize);
		if(0 <= ret) {
			OprosSleep(TRANSACTION_DELAY);

			return true;
		}
	}
	return false;
}

bool DynamixelUart::Transaction (unsigned char *sendPacket, int sendPacketSize, unsigned char *receivePacket, int receivePacketSize)
{
	unsigned char _deviceStatus;

	int ret;
	for (int i=0; i<RETRY_COUNT; ++i) {
		ret = SendPacket (sendPacket, sendPacketSize);
		if(0 <= ret) {
			OprosSleep(TRANSACTION_DELAY);

			ret = ReceivePacket (receivePacket, receivePacketSize);
			if (0 <= ret) {
				_deviceStatus = receivePacket[4];
				return true;
			}
		}
	}
	return false;
}

bool DynamixelUart::Ping ()
{
	unsigned char sendPacket[6] = { 0xFF, 0xFF, id, 2, PING, 0 };
	sendPacket[5] = Checksum (&sendPacket[2], 3);
	unsigned char recvPacket[6];

	return Transaction(sendPacket, 6, recvPacket, 6);
}

bool DynamixelUart::ResetMemory ()
{
	//!!! 주의: ID를 1로 재설정한다. 
	unsigned char sendPacket[6] = { 0xFF, 0xFF, id, 2, RESET, 0 };
	sendPacket[5] = Checksum (&sendPacket[2], 3);
	unsigned char recvPacket[6];

	return Transaction(sendPacket, 6, recvPacket, 6);
}

bool DynamixelUart::WriteByte(unsigned char index, unsigned char data)
{
	unsigned char sendPacket[8] = { 0xFF, 0xFF, id, 4, WRITE_DATA, index, data };
	sendPacket[7] = Checksum (&sendPacket[2], 5);

	return Transaction(sendPacket, 8);
}

bool DynamixelUart::WriteBytes (unsigned char index, unsigned char *data, int dataSize, int len, bool sync_flag)
{
	assert (dataSize + 7 <= MAX_PACKET_SIZE);

	unsigned char sendPacket[MAX_PACKET_SIZE] = { 0xFF, 0xFF, id, dataSize + 4, (sync_flag)? SYNC_WRITE : WRITE_DATA, index , len};
	memcpy (&sendPacket[7], data, dataSize);
	sendPacket[dataSize + 7] = Checksum (&sendPacket[2], dataSize + 5);

	DumpPacket (sendPacket, dataSize+8);

	return Transaction (sendPacket, dataSize + 8);
}

bool DynamixelUart::WriteWord (unsigned char index, unsigned short data)
{
	unsigned char sendPacket[9] = { 0xFF, 0xFF, id, 5, WRITE_DATA, index, (unsigned char)(data >> 0), (unsigned char)(data >> 8), 0 };
	sendPacket[8] = Checksum (&sendPacket[2], 6);

	return Transaction(sendPacket, 9);
}

bool DynamixelUart::WriteReg (unsigned char index, unsigned short data)
{
	unsigned char sendPacket[9] = { 0xFF, 0xFF, id, 5, REG_WRITE, index, (unsigned char)(data >> 0), (unsigned char)(data >> 8), 0 };
	sendPacket[8] = Checksum (&sendPacket[2], 6);

	return Transaction(sendPacket, 9);
}

bool DynamixelUart::ReadByte (unsigned char index, unsigned char *data)
{
	unsigned char sendPacket[8] = { 0xFF, 0xFF, id, 4, READ_DATA, index, 1, 0 };
	sendPacket[7] = Checksum (&sendPacket[2], 5);
	unsigned char recvPacket[7];

	if (Transaction (sendPacket, 8, recvPacket, 7)) {
		*data = *(unsigned char *)&recvPacket[5];
		return true;
	}
	return false;
}

bool DynamixelUart::ReadBytes (unsigned char index, unsigned char *data, int dataSize)
{
	assert (dataSize + 6 <= MAX_PACKET_SIZE);

	unsigned char sendPacket[8] = { 0xFF, 0xFF, id, 4, READ_DATA, index, dataSize, 0 };
	sendPacket[7] = Checksum (&sendPacket[2], 5);
	unsigned char recvPacket[MAX_PACKET_SIZE];

	if (Transaction (sendPacket, 8, recvPacket, 6+dataSize)) {
		memcpy (data, &recvPacket[5], dataSize);
		return true;
	}
	return false;
}

bool DynamixelUart::ReadWord (unsigned char index, unsigned short *data)
{
	unsigned char sendPacket[8] = { 0xFF, 0xFF, id, 4, READ_DATA, index, 2, 0 };
	sendPacket[7] = Checksum (&sendPacket[2], 5);
	unsigned char recvPacket[8];

	if (Transaction (sendPacket, 8, recvPacket, 8)) {
		*data = *(unsigned short *)&recvPacket[5];
		return true;
	}
	return false;
}

bool DynamixelUart::GetModelNumber(unsigned short &modelNumber)
{
	return ReadWord(MODEL_NUMBER_W, &modelNumber);
}

bool DynamixelUart::GetFirmwareVersion(unsigned char &firmwareVersion)
{
	return ReadByte(VERSION_OF_FIRMWARE, &firmwareVersion);
}

bool DynamixelUart::SetID(unsigned char id)
{
	return WriteByte(ID, id);
}

bool DynamixelUart::GetID(unsigned char &id)
{
	return ReadByte(ID, &id);
}

bool DynamixelUart::SetBaudRate(unsigned char baudRate)
{
	return WriteByte(BAUDRATE, baudRate);
}

bool DynamixelUart::GetBaudRate(unsigned char &baudRate)
{
	return ReadByte(BAUDRATE, &baudRate);
}

bool DynamixelUart::SetReturnDelayTime(unsigned char returnDelayTime)
{
	return WriteByte(RETURN_DELAY_TIME, returnDelayTime);
}

bool DynamixelUart::GetreturnDelayTime(unsigned char &returnDelayTime)
{
	return ReadByte(RETURN_DELAY_TIME, &returnDelayTime);
}

bool DynamixelUart::SetCWAngleLimit(unsigned short cwAngleLimit)
{
	return WriteWord(CW_ANGLE_LIMIT_W, cwAngleLimit);
}

bool DynamixelUart::GetCWAngleLimit(unsigned short &cwAngleLimit)
{
	return ReadWord(CW_ANGLE_LIMIT_W, &cwAngleLimit);
}

bool DynamixelUart::SetCCWAngleLimit(unsigned short ccwAngleLimit)
{
	return WriteWord(CCW_ANGLE_LIMIT_W, ccwAngleLimit);
}

bool DynamixelUart::GetCCWAngleLimit(unsigned short &ccwAngleLimit)
{
	return ReadWord(CCW_ANGLE_LIMIT_W, &ccwAngleLimit);
}

bool DynamixelUart::SetTemperatureLimit(unsigned char temperatureLimit)
{
	return WriteByte(HIGHTEST_LIMIT_TEMPERATURE, temperatureLimit);
}

bool DynamixelUart::GetTemperatureLimit(unsigned char &temperatureLimit)
{
	return ReadByte(HIGHTEST_LIMIT_TEMPERATURE, &temperatureLimit);
}

bool DynamixelUart::SetLowestVoltageLimit(unsigned char lowestVoltageLimit)
{
	return WriteByte(LOWEST_LIMIT_VOLTAGE, lowestVoltageLimit);
}

bool DynamixelUart::GetLowestVoltageLimit(unsigned char &lowestVoltageLimit)
{
	return ReadByte(LOWEST_LIMIT_VOLTAGE, &lowestVoltageLimit);
}

bool DynamixelUart::SetHighestVoltageLimit(unsigned char highestVoltageLimit)
{
	return WriteByte(HIGHEST_LIMIT_VOLTAGE, highestVoltageLimit);
}

bool DynamixelUart::GetHighestVoltageLimit(unsigned char &highestVoltageLimit)
{
	return ReadByte(HIGHEST_LIMIT_VOLTAGE, &highestVoltageLimit);
}

bool DynamixelUart::SetMaximumTorque(unsigned short maximumTorque)
{
	return WriteWord(MAX_TORQUE_W, maximumTorque);
}

bool DynamixelUart::GetMaximumTorque(unsigned short &maximumTorque)
{
	return ReadWord(MAX_TORQUE_W, &maximumTorque);
}

bool DynamixelUart::SetStatusReturnLevel(unsigned char statusReturnLevel)
{
	return WriteByte(STATUS_RETURN_VALUE, statusReturnLevel);
}

bool DynamixelUart::GetStatusReturnLevel(unsigned char &statusReturnLevel)
{
	return ReadByte(STATUS_RETURN_VALUE, &statusReturnLevel);
}

bool DynamixelUart::SetAlarmLED(unsigned char alarmLed)
{
	return WriteByte(ALARM_LED, alarmLed);
}

bool DynamixelUart::GetAlarmLED(unsigned char &alarmLed)
{
	return ReadByte(ALARM_LED, &alarmLed);
}

bool DynamixelUart::SetTorqueEnable(unsigned char torqueEnable)
{
	return WriteByte(TORQUE_EABLE, torqueEnable);
}

bool DynamixelUart::GetTorqueEnable(unsigned char &torqueEnable)
{
	return ReadByte(TORQUE_EABLE, &torqueEnable);
}

bool DynamixelUart::SetLED(unsigned char led)
{
	return WriteByte(LED, led);
}

bool DynamixelUart::GetLED(unsigned char &led)
{
	return ReadByte(LED, &led);
}

bool DynamixelUart::SetCWComplianceMargin(unsigned char cwComplianceMargin)
{
	return WriteByte(CW_COMPLIANCE_MARGIN, cwComplianceMargin);
}

bool DynamixelUart::GetCWComplianceMargin(unsigned char &cwComplianceMargin)
{
	return ReadByte(CW_COMPLIANCE_MARGIN, &cwComplianceMargin);
}

bool DynamixelUart::SetCCWComplianceMargin(unsigned char ccwComplianceMargin)
{
	return WriteByte(CCW_COMPLIANCE_MARGIN, ccwComplianceMargin);
}

bool DynamixelUart::GetCCWComplianceMargin(unsigned char &ccwComplianceMargin)
{
	return ReadByte(CCW_COMPLIANCE_MARGIN, &ccwComplianceMargin);
}

bool DynamixelUart::SetCWComplianceSlope(unsigned char cwComplianceSlope)
{
	return WriteByte(CW_COMPLICANCE_SLOPE, cwComplianceSlope);
}

bool DynamixelUart::GetCWComplianceSlope(unsigned char &cwComplianceSlope)
{
	return ReadByte(CW_COMPLICANCE_SLOPE, &cwComplianceSlope);
}

bool DynamixelUart::SetCCWComplianceSlope(unsigned char ccwComplianceSlope)
{
	return WriteByte(CCW_COMPLICANCE_SLOPE, ccwComplianceSlope);
}

bool DynamixelUart::GetCCWComplianceSlope(unsigned char &ccwComplianceSlope)
{
	return ReadByte(CCW_COMPLICANCE_SLOPE, &ccwComplianceSlope);
}

bool DynamixelUart::SetGoalPosition(unsigned short goalPosition)
{
	return WriteWord(GOAL_POSITION_W, goalPosition);
}

bool DynamixelUart::GetGoalPosition(unsigned short &goalPosition)
{
	return ReadWord(GOAL_POSITION_W, &goalPosition);
}

bool DynamixelUart::SetMovingSpeed(unsigned short movingSpeed)
{
	return WriteWord(MOVING_SPEED_W, movingSpeed);
}

bool DynamixelUart::GetMovingSpeed(unsigned short &movingSpeed)
{
	return ReadWord(MOVING_SPEED_W, &movingSpeed);
}

bool DynamixelUart::SetTorqueLimit(unsigned short torqueLimit)
{
	return WriteWord(TORQUE_LIMIT_W, torqueLimit);
}

bool DynamixelUart::GetTorqueLimit(unsigned short &torqueLimit)
{
	return ReadWord(TORQUE_LIMIT_W, &torqueLimit);
}

bool DynamixelUart::GetPresentPosition(unsigned short &presentPosition)
{
	return ReadWord(PRESENT_POSITION_W, &presentPosition);
}

bool DynamixelUart::GetPresentSpeed(unsigned short &presentSpeed)
{
	return ReadWord(PRESENT_SPEED_W, &presentSpeed);
}

bool DynamixelUart::GetPresentLoad(unsigned short &presentLoad)
{
	return ReadWord(PRESENT_LOAD_W, &presentLoad);
}

bool DynamixelUart::GetPresentVoltage(unsigned char &presentVoltage)
{
	return ReadByte(PRESENT_VOLTAGE, &presentVoltage);
}

bool DynamixelUart::GetPresentTemperature(unsigned char &presentTemperature)
{
	return ReadByte(PRESETN_TEMPERATURE, &presentTemperature);
}

bool DynamixelUart::GetRegistered(unsigned char &registered)
{
	return ReadByte(REGISTERED_INSTRUCTION, &registered);;
}

bool DynamixelUart::GetMoving(unsigned char &moving)
{
	return ReadByte(MOVING, &moving);
}

bool DynamixelUart::SetEEPROMLock(unsigned char eepromLock)
{
	return WriteByte(LOCK, eepromLock);
}

bool DynamixelUart::GetEEPROMLock(unsigned char &eepromLock)
{
	return ReadByte(LOCK, &eepromLock);
}

bool DynamixelUart::SetPunch(unsigned short punch)
{
	return WriteWord(PUNCH_W, punch);
}

bool DynamixelUart::GetPunch(unsigned short &punch)
{
	return ReadWord(PUNCH_W, &punch);
}
