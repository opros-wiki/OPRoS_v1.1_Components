#include <assert.h>
#include <string.h>

#include "DynamixelUART.h"
#include "DynamixelUARTDef.h"

#include "device/OprosPrintMessage.h"
#include "device/OprosTimer.h"

#define RETRY_COUNT				2
#define TRANSACTION_DELAY		3
#define TRANSACTION_TIMEOUT		100

#define	MAX_PACKET_SIZE			256
#define MAX_SYNCPACKET_SIZE		143

DynamixelUART::DynamixelUART(Uart *uart, unsigned char id)
{
	this->uart = uart;
	this->id = id;
}

DynamixelUART::~DynamixelUART(void)
{
	uart = NULL;
	id = 0xFF;
}

unsigned char DynamixelUART::Checksum(unsigned char *buf, int dataSize)
{
	unsigned char result = 0;
	for(int i = 0; i < dataSize; ++i) {
		result += *(buf + i);
	}
	return ~result;
}


void DynamixelUART::DumpPacket (unsigned char *packet, int packetSize)
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

int DynamixelUART::SendPacket (unsigned char *data, int dataSize)
{
	int size = uart->Write (data, dataSize);
	return size;
}

int DynamixelUART::ReceivePacket (unsigned char *data, int dataSize)
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

bool DynamixelUART::Transaction (unsigned char *sendPacket, int sendPacketSize)
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

bool DynamixelUART::Transaction (unsigned char *sendPacket, int sendPacketSize, unsigned char *receivePacket, int receivePacketSize)
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

bool DynamixelUART::Ping ()
{
	unsigned char sendPacket[6] = { 0xFF, 0xFF, id, 2, PING, 0 };
	sendPacket[5] = Checksum (&sendPacket[2], 3);
	unsigned char recvPacket[6];

	return Transaction(sendPacket, 6, recvPacket, 6);
}

bool DynamixelUART::ResetMemory ()
{
	//!!! 주의: ID를 1로 재설정한다. 
	unsigned char sendPacket[6] = { 0xFF, 0xFF, id, 2, RESET, 0 };
	sendPacket[5] = Checksum (&sendPacket[2], 3);
	unsigned char recvPacket[6];

	return Transaction(sendPacket, 6, recvPacket, 6);
}

bool DynamixelUART::WriteByte(unsigned char index, unsigned char data)
{
	unsigned char sendPacket[8] = { 0xFF, 0xFF, id, 4, WRITE_DATA, index, data };
	sendPacket[7] = Checksum (&sendPacket[2], 5);

	return Transaction(sendPacket, 8);
}

bool DynamixelUART::WriteBytes (unsigned char index, unsigned char *data, int dataSize, int len, bool sync_flag)
{
	assert (dataSize + 7 <= MAX_PACKET_SIZE);

	unsigned char sendPacket[MAX_PACKET_SIZE] = { 0xFF, 0xFF, id, dataSize + 4, (sync_flag)? SYNC_WRITE : WRITE_DATA, index , len};
	memcpy (&sendPacket[7], data, dataSize);
	sendPacket[dataSize + 7] = Checksum (&sendPacket[2], dataSize + 5);

	DumpPacket (sendPacket, dataSize+8);

	return Transaction (sendPacket, dataSize + 8);
}

bool DynamixelUART::WriteWord (unsigned char index, unsigned short data)
{
	unsigned char sendPacket[9] = { 0xFF, 0xFF, id, 5, WRITE_DATA, index, (unsigned char)(data >> 0), (unsigned char)(data >> 8), 0 };
	sendPacket[8] = Checksum (&sendPacket[2], 6);

	return Transaction(sendPacket, 9);
}

bool DynamixelUART::WriteReg (unsigned char index, unsigned short data)
{
	unsigned char sendPacket[9] = { 0xFF, 0xFF, id, 5, REG_WRITE, index, (unsigned char)(data >> 0), (unsigned char)(data >> 8), 0 };
	sendPacket[8] = Checksum (&sendPacket[2], 6);

	return Transaction(sendPacket, 9);
}

bool DynamixelUART::ReadByte (unsigned char index, unsigned char *data)
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

bool DynamixelUART::ReadBytes (unsigned char index, unsigned char *data, int dataSize)
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

bool DynamixelUART::ReadWord (unsigned char index, unsigned short *data)
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

bool DynamixelUART::GetModelNumber(unsigned short &modelNumber)
{
	return ReadWord(MODEL_NUMBER_W, &modelNumber);
}

bool DynamixelUART::GetFirmwareVersion(unsigned char &firmwareVersion)
{
	return ReadByte(VERSION_OF_FIRMWARE, &firmwareVersion);
}

bool DynamixelUART::SetID(unsigned char id)
{
	return WriteByte(ID, id);
}

bool DynamixelUART::GetID(unsigned char &id)
{
	return ReadByte(ID, &id);
}

bool DynamixelUART::SetBaudRate(unsigned char baudRate)
{
	return WriteByte(BAUDRATE, baudRate);
}

bool DynamixelUART::GetBaudRate(unsigned char &baudRate)
{
	return ReadByte(BAUDRATE, &baudRate);
}

bool DynamixelUART::SetReturnDelayTime(unsigned char returnDelayTime)
{
	return WriteByte(RETURN_DELAY_TIME, returnDelayTime);
}

bool DynamixelUART::GetreturnDelayTime(unsigned char &returnDelayTime)
{
	return ReadByte(RETURN_DELAY_TIME, &returnDelayTime);
}

bool DynamixelUART::SetCWAngleLimit(unsigned short cwAngleLimit)
{
	return WriteWord(CW_ANGLE_LIMIT_W, cwAngleLimit);
}

bool DynamixelUART::GetCWAngleLimit(unsigned short &cwAngleLimit)
{
	return ReadWord(CW_ANGLE_LIMIT_W, &cwAngleLimit);
}

bool DynamixelUART::SetCCWAngleLimit(unsigned short ccwAngleLimit)
{
	return WriteWord(CCW_ANGLE_LIMIT_W, ccwAngleLimit);
}

bool DynamixelUART::GetCCWAngleLimit(unsigned short &ccwAngleLimit)
{
	return ReadWord(CCW_ANGLE_LIMIT_W, &ccwAngleLimit);
}

bool DynamixelUART::SetTemperatureLimit(unsigned char temperatureLimit)
{
	return WriteByte(HIGHTEST_LIMIT_TEMPERATURE, temperatureLimit);
}

bool DynamixelUART::GetTemperatureLimit(unsigned char &temperatureLimit)
{
	return ReadByte(HIGHTEST_LIMIT_TEMPERATURE, &temperatureLimit);
}

bool DynamixelUART::SetLowestVoltageLimit(unsigned char lowestVoltageLimit)
{
	return WriteByte(LOWEST_LIMIT_VOLTAGE, lowestVoltageLimit);
}

bool DynamixelUART::GetLowestVoltageLimit(unsigned char &lowestVoltageLimit)
{
	return ReadByte(LOWEST_LIMIT_VOLTAGE, &lowestVoltageLimit);
}

bool DynamixelUART::SetHighestVoltageLimit(unsigned char highestVoltageLimit)
{
	return WriteByte(HIGHEST_LIMIT_VOLTAGE, highestVoltageLimit);
}

bool DynamixelUART::GetHighestVoltageLimit(unsigned char &highestVoltageLimit)
{
	return ReadByte(HIGHEST_LIMIT_VOLTAGE, &highestVoltageLimit);
}

bool DynamixelUART::SetMaximumTorque(unsigned short maximumTorque)
{
	return WriteWord(MAX_TORQUE_W, maximumTorque);
}

bool DynamixelUART::GetMaximumTorque(unsigned short &maximumTorque)
{
	return ReadWord(MAX_TORQUE_W, &maximumTorque);
}

bool DynamixelUART::SetStatusReturnLevel(unsigned char statusReturnLevel)
{
	return WriteByte(STATUS_RETURN_VALUE, statusReturnLevel);
}

bool DynamixelUART::GetStatusReturnLevel(unsigned char &statusReturnLevel)
{
	return ReadByte(STATUS_RETURN_VALUE, &statusReturnLevel);
}

bool DynamixelUART::SetAlarmLED(unsigned char alarmLed)
{
	return WriteByte(ALARM_LED, alarmLed);
}

bool DynamixelUART::GetAlarmLED(unsigned char &alarmLed)
{
	return ReadByte(ALARM_LED, &alarmLed);
}

bool DynamixelUART::SetAlarmShutdown( unsigned char alarmShutdown )
{
	return WriteByte(ALARM_SHUTDOWN, alarmShutdown);
}

bool DynamixelUART::GetAlarmShutdown( unsigned char &alarmShutdown )
{
	return ReadByte(ALARM_SHUTDOWN, &alarmShutdown);
}

bool DynamixelUART::SetTorqueEnable(unsigned char torqueEnable)
{
	return WriteByte(TORQUE_EABLE, torqueEnable);
}

bool DynamixelUART::GetTorqueEnable(unsigned char &torqueEnable)
{
	return ReadByte(TORQUE_EABLE, &torqueEnable);
}

bool DynamixelUART::SetLED(unsigned char led)
{
	return WriteByte(LED, led);
}

bool DynamixelUART::GetLED(unsigned char &led)
{
	return ReadByte(LED, &led);
}

bool DynamixelUART::SetCWComplianceMargin(unsigned char cwComplianceMargin)
{
	return WriteByte(CW_COMPLIANCE_MARGIN, cwComplianceMargin);
}

bool DynamixelUART::GetCWComplianceMargin(unsigned char &cwComplianceMargin)
{
	return ReadByte(CW_COMPLIANCE_MARGIN, &cwComplianceMargin);
}

bool DynamixelUART::SetCCWComplianceMargin(unsigned char ccwComplianceMargin)
{
	return WriteByte(CCW_COMPLIANCE_MARGIN, ccwComplianceMargin);
}

bool DynamixelUART::GetCCWComplianceMargin(unsigned char &ccwComplianceMargin)
{
	return ReadByte(CCW_COMPLIANCE_MARGIN, &ccwComplianceMargin);
}

bool DynamixelUART::SetCWComplianceSlope(unsigned char cwComplianceSlope)
{
	return WriteByte(CW_COMPLICANCE_SLOPE, cwComplianceSlope);
}

bool DynamixelUART::GetCWComplianceSlope(unsigned char &cwComplianceSlope)
{
	return ReadByte(CW_COMPLICANCE_SLOPE, &cwComplianceSlope);
}

bool DynamixelUART::SetCCWComplianceSlope(unsigned char ccwComplianceSlope)
{
	return WriteByte(CCW_COMPLICANCE_SLOPE, ccwComplianceSlope);
}

bool DynamixelUART::GetCCWComplianceSlope(unsigned char &ccwComplianceSlope)
{
	return ReadByte(CCW_COMPLICANCE_SLOPE, &ccwComplianceSlope);
}

bool DynamixelUART::SetGoalPosition(unsigned short goalPosition)
{
	return WriteWord(GOAL_POSITION_W, goalPosition);
}

bool DynamixelUART::GetGoalPosition(unsigned short &goalPosition)
{
	return ReadWord(GOAL_POSITION_W, &goalPosition);
}

bool DynamixelUART::SetMovingSpeed(unsigned short movingSpeed)
{
	return WriteWord(MOVING_SPEED_W, movingSpeed);
}

bool DynamixelUART::GetMovingSpeed(unsigned short &movingSpeed)
{
	return ReadWord(MOVING_SPEED_W, &movingSpeed);
}

bool DynamixelUART::SetTorqueLimit(unsigned short torqueLimit)
{
	return WriteWord(TORQUE_LIMIT_W, torqueLimit);
}

bool DynamixelUART::GetTorqueLimit(unsigned short &torqueLimit)
{
	return ReadWord(TORQUE_LIMIT_W, &torqueLimit);
}

bool DynamixelUART::GetPresentPosition(unsigned short &presentPosition)
{
	return ReadWord(PRESENT_POSITION_W, &presentPosition);
}

bool DynamixelUART::GetPresentSpeed(unsigned short &presentSpeed)
{
	return ReadWord(PRESENT_SPEED_W, &presentSpeed);
}

bool DynamixelUART::GetPresentLoad(unsigned short &presentLoad)
{
	return ReadWord(PRESENT_LOAD_W, &presentLoad);
}

bool DynamixelUART::GetPresentVoltage(unsigned char &presentVoltage)
{
	return ReadByte(PRESENT_VOLTAGE, &presentVoltage);
}

bool DynamixelUART::GetPresentTemperature(unsigned char &presentTemperature)
{
	return ReadByte(PRESETN_TEMPERATURE, &presentTemperature);
}

bool DynamixelUART::GetRegistered(unsigned char &registered)
{
	return ReadByte(REGISTERED_INSTRUCTION, &registered);;
}

bool DynamixelUART::GetMoving(unsigned char &moving)
{
	return ReadByte(MOVING, &moving);
}

bool DynamixelUART::SetEEPROMLock(unsigned char eepromLock)
{
	return WriteByte(LOCK, eepromLock);
}

bool DynamixelUART::GetEEPROMLock(unsigned char &eepromLock)
{
	return ReadByte(LOCK, &eepromLock);
}

bool DynamixelUART::SetPunch(unsigned short punch)
{
	return WriteWord(PUNCH_W, punch);
}

bool DynamixelUART::GetPunch(unsigned short &punch)
{
	return ReadWord(PUNCH_W, &punch);
}

