#include "Dynamixel.h"

#include <cassert>

typedef Dynamixel::ID ID;

Dynamixel::Dynamixel( ID id_ /*= ID(0)*/, opros::smart_ptr<Uart> uart_ /*= NULL*/ )
	: id(id_), uart(uart_)
{

}

Dynamixel::~Dynamixel()
{

}

void Dynamixel::SetUart( opros::smart_ptr<Uart>& uart_ )
{
	uart = uart_;
}

void Dynamixel::SetId( ID id_ )
{
	id = id_;
}

ID Dynamixel::GetId()
{
	return id;
}

bool Dynamixel::GetModel( unsigned short& modelNumber, unsigned char& firmwareVersion )
{
	return true;
}

bool Dynamixel::ChangeId( ID id_ )
{
	return true;
}

bool Dynamixel::SetBaudrate( unsigned int baudrate )
{
	unsigned int value = unsigned int(2000000.0 / baudrate - 1);

	assert(value & ~(0xFF));
	
	return true;
}

int Dynamixel::RegisterAdress::SizeOfRegister()
{
	switch(adress)
	{
	case MODEL_NUMBER_W:
	case CW_ANGLE_LIMIT_W:
	case CCW_ANGLE_LIMIT_W:
	case MAX_TORQUE_W:
	case GOAL_POSITION_W:
	case MOVING_SPEED_W:
	case TORQUE_LIMIT_W:
	case PRESENT_POSITION_W:
	case PRESENT_SPEED_W:
	case PRESENT_LOAD_W:
	case PUNCH_W:
		return 2;

	case VERSION_OF_FIRMWARE: 
	case ID:
	case BAUDRATE:
	case RETURN_DELAY_TIME:
	case HIGHTEST_LIMIT_TEMPERATURE:
	case LOWEST_LIMIT_VOLTAGE:
	case HIGHEST_LIMIT_VOLTAGE:
	case STATUS_RETURN_LEVEL:
	case ALARM_LED:
	case ALARM_SHUTDOWN:
	case TORQUE_EABLE:
	case LED:
	case CW_COMPLIANCE_MARGIN:
	case CCW_COMPLIANCE_MARGIN:
	case CW_COMPLIANCE_SLOPE:
	case CCW_COMPLIANCE_SLOPE:
	case PRESENT_VOLTAGE:
	case PRESENT_TEMPERATURE:
	case REGISTERED_INSTRUCTION:
	case MOVING:
	case LOCK:
		return 1;

	default:
		return 0;
	}
}


unsigned char Dynamixel::InstructionPacket::Length()
{
	return 1 + parameter.size() + 1; // instruction 크기 + prameter 크기 + checksum 크기
}

unsigned char Dynamixel::InstructionPacket::CalculateCheckSum()
{
	unsigned char checkSum = 0x00;
	const unsigned char length = Length();

	checkSum += id;
	checkSum += length;

	for (size_t i = 0; i < parameter.size(); i++)
	{
		checkSum += parameter[i];
	}
	checkSum = ~checkSum;

	return checkSum;
}

vector<unsigned char> Dynamixel::InstructionPacket::GetByte()
{
	vector<unsigned char> data;

	return data;
}

Dynamixel::InstructionPacket& Dynamixel::InstructionPacket::AddParameter( unsigned char data )
{
	parameter.push_back(data);
	return *this;
}

Dynamixel::InstructionPacket& Dynamixel::InstructionPacket::AddParameter( unsigned short data )
{
	unsigned char* temp = (unsigned char*)(&data);
	parameter.push_back(temp[0]);
	parameter.push_back(temp[1]);
	return *this;
}

Dynamixel::InstructionPacket& Dynamixel::InstructionPacket::AddParameter( unsigned char* data, int size )
{
	for (int i = 0; i < size; i++)
	{
		parameter.push_back(data[i]);
	}

	return *this;
}

Dynamixel::InstructionPacket& Dynamixel::InstructionPacket::AddParaemter( InstructionPacket& instruction )
{
	assert(registerAdress != instruction.registerAdress);

	const vector<unsigned char>& instructionParameter = instruction.parameter;

	for (size_t i = 0; i < instructionParameter.size(); i++)
	{
		parameter.push_back(instructionParameter[i]);
	}

	return *this;
}


