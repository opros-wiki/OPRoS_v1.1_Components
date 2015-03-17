#pragma once

#define	INSTRUCTION_ERROR	0x40	//정의되지 않은 Instruction이 전송된 경우. 또는 reg_write 명령없이 action명령이 전달된 경우 1로 설정됨
#define	OVERLOAD_ERROR		0x20	//지정된 최대 Torque로 현재의 하중을 제어할 수 없을 때 1로 설정됨
#define	CHECKSUM_ERROR		0x10	//전송된 Instruction Packet의 Checksum이 맞지 않을 때1로 설정됨
#define	RANGE_ERROR			0x08	//사용범위를 벗아난 명령일 경우 1로 설정됨.
#define	OVERHEATING_ERROR	0x04	//Dynamixel 내부 온도가 Control Table에 설정된 동작 온도범위를 벗어났을 때 1로 설정됨
#define	ANGLELIMIT_ERROR	0x02	//Goal Position이 CW Angle Limit ~ CCW Angle Limit 범위밖의 값으로 Writing 되었을때 1로 설정됨
#define	INPUTVOLTAGE_ERROR	0x01	//인가된 전압이 Control Table에 설정된 동작 전압 범위를벗어났을 경우 1로 설정됨

#define ID_BROADCAST		0xFE	//연결된 모든 모터가 명령패킷을 수행하며 Status packet은 리턴되지 않는다.

#define	START_PACK			0xFFFF
#define	PING				0x01	//수행내용 없음. 제어기가 Status Packet을 받고자 할 경우 사용
#define	READ_DATA			0x02	//DATA를 읽는 명령
#define	WRITE_DATA			0x03	//DATA를 쓰는 명령
#define	REG_WRITE			0x04	//WRITE DATA와 내용은 유사하나, 대기상태로 있다가 ACTION명령이 도착해야 수행됨
#define	ACTION				0x05	//REG WRITE로 등록된 동작을 시작하라는 명령
#define	RESET				0x06	//상태를 공장 출하 상태로 복귀시키는 명령
#define	SYNC_WRITE			0x83	//한번에 여러개의 모터들을 동시에 제어하고자 할 때 사용되는 명령

//	Control Table
#define	MODEL_NUMBER_W				0x00	//	RD
#define	VERSION_OF_FIRMWARE			0x02	//	RD
#define	ID							0x03	//	RD, WR
#define	BAUDRATE					0x04	//	RD,	WR
#define	RETURN_DELAY_TIME			0x05	//	RD, WR
#define	CW_ANGLE_LIMIT_W			0x06	//	RD, WR
#define	CCW_ANGLE_LIMIT_W			0x08	//	RD, WR
#define	HIGHTEST_LIMIT_TEMPERATURE	0x0B	//	RD, WR
#define	LOWEST_LIMIT_VOLTAGE		0x0C	//	RD, WR
#define	HIGHEST_LIMIT_VOLTAGE		0x0D	//	RD, WR
#define	MAX_TORQUE_W				0x0E	//	RD, WR
#define	STATUS_RETURN_VALUE			0x10	//	RD, WR
#define	ALARM_LED					0x11	//	RD, WR
#define	ALARM_SHUTDOWN				0x12	//	RD, WR
#define	DOWN_CALIBRATION_W			0x14	//	RD
#define	UP_CALIBRATION_W			0x16	//	RD
#define	TORQUE_EABLE				0x18	//	RD, WR
#define	LED							0x19	//	RD, WR
#define	CW_COMPLIANCE_MARGIN		0x1A	//	RD, WR
#define	CCW_COMPLIANCE_MARGIN		0x1B	//	RD, WR
#define	CW_COMPLICANCE_SLOPE		0x1C	//	RD, WR
#define	CCW_COMPLICANCE_SLOPE		0x1D	//	RD, WR
#define	GOAL_POSITION_W				0x1E	//	RD, WR
#define	MOVING_SPEED_W				0x20	//	RD, WR
#define	TORQUE_LIMIT_W				0x22	//	RD, WR
#define	PRESENT_POSITION_W			0x24	//	RD
#define	PRESENT_SPEED_W				0x26	//	RD
#define	PRESENT_LOAD_W				0x28	//	RD
#define	PRESENT_VOLTAGE				0x2A	//	RD
#define	PRESETN_TEMPERATURE			0x2B	//	RD
#define	REGISTERED_INSTRUCTION		0x2C	//	RD, WR
#define	MOVING						0x2E	//	RD, WR
#define	LOCK						0x2F	//	RD, WR
#define	PUNCH_W						0x30	//	RD, WR
