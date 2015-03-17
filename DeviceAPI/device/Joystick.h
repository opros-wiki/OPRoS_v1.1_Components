#ifndef	_JOYSTICK_H_
#define	_JOYSTICK_H_

#include "Device.h"

class Joystick : public Device
{
public:
	Joystick(void) { };
	virtual ~Joystick(void) { };

public:
	virtual int GetJoystickData(JoystickData &joystickData) { return API_NOT_SUPPORTED; };
};

#endif	//	_JOYSTICK_H_
