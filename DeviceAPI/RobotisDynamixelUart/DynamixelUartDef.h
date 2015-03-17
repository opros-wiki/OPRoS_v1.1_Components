#pragma once

#define	INSTRUCTION_ERROR	0x40	//���ǵ��� ���� Instruction�� ���۵� ���. �Ǵ� reg_write ��ɾ��� action����� ���޵� ��� 1�� ������
#define	OVERLOAD_ERROR		0x20	//������ �ִ� Torque�� ������ ������ ������ �� ���� �� 1�� ������
#define	CHECKSUM_ERROR		0x10	//���۵� Instruction Packet�� Checksum�� ���� ���� ��1�� ������
#define	RANGE_ERROR			0x08	//�������� ���Ƴ� ����� ��� 1�� ������.
#define	OVERHEATING_ERROR	0x04	//Dynamixel ���� �µ��� Control Table�� ������ ���� �µ������� ����� �� 1�� ������
#define	ANGLELIMIT_ERROR	0x02	//Goal Position�� CW Angle Limit ~ CCW Angle Limit �������� ������ Writing �Ǿ����� 1�� ������
#define	INPUTVOLTAGE_ERROR	0x01	//�ΰ��� ������ Control Table�� ������ ���� ���� ����������� ��� 1�� ������

#define ID_BROADCAST		0xFE	//����� ��� ���Ͱ� �����Ŷ�� �����ϸ� Status packet�� ���ϵ��� �ʴ´�.

#define	START_PACK			0xFFFF
#define	PING				0x01	//���೻�� ����. ����Ⱑ Status Packet�� �ް��� �� ��� ���
#define	READ_DATA			0x02	//DATA�� �д� ���
#define	WRITE_DATA			0x03	//DATA�� ���� ���
#define	REG_WRITE			0x04	//WRITE DATA�� ������ �����ϳ�, �����·� �ִٰ� ACTION����� �����ؾ� �����
#define	ACTION				0x05	//REG WRITE�� ��ϵ� ������ �����϶�� ���
#define	RESET				0x06	//���¸� ���� ���� ���·� ���ͽ�Ű�� ���
#define	SYNC_WRITE			0x83	//�ѹ��� �������� ���͵��� ���ÿ� �����ϰ��� �� �� ���Ǵ� ���

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
