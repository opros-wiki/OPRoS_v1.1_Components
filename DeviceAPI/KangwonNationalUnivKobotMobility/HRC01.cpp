#include <Windows.h>

#include "HRC01.h"
#include "HRC01_MC_DLL.h"

#define	MAX_GAIN			32767
#define	MAX_ACCELERATION	1073741823
#define	MAX_VELOCITY		1073741823

HRC01::HRC01(void)
{
	isOpened = false;
}

HRC01::~HRC01(void)
{
	Close();
}

int HRC01::Open(void)
{
	if(isOpened == true) {
		return -1;
	}

	if(OpenDevice()) {
		isOpened = true;
	}
	else {
		return -1;
	}

	return 0;
}

void HRC01::Close(void)
{
	if(isOpened == true) {
		Disable();
		CloseDevice();
	}
}

int HRC01::Reset(int axis)
{
	if(isOpened == false) return -1;

	switch(axis) {
		case 0 :
			if(CR_Soft_Reset_3() == FALSE) return -1;
			break;
		case 1 :
		case 2 :
		case 3 :
			if(CR_soft_Reset(axis) == FALSE) return -1;
			break;
		default :
			return -1;
	}

	return 0;
}

int HRC01::Enable(int axis)
{
	if(isOpened == false) return -1;

	switch(axis) {
		case 0 :
			if(CR_absolute_velo_change(1, 0) == FALSE)	return -1;
			if(CR_absolute_velo_change(2, 0) == FALSE)	return -1;
			if(CR_absolute_velo_change(3, 0) == FALSE)	return -1;

			break;
		case 1 :
		case 2 :
		case 3 :
			if(CR_absolute_velo_change(axis, 0) == FALSE)	return -1;
		default :
			return -1;
	}

	return 0;
}

int HRC01::Disable(int axis)
{
	if(isOpened == false) return -1;

	switch(axis) {
		case 0	:
			if(CR_motor_off_3() == FALSE) return -1;
			break;
		case 1 :
		case 2 :
		case 3 :
			if(CR_motor_off(axis) == FALSE)	return -1;
			break;
		default :
			return -1;
	}

	return 0;
}

int HRC01::Stop(int axis)
{
	if(isOpened == false) return -1;

	int ret;

	switch(axis) {
		case 0 :
			ret = CR_stop_smoothly_3();
			break;
		case 1 :
		case 2 :
		case 3 :
			ret = CR_stop_smoothly(axis);
			break;
		default :
			return -1;
	}

	return ret;
}

int HRC01::QuickStop(int axis)
{
	if(isOpened == false) return -1;

	int ret;

	switch(axis) {
		case 0 :
			ret = CR_stop_abruptly_3();
			break;
		case 1 :
		case 2 :
		case 3 :
			ret = CR_stop_abruptly(axis);
			break;
		default :
			return -1;
	}

	return ret;
}

int HRC01::IsRunning(int axis, bool &isRunning)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	int ret = CR_Motion_Status(axis);
	if(ret != 0)	isRunning = true;
	else			isRunning = false;

	return 0;
}

int HRC01::SetPIDGain(int axis, unsigned short kp, unsigned short ki, unsigned short kd, unsigned short iSum)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	if(kp > MAX_GAIN)	kp = MAX_GAIN;
	if(ki > MAX_GAIN)	ki = MAX_GAIN;
	if(kd > MAX_GAIN)	kd = MAX_GAIN;
	if(iSum > MAX_GAIN)	iSum = MAX_GAIN;

	if(CR_kpki(axis, kp, ki) == FALSE) {
		return -1;
	}
	if(CR_kdil(axis, kd, iSum) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetPIDGain(	unsigned short kp1, unsigned short ki1, unsigned short kd1, unsigned short iSum1,
					unsigned short kp2, unsigned short ki2, unsigned short kd2, unsigned short iSum2,
					unsigned short kp3, unsigned short ki3, unsigned short kd3, unsigned short iSum3)
{
	if(isOpened == false) return -1;

	if(kp1 > MAX_GAIN)		kp1 = MAX_GAIN;
	if(ki1 > MAX_GAIN)		ki1 = MAX_GAIN;
	if(kd1 > MAX_GAIN)		kd1 = MAX_GAIN;
	if(iSum1 > MAX_GAIN)	iSum1 = MAX_GAIN;

	if(kp2 > MAX_GAIN)		kp2 = MAX_GAIN;
	if(ki2 > MAX_GAIN)		ki2 = MAX_GAIN;
	if(kd2 > MAX_GAIN)		kd2 = MAX_GAIN;
	if(iSum2 > MAX_GAIN)	iSum2 = MAX_GAIN;

	if(kp3 > MAX_GAIN)		kp3 = MAX_GAIN;
	if(ki3 > MAX_GAIN)		ki3 = MAX_GAIN;
	if(kd3 > MAX_GAIN)		kd3 = MAX_GAIN;
	if(iSum3 > MAX_GAIN)	iSum3 = MAX_GAIN;

	if(CR_kpki_3(kp1, ki1, kp2, ki2, kp3, ki3) == FALSE) {
		return -1;
	}

	if(CR_kdil_3(kd1, iSum1, kd2, iSum2, kd3, iSum3) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetAcceleration(int axis, unsigned int acceleration)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	if(acceleration > MAX_ACCELERATION) {
		acceleration = MAX_ACCELERATION;
	}

	if(CR_absolute_acc(axis, acceleration) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetAcceleration(unsigned int acceleration1, unsigned int acceleration2, unsigned int acceleration3)
{
	if(isOpened == false) return -1;

	if(acceleration1 > MAX_ACCELERATION)	acceleration1 = MAX_ACCELERATION;
	if(acceleration2 > MAX_ACCELERATION)	acceleration2 = MAX_ACCELERATION;
	if(acceleration3 > MAX_ACCELERATION)	acceleration3 = MAX_ACCELERATION;

	if(CR_absolute_acc_3(acceleration1, acceleration2, acceleration3) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetMaximumVelocity(int axis, unsigned int maximumVelocity)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	if(maximumVelocity > MAX_VELOCITY)	maximumVelocity = MAX_VELOCITY;

	if(CR_absolute_velo(axis, maximumVelocity) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetMaximumVelocity(unsigned int maximumVelocity1, unsigned int maximumVelocity2, unsigned int maximumVelocity3)
{
	if(isOpened == false) return -1;

	if(maximumVelocity1 > MAX_VELOCITY)	maximumVelocity1 = MAX_VELOCITY;
	if(maximumVelocity2 > MAX_VELOCITY)	maximumVelocity2 = MAX_VELOCITY;
	if(maximumVelocity3 > MAX_VELOCITY)	maximumVelocity3 = MAX_VELOCITY;

	if(CR_absolute_velo_3(maximumVelocity1, maximumVelocity2, maximumVelocity3) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetVelocity(int axis, int velocity)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	if(velocity > MAX_VELOCITY || velocity < -MAX_VELOCITY) {
		return -1;
	}

	int dir;
	unsigned int vel;

	if(velocity >= 0) {
		dir = 1;
		vel = (unsigned int)velocity;
	}
	else {
		dir = 0;
		vel = (unsigned int)(-velocity);
	}

	if(CR_absolute_velo_start(axis, dir, vel) == FALSE) {
		return -1;
	}

	return 0;
}

int HRC01::SetVelocity(int velocity1, int velocity2, int velocity3)
{
	if(isOpened == false) return -1;

	if(velocity1 > MAX_VELOCITY || velocity1 < -MAX_VELOCITY)		return -1;
	if(velocity2 > MAX_VELOCITY || velocity2 < -MAX_VELOCITY)		return -1;
	if(velocity3 > MAX_VELOCITY || velocity3 < -MAX_VELOCITY)		return -1;

	int dir[3];
	unsigned int vel[3];

	if(velocity1 >= 0) {
		dir[0] = 1;
		vel[0] = (unsigned int)velocity1;
	}
	else {
		dir[0] = 0;
		vel[0] = (unsigned int)(-velocity1);
	}

	if(velocity2 >= 0) {
		dir[1] = 1;
		vel[1] = (unsigned int)velocity2;
	}
	else {
		dir[1] = 0;
		vel[1] = (unsigned int)(-velocity2);
	}

	if(velocity3 >= 0) {
		dir[2] = 1;
		vel[2] = (unsigned int)velocity3;
	}
	else {
		dir[2] = 0;
		vel[2] = (unsigned int)(-velocity3);
	}

	if(CR_absolute_velo_start_3(dir[0], vel[0], dir[1], vel[1], dir[2], vel[2]) == FALSE) {
		return -1;
	}
	
	return 0;
}

int HRC01::GetVelocity(int axis, int &velocity)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	velocity = (int)CR_RRVelocity(axis);

	return 0;
}

void HRC01::GetVelocity(int velocity[3])
{
	if(isOpened == false) {
		velocity[0] = velocity[1] = velocity[2] = 0;
	}

	CR_RRVelocity_3((unsigned int *)velocity);
}

int HRC01::SetPosition(int axis, int position)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	return CR_absolute_pos_start(axis, position);
}

int HRC01::SetPosition(int position1, int position2, int position3)
{
	if(isOpened == false) return -1;

	return CR_absolute_pos_start_3(position1, position2, position3);
}

int HRC01::SetPositionDelta(int axis, int position)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	return CR_relative_pos_start(axis, position);
}

int HRC01::SetPositionDelta(int position1, int position2, int position3)
{
	if(isOpened == false) return -1;

	return CR_relative_pos_start_3(position1, position2, position3);
}

int HRC01::GetPosition(int axis, int &position)
{
	if(isOpened == false) return -1;

	if(axis < 1 || axis > 3) {
		return -1;
	}

	position = CR_RRposition(axis);

	return 0;
}

void HRC01::GetPosition(int position[3])
{
	if(isOpened == false) {
		position[0] = position[1] = position[2] = 0;
	}

	CR_RRposition_3(position);
}
