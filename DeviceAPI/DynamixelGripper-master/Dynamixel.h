#ifndef __DYNAMIXEL_H__
#define __DYNAMIXEL_H__

#include <device/ServoActuator.h>
#include <device/Uart.h>
#include <opros_smart_ptr.h>

class Dynamixel
{
public:
	class ID
	{
	public:
		enum Type
		{
			ID_BROADCAST = 0xFE	//연결된 모든 모터가 명령패킷을 수행하며 Status packet은 리턴되지 않는다.
		};

	public:
		explicit ID(unsigned char id_)
		{
			id = static_cast<Type>(id_);
		}

		explicit ID(Type id_)
		{
			id = id_;
		}
	
	public:
		inline Type value()
		{
			return id;
		}

		inline operator Type()
		{
			return id;
		}

		inline operator unsigned char()
		{
			return static_cast<unsigned char>(id);
		}

	private:
		Type id;
	};

public:
	class Error
	{
	public:
		enum Type
		{
			NONE = 0x00,
			INSTRUCTION_ERROR = 0x40,	//정의되지 않은 Instruction이 전송된 경우. 또는 reg_write 명령없이 action명령이 전달된 경우 1로 설정됨
			OVERLOAD_ERROR = 0x20,		//지정된 최대 Torque로 현재의 하중을 제어할 수 없을 때 1로 설정됨
			CHECKSUM_ERROR = 0x10,		//전송된 Instruction Packet의 Checksum이 맞지 않을 때1로 설정됨
			RANGE_ERROR = 0x08,			//사용범위를 벗아난 명령일 경우 1로 설정됨.
			OVERHEATING_ERROR = 0x04,	//Dynamixel 내부 온도가 Control Table에 설정된 동작 온도범위를 벗어났을 때 1로 설정됨
			ANGLELIMIT_ERROR = 0x02,	//Goal Position이 CW Angle Limit ~ CCW Angle Limit 범위밖의 값으로 Writing 되었을때 1로 설정됨
			INPUTVOLTAGE_ERROR = 0x01,	//인가된 전압이 Control Table에 설정된 동작 전압 범위를벗어났을 경우 1로 설정됨
			ALL_ERROR = (INSTRUCTION_ERROR|OVERLOAD_ERROR | CHECKSUM_ERROR | RANGE_ERROR | OVERHEATING_ERROR|ANGLELIMIT_ERROR |	INPUTVOLTAGE_ERROR)	//모든 에러 
		};

	public:
		explicit Error(Type error_ = NONE)
		{
			error = error_;
		}

	public:
		inline Type value()
		{
			return error;
		}

		inline operator Type()
		{
			return error;
		}

		inline operator unsigned char()
		{
			return static_cast<unsigned char>(error);
		}

	private:
		Type error;
	};

	class Instruction 
	{
	public:
		enum Type
		{
			PING = 0x01,		//수행내용 없음. 제어기가 Status Packet을 받고자 할 경우 사용
			READ_DATA = 0x02,	//DATA를 읽는 명령
			WRITE_DATA = 0x03,	//DATA를 쓰는 명령
			REG_WRITE = 0x04,	//WRITE DATA와 내용은 유사하나, 대기상태로 있다가 ACTION명령이 도착해야 수행됨
			ACTION = 0x05,		//REG WRITE로 등록된 동작을 시작하라는 명령
			RESET = 0x06,		//상태를 공장 출하 상태로 복귀시키는 명령
			SYNC_WRITE = 0x83	//한번에 여러개의 모터들을 동시에 제어하고자 할 때 사용되는 명령
		};

	public:
		explicit Instruction(Type instruction_ = WRITE_DATA)
		{
			instruction = instruction_;
		}

	public:
		inline Type value()
		{
			return instruction;
		}

		inline operator Type()
		{
			return instruction;
		}

		inline operator unsigned char()
		{
			return static_cast<unsigned char>(instruction);
		}

	private:
		Type instruction;
	};

	class RegisterAdress
	{
	public:		
		enum Type
		{	
			NONE = -1,
			MODEL_NUMBER_W = 0x00,				//	RD
			VERSION_OF_FIRMWARE = 0x02,			//	RD
			ID = 0x03,							//	RD, WR
			BAUDRATE = 0x04,					//	RD,	WR
			RETURN_DELAY_TIME = 0x05,			//	RD, WR
			CW_ANGLE_LIMIT_W = 0x06,			//	RD, WR
			CCW_ANGLE_LIMIT_W = 0x08,			//	RD, WR
			DRIVE_MODE = 0x0A,					//	RD, WR
			HIGHTEST_LIMIT_TEMPERATURE = 0x0B,	//	RD, WR
			LOWEST_LIMIT_VOLTAGE = 0x0C,		//	RD, WR
			HIGHEST_LIMIT_VOLTAGE = 0x0D,		//	RD, WR
			MAX_TORQUE_W = 0x0E,				//	RD, WR
			STATUS_RETURN_LEVEL = 0x10,			//	RD, WR
			ALARM_LED = 0x11,					//	RD, WR
			ALARM_SHUTDOWN = 0x12,				//	RD, WR
			TORQUE_EABLE = 0x18,				//	RD, WR
			LED = 0x19,							//	RD, WR
			CW_COMPLIANCE_MARGIN = 0x1A,		//	RD, WR
			CCW_COMPLIANCE_MARGIN = 0x1B,		//	RD, WR
			CW_COMPLIANCE_SLOPE = 0x1C,			//	RD, WR
			CCW_COMPLIANCE_SLOPE = 0x1D,		//	RD, WR
			GOAL_POSITION_W = 0x1E,				//	RD, WR
			MOVING_SPEED_W = 0x20,				//	RD, WR
			TORQUE_LIMIT_W = 0x22,				//	RD, WR
			PRESENT_POSITION_W = 0x24,			//	RD
			PRESENT_SPEED_W = 0x26,				//	RD
			PRESENT_LOAD_W = 0x28,				//	RD
			PRESENT_VOLTAGE = 0x2A,				//	RD
			PRESENT_TEMPERATURE = 0x2B,			//	RD
			REGISTERED_INSTRUCTION = 0x2C,		//	RD
			MOVING = 0x2E,						//	RD
			LOCK = 0x2F,						//	RD, WR
			PUNCH_W = 0x30,						//	RD, WR
			SENSED_CURRENT_W = 0x38				//	RD
		};

	public:
		explicit RegisterAdress(Type adress_ = NONE)
		{
			adress = adress_;
		}

	public:
		inline Type value()
		{
			return adress;
		}

		inline operator Type()
		{
			return adress;
		}

		inline operator unsigned char()
		{
			return static_cast<unsigned char>(adress);
		}

		bool operator==(const RegisterAdress& rhs)
		{
			return adress == rhs.adress;
		}

		bool operator!=(const RegisterAdress& rhs)
		{
			return adress != rhs.adress;
		}

		int SizeOfRegister();

	private:
		Type adress;
	};

	class InstructionPacket
	{
	private:
		static const unsigned short START_PACKET = 0xFFFF;

	public:
		InstructionPacket(const ID& id_ = ID(0), const Instruction& instruction_ = Instruction(Instruction::WRITE_DATA))
			: id(id_), instruction(instruction_)
		{}

	public:
		inline unsigned char Length();

		inline InstructionPacket& AddParameter(unsigned char data);
		inline InstructionPacket& AddParameter(unsigned short data);
		InstructionPacket& AddParameter(unsigned char* data, int size);
		InstructionPacket& AddParaemter(InstructionPacket& instruction);

		vector<unsigned char> GetByte();

	private:
		unsigned char CalculateCheckSum();

	private:
		ID id;
		Instruction instruction;
		RegisterAdress registerAdress;
		vector<unsigned char> parameter;
	};

	class StatusPaceket
	{


	private:
		Error error;
	};



public:
	static const unsigned int RETRY_COUNT = 3;

public:
	Dynamixel(ID id_ = ID(0), opros::smart_ptr<Uart> uart_ = NULL);
	virtual ~Dynamixel();

public:
	inline void SetUart(opros::smart_ptr<Uart>& uart_);
	inline void SetId(ID id_);
	inline ID GetId();

public:
	bool GetModel(unsigned short& modelNumber, unsigned char& firmwareVersion);
	bool ChangeId(ID id_);
	bool SetBaudrate(unsigned int baudrate);
	bool SetReturnDelayTime(unsigned int us);
	bool GetReturnDelayTime(unsigned int& us);
	bool SetCwAngleLimit(unsigned int degree);
	bool GetCwAngleLimit(unsigned int& degree);
	bool SetCcwAngleLimit(unsigned int degree);
	bool GetCcwAngleLimit(unsigned int& degree);
	bool SetHightestLimitTemperature(unsigned int temp);
	bool GetHightestLimitTemperature(unsigned int& temp);
	bool SetHightestLimitVoltage(unsigned int volt);
	bool GetHightestLimitVoltage(unsigned int& volt);
	bool SetLowestLimitVoltage(unsigned int volt);
	bool GetLowestLimitVoltage(unsigned int& volt);
	bool SetMaxTorque(unsigned int torque);
	bool GetMaxTorque(unsigned int& torque);
	bool SetStatusReturnValue(bool isEnabled);
	bool GetStatusReturnValue(bool& isEnabled);
	bool SetAlarmLed(bool isEnabled);
	bool GetAlramLed(bool& isEnabled);
	bool SetAlarmShutdown(bool isEnabled);
	bool GetAlramShutdown(bool& isEnabled);
	bool GetDownCalibration();
	bool GetUpCalibration();
	bool SetTorqueEnable(bool isEnabled);
	bool GetTorqueEnable(bool& isEnabled);
	bool SetLed();
	bool GetLed();
	bool SetCwComplianceMargin();
	bool GetCwComplianceMargin();
	bool SetCcwComplianceMargin();
	bool GetCcwComplianceMargin();
	bool SetCwComplianceSlope();
	bool GetCwComplianceSlope();
	bool SetCcwComplianceSlope();
	bool GetCcwComplianceSlope();
	bool SetGoalPosition();
	bool GetGoalPosition();
	bool SetMovingSpeed();
	bool GetMovingSpeed();
	bool SetTorqueLimit();
	bool GetTorqueLimit();
	bool GetPresentPowition();
	bool GetPresentSpeed();
	bool GetPresentLoad();
	bool GetPresentVoltage();
	bool GetPresentTemperature();
	//Registered Instruction
	bool SetMoving();
	bool GetMoving();
	bool SetLock();
	bool GetLock();
	bool SetPunch();
	bool GetPunch();

protected:
	bool ReadRegister(char* output, int availableSize);
	bool WriteRegister(char* input, int size);

protected:
	ID id;
	opros::smart_ptr<Uart> uart;
};

class PacketBuilder
{
	typedef opros::smart_ptr<vector<unsigned char>> RawPacket;

public:
	class Packet
	{
	public:
		Packet(RawPacket rawPacket_)
			: rawPacket(rawPacket_)
		{}

	public:
		RawPacket rawPacket;
	};

	class ParameterBuilder
	{
	public:
		ParameterBuilder(RawPacket rawPacket_)
			: rawPacket(rawPacket_)
		{}

	public:
		ParameterBuilder& AddParameter(unsigned char data)
		{
			unsigned char* pData = (unsigned char*)&data;

			rawPacket->push_back(pData[0]);

			return *this;
		}

		ParameterBuilder& AddParameter(unsigned short data)
		{
			unsigned char* pData = (unsigned char*)&data;

			rawPacket->push_back(pData[0]);
			rawPacket->push_back(pData[1]);

			return *this;
		}

		ParameterBuilder& AddParameter(unsigned int data)
		{
			unsigned char* pData = (unsigned char*)&data;

			rawPacket->push_back(pData[0]);
			rawPacket->push_back(pData[1]);
			rawPacket->push_back(pData[2]);
			rawPacket->push_back(pData[3]);

			return *this;
		}

		ParameterBuilder& AddParameter(unsigned char* data, size_t size)
		{
			for (size_t i = 0; i < size; i++)
			{
				rawPacket->push_back(data[i]);
			}

			return *this;
		}

		Packet Build()
		{
			const int headerSize = 4;
			const int dataSize = rawPacket->size() - headerSize;

			(*rawPacket)[3] = dataSize + 1;
			rawPacket->push_back(CalculateCheckSum(&(*rawPacket)[4], dataSize));

			return Packet(rawPacket);
		}

		unsigned char CalculateCheckSum(unsigned char* data, size_t size)
		{
			unsigned char checkSum = 0;

			for (size_t i = 0; i < size; i++)
			{
				checkSum += data[i];
			}

			checkSum = ~checkSum;

			return checkSum;
		}

	private:
		RawPacket rawPacket;
	};


	class InstructionBuilder
	{
	public:
		InstructionBuilder(RawPacket rawPacket_)
			: rawPacket(rawPacket_)
		{}

	public:
		ParameterBuilder SetInstruction(Dynamixel::Instruction instruction)
		{
			rawPacket->push_back(instruction);

			return ParameterBuilder(rawPacket);
		}
		
		ParameterBuilder SetError(Dynamixel::Error error)
		{
			rawPacket->push_back(error);

			return ParameterBuilder(rawPacket);
		}

	private:
		RawPacket rawPacket;
	};

	class IDBuilder
	{
	public:
		IDBuilder(RawPacket rawPacket_)
			: rawPacket(rawPacket_)
		{}
	public:
		InstructionBuilder SetID(Dynamixel::ID id)
		{			
			rawPacket->push_back(id); // ID
			rawPacket->push_back(0); //아직은 확정 지을 수없는 Length

			return InstructionBuilder(rawPacket);
		}

	private:
		RawPacket rawPacket;
	};

	static IDBuilder Start()
	{
		RawPacket rawPacket(new vector<unsigned char>);
		rawPacket->reserve(143);

		//Start Pacekt
		rawPacket->push_back(0xFF);
		rawPacket->push_back(0xFF);

		return IDBuilder(rawPacket);
	}

};

#endif //__DYNAMIXEL_H__

