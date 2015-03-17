#include "RobotEver_Mobility.h"
#include "LIB_MTC_BXX_DLL.h"
#include "OprosMath.h"
#include "ParsingUserMessage.h"
#include "OprosPrintMessage.h"
#include "windows.h"


RobotEver_Mobility::RobotEver_Mobility(void)
{

}

RobotEver_Mobility::~RobotEver_Mobility(void)
{

}

bool RobotEver_Mobility::initialize(Property parameter)
{
	if(setParameter(parameter) == false) {
		return false;
	}

	if(OpenDevice())
	{
		CR_define_home_3();	//엔코더 초기화
		CR_motor_off_3();	//모든 모터 정지

		CR_kpki(RIGHT_WHEEL, 100, 0);	//PID값 설정
		CR_kdil(RIGHT_WHEEL, 400, 1000);
		CR_kpki(LEFT_WHEEL, 100, 0);
		CR_kdil(LEFT_WHEEL, 400, 1000);

		CR_absolute_velo(RIGHT_WHEEL, (unsigned int)(ENCODER*4*GEAR_RATIO*0.7));	//속도 설정
		CR_absolute_velo(LEFT_WHEEL, (unsigned int)(ENCODER*4*GEAR_RATIO*0.7));

		CR_absolute_acc(RIGHT_WHEEL, (unsigned int)(ENCODER*4*GEAR_RATIO*1));		//가속도 설정
		CR_absolute_acc(LEFT_WHEEL, (unsigned int)(ENCODER*4*GEAR_RATIO*1));


		for(int j=0;j<3;j++)
		{
			buf2[j] = 999999999;
			buf1[j] = 999999999;
		}

		//Currunt Limit 설정..
		BYTE	bufTmp[6]={0,};	
		bufTmp[0] = 0x01;
		bufTmp[1] = (BYTE)((4000 >> 4) & 0xFF);	//1번모터 (LEFT) 4A 로 설정
		bufTmp[2] = 0x01;
		bufTmp[3] = (BYTE)((4000 >> 4) & 0xFF);	//2번모터 (RIGHT) 4A 로 설정
		bufTmp[4] = 0x01;
		bufTmp[5] = (BYTE)((4000 >> 4) & 0xFF);	//3번모터 (X)	4A 로 설정
		CurrentLimitChange(bufTmp);
	}

	isRunning = false;

	return true;
}

bool RobotEver_Mobility::execute(void)
{

/*	switch(mobilityCommand)
	{
	case 0:
		drive(0, 0);
		break;
	case 1: 
		drive(distance1, 0);
		break;
	case 2:
		drive(distance1, 0);
		break;
	case 3: 
		drive(0, degree1);
		break;
	case 4:
		drive(0, degree1);
		break;
	}*/
	return true;
}

bool RobotEver_Mobility::update(void)
{
	return true;
}

bool RobotEver_Mobility::finalize(void)
{
	CR_motor_off(1);
	CR_motor_off(2);
	CR_motor_off(3);

	CloseDevice();
	return true;
}

bool RobotEver_Mobility::enable(void)
{
	location.x = location.y = location.th = 0.0;
	return true;
}

bool RobotEver_Mobility::disable(void)
{
	return true;
}

bool RobotEver_Mobility::setParameter(Property parameter)
{
	printf("RobotEver_Mobility::setParamete\n\n");

	this->parameter = parameter;
	std::string parameterValue;

	if ((parameterValue = this->parameter.getValue ("axleDistance")) == "") {
		printf ("ERROR -> invalid axleDistance parameter\n");
		return false;
	}
	axleDistance = atof (parameterValue.c_str ());
	printf("RobotEver_Mobility::setParamete\n\n");
	return true;
}

Property RobotEver_Mobility::getParameter(void)
{
	return parameter;
}

bool RobotEver_Mobility::setPosition(ObjectLocation position)
{
	location = position;

	return true;
}

bool RobotEver_Mobility::getPosition(ObjectLocation *position)
{
	*position = location;

	return true;
}

bool RobotEver_Mobility::readEncoder(long *leftEncoder, long *rightEncoder)
{
	return true;
}

bool RobotEver_Mobility::move(double distance)
{
	distance1 = (int)distance*333333;
	if(distance1==0)
	{
		isRunning = false;
	}
	else if(distance1>0)
	{
		CR_relative_pos_start(RIGHT_WHEEL,distance1);
		CR_relative_pos_start(LEFT_WHEEL,-distance1);
		mobilityCommand = 1;
		isRunning = true;
	}
	else
	{
		CR_relative_pos_start(RIGHT_WHEEL,distance1);
		CR_relative_pos_start(LEFT_WHEEL,-distance1);
		mobilityCommand = 2;
		isRunning = true;
	}
	return true;
}

bool RobotEver_Mobility::rotate(double degree)
{
	degree1 = (int)degree*635;
	if(degree1==0)
	{
		isRunning = false;
	}
	else if(degree1>0)
	{
		CR_relative_pos_start(RIGHT_WHEEL,-degree1);
		CR_relative_pos_start(LEFT_WHEEL, -degree1);
		mobilityCommand = 3;
		isRunning = true;
	}
	else
	{
		CR_relative_pos_start(RIGHT_WHEEL,-degree1);
		CR_relative_pos_start(LEFT_WHEEL, -degree1);
		mobilityCommand = 4;
		isRunning = true;
	}
	return true;
}

bool RobotEver_Mobility::drive(double forwardVelocity, double angularVelocity)
{
	int leftdir, rightdir;
	if(forwardVelocity > 0)
		leftdir = CW;	
	else
		leftdir = CCW;
	if(angularVelocity > 0)
		rightdir = CCW;	
	else
		rightdir = CW;

	CR_absolute_velo_start(RIGHT_WHEEL, leftdir, (unsigned int)ENCODER*4*GEAR_RATIO*forwardVelocity);
	CR_absolute_velo_start(LEFT_WHEEL, rightdir, (unsigned int)ENCODER*4*GEAR_RATIO*angularVelocity);		//속도 이동
	return true;
}

bool RobotEver_Mobility::stop(void)
{
	for(int j=0;j<3;j++)
	{
		buf1[j] = 999999999;
	}
	CR_stop_abruptly(1);
	CR_stop_abruptly(2);
	isRunning = false;
	return true;
}

bool RobotEver_Mobility::isWheelRunning(void)
{
	return isRunning;
}

bool RobotEver_Mobility::setParameter (int index, int parameter)
{
	return false;


}

bool RobotEver_Mobility::getParameter (int index, int *parameter)
{
	return false;
}



extern "C"
{
	__declspec(dllexport) OprosApi *getAPI();
	__declspec(dllexport) void releaseAPI(OprosApi *api);
}

OprosApi *getAPI()
{
	return new RobotEver_Mobility();
}

void releaseAPI(OprosApi *api)
{
	delete api;
}
