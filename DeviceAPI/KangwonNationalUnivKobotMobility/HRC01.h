#pragma once

class HRC01
{
public:
	HRC01(void);
	virtual ~HRC01(void);

private:
	bool isOpened;

public:
	int Open(void);
	void Close(void);

	//	PID 게인, 속도, 가속도, 위치를 초기화
	int Reset(int axis = 0);
	//	
	int Enable(int axis = 0);
	int Disable(int axis = 0);
	int Stop(int axis = 0);
	int QuickStop(int axis = 0);
	int IsRunning(int axis, bool &isRunning);

	//	PID Gain, [0 ~ 32767]
	int SetPIDGain(int axis, unsigned short kp, unsigned short ki, unsigned short kd, unsigned short iSum = 32767);
	int SetPIDGain(	unsigned short kp1, unsigned short ki1, unsigned short kd1, unsigned short iSum1,
					unsigned short kp2, unsigned short ki2, unsigned short kd2, unsigned short iSum2,
					unsigned short kp3, unsigned short ki3, unsigned short kd3, unsigned short iSum3);

	//	엔코더의 값을 내부적으로 4체배하여 사용
	//	Encoder Pulse : 500 -> 500 * 4 = 2000 [pulse]
	
	//	Acceleration, 단위 : pulse/s^2 [0 ~ 1073741823]
	int SetAcceleration(int axis, unsigned int acceleration);
	int SetAcceleration(unsigned int acceleration1, unsigned int acceleration2, unsigned int acceleration3);

	//	Maximum Velocity, 단위 : pulse/s [0 ~ 1073741823]
	int SetMaximumVelocity(int axis, unsigned int maximumVelocity);
	int SetMaximumVelocity(unsigned int maximumVelocity1, unsigned int maximumVelocity2, unsigned int maximumVelocity3);

	//	Velocity Control, 단위 : pulse/s
	int SetVelocity(int axis, int velocity);
	int SetVelocity(int velocity1, int velocity2, int velocity3);
	int GetVelocity(int axis, int &velocity);
	void GetVelocity(int velocity[3]);


	//	Position Control, 단위 : pulse [-1073741823 ~ 1073741823]
	int SetPosition(int axis, int position);
	int SetPosition(int position1, int position2, int position3);
	int SetPositionDelta(int axis, int position);
	int SetPositionDelta(int position1, int position2, int position3);
	int GetPosition(int axis, int &position);
	void GetPosition(int position[3]);

};
